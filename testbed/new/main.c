#include "SDL.h"
#include "SDL_thread.h"

#include <shared.h>
#include <osd.h>
#include <ntsc/sms_ntsc.h>
#include <ntsc/md_ntsc.h>

#define SOUND_FREQUENCY 48000
#define SOUND_SAMPLES_SIZE  2048

#define VIDEO_WIDTH  320
#define VIDEO_HEIGHT 240

char GG_ROM[256];
char AR_ROM[256];
char SK_ROM[256];
char SK_UPMEM[256];
char GG_BIOS[256];
char CD_BIOS_EU[256];
char CD_BIOS_US[256];
char CD_BIOS_JP[256];
char MD_BIOS[256];
char MS_BIOS_US[256];
char MS_BIOS_EU[256];
char MS_BIOS_JP[256];

struct config_t config;

int joynum = 0;

int log_error   = 0;
int debug_on    = 0;
int turbo_mode  = 0;
int use_sound   = 1;
int fullscreen  = 0; /* SDL_WINDOW_FULLSCREEN */

struct {
  SDL_Window* window;
  SDL_Surface* surf_screen;
  SDL_Surface* surf_bitmap;
  SDL_Rect srect;
  SDL_Rect drect;
  Uint32 frames_rendered;
} sdl_video;

/* sound */

struct {
  char* current_pos;
  char* buffer;
  int current_emulated_samples;
} sdl_sound;


 uint8 brm_format[0x40] =
{
  0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x00,0x00,0x00,0x00,0x40,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x53,0x45,0x47,0x41,0x5f,0x43,0x44,0x5f,0x52,0x4f,0x4d,0x00,0x01,0x00,0x00,0x00,
  0x52,0x41,0x4d,0x5f,0x43,0x41,0x52,0x54,0x52,0x49,0x44,0x47,0x45,0x5f,0x5f,0x5f
};


 short soundframe[SOUND_SAMPLES_SIZE];

int sdl_input_update(void);

void osd_input_update(void)
{
  sdl_input_update();
}

void set_config_defaults(void)
{
  int i;

  /* sound options */
  config.psg_preamp     = 150;
  config.fm_preamp      = 100;
  config.cdda_volume    = 100;
  config.pcm_volume     = 100;
  config.hq_fm          = 1;
  config.hq_psg         = 1;
  config.filter         = 1;
  config.low_freq       = 200;
  config.high_freq      = 8000;
  config.lg             = 100;
  config.mg             = 100;
  config.hg             = 100;
  config.lp_range       = 0x9999; /* 0.6 in 0.16 fixed point */
  config.ym2612         = 0;
  config.ym2413         = 2; /* = AUTO (0 = always OFF, 1 = always ON) */
  config.ym3438         = 0;
  config.opll           = 0;
  config.mono           = 0;

  /* system options */
  config.system         = 0; /* = AUTO (or SYSTEM_SG, SYSTEM_SGII, SYSTEM_SGII_RAM_EXT, SYSTEM_MARKIII, SYSTEM_SMS, SYSTEM_SMS2, SYSTEM_GG, SYSTEM_MD) */
  config.region_detect  = 0; /* = AUTO (1 = USA, 2 = EUROPE, 3 = JAPAN/NTSC, 4 = JAPAN/PAL) */
  config.vdp_mode       = 0; /* = AUTO (1 = NTSC, 2 = PAL) */
  config.master_clock   = 0; /* = AUTO (1 = NTSC, 2 = PAL) */
  config.force_dtack    = 0;
  config.addr_error     = 1;
  config.bios           = 0;
  config.lock_on        = 0; /* = OFF (or TYPE_SK, TYPE_GG & TYPE_AR) */
  config.add_on         = 0; /* = HW_ADDON_AUTO (or HW_ADDON_MEGACD, HW_ADDON_MEGASD & HW_ADDON_ONE) */
  config.cd_latency     = 1;

  /* display options */
  config.overscan = 0;  /* 3 = all borders (0 = no borders , 1 = vertical borders only, 2 = horizontal borders only) */
  config.gg_extra = 0;  /* 1 = show extended Game Gear screen (256x192) */
  config.render   = 0;  /* 1 = double resolution output (only when interlaced mode 2 is enabled) */
  config.ntsc     = 0;
  config.lcd      = 0;  /* 0.8 fixed point */
  config.enhanced_vscroll = 0;
  config.enhanced_vscroll_limit = 8;

  /* controllers options */
  input.system[0]       = SYSTEM_GAMEPAD;
  input.system[1]       = SYSTEM_GAMEPAD;
  config.gun_cursor[0]  = 1;
  config.gun_cursor[1]  = 1;
  config.invert_mouse   = 0;
  for (i=0;i<MAX_INPUTS;i++)
  {
    /* autodetected control pad type */
    config.input[i].padtype = DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B;
  }
}

 void sdl_sound_callback(void *userdata, Uint8 *stream, int len)
{
  if(sdl_sound.current_emulated_samples < len) {
    memset(stream, 0, len);
  }
  else {
    memcpy(stream, sdl_sound.buffer, len);
    /* loop to compensate desync */
    do {
      sdl_sound.current_emulated_samples -= len;
    } while(sdl_sound.current_emulated_samples > 2 * len);
    memcpy(sdl_sound.buffer,
           sdl_sound.current_pos - sdl_sound.current_emulated_samples,
           sdl_sound.current_emulated_samples);
    sdl_sound.current_pos = sdl_sound.buffer + sdl_sound.current_emulated_samples;
  }
}

 int sdl_sound_init()
{
  int n;
  SDL_AudioSpec as_desired;

  if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Audio initialization failed", sdl_video.window);
    return 0;
  }

  as_desired.freq     = SOUND_FREQUENCY;
  as_desired.format   = AUDIO_S16SYS;
  as_desired.channels = 2;
  as_desired.samples  = SOUND_SAMPLES_SIZE;
  as_desired.callback = sdl_sound_callback;

  if(SDL_OpenAudio(&as_desired, NULL) < 0) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Audio open failed", sdl_video.window);
    return 0;
  }

  sdl_sound.current_emulated_samples = 0;
  n = SOUND_SAMPLES_SIZE * 2 * sizeof(short) * 20;
  sdl_sound.buffer = (char*)malloc(n);
  if(!sdl_sound.buffer) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't allocate audio buffer", sdl_video.window);
    return 0;
  }
  memset(sdl_sound.buffer, 0, n);
  sdl_sound.current_pos = sdl_sound.buffer;
  return 1;
}

 void sdl_sound_update(int enabled)
{
  int size = audio_update(soundframe) * 2;

  if (enabled)
  {
    int i;
    short *out;

    SDL_LockAudio();
    out = (short*)sdl_sound.current_pos;
    for(i = 0; i < size; i++)
    {
      *out++ = soundframe[i];
    }
    sdl_sound.current_pos = (char*)out;
    sdl_sound.current_emulated_samples += size * sizeof(short);
    SDL_UnlockAudio();
  }
}

 void sdl_sound_close()
{
  SDL_PauseAudio(1);
  SDL_CloseAudio();
  if (sdl_sound.buffer)
    free(sdl_sound.buffer);
}

/* video */
md_ntsc_t *md_ntsc;
sms_ntsc_t *sms_ntsc;

 int sdl_video_init()
{
#if defined(USE_8BPP_RENDERING)
  const unsigned long surface_format = SDL_PIXELFORMAT_RGB332;
#elif defined(USE_15BPP_RENDERING)
  const unsigned long surface_format = SDL_PIXELFORMAT_RGB555;
#elif defined(USE_16BPP_RENDERING)
  const unsigned long surface_format = SDL_PIXELFORMAT_RGB565;
#elif defined(USE_32BPP_RENDERING)
  const unsigned long surface_format = SDL_PIXELFORMAT_RGB888;
#endif

  if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Video initialization failed", sdl_video.window);
    return 0;
  }
  sdl_video.window = SDL_CreateWindow("Genesis Plus GX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH, VIDEO_HEIGHT, fullscreen);
  sdl_video.surf_screen  = SDL_GetWindowSurface(sdl_video.window);
  sdl_video.surf_bitmap = SDL_CreateRGBSurfaceWithFormat(0, 720, 576, SDL_BITSPERPIXEL(surface_format), surface_format);
  sdl_video.frames_rendered = 0;
  SDL_ShowCursor(0);
  return 1;
}

 void sdl_video_update()
{
  if (system_hw == SYSTEM_MCD)
  {
    system_frame_scd(0);
  }
  else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    system_frame_gen(0);
  }
  else	
  {
    system_frame_sms(0);
  }

  /* viewport size changed */
  if(bitmap.viewport.changed & 1)
  {
    bitmap.viewport.changed &= ~1;

    /* source bitmap */
    sdl_video.srect.w = bitmap.viewport.w+2*bitmap.viewport.x;
    sdl_video.srect.h = bitmap.viewport.h+2*bitmap.viewport.y;
    sdl_video.srect.x = 0;
    sdl_video.srect.y = 0;
    if (sdl_video.srect.w > sdl_video.surf_screen->w)
    {
      sdl_video.srect.x = (sdl_video.srect.w - sdl_video.surf_screen->w) / 2;
      sdl_video.srect.w = sdl_video.surf_screen->w;
    }
    if (sdl_video.srect.h > sdl_video.surf_screen->h)
    {
      sdl_video.srect.y = (sdl_video.srect.h - sdl_video.surf_screen->h) / 2;
      sdl_video.srect.h = sdl_video.surf_screen->h;
    }

    /* destination bitmap */
    sdl_video.drect.w = sdl_video.srect.w;
    sdl_video.drect.h = sdl_video.srect.h;
    sdl_video.drect.x = (sdl_video.surf_screen->w - sdl_video.drect.w) / 2;
    sdl_video.drect.y = (sdl_video.surf_screen->h - sdl_video.drect.h) / 2;

    /* clear destination surface */
    SDL_FillRect(sdl_video.surf_screen, 0, 0);

#if 0
    if (config.render && (interlaced || config.ntsc))  rect.h *= 2;
    if (config.ntsc) rect.w = (reg[12]&1) ? MD_NTSC_OUT_WIDTH(rect.w) : SMS_NTSC_OUT_WIDTH(rect.w);
    if (config.ntsc)
    {
      sms_ntsc = (sms_ntsc_t *)malloc(sizeof(sms_ntsc_t));
      md_ntsc = (md_ntsc_t *)malloc(sizeof(md_ntsc_t));

      switch (config.ntsc)
      {
        case 1:
          sms_ntsc_init(sms_ntsc, &sms_ntsc_composite);
          md_ntsc_init(md_ntsc, &md_ntsc_composite);
          break;
        case 2:
          sms_ntsc_init(sms_ntsc, &sms_ntsc_svideo);
          md_ntsc_init(md_ntsc, &md_ntsc_svideo);
          break;
        case 3:
          sms_ntsc_init(sms_ntsc, &sms_ntsc_rgb);
          md_ntsc_init(md_ntsc, &md_ntsc_rgb);
          break;
      }
    }
    else
    {
      if (sms_ntsc)
      {
        free(sms_ntsc);
        sms_ntsc = NULL;
      }

      if (md_ntsc)
      {
        free(md_ntsc);
        md_ntsc = NULL;
      }
    }
#endif
  }

  SDL_BlitSurface(sdl_video.surf_bitmap, &sdl_video.srect, sdl_video.surf_screen, &sdl_video.drect);
  SDL_UpdateWindowSurface(sdl_video.window);

  ++sdl_video.frames_rendered;
}

 void sdl_video_close()
{
  SDL_FreeSurface(sdl_video.surf_bitmap);
  SDL_DestroyWindow(sdl_video.window);
}

/* Timer Sync */

struct {
  SDL_sem* sem_sync;
  unsigned ticks;
} sdl_sync;

 Uint32 sdl_sync_timer_callback(Uint32 interval, void *param)
{
  SDL_SemPost(sdl_sync.sem_sync);
  sdl_sync.ticks++;
  if (sdl_sync.ticks == (vdp_pal ? 50 : 20))
  {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = vdp_pal ? (sdl_video.frames_rendered / 3) : sdl_video.frames_rendered;
    userevent.data1 = NULL;
    userevent.data2 = NULL;
    sdl_sync.ticks = sdl_video.frames_rendered = 0;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
  }
  return interval;
}

 int sdl_sync_init()
{
  if(SDL_InitSubSystem(SDL_INIT_TIMER|SDL_INIT_EVENTS) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Timer initialization failed", sdl_video.window);
    return 0;
  }

  sdl_sync.sem_sync = SDL_CreateSemaphore(0);
  sdl_sync.ticks = 0;
  return 1;
}

 void sdl_sync_close()
{
  if(sdl_sync.sem_sync)
    SDL_DestroySemaphore(sdl_sync.sem_sync);
}

 const uint16 vc_table[4][2] =
{
  /* NTSC, PAL */
  {0xDA , 0xF2},  /* Mode 4 (192 lines) */
  {0xEA , 0x102}, /* Mode 5 (224 lines) */
  {0xDA , 0xF2},  /* Mode 4 (192 lines) */
  {0x106, 0x10A}  /* Mode 5 (240 lines) */
};

 int sdl_control_update(SDL_Keycode keystate)
{
    switch (keystate)
    {
      case SDLK_TAB:
      {
        system_reset();
        break;
      }

      case SDLK_F1:
      {
        if (SDL_ShowCursor(-1)) SDL_ShowCursor(0);
        else SDL_ShowCursor(1);
        break;
      }

      case SDLK_F2:
      {
        fullscreen = (fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
        SDL_SetWindowFullscreen(sdl_video.window, fullscreen);
        sdl_video.surf_screen  = SDL_GetWindowSurface(sdl_video.window);
        bitmap.viewport.changed = 1;
        break;
      }

      case SDLK_F3:
      {
        if (config.bios == 0) config.bios = 3;
        else if (config.bios == 3) config.bios = 1;
        break;
      }

      case SDLK_F4:
      {
        if (!turbo_mode) use_sound ^= 1;
        break;
      }

      case SDLK_F5:
      {
        log_error ^= 1;
        break;
      }

      case SDLK_F6:
      {
        if (!use_sound)
        {
          turbo_mode ^=1;
          sdl_sync.ticks = 0;
        }
        break;
      }

      case SDLK_F9:
      {
        config.region_detect = (config.region_detect + 1) % 5;
        get_region(0);

        /* framerate has changed, reinitialize audio timings */
        audio_init(snd.sample_rate, 0);

        /* system with region BIOS should be reinitialized */
        if ((system_hw == SYSTEM_MCD) || ((system_hw & SYSTEM_SMS) && (config.bios & 1)))
        {
          system_init();
          system_reset();
        }
        else
        {
          /* reinitialize I/O region register */
          if (system_hw == SYSTEM_MD)
          {
            io_reg[0x00] = 0x20 | region_code | (config.bios & 1);
          }
          else
          {
            io_reg[0x00] = 0x80 | (region_code >> 1);
          }

          /* reinitialize VDP */
          if (vdp_pal)
          {
            status |= 1;
            lines_per_frame = 313;
          }
          else
          {
            status &= ~1;
            lines_per_frame = 262;
          }

          /* reinitialize VC max value */
          switch (bitmap.viewport.h)
          {
            case 192:
              vc_max = vc_table[0][vdp_pal];
              break;
            case 224:
              vc_max = vc_table[1][vdp_pal];
              break;
            case 240:
              vc_max = vc_table[3][vdp_pal];
              break;
          }
        }
        break;
      }

      case SDLK_F10:
      {
        gen_reset(0);
        break;
      }

      case SDLK_F11:
      {
        config.overscan =  (config.overscan + 1) & 3;
        if ((system_hw == SYSTEM_GG) && !config.gg_extra)
        {
          bitmap.viewport.x = (config.overscan & 2) ? 14 : -48;
        }
        else
        {
          bitmap.viewport.x = (config.overscan & 2) * 7;
        }
        bitmap.viewport.changed = 3;
        break;
      }

      case SDLK_F12:
      {
        joynum = (joynum + 1) % MAX_DEVICES;
        while (input.dev[joynum] == NO_DEVICE)
        {
          joynum = (joynum + 1) % MAX_DEVICES;
        }
        break;
      }

      case SDLK_ESCAPE:
      {
        return 0;
      }

      default:
        break;
    }

   return 1;
}

__thread t_input* __tmpInput = NULL;

void initialize ()
{
  __tmpInput = (t_input*) calloc (1, sizeof(t_input));
  __tmpInput->dev[0] = 0;  
  __tmpInput->dev[1] = 255;

  /**
   * Allocating large buffers
  */

  // cart_hw/areplay.h

  action_replay.ram = calloc(sizeof(uint8), 0x10000);
  
  // cart_hw/md_cart.h

  ext.md_cart.lockrom = calloc(sizeof(uint8), 0x10000);
  ext.md_cart.rom = calloc(sizeof(uint8), MAXROMSIZE);

  // cart_hw/sram.h

  sram.sram = calloc(sizeof(uint8), 0x10000);

  // cd_hw/cd_cart.h

  ext.cd_hw.cartridge.area = calloc(sizeof(uint8), 0x810000);

  // cd_hw/cdc.h

  memset(&cdc, 0, sizeof(cdc_t));
  ext.cd_hw.cdc_hw.ram = calloc(sizeof(uint8), 0x4000 + 2352);
  
  // cd_hw/gfx.h

  memset(&ext.cd_hw.gfx_hw, 0, sizeof(gfx_t));
  ext.cd_hw.gfx_hw.lut_offset = (uint16*) calloc(sizeof(uint16), 0x8000);

  for (size_t i = 0; i < 4; i++) 
  {
    ext.cd_hw.gfx_hw.lut_prio[i] = (uint8**) calloc (sizeof(uint8*), 0x100);
    for (size_t j = 0; j < 0x100; j++)  ext.cd_hw.gfx_hw.lut_prio[i][j] = (uint8*) calloc (sizeof(uint8), 0x100);
  }
    
  ext.cd_hw.gfx_hw.lut_pixel = (uint8*) calloc(sizeof(uint8), 0x200);
  ext.cd_hw.gfx_hw.lut_cell  = (uint8*) calloc(sizeof(uint8), 0x100);

  // cd_hw/pcm.h

  memset(&ext.cd_hw.pcm_hw, 0, sizeof(pcm_t));
  ext.cd_hw.pcm_hw.ram = (uint8*) calloc(sizeof(uint8), 0x10000);

  // cd_hw/scd.h

  ext.cd_hw.bootrom     = (uint8*) calloc(sizeof(uint8), 0x20000);
  ext.cd_hw.prg_ram     = (uint8*) calloc(sizeof(uint8), 0x80000);
  ext.cd_hw.word_ram[0] = (uint8*) calloc(sizeof(uint8), 0x20000);
  ext.cd_hw.word_ram[1] = (uint8*) calloc(sizeof(uint8), 0x20000);
  ext.cd_hw.word_ram_2M = (uint8*) calloc(sizeof(uint8), 0x40000);
  ext.cd_hw.bram        = (uint8*) calloc(sizeof(uint8), 0x2000);
  
  // sound.h

  fm_buffer = (int*) calloc(sizeof(int), 1080 * 2 * 48);

  // z80.h

  SZ        = (UINT8* ) calloc(sizeof(UINT8), 256);
  SZ_BIT    = (UINT8* ) calloc(sizeof(UINT8), 256);
  SZP       = (UINT8* ) calloc(sizeof(UINT8), 256);
  SZHV_inc  = (UINT8* ) calloc(sizeof(UINT8), 256);
  SZHV_dec  = (UINT8* ) calloc(sizeof(UINT8), 256);
  SZHVC_add = (UINT8* ) calloc(sizeof(UINT8), 2*256*256);
  SZHVC_sub = (UINT8* ) calloc(sizeof(UINT8), 2*256*256);

  // genesis.h

  boot_rom = (uint8*) calloc(sizeof(uint8), 0x800);
  work_ram = (uint8*) calloc(sizeof(uint8), 0x10000);
  zram     = (uint8*) calloc(sizeof(uint8), 0x2000);

  // system.h

  bitmap.data = (uint8_t*) calloc(sizeof(uint8_t), 1024*1024*4);

  // vdp_ctrl.h

  sat  = (uint8*) calloc (sizeof(uint8), 0x400);
  vram = (uint8*) calloc (sizeof(uint8), 0x10000);
  bg_name_dirty = (uint8 *) calloc (sizeof(uint8 ), 0x800);
  bg_name_list  = (uint16*) calloc (sizeof(uint16), 0x800);
  // vdp_render.h

  bg_pattern_cache = (uint8      *) calloc (sizeof(uint8      ), 0x80000);
  name_lut         = (uint8      *) calloc (sizeof(uint8      ), 0x400);
  bp_lut           = (uint32     *) calloc (sizeof(uint32     ), 0x10000);
  lut[0]           = (uint8      *) calloc (sizeof(uint8      ), LUT_SIZE);
  lut[1]           = (uint8      *) calloc (sizeof(uint8      ), LUT_SIZE);
  lut[2]           = (uint8      *) calloc (sizeof(uint8      ), LUT_SIZE);
  lut[3]           = (uint8      *) calloc (sizeof(uint8      ), LUT_SIZE);
  lut[4]           = (uint8      *) calloc (sizeof(uint8      ), LUT_SIZE);
  lut[5]           = (uint8      *) calloc (sizeof(uint8      ), LUT_SIZE);
  pixel            = (PIXEL_OUT_T*) calloc (sizeof(PIXEL_OUT_T), 0x100);
  pixel_lut[0]     = (PIXEL_OUT_T*) calloc (sizeof(PIXEL_OUT_T), 0x200);
  pixel_lut[1]     = (PIXEL_OUT_T*) calloc (sizeof(PIXEL_OUT_T), 0x200);
  pixel_lut[2]     = (PIXEL_OUT_T*) calloc (sizeof(PIXEL_OUT_T), 0x200);
  pixel_lut_m4     = (PIXEL_OUT_T*) calloc (sizeof(PIXEL_OUT_T), 0x40);
  linebuf[0]       = (uint8      *) calloc (sizeof(uint8      ), 0x200);
  linebuf[1]       = (uint8      *) calloc (sizeof(uint8      ), 0x200);

  /* set default config */
  set_config_defaults();

  strcpy(GG_ROM    ,  "./ggenie.bin");
  strcpy(AR_ROM    ,  "./areplay.bin");
  strcpy(SK_ROM    ,  "./sk.bin");
  strcpy(SK_UPMEM  ,  "./sk2chip.bin");
  strcpy(CD_BIOS_US,  "./bios_CD_U.bin");
  strcpy(CD_BIOS_EU,  "./bios_CD_E.bin");
  strcpy(CD_BIOS_JP,  "./bios_CD_J.bin");
  strcpy(MD_BIOS   ,  "./bios_MD.bin");
  strcpy(MS_BIOS_US,  "./bios_U.sms");
  strcpy(MS_BIOS_EU,  "./bios_E.sms");
  strcpy(MS_BIOS_JP,  "./bios_J.sms");
  strcpy(GG_BIOS   ,  "./bios.gg");

  /* mark all BIOS as unloaded */
  system_bios = 0;

  /* Genesis BOOT ROM support (2KB max) */
  memset(boot_rom, 0xFF, 0x800);
  FILE* fp;
  fp = fopen(MD_BIOS, "rb");
  if (fp != NULL)
  {
    int i;

    /* read BOOT ROM */
    fread(boot_rom, 1, 0x800, fp);
    fclose(fp);

    /* check BOOT ROM */
    if (!memcmp((char *)(boot_rom + 0x120),"GENESIS OS", 10))
    {
      /* mark Genesis BIOS as loaded */
      system_bios = SYSTEM_MD;
    }

    /* Byteswap ROM */
    for (i=0; i<0x800; i+=2)
    {
      uint8 temp = boot_rom[i];
      boot_rom[i] = boot_rom[i+1];
      boot_rom[i+1] = temp;
    }
  }
}

void initializeVideoOutput()
{
    /* initialize SDL */
  if(SDL_Init(0) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL initialization failed", sdl_video.window);
    return;
  }
  sdl_video_init();
  if (use_sound) sdl_sound_init();
  sdl_sync_init();

  /* initialize Genesis virtual system */
  SDL_LockSurface(sdl_video.surf_bitmap);
  memset(&bitmap, 0, sizeof(t_bitmap));
  bitmap.width        = 720;
  bitmap.height       = 576;
#if defined(USE_8BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 1);
#elif defined(USE_15BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 2);
#elif defined(USE_16BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 2);
#elif defined(USE_32BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 4);
#endif
  bitmap.data         = sdl_video.surf_bitmap->pixels;
  SDL_UnlockSurface(sdl_video.surf_bitmap);
  bitmap.viewport.changed = 3;
}

void loadROM(char* filePath)
{
    FILE* fp;

    /* Load game file */
    if(!load_rom(filePath))
    {
      char caption[256];
      sprintf(caption, "Error loading file `%s'.", filePath);
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", caption, sdl_video.window);
      return;
    }

    /* initialize system hardware */
    audio_init(SOUND_FREQUENCY, 0);
    system_init();

    /* Mega CD specific */
    if (system_hw == SYSTEM_MCD)
    {
      /* load internal backup RAM */
      fp = fopen("./scd.brm", "rb");
      if (fp!=NULL)
      {
        fread(scd.bram, 0x2000, 1, fp);
        fclose(fp);
      }

      /* check if internal backup RAM is formatted */
      if (memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20))
      {
        /* clear internal backup RAM */
        memset(scd.bram, 0x00, 0x200);

        /* Internal Backup RAM size fields */
        brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = 0x00;
        brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (0x2000 / 64) - 3;

        /* format internal backup RAM */
        memcpy(scd.bram + 0x2000 - 0x40, brm_format, 0x40);
      }

      /* load cartridge backup RAM */
      if (scd.cartridge.id)
      {
        fp = fopen("./cart.brm", "rb");
        if (fp!=NULL)
        {
          fread(scd.cartridge.area, scd.cartridge.mask + 1, 1, fp);
          fclose(fp);
        }

        /* check if cartridge backup RAM is formatted */
        if (memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20))
        {
          /* clear cartridge backup RAM */
          memset(scd.cartridge.area, 0x00, scd.cartridge.mask + 1);

          /* Cartridge Backup RAM size fields */
          brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = (((scd.cartridge.mask + 1) / 64) - 3) >> 8;
          brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (((scd.cartridge.mask + 1) / 64) - 3) & 0xff;

          /* format cartridge backup RAM */
          memcpy(scd.cartridge.area + scd.cartridge.mask + 1 - sizeof(brm_format), brm_format, sizeof(brm_format));
        }
      }
    }

    if (sram.on)
    {
      /* load SRAM */
      fp = fopen("./game.srm", "rb");
      if (fp!=NULL)
      {
        fread(sram.sram,0x10000,1, fp);
        fclose(fp);
      }
    }

    /* reset system hardware */
    system_reset();

    if(use_sound) SDL_PauseAudio(0);
}

void renderFrame()
{
  sdl_video_update();
  sdl_sound_update(use_sound);
}

int sdl_input_update(void)
{
  input = *__tmpInput;
  return 0;
}

void advanceFrame(const uint16_t controller1, const uint16_t controller2)
{
  __tmpInput->pad[0] = controller1;
  __tmpInput->pad[1] = controller2;

  if (system_hw == SYSTEM_MCD)
  {
    system_frame_scd(1);
  }
  else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    system_frame_gen(1);
  }
  else	
  {
    system_frame_sms(1);
  }

  sdl_sound_update(0);
}

void finalizeVideoOutput()
{
  sdl_video_close();
  sdl_sound_close();
  sdl_sync_close();
  SDL_Quit();
}

int oldMain (int argc, char **argv)
{
  FILE *fp;
  int running = 1;

  /* Print help if no game specified */
  if(argc < 2)
  {
    char caption[256];
    sprintf(caption, "Genesis Plus GX\\SDL\nusage: %s gamename\n", argv[0]);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", caption, sdl_video.window);
    return 1;
  }

  /* set default config */
  set_config_defaults();

  /* mark all BIOS as unloaded */
  system_bios = 0;

  /* Genesis BOOT ROM support (2KB max) */
  memset(boot_rom, 0xFF, 0x800);
  fp = fopen(MD_BIOS, "rb");
  if (fp != NULL)
  {
    int i;

    /* read BOOT ROM */
    fread(boot_rom, 1, 0x800, fp);
    fclose(fp);

    /* check BOOT ROM */
    if (!memcmp((char *)(boot_rom + 0x120),"GENESIS OS", 10))
    {
      /* mark Genesis BIOS as loaded */
      system_bios = SYSTEM_MD;
    }

    /* Byteswap ROM */
    for (i=0; i<0x800; i+=2)
    {
      uint8 temp = boot_rom[i];
      boot_rom[i] = boot_rom[i+1];
      boot_rom[i+1] = temp;
    }
  }

  /* initialize SDL */
  if(SDL_Init(0) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL initialization failed", sdl_video.window);
    return 1;
  }
  sdl_video_init();
  if (use_sound) sdl_sound_init();
  sdl_sync_init();

  /* initialize Genesis virtual system */
  SDL_LockSurface(sdl_video.surf_bitmap);
  memset(&bitmap, 0, sizeof(t_bitmap));
  bitmap.width        = 720;
  bitmap.height       = 576;
#if defined(USE_8BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 1);
#elif defined(USE_15BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 2);
#elif defined(USE_16BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 2);
#elif defined(USE_32BPP_RENDERING)
  bitmap.pitch        = (bitmap.width * 4);
#endif
  bitmap.data         = sdl_video.surf_bitmap->pixels;
  SDL_UnlockSurface(sdl_video.surf_bitmap);
  bitmap.viewport.changed = 3;

  /* Load game file */
  if(!load_rom(argv[1]))
  {
    char caption[256];
    sprintf(caption, "Error loading file `%s'.", argv[1]);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", caption, sdl_video.window);
    return 1;
  }

  /* initialize system hardware */
  audio_init(SOUND_FREQUENCY, 0);
  system_init();

  /* Mega CD specific */
  if (system_hw == SYSTEM_MCD)
  {
    /* load internal backup RAM */
    fp = fopen("./scd.brm", "rb");
    if (fp!=NULL)
    {
      fread(scd.bram, 0x2000, 1, fp);
      fclose(fp);
    }

    /* check if internal backup RAM is formatted */
    if (memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20))
    {
      /* clear internal backup RAM */
      memset(scd.bram, 0x00, 0x200);

      /* Internal Backup RAM size fields */
      brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = 0x00;
      brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (0x2000 / 64) - 3;

      /* format internal backup RAM */
      memcpy(scd.bram + 0x2000 - 0x40, brm_format, 0x40);
    }

    /* load cartridge backup RAM */
    if (scd.cartridge.id)
    {
      fp = fopen("./cart.brm", "rb");
      if (fp!=NULL)
      {
        fread(scd.cartridge.area, scd.cartridge.mask + 1, 1, fp);
        fclose(fp);
      }

      /* check if cartridge backup RAM is formatted */
      if (memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20))
      {
        /* clear cartridge backup RAM */
        memset(scd.cartridge.area, 0x00, scd.cartridge.mask + 1);

        /* Cartridge Backup RAM size fields */
        brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = (((scd.cartridge.mask + 1) / 64) - 3) >> 8;
        brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (((scd.cartridge.mask + 1) / 64) - 3) & 0xff;

        /* format cartridge backup RAM */
        memcpy(scd.cartridge.area + scd.cartridge.mask + 1 - sizeof(brm_format), brm_format, sizeof(brm_format));
      }
    }
  }

  if (sram.on)
  {
    /* load SRAM */
    fp = fopen("./game.srm", "rb");
    if (fp!=NULL)
    {
      fread(sram.sram,0x10000,1, fp);
      fclose(fp);
    }
  }

  /* reset system hardware */
  system_reset();

  if(use_sound) SDL_PauseAudio(0);

  /* 3 frames = 50 ms (60hz) or 60 ms (50hz) */
  if(sdl_sync.sem_sync)
    SDL_AddTimer(vdp_pal ? 60 : 50, sdl_sync_timer_callback, NULL);

  /* emulation loop */
  while(running)
  {
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_USEREVENT:
        {
          char caption[100];
          sprintf(caption,"Genesis Plus GX - %d fps - %s", event.user.code, (rominfo.international[0] != 0x20) ? rominfo.international : rominfo.domestic);
          SDL_SetWindowTitle(sdl_video.window, caption);
          break;
        }

        case SDL_QUIT:
        {
          running = 0;
          break;
        }

        case SDL_KEYDOWN:
        {
          running = sdl_control_update(event.key.keysym.sym);
          break;
        }
      }
    }

    sdl_video_update();
    sdl_sound_update(use_sound);

    if(!turbo_mode && sdl_sync.sem_sync && sdl_video.frames_rendered % 3 == 0)
    {
      SDL_SemWait(sdl_sync.sem_sync);
    }
  }

  if (system_hw == SYSTEM_MCD)
  {
    /* save internal backup RAM (if formatted) */
    if (!memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20))
    {
      fp = fopen("./scd.brm", "wb");
      if (fp!=NULL)
      {
        fwrite(scd.bram, 0x2000, 1, fp);
        fclose(fp);
      }
    }

    /* save cartridge backup RAM (if formatted) */
    if (scd.cartridge.id)
    {
      if (!memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20))
      {
        fp = fopen("./cart.brm", "wb");
        if (fp!=NULL)
        {
          fwrite(scd.cartridge.area, scd.cartridge.mask + 1, 1, fp);
          fclose(fp);
        }
      }
    }
  }

  if (sram.on)
  {
    /* save SRAM */
    fp = fopen("./game.srm", "wb");
    if (fp!=NULL)
    {
      fwrite(sram.sram,0x10000,1, fp);
      fclose(fp);
    }
  }

  audio_shutdown();

  sdl_video_close();
  sdl_sound_close();
  sdl_sync_close();
  SDL_Quit();

  return 0;
}

uint8_t* getWorkRamPtr()
{
  return work_ram;
}