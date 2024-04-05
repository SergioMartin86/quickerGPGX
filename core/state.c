#include <string.h>
#include "state.h"

// cart_hw/svp/svp.hc

// Special case, as svp is inside the cart.rom allocation

// cart_hw/svp/svp16.h

__thread ssp1601_t *ssp = NULL;
__thread unsigned short *PC;
__thread int g_cycles;

// cart_hw/areplay.c

__thread struct action_replay_t action_replay;

// cart_hw/eeprom_93c.c

__thread T_EEPROM_93C eeprom_93c;

// cart_hw/eeprom_i2c.c

__thread struct eeprom_i2c_t eeprom_i2c;

// cart_hw/eeprom_spi.c

__thread T_EEPROM_SPI spi_eeprom;

// cart_hw/ggenie.c

__thread struct ggenie_t ggenie;

// cart_hw/megasd.c

__thread T_MEGASD_HW megasd_hw;

// cart_hw/sram.c

__thread T_SRAM sram;

// cart_hw/sms_cart.h

romhw_t cart_rom;
romhw_t bios_rom;

// cd_hw/cdc.h

__thread void (*cdc_dma_w)(unsigned int length);  /* active DMA callback */
__thread void (*cdc_halted_dma_w)(unsigned int length);  /* halted DMA callback */

// cd_hw/cdd.c

#if defined(USE_LIBCHDR)
__thread chd_file *libCHDRfile;
#endif

__thread cdStream *cdTrackStreams[100];
__thread cdStream *cdTocStream;

// input_hw/activator.c

__thread struct activator_t activator[2];

// input_hw/gamepad.c

__thread struct gamepad_t gamepad[MAX_DEVICES];
__thread struct flipflop_t flipflop[2];
__thread uint8_t latch;

// input_hw/graphic_board.c

__thread struct graphic_board_t board;

// input_hw/input.c

__thread t_input input;
__thread int old_system[2] = {-1,-1};

// input_hw/lightgun.c

__thread struct lightgun_t lightgun;

// input_hw/mouse.c

__thread struct mouse_t mouse;

// input_hw/paddle.c

__thread struct paddle_t paddle[2];

// input_hw/sportspad.c

__thread struct sportspad_t sportspad[2];

// input_hw/teamplayer.c

__thread struct teamplayer_t teamplayer[2];

// input_hw/terebi_oekaki.c

__thread struct tablet_t tablet;

// input_hw/xe_1ap.c

__thread struct xe_1ap_t xe_1ap[2];

// m68k/m68k.c

__thread m68ki_cpu_core m68k;
__thread m68ki_cpu_core s68k;

// m68k/m68kcpu.c

__thread int m68k_irq_latency;

// m68k/s68kcpu.c

__thread int s68k_irq_latency;

// sound/psg.c

__thread struct psg_t psg;

// sound/sound.c

#if defined(HAVE_YM3438_CORE) || defined(HAVE_OPLL_CORE)
__thread int fm_buffer[1080 * 2 * 24]; // FM output buffer (large enough to hold a whole frame at original chips rate) 
#else
__thread int fm_buffer[1080 * 2];
#endif

__thread int fm_last[2];
__thread int *fm_ptr;
__thread int fm_cycles_ratio; // Cycle-accurate FM samples
__thread int fm_cycles_start;
__thread int fm_cycles_count;
__thread int fm_cycles_busy;

#ifdef HAVE_YM3438_CORE
__thread ym3438_t ym3438;
__thread short ym3438_accm[24][2];
__thread int ym3438_sample[2];
__thread int ym3438_cycles;
#endif

#ifdef HAVE_OPLL_CORE
__thread opll_t opll;
__thread int opll_accm[18][2];
__thread int opll_sample;
__thread int opll_cycles;
__thread int opll_status;
#endif

// sound/ym2413.h

__thread signed int output[2];
__thread uint32_t  LFO_AM;
__thread int32_t  LFO_PM;
__thread YM2413 ym2413; /* emulated chip */

// sound/ym2612.c

__thread YM2612 ym2612; /* emulated chip */
__thread int32_t  m2,c1,c2;   /* current chip state - Phase Modulation input for operators 2,3,4 */
__thread int32_t  mem;        /* one sample delay memory */
__thread int32_t  out_fm[6];  /* outputs of working channels */
__thread uint32_t op_mask[8][4];  /* operator output bitmasking (DAC quantization) */
__thread int chip_type = YM2612_DISCRETE; /* chip type */

// z80/z80.c

__thread Z80_Regs Z80;
__thread uint8_t z80_last_fetch;
__thread unsigned char *z80_readmap[64];
__thread unsigned char *z80_writemap[64];
__thread uint32_t EA;
__thread uint8_t SZ[256];       /* zero and sign flags */
__thread uint8_t SZ_BIT[256];   /* zero, sign and parity/overflow (=zero) flags for BIT opcode */
__thread uint8_t SZP[256];      /* zero, sign and parity flags */
__thread uint8_t SZHV_inc[256]; /* zero, sign, half carry and overflow flags INC r8 */
__thread uint8_t SZHV_dec[256]; /* zero, sign, half carry and overflow flags DEC r8 */
__thread uint8_t SZHVC_add[2*256*256]; /* flags for ADD opcode */
__thread uint8_t SZHVC_sub[2*256*256]; /* flags for SUB opcode */

#ifdef Z80_OVERCLOCK_SHIFT
__thread uint32_t z80_cycle_ratio;
#endif

// genesis.c

// Cartdrigde / CD information
#ifdef USE_DYNAMIC_ALLOC
__thread external_t *ext;
#else
__thread external_t ext;
#endif

__thread uint8_t boot_rom[0x800];
__thread uint8_t work_ram[0x10000];
__thread uint8_t zram[0x2000];
__thread uint32_t zbank;
__thread uint8_t zstate;
__thread uint8_t pico_current;
__thread uint8_t tmss[4];     // TMSS security register

// io_ctrl.c

__thread uint8_t io_reg[0x10];
__thread uint8_t region_code = REGION_USA;
__thread struct port_t port[3];

// load_rom.c

__thread ROMINFO rominfo;
__thread uint8_t romtype;
__thread uint8_t rom_region;

// membnk.c

__thread t_zbank_memory_map zbank_memory_map[256];

// system.c

__thread t_bitmap bitmap;
__thread t_snd snd;
__thread uint32_t mcycles_vdp;
__thread uint8_t system_hw;
__thread uint8_t system_bios;
__thread uint32_t system_clock;
__thread int16_t SVP_cycles = 800; 
__thread uint8_t pause_b;
__thread EQSTATE eq[2];
__thread int16_t llp,rrp;

// vdp.c

__thread uint8_t ALIGNED_(4) sat[0x400];     /* Internal copy of sprite attribute table */
__thread uint8_t ALIGNED_(4) vram[0x10000];  /* Video RAM (64K x 8-bit) */
__thread uint8_t ALIGNED_(4) cram[0x80];     /* On-chip color RAM (64 x 9-bit) */
__thread uint8_t ALIGNED_(4) vsram[0x80];    /* On-chip vertical scroll RAM (40 x 11-bit) */
__thread uint8_t reg[0x20];                  /* Internal VDP registers (23 x 8-bit) */
__thread uint8_t hint_pending;               /* 0= Line interrupt is pending */
__thread uint8_t vint_pending;               /* 1= Frame interrupt is pending */
__thread uint16_t status;                    /* VDP status flags */
__thread uint32_t dma_length;                /* DMA remaining length */
__thread uint32_t dma_endCycles;             /* DMA end cycle */
__thread uint8_t dma_type;                   /* DMA mode */
__thread uint16_t ntab;                      /* Name table A base address */
__thread uint16_t ntbb;                      /* Name table B base address */
__thread uint16_t ntwb;                      /* Name table W base address */
__thread uint16_t satb;                      /* Sprite attribute table base address */
__thread uint16_t hscb;                      /* Horizontal scroll table base address */
__thread uint8_t bg_name_dirty[0x800];       /* 1= This pattern is dirty */
__thread uint16_t bg_name_list[0x800];       /* List of modified pattern indices */
__thread uint16_t bg_list_index;             /* # of modified patterns in list */
__thread uint8_t hscroll_mask;               /* Horizontal Scrolling line mask */
__thread uint8_t playfield_shift;            /* Width of planes A, B (in bits) */
__thread uint8_t playfield_col_mask;         /* Playfield column mask */
__thread uint16_t playfield_row_mask;        /* Playfield row mask */
__thread uint16_t vscroll;                   /* Latched vertical scroll value */
__thread uint8_t odd_frame;                  /* 1: odd field, 0: even field */
__thread uint8_t im2_flag;                   /* 1= Interlace mode 2 is being used */
__thread uint8_t interlaced;                 /* 1: Interlaced mode 1 or 2 */
__thread uint8_t vdp_pal;                    /* 1: PAL , 0: NTSC (default) */
__thread uint8_t h_counter;                  /* Horizontal counter */
__thread uint16_t v_counter;                 /* Vertical counter */
__thread uint16_t vc_max;                    /* Vertical counter overflow value */
__thread uint16_t lines_per_frame;           /* PAL: 313 lines, NTSC: 262 lines */
__thread uint16_t max_sprite_pixels;         /* Max. sprites pixels per line (parsing & rendering) */
__thread uint32_t fifo_cycles[4];            /* VDP FIFO read-out cycles */
__thread uint32_t hvc_latch;                 /* latched HV counter */
__thread uint32_t vint_cycle;                /* VINT occurence cycle */
__thread const uint8_t *hctab;               /* pointer to H Counter table */

__thread uint8_t border;            /* Border color index */
__thread uint8_t pending;           /* Pending write flag */
__thread uint8_t code;              /* Code register */
__thread uint16_t addr;             /* Address register */
__thread uint16_t addr_latch;       /* Latched A15, A14 of address */
__thread uint16_t sat_base_mask;    /* Base bits of SAT */
__thread uint16_t sat_addr_mask;    /* Index bits of SAT */
__thread uint16_t dma_src;          /* DMA source address */
__thread int dmafill;             /* DMA Fill pending flag */
__thread int cached_write;        /* 2nd part of 32-bit CTRL port write (Genesis mode) or LSB of CRAM data (Game Gear mode) */
__thread uint16_t fifo[4];          /* FIFO ring-buffer */
__thread int fifo_idx;            /* FIFO write index */
__thread int fifo_byte_access;    /* FIFO byte access flag */
__thread int *fifo_timing;        /* FIFO slots timing table */
__thread int hblank_start_cycle;  /* HBLANK flag set cycle */
__thread int hblank_end_cycle;    /* HBLANK flag clear cycle */

// vdp_render.c

__thread struct clip_t clip[2];
__thread uint8_t ALIGNED_(4) bg_pattern_cache[0x80000]; /* Cached and flipped patterns */
__thread uint8_t name_lut[0x400]; /* Sprite pattern name offset look-up table (Mode 5) */
__thread uint32_t bp_lut[0x10000]; /* Bitplane to packed pixel look-up table (Mode 4) */
__thread uint8_t lut[LUT_MAX][LUT_SIZE]; /* Layer priority pixel look-up tables */
__thread PIXEL_OUT_T pixel[0x100]; /* Output pixel data look-up tables*/
__thread PIXEL_OUT_T pixel_lut[3][0x200];
__thread PIXEL_OUT_T pixel_lut_m4[0x40];
__thread uint8_t linebuf[2][0x200]; /* Background & Sprite line buffers */
__thread uint8_t spr_ovr; /* Sprite limit flag */
__thread object_info_t obj_info[2][MAX_SPRITES_PER_LINE];
__thread uint8_t object_count[2]; /* Sprite Counter */
__thread uint16_t spr_col; /* Sprite Collision Info */


size_t saveState(uint8_t* buffer)
{
  size_t pos = 0;
  
  // cart_hw/svp/svp16.h

  // // Special treatment for SVP, since it is stored inside the ROM that we otherwise do not store
  // if (svp != NULL)
  // {
  //  if (buffer != NULL) { memcpy(&buffer[pos], svp                  , sizeof(svp_t               )); } pos += sizeof(svp_t               );
  //  if (buffer != NULL) { memcpy(&buffer[pos], ssp                  , sizeof(ssp1601_t           )); } pos += sizeof(ssp1601_t           );

  //  // SSP PC, store as offset to svp->iram_rom
  //  int64_t SSPPC = GET_PC();
  //  if (buffer != NULL) { memcpy(&buffer[pos], &SSPPC               , sizeof(SSPPC               )); } pos += sizeof(SSPPC               );
  //  if (buffer != NULL) { memcpy(&buffer[pos], &g_cycles            , sizeof(g_cycles            )); } pos += sizeof(g_cycles            );
  // }

  // // cart_hw/areplay.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &action_replay       , sizeof(action_replay       )); } pos += sizeof(action_replay       );

  // // cart_hw/eeprom_93c.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &eeprom_93c          , sizeof(eeprom_93c          )); } pos += sizeof(eeprom_93c          );

  // // cart_hw/eeprom_i2c.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &eeprom_i2c          , sizeof(eeprom_i2c          )); } pos += sizeof(eeprom_i2c          );

  // // cart_hw/eeprom_spi.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &spi_eeprom          , sizeof(spi_eeprom          )); } pos += sizeof(spi_eeprom          );

  // // cart_hw/ggenie.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &ggenie              , sizeof(ggenie              )); } pos += sizeof(ggenie              );

  // // cart_hw/megasd.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &megasd_hw           , sizeof(megasd_hw           )); } pos += sizeof(megasd_hw           );

  // // cart_hw/sram.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &sram                , sizeof(sram                )); } pos += sizeof(sram                );

  // // cd_hw/cdd.c

  // if (system_hw == SYSTEM_MCD)
  // {
  //   #ifdef USE_DYNAMIC_ALLOC
  //    cd_hw_t* cdData = (cd_hw_t*)ext;
  //   #else
  //    cd_hw_t* cdData = (cd_hw_t*)&ext;
  //   #endif

  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->prg_ram            , sizeof(cdData->prg_ram            )); } pos += sizeof(cdData->prg_ram           );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->word_ram           , sizeof(cdData->word_ram           )); } pos += sizeof(cdData->word_ram          );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->word_ram_2M        , sizeof(cdData->word_ram_2M        )); } pos += sizeof(cdData->word_ram_2M       );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->bram               , sizeof(cdData->bram               )); } pos += sizeof(cdData->bram              );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->regs               , sizeof(cdData->regs               )); } pos += sizeof(cdData->regs              );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->cycles             , sizeof(cdData->cycles             )); } pos += sizeof(cdData->cycles            );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->cycles_per_line    , sizeof(cdData->cycles_per_line    )); } pos += sizeof(cdData->cycles_per_line   );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->stopwatch          , sizeof(cdData->stopwatch          )); } pos += sizeof(cdData->stopwatch         );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->timer              , sizeof(cdData->timer              )); } pos += sizeof(cdData->timer             );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->pending            , sizeof(cdData->pending            )); } pos += sizeof(cdData->pending           );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->dmna               , sizeof(cdData->dmna               )); } pos += sizeof(cdData->dmna              );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->type               , sizeof(cdData->type               )); } pos += sizeof(cdData->type              );
  //   // if (buffer != NULL) { memcpy(&buffer[pos], &cdData->gfx_hw             , sizeof(cdData->gfx_hw             )); } pos += sizeof(cdData->gfx_hw            );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->cdc_hw             , sizeof(cdData->cdc_hw             )); } pos += sizeof(cdData->cdc_hw            );
  //   if (buffer != NULL) { memcpy(&buffer[pos], &cdData->cdd_hw             , sizeof(cdData->cdd_hw             )); } pos += sizeof(cdData->cdd_hw            );
  //   // if (buffer != NULL) { memcpy(&buffer[pos], &cdData->pcm_hw             , sizeof(cdData->pcm_hw             )); } pos += sizeof(cdData->pcm_hw            );       
  // }

  // input_hw/activator.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &activator           , sizeof(activator           )); } pos += sizeof(activator           );

  // input_hw/gamepad.c
  
  if (buffer != NULL) { memcpy(&buffer[pos], &gamepad             , sizeof(gamepad             )); } pos += sizeof(gamepad             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &flipflop            , sizeof(flipflop            )); } pos += sizeof(flipflop            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &latch               , sizeof(latch               )); } pos += sizeof(latch               );

  // // input_hw/graphic_board.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &board               , sizeof(board               )); } pos += sizeof(board               );

  // // input_hw/input.c
  
  // if (buffer != NULL) { memcpy(&buffer[pos], &input               , sizeof(input               )); } pos += sizeof(input               );
  // if (buffer != NULL) { memcpy(&buffer[pos], &old_system          , sizeof(old_system          )); } pos += sizeof(old_system          );

  // // input_hw/lightgun.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &lightgun            , sizeof(lightgun            )); } pos += sizeof(lightgun            );

  // // input_hw/mouse.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &mouse               , sizeof(mouse               )); } pos += sizeof(mouse               );

  // // input_hw/paddle.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &paddle              , sizeof(paddle              )); } pos += sizeof(paddle              );

  // // input_hw/sportspad.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &sportspad           , sizeof(sportspad           )); } pos += sizeof(sportspad           );

  // // input_hw/teamplayer.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &teamplayer          , sizeof(teamplayer          )); } pos += sizeof(teamplayer          );
  
  // // input_hw/terebi_oekaki.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &tablet              , sizeof(tablet              )); } pos += sizeof(tablet              );

  // // input_hw/xe_1ap.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &xe_1ap              , sizeof(xe_1ap              )); } pos += sizeof(xe_1ap              );

  // m68k/m68k.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &m68k                , sizeof(m68k                )); } pos += sizeof(m68k                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &s68k                , sizeof(s68k                )); } pos += sizeof(s68k                );

  // // m68k/m68kcpu.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &m68k_irq_latency    , sizeof(m68k_irq_latency    )); } pos += sizeof(m68k_irq_latency    );

  // // m68k/s68kcpu.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &s68k_irq_latency    , sizeof(s68k_irq_latency    )); } pos += sizeof(s68k_irq_latency    );

  // // sound/psg.c

  // if (buffer != NULL) { memcpy(&buffer[pos], & psg                , sizeof( psg                )); } pos += sizeof( psg                );
  
  // // sound/sound.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &fm_buffer           , sizeof(fm_buffer           )); } pos += sizeof(fm_buffer           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fm_last             , sizeof(fm_last             )); } pos += sizeof(fm_last             );
  // //if (buffer != NULL) { memcpy(&buffer[pos], fm_ptr               , sizeof(fm_ptr             )); } pos += sizeof(*fm_ptr             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fm_cycles_ratio     , sizeof(fm_cycles_ratio     )); } pos += sizeof(fm_cycles_ratio     );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fm_cycles_start     , sizeof(fm_cycles_start     )); } pos += sizeof(fm_cycles_start     );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fm_cycles_count     , sizeof(fm_cycles_count     )); } pos += sizeof(fm_cycles_count     );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fm_cycles_busy      , sizeof(fm_cycles_busy      )); } pos += sizeof(fm_cycles_busy      );

  // #ifdef HAVE_YM3438_CORE
  // if (buffer != NULL) { memcpy(&buffer[pos], &ym3438              , sizeof(ym3438              )); } pos += sizeof(ym3438              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ym3438_accm         , sizeof(ym3438_accm         )); } pos += sizeof(ym3438_accm         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ym3438_sample       , sizeof(ym3438_sample       )); } pos += sizeof(ym3438_sample       );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ym3438_cycles       , sizeof(ym3438_cycles       )); } pos += sizeof(ym3438_cycles       );
  // #endif

  // #ifdef HAVE_OPLL_CORE
  // if (buffer != NULL) { memcpy(&buffer[pos], &opll                , sizeof(opll                )); } pos += sizeof(opll                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &opll_accm           , sizeof(opll_accm           )); } pos += sizeof(opll_accm           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &opll_sample         , sizeof(opll_sample         )); } pos += sizeof(opll_sample         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &opll_cycles         , sizeof(opll_cycles         )); } pos += sizeof(opll_cycles         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &opll_status         , sizeof(opll_status         )); } pos += sizeof(opll_status         );
  // #endif

  // // sound/ym2413.h

  // if (buffer != NULL) { memcpy(&buffer[pos], &output              , sizeof(output              )); } pos += sizeof(output              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &LFO_AM              , sizeof(LFO_AM              )); } pos += sizeof(LFO_AM              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &LFO_PM              , sizeof(LFO_PM              )); } pos += sizeof(LFO_PM              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ym2413              , sizeof(ym2413              )); } pos += sizeof(ym2413              );

  // // sound/ym2612.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &ym2612              , sizeof(ym2612              )); } pos += sizeof(ym2612              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &m2                  , sizeof(m2                  )); } pos += sizeof(m2                  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &c1                  , sizeof(c1                  )); } pos += sizeof(c1                  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &c2                  , sizeof(c2                  )); } pos += sizeof(c2                  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &mem                 , sizeof(mem                 )); } pos += sizeof(mem                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &out_fm              , sizeof(out_fm              )); } pos += sizeof(out_fm              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &op_mask             , sizeof(op_mask             )); } pos += sizeof(op_mask             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &chip_type           , sizeof(chip_type           )); } pos += sizeof(chip_type           );

  // // z80/z80.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &Z80                 , sizeof(Z80                 )); } pos += sizeof(Z80                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &z80_last_fetch      , sizeof(z80_last_fetch      )); } pos += sizeof(z80_last_fetch      );
  // if (buffer != NULL) { memcpy(&buffer[pos], &z80_readmap         , sizeof(z80_readmap         )); } pos += sizeof(z80_readmap         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &z80_writemap        , sizeof(z80_writemap        )); } pos += sizeof(z80_writemap        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &EA                  , sizeof(EA                  )); } pos += sizeof(EA                  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZ                  , sizeof(SZ                  )); } pos += sizeof(SZ                  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZ_BIT              , sizeof(SZ_BIT              )); } pos += sizeof(SZ_BIT              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZP                 , sizeof(SZP                 )); } pos += sizeof(SZP                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZHV_inc            , sizeof(SZHV_inc            )); } pos += sizeof(SZHV_inc            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZHV_dec            , sizeof(SZHV_dec            )); } pos += sizeof(SZHV_dec            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZHVC_add           , sizeof(SZHVC_add           )); } pos += sizeof(SZHVC_add           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SZHVC_sub           , sizeof(SZHVC_sub           )); } pos += sizeof(SZHVC_sub           );
  
  // #ifdef Z80_OVERCLOCK_SHIFT
  // if (buffer != NULL) { memcpy(&buffer[pos], &z80_cycle_ratio     , sizeof(z80_cycle_ratio     )); } pos += sizeof(z80_cycle_ratio     );
  // #endif

  // // genesis.c

  // // if (buffer != NULL) { memcpy(&buffer[pos], &boot_rom            , sizeof(boot_rom            )); } pos += sizeof(boot_rom            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &work_ram            , sizeof(work_ram            )); } pos += sizeof(work_ram            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &zram                , sizeof(zram                )); } pos += sizeof(zram                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &zbank               , sizeof(zbank               )); } pos += sizeof(zbank               );
  // if (buffer != NULL) { memcpy(&buffer[pos], &zstate              , sizeof(zstate              )); } pos += sizeof(zstate              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &pico_current        , sizeof(pico_current        )); } pos += sizeof(pico_current        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &tmss                , sizeof(tmss                )); } pos += sizeof(tmss                );

  // // io_ctrl.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &io_reg              , sizeof(io_reg              )); } pos += sizeof(io_reg              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &region_code         , sizeof(region_code         )); } pos += sizeof(region_code         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &port                , sizeof(port                )); } pos += sizeof(port                );

  // // load_rom.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &rominfo             , sizeof(rominfo             )); } pos += sizeof(rominfo             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &romtype             , sizeof(romtype             )); } pos += sizeof(romtype             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &rom_region          , sizeof(rom_region          )); } pos += sizeof(rom_region          );

  // // membnk.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &zbank_memory_map    , sizeof(zbank_memory_map    )); } pos += sizeof(zbank_memory_map    );

  // system.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &bitmap              , sizeof(bitmap              )); } pos += sizeof(bitmap              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &snd                 , sizeof(snd                 )); } pos += sizeof(snd                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &mcycles_vdp         , sizeof(mcycles_vdp         )); } pos += sizeof(mcycles_vdp         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &system_hw           , sizeof(system_hw           )); } pos += sizeof(system_hw           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &system_bios         , sizeof(system_bios         )); } pos += sizeof(system_bios         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &system_clock        , sizeof(system_clock        )); } pos += sizeof(system_clock        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &SVP_cycles          , sizeof(SVP_cycles          )); } pos += sizeof(SVP_cycles          );
  if (buffer != NULL) { memcpy(&buffer[pos], &pause_b             , sizeof(pause_b             )); } pos += sizeof(pause_b             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &eq                  , sizeof(eq                  )); } pos += sizeof(eq                  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &llp                 , sizeof(llp                 )); } pos += sizeof(llp                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &rrp                 , sizeof(rrp                 )); } pos += sizeof(rrp                 );

  // // vdp.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &sat                 , sizeof(sat                 )); } pos += sizeof(sat                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vram                , sizeof(vram                )); } pos += sizeof(vram                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &cram                , sizeof(cram                )); } pos += sizeof(cram                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vsram               , sizeof(vsram               )); } pos += sizeof(vsram               );
  // if (buffer != NULL) { memcpy(&buffer[pos], &reg                 , sizeof(reg                 )); } pos += sizeof(reg                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &hint_pending        , sizeof(hint_pending        )); } pos += sizeof(hint_pending        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vint_pending        , sizeof(vint_pending        )); } pos += sizeof(vint_pending        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &status              , sizeof(status              )); } pos += sizeof(status              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &dma_length          , sizeof(dma_length          )); } pos += sizeof(dma_length          );
  // if (buffer != NULL) { memcpy(&buffer[pos], &dma_endCycles       , sizeof(dma_endCycles       )); } pos += sizeof(dma_endCycles       );
  // if (buffer != NULL) { memcpy(&buffer[pos], &dma_type            , sizeof(dma_type            )); } pos += sizeof(dma_type            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ntab                , sizeof(ntab                )); } pos += sizeof(ntab                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ntbb                , sizeof(ntbb                )); } pos += sizeof(ntbb                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &ntwb                , sizeof(ntwb                )); } pos += sizeof(ntwb                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &satb                , sizeof(satb                )); } pos += sizeof(satb                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &hscb                , sizeof(hscb                )); } pos += sizeof(hscb                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &bg_name_dirty       , sizeof(bg_name_dirty       )); } pos += sizeof(bg_name_dirty       );
  // if (buffer != NULL) { memcpy(&buffer[pos], &bg_name_list        , sizeof(bg_name_list        )); } pos += sizeof(bg_name_list        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &bg_list_index       , sizeof(bg_list_index       )); } pos += sizeof(bg_list_index       );
  // if (buffer != NULL) { memcpy(&buffer[pos], &hscroll_mask        , sizeof(hscroll_mask        )); } pos += sizeof(hscroll_mask        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &playfield_shift     , sizeof(playfield_shift     )); } pos += sizeof(playfield_shift     );
  // if (buffer != NULL) { memcpy(&buffer[pos], &playfield_col_mask  , sizeof(playfield_col_mask  )); } pos += sizeof(playfield_col_mask  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &playfield_row_mask  , sizeof(playfield_row_mask  )); } pos += sizeof(playfield_row_mask  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vscroll             , sizeof(vscroll             )); } pos += sizeof(vscroll             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &odd_frame           , sizeof(odd_frame           )); } pos += sizeof(odd_frame           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &im2_flag            , sizeof(im2_flag            )); } pos += sizeof(im2_flag            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &interlaced          , sizeof(interlaced          )); } pos += sizeof(interlaced          );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vdp_pal             , sizeof(vdp_pal             )); } pos += sizeof(vdp_pal             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &h_counter           , sizeof(h_counter           )); } pos += sizeof(h_counter           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &v_counter           , sizeof(v_counter           )); } pos += sizeof(v_counter           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vc_max              , sizeof(vc_max              )); } pos += sizeof(vc_max              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &lines_per_frame     , sizeof(lines_per_frame     )); } pos += sizeof(lines_per_frame     );
  // if (buffer != NULL) { memcpy(&buffer[pos], &max_sprite_pixels   , sizeof(max_sprite_pixels   )); } pos += sizeof(max_sprite_pixels   );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fifo_cycles         , sizeof(fifo_cycles         )); } pos += sizeof(fifo_cycles         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &hvc_latch           , sizeof(hvc_latch           )); } pos += sizeof(hvc_latch           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &vint_cycle          , sizeof(vint_cycle          )); } pos += sizeof(vint_cycle          );
  // // if (buffer != NULL) { memcpy(&buffer[pos], &hctab               , sizeof(hctab               )); } pos += sizeof(hctab               );
  // if (buffer != NULL) { memcpy(&buffer[pos], &border              , sizeof(border              )); } pos += sizeof(border              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &pending             , sizeof(pending             )); } pos += sizeof(pending             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &code                , sizeof(code                )); } pos += sizeof(code                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &addr                , sizeof(addr                )); } pos += sizeof(addr                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &addr_latch          , sizeof(addr_latch          )); } pos += sizeof(addr_latch          );
  // if (buffer != NULL) { memcpy(&buffer[pos], &sat_base_mask       , sizeof(sat_base_mask       )); } pos += sizeof(sat_base_mask       );
  // if (buffer != NULL) { memcpy(&buffer[pos], &sat_addr_mask       , sizeof(sat_addr_mask       )); } pos += sizeof(sat_addr_mask       );
  // if (buffer != NULL) { memcpy(&buffer[pos], &dma_src             , sizeof(dma_src             )); } pos += sizeof(dma_src             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &dmafill             , sizeof(dmafill             )); } pos += sizeof(dmafill             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &cached_write        , sizeof(cached_write        )); } pos += sizeof(cached_write        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fifo                , sizeof(fifo                )); } pos += sizeof(fifo                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fifo_idx            , sizeof(fifo_idx            )); } pos += sizeof(fifo_idx            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &fifo_byte_access    , sizeof(fifo_byte_access    )); } pos += sizeof(fifo_byte_access    );
  // // if (buffer != NULL) { memcpy(&buffer[pos], &fifo_timing         , sizeof(fifo_timing         )); } pos += sizeof(fifo_timing         );
  // if (buffer != NULL) { memcpy(&buffer[pos], &hblank_start_cycle  , sizeof(hblank_start_cycle  )); } pos += sizeof(hblank_start_cycle  );
  // if (buffer != NULL) { memcpy(&buffer[pos], &hblank_end_cycle    , sizeof(hblank_end_cycle    )); } pos += sizeof(hblank_end_cycle    );

  // // vdp_render.c

  // if (buffer != NULL) { memcpy(&buffer[pos], &clip                , sizeof(clip                )); } pos += sizeof(clip                );
  // if (buffer != NULL) { memcpy(&buffer[pos], &bg_pattern_cache    , sizeof(bg_pattern_cache    )); } pos += sizeof(bg_pattern_cache    );
  // if (buffer != NULL) { memcpy(&buffer[pos], &name_lut            , sizeof(name_lut            )); } pos += sizeof(name_lut            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &bp_lut              , sizeof(bp_lut              )); } pos += sizeof(bp_lut              );
  // if (buffer != NULL) { memcpy(&buffer[pos], &lut                 , sizeof(lut                 )); } pos += sizeof(lut                 );
  // if (buffer != NULL) { memcpy(&buffer[pos], &pixel               , sizeof(pixel               )); } pos += sizeof(pixel               );
  // if (buffer != NULL) { memcpy(&buffer[pos], &pixel_lut           , sizeof(pixel_lut           )); } pos += sizeof(pixel_lut           );
  // if (buffer != NULL) { memcpy(&buffer[pos], &pixel_lut_m4        , sizeof(pixel_lut_m4        )); } pos += sizeof(pixel_lut_m4        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &linebuf             , sizeof(linebuf             )); } pos += sizeof(linebuf             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &spr_ovr             , sizeof(spr_ovr             )); } pos += sizeof(spr_ovr             );
  // if (buffer != NULL) { memcpy(&buffer[pos], &obj_info            , sizeof(obj_info            )); } pos += sizeof(obj_info            );
  // if (buffer != NULL) { memcpy(&buffer[pos], &object_count        , sizeof(object_count        )); } pos += sizeof(object_count        );
  // if (buffer != NULL) { memcpy(&buffer[pos], &spr_col             , sizeof(spr_col             )); } pos += sizeof(spr_col             );

  return pos;
}

void loadState(const uint8_t* buffer)
{
  size_t pos = 0;

  // // cart_hw/svp/svp16.h

  // if (svp != NULL)
  // {
  //  // Special treatment for SVP, since it is stored inside the ROM that we otherwise do not store
  //  if (buffer != NULL) { memcpy(svp                  , &buffer[pos],  sizeof(svp_t               )); } pos += sizeof(svp_t               );
  //  if (buffer != NULL) { memcpy(ssp                  , &buffer[pos],  sizeof(ssp1601_t           )); } pos += sizeof(ssp1601_t           );

  //  // SSP PC, store as offset to svp->iram_rom
  //  int64_t SSPPC = 0;
  //  if (buffer != NULL) { memcpy(&SSPPC               , &buffer[pos],  sizeof(SSPPC              )); } pos += sizeof(SSPPC               );
  //  SET_PC(SSPPC);

  //  if (buffer != NULL) { memcpy(&g_cycles            , &buffer[pos],  sizeof(g_cycles            )); } pos += sizeof(g_cycles            );
  // }
  
  // // cart_hw/areplay.c

  // if (buffer != NULL) { memcpy(&action_replay       , &buffer[pos],  sizeof(action_replay       )); } pos += sizeof(action_replay       );

  // // cart_hw/eeprom_93c.c

  // if (buffer != NULL) { memcpy(&eeprom_93c          , &buffer[pos],  sizeof(eeprom_93c          )); } pos += sizeof(eeprom_93c          );

  // // cart_hw/eeprom_i2c.c

  // if (buffer != NULL) { memcpy(&eeprom_i2c          , &buffer[pos],  sizeof(eeprom_i2c          )); } pos += sizeof(eeprom_i2c          );

  // // cart_hw/eeprom_spi.c

  // if (buffer != NULL) { memcpy(&spi_eeprom          , &buffer[pos],  sizeof(spi_eeprom          )); } pos += sizeof(spi_eeprom          );

  // // cart_hw/ggenie.c

  // if (buffer != NULL) { memcpy(&ggenie              , &buffer[pos],  sizeof(ggenie              )); } pos += sizeof(ggenie              );

  // // cart_hw/megasd.c

  // if (buffer != NULL) { memcpy(&megasd_hw           , &buffer[pos],  sizeof(megasd_hw           )); } pos += sizeof(megasd_hw           );

  // // cart_hw/sram.c

  // if (buffer != NULL) { memcpy(&sram                , &buffer[pos],  sizeof(sram                )); } pos += sizeof(sram                );

  // // cd_hw/cdd.c

  //   if (system_hw == SYSTEM_MCD)
  // {
  //   #ifdef USE_DYNAMIC_ALLOC
  //    cd_hw_t* cdData = (cd_hw_t*)ext;
  //   #else
  //    cd_hw_t* cdData = (cd_hw_t*)&ext;
  //   #endif

  //   if (buffer != NULL) { memcpy(&cdData->prg_ram            , &buffer[pos],  sizeof(cdData->prg_ram            )); } pos += sizeof(cdData->prg_ram           );
  //   if (buffer != NULL) { memcpy(&cdData->word_ram           , &buffer[pos],  sizeof(cdData->word_ram           )); } pos += sizeof(cdData->word_ram          );
  //   if (buffer != NULL) { memcpy(&cdData->word_ram_2M        , &buffer[pos],  sizeof(cdData->word_ram_2M        )); } pos += sizeof(cdData->word_ram_2M       );
  //   if (buffer != NULL) { memcpy(&cdData->bram               , &buffer[pos],  sizeof(cdData->bram               )); } pos += sizeof(cdData->bram              );
  //   if (buffer != NULL) { memcpy(&cdData->regs               , &buffer[pos],  sizeof(cdData->regs               )); } pos += sizeof(cdData->regs              );
  //   if (buffer != NULL) { memcpy(&cdData->cycles             , &buffer[pos],  sizeof(cdData->cycles             )); } pos += sizeof(cdData->cycles            );
  //   if (buffer != NULL) { memcpy(&cdData->cycles_per_line    , &buffer[pos],  sizeof(cdData->cycles_per_line    )); } pos += sizeof(cdData->cycles_per_line   );
  //   if (buffer != NULL) { memcpy(&cdData->stopwatch          , &buffer[pos],  sizeof(cdData->stopwatch          )); } pos += sizeof(cdData->stopwatch         );
  //   if (buffer != NULL) { memcpy(&cdData->timer              , &buffer[pos],  sizeof(cdData->timer              )); } pos += sizeof(cdData->timer             );
  //   if (buffer != NULL) { memcpy(&cdData->pending            , &buffer[pos],  sizeof(cdData->pending            )); } pos += sizeof(cdData->pending           );
  //   if (buffer != NULL) { memcpy(&cdData->dmna               , &buffer[pos],  sizeof(cdData->dmna               )); } pos += sizeof(cdData->dmna              );
  //   if (buffer != NULL) { memcpy(&cdData->type               , &buffer[pos],  sizeof(cdData->type               )); } pos += sizeof(cdData->type              );
  //   // if (buffer != NULL) { memcpy(&cdData->gfx_hw             , &buffer[pos],  sizeof(cdData->gfx_hw             )); } pos += sizeof(cdData->gfx_hw            );
  //   if (buffer != NULL) { memcpy(&cdData->cdc_hw             , &buffer[pos],  sizeof(cdData->cdc_hw             )); } pos += sizeof(cdData->cdc_hw            );
  //   if (buffer != NULL) { memcpy(&cdData->cdd_hw             , &buffer[pos],  sizeof(cdData->cdd_hw             )); } pos += sizeof(cdData->cdd_hw            );
  //   // if (buffer != NULL) { memcpy(&cdData->pcm_hw             , &buffer[pos],  sizeof(cdData->pcm_hw             )); } pos += sizeof(cdData->pcm_hw            );       
  // }

  // // input_hw/activator.c

  // if (buffer != NULL) { memcpy(&activator           , &buffer[pos],  sizeof(activator           )); } pos += sizeof(activator           );

  // input_hw/gamepad.c

  if (buffer != NULL) { memcpy(&gamepad             , &buffer[pos],  sizeof(gamepad             )); } pos += sizeof(gamepad             );
  // if (buffer != NULL) { memcpy(&flipflop            , &buffer[pos],  sizeof(flipflop            )); } pos += sizeof(flipflop            );
  // if (buffer != NULL) { memcpy(&latch               , &buffer[pos],  sizeof(latch               )); } pos += sizeof(latch               );

  // // input_hw/graphic_board.c

  // if (buffer != NULL) { memcpy(&board               , &buffer[pos],  sizeof(board               )); } pos += sizeof(board               );

  // // input_hw/input.c

  // if (buffer != NULL) { memcpy(&input               , &buffer[pos],  sizeof(input               )); } pos += sizeof(input               );
  // if (buffer != NULL) { memcpy(&old_system          , &buffer[pos],  sizeof(old_system          )); } pos += sizeof(old_system          );

  // // input_hw/lightgun.c
  
  // if (buffer != NULL) { memcpy(&lightgun            , &buffer[pos],  sizeof(lightgun            )); } pos += sizeof(lightgun            );

  // // input_hw/mouse.c

  // if (buffer != NULL) { memcpy(&mouse               , &buffer[pos],  sizeof(mouse               )); } pos += sizeof(mouse               );

  // // input_hw/paddle.c

  // if (buffer != NULL) { memcpy(&paddle              , &buffer[pos],  sizeof(paddle              )); } pos += sizeof(paddle              );

  // // input_hw/sportspad.c

  // if (buffer != NULL) { memcpy(&sportspad           , &buffer[pos],  sizeof(sportspad           )); } pos += sizeof(sportspad           );

  // // input_hw/teamplayer.c

  // if (buffer != NULL) { memcpy(&teamplayer          , &buffer[pos],  sizeof(teamplayer          )); } pos += sizeof(teamplayer          );

  // // input_hw/terebi_oekaki.c

  // if (buffer != NULL) { memcpy(&tablet              , &buffer[pos],  sizeof(tablet              )); } pos += sizeof(tablet              );

  // // input_hw/xe_1ap.c

  // if (buffer != NULL) { memcpy(&xe_1ap              , &buffer[pos],  sizeof(xe_1ap              )); } pos += sizeof(xe_1ap              );

  // m68k/m68k.c

  // if (buffer != NULL) { memcpy(&m68k                , &buffer[pos],  sizeof(m68k                )); } pos += sizeof(m68k                );
  // if (buffer != NULL) { memcpy(&s68k                , &buffer[pos],  sizeof(s68k                )); } pos += sizeof(s68k                );

  // // m68k/m68kcpu.c

  // if (buffer != NULL) { memcpy(&m68k_irq_latency    , &buffer[pos],  sizeof(m68k_irq_latency    )); } pos += sizeof(m68k_irq_latency    );

  // // m68k/s68kcpu.c

  // if (buffer != NULL) { memcpy(&s68k_irq_latency    , &buffer[pos],  sizeof(s68k_irq_latency    )); } pos += sizeof(s68k_irq_latency    );

  // // sound/psg.c

  // if (buffer != NULL) { memcpy(& psg                , &buffer[pos],  sizeof( psg                )); } pos += sizeof( psg                );

  // // sound/sound.c

  // if (buffer != NULL) { memcpy(&fm_buffer           , &buffer[pos],  sizeof(fm_buffer           )); } pos += sizeof(fm_buffer           );
  // if (buffer != NULL) { memcpy(&fm_last             , &buffer[pos],  sizeof(fm_last             )); } pos += sizeof(fm_last             );
  // //if (buffer != NULL) { memcpy(&*fm_ptr             , &buffer[pos],  sizeof(*fm_ptr             )); } pos += sizeof(*fm_ptr             );
  // if (buffer != NULL) { memcpy(&fm_cycles_ratio     , &buffer[pos],  sizeof(fm_cycles_ratio     )); } pos += sizeof(fm_cycles_ratio     );
  // if (buffer != NULL) { memcpy(&fm_cycles_start     , &buffer[pos],  sizeof(fm_cycles_start     )); } pos += sizeof(fm_cycles_start     );
  // if (buffer != NULL) { memcpy(&fm_cycles_count     , &buffer[pos],  sizeof(fm_cycles_count     )); } pos += sizeof(fm_cycles_count     );
  // if (buffer != NULL) { memcpy(&fm_cycles_busy      , &buffer[pos],  sizeof(fm_cycles_busy      )); } pos += sizeof(fm_cycles_busy      );

  // #ifdef HAVE_YM3438_CORE
  // if (buffer != NULL) { memcpy(&ym3438              , &buffer[pos],  sizeof(ym3438              )); } pos += sizeof(ym3438              );
  // if (buffer != NULL) { memcpy(&ym3438_accm         , &buffer[pos],  sizeof(ym3438_accm         )); } pos += sizeof(ym3438_accm         );
  // if (buffer != NULL) { memcpy(&ym3438_sample       , &buffer[pos],  sizeof(ym3438_sample       )); } pos += sizeof(ym3438_sample       );
  // if (buffer != NULL) { memcpy(&ym3438_cycles       , &buffer[pos],  sizeof(ym3438_cycles       )); } pos += sizeof(ym3438_cycles       );
  // #endif

  // #ifdef HAVE_OPLL_CORE
  // if (buffer != NULL) { memcpy(&opll                , &buffer[pos],  sizeof(opll                )); } pos += sizeof(opll                );
  // if (buffer != NULL) { memcpy(&opll_accm           , &buffer[pos],  sizeof(opll_accm           )); } pos += sizeof(opll_accm           );
  // if (buffer != NULL) { memcpy(&opll_sample         , &buffer[pos],  sizeof(opll_sample         )); } pos += sizeof(opll_sample         );
  // if (buffer != NULL) { memcpy(&opll_cycles         , &buffer[pos],  sizeof(opll_cycles         )); } pos += sizeof(opll_cycles         );
  // if (buffer != NULL) { memcpy(&opll_status         , &buffer[pos],  sizeof(opll_status         )); } pos += sizeof(opll_status         );
  // #endif

  // // sound/ym2413.h

  // if (buffer != NULL) { memcpy(&output              , &buffer[pos],  sizeof(output              )); } pos += sizeof(output              );
  // if (buffer != NULL) { memcpy(&LFO_AM              , &buffer[pos],  sizeof(LFO_AM              )); } pos += sizeof(LFO_AM              );
  // if (buffer != NULL) { memcpy(&LFO_PM              , &buffer[pos],  sizeof(LFO_PM              )); } pos += sizeof(LFO_PM              );
  // if (buffer != NULL) { memcpy(&ym2413              , &buffer[pos],  sizeof(ym2413              )); } pos += sizeof(ym2413              );

  // // sound/ym2612.c

  // if (buffer != NULL) { memcpy(&ym2612              , &buffer[pos],  sizeof(ym2612              )); } pos += sizeof(ym2612              );
  // if (buffer != NULL) { memcpy(&m2                  , &buffer[pos],  sizeof(m2                  )); } pos += sizeof(m2                  );
  // if (buffer != NULL) { memcpy(&c1                  , &buffer[pos],  sizeof(c1                  )); } pos += sizeof(c1                  );
  // if (buffer != NULL) { memcpy(&c2                  , &buffer[pos],  sizeof(c2                  )); } pos += sizeof(c2                  );
  // if (buffer != NULL) { memcpy(&mem                 , &buffer[pos],  sizeof(mem                 )); } pos += sizeof(mem                 );
  // if (buffer != NULL) { memcpy(&out_fm              , &buffer[pos],  sizeof(out_fm              )); } pos += sizeof(out_fm              );
  // if (buffer != NULL) { memcpy(&op_mask             , &buffer[pos],  sizeof(op_mask             )); } pos += sizeof(op_mask             );
  // if (buffer != NULL) { memcpy(&chip_type           , &buffer[pos],  sizeof(chip_type           )); } pos += sizeof(chip_type           );

  // // z80/z80.c

  // if (buffer != NULL) { memcpy(&Z80                 , &buffer[pos],  sizeof(Z80                 )); } pos += sizeof(Z80                 );
  // if (buffer != NULL) { memcpy(&z80_last_fetch      , &buffer[pos],  sizeof(z80_last_fetch      )); } pos += sizeof(z80_last_fetch      );
  // if (buffer != NULL) { memcpy(&z80_readmap         , &buffer[pos],  sizeof(z80_readmap         )); } pos += sizeof(z80_readmap         );
  // if (buffer != NULL) { memcpy(&z80_writemap        , &buffer[pos],  sizeof(z80_writemap        )); } pos += sizeof(z80_writemap        );
  // if (buffer != NULL) { memcpy(&EA                  , &buffer[pos],  sizeof(EA                  )); } pos += sizeof(EA                  );
  // if (buffer != NULL) { memcpy(&SZ                  , &buffer[pos],  sizeof(SZ                  )); } pos += sizeof(SZ                  );
  // if (buffer != NULL) { memcpy(&SZ_BIT              , &buffer[pos],  sizeof(SZ_BIT              )); } pos += sizeof(SZ_BIT              );
  // if (buffer != NULL) { memcpy(&SZP                 , &buffer[pos],  sizeof(SZP                 )); } pos += sizeof(SZP                 );
  // if (buffer != NULL) { memcpy(&SZHV_inc            , &buffer[pos],  sizeof(SZHV_inc            )); } pos += sizeof(SZHV_inc            );
  // if (buffer != NULL) { memcpy(&SZHV_dec            , &buffer[pos],  sizeof(SZHV_dec            )); } pos += sizeof(SZHV_dec            );
  // if (buffer != NULL) { memcpy(&SZHVC_add           , &buffer[pos],  sizeof(SZHVC_add           )); } pos += sizeof(SZHVC_add           );
  // if (buffer != NULL) { memcpy(&SZHVC_sub           , &buffer[pos],  sizeof(SZHVC_sub           )); } pos += sizeof(SZHVC_sub           );

  // #ifdef Z80_OVERCLOCK_SHIFT
  // if (buffer != NULL) { memcpy(&z80_cycle_ratio     , &buffer[pos],  sizeof(z80_cycle_ratio     )); } pos += sizeof(z80_cycle_ratio     );
  // #endif

  // // genesis.c

  // // if (buffer != NULL) { memcpy(&boot_rom            , &buffer[pos],  sizeof(boot_rom            )); } pos += sizeof(boot_rom            );
  // if (buffer != NULL) { memcpy(&work_ram            , &buffer[pos],  sizeof(work_ram            )); } pos += sizeof(work_ram            );
  // if (buffer != NULL) { memcpy(&zram                , &buffer[pos],  sizeof(zram                )); } pos += sizeof(zram                );
  // if (buffer != NULL) { memcpy(&zbank               , &buffer[pos],  sizeof(zbank               )); } pos += sizeof(zbank               );
  // if (buffer != NULL) { memcpy(&zstate              , &buffer[pos],  sizeof(zstate              )); } pos += sizeof(zstate              );
  // if (buffer != NULL) { memcpy(&pico_current        , &buffer[pos],  sizeof(pico_current        )); } pos += sizeof(pico_current        );
  // if (buffer != NULL) { memcpy(&tmss                , &buffer[pos],  sizeof(tmss                )); } pos += sizeof(tmss                );

  // // io_ctrl.c

  // if (buffer != NULL) { memcpy(&io_reg              , &buffer[pos],  sizeof(io_reg              )); } pos += sizeof(io_reg              );
  // if (buffer != NULL) { memcpy(&region_code         , &buffer[pos],  sizeof(region_code         )); } pos += sizeof(region_code         );
  // if (buffer != NULL) { memcpy(&port                , &buffer[pos],  sizeof(port                )); } pos += sizeof(port                );

  // // load_rom.c

  // if (buffer != NULL) { memcpy(&rominfo             , &buffer[pos],  sizeof(rominfo             )); } pos += sizeof(rominfo             );
  // if (buffer != NULL) { memcpy(&romtype             , &buffer[pos],  sizeof(romtype             )); } pos += sizeof(romtype             );
  // if (buffer != NULL) { memcpy(&rom_region          , &buffer[pos],  sizeof(rom_region          )); } pos += sizeof(rom_region          );

  // // membnk.c

  // if (buffer != NULL) { memcpy(&zbank_memory_map    , &buffer[pos],  sizeof(zbank_memory_map    )); } pos += sizeof(zbank_memory_map    );

  // system.c

  // if (buffer != NULL) { memcpy(&bitmap              , &buffer[pos],  sizeof(bitmap              )); } pos += sizeof(bitmap              );
  // if (buffer != NULL) { memcpy(&snd                 , &buffer[pos],  sizeof(snd                 )); } pos += sizeof(snd                 );
  // if (buffer != NULL) { memcpy(&mcycles_vdp         , &buffer[pos],  sizeof(mcycles_vdp         )); } pos += sizeof(mcycles_vdp         );
  // if (buffer != NULL) { memcpy(&system_hw           , &buffer[pos],  sizeof(system_hw           )); } pos += sizeof(system_hw           );
  // if (buffer != NULL) { memcpy(&system_bios         , &buffer[pos],  sizeof(system_bios         )); } pos += sizeof(system_bios         );
  // if (buffer != NULL) { memcpy(&system_clock        , &buffer[pos],  sizeof(system_clock        )); } pos += sizeof(system_clock        );
  // if (buffer != NULL) { memcpy(&SVP_cycles          , &buffer[pos],  sizeof(SVP_cycles          )); } pos += sizeof(SVP_cycles          );
  if (buffer != NULL) { memcpy(&pause_b             , &buffer[pos],  sizeof(pause_b             )); } pos += sizeof(pause_b             );
  // if (buffer != NULL) { memcpy(&eq                  , &buffer[pos],  sizeof(eq                  )); } pos += sizeof(eq                  );
  // if (buffer != NULL) { memcpy(&llp                 , &buffer[pos],  sizeof(llp                 )); } pos += sizeof(llp                 );
  // if (buffer != NULL) { memcpy(&rrp                 , &buffer[pos],  sizeof(rrp                 )); } pos += sizeof(rrp                 );

  // // vdp.c

  // if (buffer != NULL) { memcpy(&sat                 , &buffer[pos],  sizeof(sat                 )); } pos += sizeof(sat                 );
  // if (buffer != NULL) { memcpy(&vram                , &buffer[pos],  sizeof(vram                )); } pos += sizeof(vram                );
  // if (buffer != NULL) { memcpy(&cram                , &buffer[pos],  sizeof(cram                )); } pos += sizeof(cram                );
  // if (buffer != NULL) { memcpy(&vsram               , &buffer[pos],  sizeof(vsram               )); } pos += sizeof(vsram               );
  // if (buffer != NULL) { memcpy(&reg                 , &buffer[pos],  sizeof(reg                 )); } pos += sizeof(reg                 );
  // if (buffer != NULL) { memcpy(&hint_pending        , &buffer[pos],  sizeof(hint_pending        )); } pos += sizeof(hint_pending        );
  // if (buffer != NULL) { memcpy(&vint_pending        , &buffer[pos],  sizeof(vint_pending        )); } pos += sizeof(vint_pending        );
  // if (buffer != NULL) { memcpy(&status              , &buffer[pos],  sizeof(status              )); } pos += sizeof(status              );
  // if (buffer != NULL) { memcpy(&dma_length          , &buffer[pos],  sizeof(dma_length          )); } pos += sizeof(dma_length          );
  // if (buffer != NULL) { memcpy(&dma_endCycles       , &buffer[pos],  sizeof(dma_endCycles       )); } pos += sizeof(dma_endCycles       );
  // if (buffer != NULL) { memcpy(&dma_type            , &buffer[pos],  sizeof(dma_type            )); } pos += sizeof(dma_type            );
  // if (buffer != NULL) { memcpy(&ntab                , &buffer[pos],  sizeof(ntab                )); } pos += sizeof(ntab                );
  // if (buffer != NULL) { memcpy(&ntbb                , &buffer[pos],  sizeof(ntbb                )); } pos += sizeof(ntbb                );
  // if (buffer != NULL) { memcpy(&ntwb                , &buffer[pos],  sizeof(ntwb                )); } pos += sizeof(ntwb                );
  // if (buffer != NULL) { memcpy(&satb                , &buffer[pos],  sizeof(satb                )); } pos += sizeof(satb                );
  // if (buffer != NULL) { memcpy(&hscb                , &buffer[pos],  sizeof(hscb                )); } pos += sizeof(hscb                );
  // if (buffer != NULL) { memcpy(&bg_name_dirty       , &buffer[pos],  sizeof(bg_name_dirty       )); } pos += sizeof(bg_name_dirty       );
  // if (buffer != NULL) { memcpy(&bg_name_list        , &buffer[pos],  sizeof(bg_name_list        )); } pos += sizeof(bg_name_list        );
  // if (buffer != NULL) { memcpy(&bg_list_index       , &buffer[pos],  sizeof(bg_list_index       )); } pos += sizeof(bg_list_index       );
  // if (buffer != NULL) { memcpy(&hscroll_mask        , &buffer[pos],  sizeof(hscroll_mask        )); } pos += sizeof(hscroll_mask        );
  // if (buffer != NULL) { memcpy(&playfield_shift     , &buffer[pos],  sizeof(playfield_shift     )); } pos += sizeof(playfield_shift     );
  // if (buffer != NULL) { memcpy(&playfield_col_mask  , &buffer[pos],  sizeof(playfield_col_mask  )); } pos += sizeof(playfield_col_mask  );
  // if (buffer != NULL) { memcpy(&playfield_row_mask  , &buffer[pos],  sizeof(playfield_row_mask  )); } pos += sizeof(playfield_row_mask  );
  // if (buffer != NULL) { memcpy(&vscroll             , &buffer[pos],  sizeof(vscroll             )); } pos += sizeof(vscroll             );
  // if (buffer != NULL) { memcpy(&odd_frame           , &buffer[pos],  sizeof(odd_frame           )); } pos += sizeof(odd_frame           );
  // if (buffer != NULL) { memcpy(&im2_flag            , &buffer[pos],  sizeof(im2_flag            )); } pos += sizeof(im2_flag            );
  // if (buffer != NULL) { memcpy(&interlaced          , &buffer[pos],  sizeof(interlaced          )); } pos += sizeof(interlaced          );
  // if (buffer != NULL) { memcpy(&vdp_pal             , &buffer[pos],  sizeof(vdp_pal             )); } pos += sizeof(vdp_pal             );
  // if (buffer != NULL) { memcpy(&h_counter           , &buffer[pos],  sizeof(h_counter           )); } pos += sizeof(h_counter           );
  // if (buffer != NULL) { memcpy(&v_counter           , &buffer[pos],  sizeof(v_counter           )); } pos += sizeof(v_counter           );
  // if (buffer != NULL) { memcpy(&vc_max              , &buffer[pos],  sizeof(vc_max              )); } pos += sizeof(vc_max              );
  // if (buffer != NULL) { memcpy(&lines_per_frame     , &buffer[pos],  sizeof(lines_per_frame     )); } pos += sizeof(lines_per_frame     );
  // if (buffer != NULL) { memcpy(&max_sprite_pixels   , &buffer[pos],  sizeof(max_sprite_pixels   )); } pos += sizeof(max_sprite_pixels   );
  // if (buffer != NULL) { memcpy(&fifo_cycles         , &buffer[pos],  sizeof(fifo_cycles         )); } pos += sizeof(fifo_cycles         );
  // if (buffer != NULL) { memcpy(&hvc_latch           , &buffer[pos],  sizeof(hvc_latch           )); } pos += sizeof(hvc_latch           );
  // if (buffer != NULL) { memcpy(&vint_cycle          , &buffer[pos],  sizeof(vint_cycle          )); } pos += sizeof(vint_cycle          );
  // // if (buffer != NULL) { memcpy(&hctab               , &buffer[pos],  sizeof(hctab               )); } pos += sizeof(hctab               );
  // if (buffer != NULL) { memcpy(&border              , &buffer[pos],  sizeof(border              )); } pos += sizeof(border              );
  // if (buffer != NULL) { memcpy(&pending             , &buffer[pos],  sizeof(pending             )); } pos += sizeof(pending             );
  // if (buffer != NULL) { memcpy(&code                , &buffer[pos],  sizeof(code                )); } pos += sizeof(code                );
  // if (buffer != NULL) { memcpy(&addr                , &buffer[pos],  sizeof(addr                )); } pos += sizeof(addr                );
  // if (buffer != NULL) { memcpy(&addr_latch          , &buffer[pos],  sizeof(addr_latch          )); } pos += sizeof(addr_latch          );
  // if (buffer != NULL) { memcpy(&sat_base_mask       , &buffer[pos],  sizeof(sat_base_mask       )); } pos += sizeof(sat_base_mask       );
  // if (buffer != NULL) { memcpy(&sat_addr_mask       , &buffer[pos],  sizeof(sat_addr_mask       )); } pos += sizeof(sat_addr_mask       );
  // if (buffer != NULL) { memcpy(&dma_src             , &buffer[pos],  sizeof(dma_src             )); } pos += sizeof(dma_src             );
  // if (buffer != NULL) { memcpy(&dmafill             , &buffer[pos],  sizeof(dmafill             )); } pos += sizeof(dmafill             );
  // if (buffer != NULL) { memcpy(&cached_write        , &buffer[pos],  sizeof(cached_write        )); } pos += sizeof(cached_write        );
  // if (buffer != NULL) { memcpy(&fifo                , &buffer[pos],  sizeof(fifo                )); } pos += sizeof(fifo                );
  // if (buffer != NULL) { memcpy(&fifo_idx            , &buffer[pos],  sizeof(fifo_idx            )); } pos += sizeof(fifo_idx            );
  // if (buffer != NULL) { memcpy(&fifo_byte_access    , &buffer[pos],  sizeof(fifo_byte_access    )); } pos += sizeof(fifo_byte_access    );
  // // if (buffer != NULL) { memcpy(&fifo_timing         , &buffer[pos],  sizeof(fifo_timing         )); } pos += sizeof(fifo_timing         );
  // if (buffer != NULL) { memcpy(&hblank_start_cycle  , &buffer[pos],  sizeof(hblank_start_cycle  )); } pos += sizeof(hblank_start_cycle  );
  // if (buffer != NULL) { memcpy(&hblank_end_cycle    , &buffer[pos],  sizeof(hblank_end_cycle    )); } pos += sizeof(hblank_end_cycle    );

  // // vdp_render.c

  // if (buffer != NULL) { memcpy(&clip                , &buffer[pos],  sizeof(clip                )); } pos += sizeof(clip                );
  // if (buffer != NULL) { memcpy(&bg_pattern_cache    , &buffer[pos],  sizeof(bg_pattern_cache    )); } pos += sizeof(bg_pattern_cache    );
  // if (buffer != NULL) { memcpy(&name_lut            , &buffer[pos],  sizeof(name_lut            )); } pos += sizeof(name_lut            );
  // if (buffer != NULL) { memcpy(&bp_lut              , &buffer[pos],  sizeof(bp_lut              )); } pos += sizeof(bp_lut              );
  // if (buffer != NULL) { memcpy(&lut                 , &buffer[pos],  sizeof(lut                 )); } pos += sizeof(lut                 );
  // if (buffer != NULL) { memcpy(&pixel               , &buffer[pos],  sizeof(pixel               )); } pos += sizeof(pixel               );
  // if (buffer != NULL) { memcpy(&pixel_lut           , &buffer[pos],  sizeof(pixel_lut           )); } pos += sizeof(pixel_lut           );
  // if (buffer != NULL) { memcpy(&pixel_lut_m4        , &buffer[pos],  sizeof(pixel_lut_m4        )); } pos += sizeof(pixel_lut_m4        );
  // if (buffer != NULL) { memcpy(&linebuf             , &buffer[pos],  sizeof(linebuf             )); } pos += sizeof(linebuf             );
  // if (buffer != NULL) { memcpy(&spr_ovr             , &buffer[pos],  sizeof(spr_ovr             )); } pos += sizeof(spr_ovr             );
  // if (buffer != NULL) { memcpy(&obj_info            , &buffer[pos],  sizeof(obj_info            )); } pos += sizeof(obj_info            );
  // if (buffer != NULL) { memcpy(&object_count        , &buffer[pos],  sizeof(object_count        )); } pos += sizeof(object_count        );
  // if (buffer != NULL) { memcpy(&spr_col             , &buffer[pos],  sizeof(spr_col             )); } pos += sizeof(spr_col             );

}


int scd_context_save(uint8 *state)
{
  uint16 tmp16;
  uint32 tmp32;
  int bufferptr = 0;

  /* internal harware */
  save_param(scd.regs, sizeof(scd.regs));
  save_param(&scd.cycles, sizeof(scd.cycles));
  save_param(&scd.stopwatch, sizeof(scd.stopwatch));
  save_param(&scd.timer, sizeof(scd.timer));
  save_param(&scd.pending, sizeof(scd.pending));
  save_param(&scd.dmna, sizeof(scd.dmna));

  /* GFX processor */
  bufferptr += gfx_context_save(&state[bufferptr]);

  /* CD Data controller */
  bufferptr += cdc_context_save(&state[bufferptr]);

  /* CD Drive processor */
  bufferptr += cdd_context_save(&state[bufferptr]);

  /* PCM chip */
  bufferptr += pcm_context_save(&state[bufferptr]);

  /* PRG-RAM */
  save_param(scd.prg_ram, sizeof(scd.prg_ram));

  /* Word-RAM */
  if (scd.regs[0x03>>1].byte.l & 0x04)
  {
    /* 1M mode */
    save_param(scd.word_ram, sizeof(scd.word_ram));
  }
  else
  {
    /* 2M mode */
    save_param(scd.word_ram_2M, sizeof(scd.word_ram_2M));
  }

  /* MAIN-CPU & SUB-CPU polling */
  save_param(&m68k.poll, sizeof(m68k.poll));
  save_param(&s68k.poll, sizeof(s68k.poll));

  /* H-INT default vector */
  tmp16 = *(uint16 *)(m68k.memory_map[scd.cartridge.boot].base + 0x72);
  save_param(&tmp16, 2);

  /* SUB-CPU registers */
  tmp32 = s68k_get_reg(M68K_REG_D0);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D1);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D2);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D3);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D4);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D5);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D6);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_D7);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A0);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A1);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A2);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A3);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A4);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A5);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A6);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_A7);  save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_PC);  save_param(&tmp32, 4);
  tmp16 = s68k_get_reg(M68K_REG_SR);  save_param(&tmp16, 2); 
  tmp32 = s68k_get_reg(M68K_REG_USP); save_param(&tmp32, 4);
  tmp32 = s68k_get_reg(M68K_REG_ISP); save_param(&tmp32, 4);

  /* SUB-CPU internal state */
  save_param(&s68k.cycles, sizeof(s68k.cycles));
  save_param(&s68k.int_level, sizeof(s68k.int_level));
  save_param(&s68k.stopped, sizeof(s68k.stopped));

  /* bootable MD cartridge */
  if (scd.cartridge.boot)
  {
    bufferptr += md_cart_context_save(&state[bufferptr]);
  }

  return bufferptr;
}

int scd_context_load(uint8 *state, char *version)
{
  int i;
  uint16 tmp16;
  uint32 tmp32;
  int bufferptr = 0;

  /* internal harware */
  load_param(scd.regs, sizeof(scd.regs));
  load_param(&scd.cycles, sizeof(scd.cycles));
  load_param(&scd.stopwatch, sizeof(scd.stopwatch));
  load_param(&scd.timer, sizeof(scd.timer));
  load_param(&scd.pending, sizeof(scd.pending));
  load_param(&scd.dmna, sizeof(scd.dmna));

  /* GFX processor */
  bufferptr += gfx_context_load(&state[bufferptr]);

  /* CD Data controller */
  bufferptr += cdc_context_load(&state[bufferptr]);

  /* CD Drive processor */
  bufferptr += cdd_context_load(&state[bufferptr], version);

  /* PCM chip */
  bufferptr += pcm_context_load(&state[bufferptr]);

  /* PRG-RAM */
  load_param(scd.prg_ram, sizeof(scd.prg_ram));

  /* PRG-RAM 128K bank mapped on MAIN-CPU side */
  m68k.memory_map[scd.cartridge.boot + 0x02].base = scd.prg_ram + ((scd.regs[0x03>>1].byte.l & 0xc0) << 11);
  m68k.memory_map[scd.cartridge.boot + 0x03].base = m68k.memory_map[scd.cartridge.boot + 0x02].base + 0x10000;

  /* PRG-RAM can only be accessed from MAIN 68K & Z80 if SUB-CPU is halted (Dungeon Explorer USA version) */
  if ((scd.regs[0x00].byte.l & 0x03) != 0x01)
  {
    m68k.memory_map[scd.cartridge.boot + 0x02].read8   = m68k.memory_map[scd.cartridge.boot + 0x03].read8   = NULL;
    m68k.memory_map[scd.cartridge.boot + 0x02].read16  = m68k.memory_map[scd.cartridge.boot + 0x03].read16  = NULL;
    m68k.memory_map[scd.cartridge.boot + 0x02].write8  = m68k.memory_map[scd.cartridge.boot + 0x03].write8  = NULL;
    m68k.memory_map[scd.cartridge.boot + 0x02].write16 = m68k.memory_map[scd.cartridge.boot + 0x03].write16 = NULL;
    zbank_memory_map[scd.cartridge.boot + 0x02].read   = zbank_memory_map[scd.cartridge.boot + 0x03].read   = NULL;
    zbank_memory_map[scd.cartridge.boot + 0x02].write  = zbank_memory_map[scd.cartridge.boot + 0x03].write  = NULL;
  }
  else
  {
    m68k.memory_map[scd.cartridge.boot + 0x02].read8   = m68k.memory_map[scd.cartridge.boot + 0x03].read8   = m68k_read_bus_8;
    m68k.memory_map[scd.cartridge.boot + 0x02].read16  = m68k.memory_map[scd.cartridge.boot + 0x03].read16  = m68k_read_bus_16;
    m68k.memory_map[scd.cartridge.boot + 0x02].write8  = m68k.memory_map[scd.cartridge.boot + 0x03].write8  = m68k_unused_8_w;
    m68k.memory_map[scd.cartridge.boot + 0x02].write16 = m68k.memory_map[scd.cartridge.boot + 0x03].write16 = m68k_unused_16_w;
    zbank_memory_map[scd.cartridge.boot + 0x02].read   = zbank_memory_map[scd.cartridge.boot + 0x03].read   = zbank_unused_r;
    zbank_memory_map[scd.cartridge.boot + 0x02].write  = zbank_memory_map[scd.cartridge.boot + 0x03].write  = zbank_unused_w;
  }

  /* Word-RAM */
  if (scd.regs[0x03>>1].byte.l & 0x04)
  {
    /* 1M Mode */
    load_param(scd.word_ram, sizeof(scd.word_ram));

    if (scd.regs[0x03>>1].byte.l & 0x01)
    {
      /* Word-RAM 1 assigned to MAIN-CPU */
      for (i=scd.cartridge.boot+0x20; i<scd.cartridge.boot+0x22; i++)
      {
        /* Word-RAM 1 data mapped at $200000-$21FFFF */
        m68k.memory_map[i].base = scd.word_ram[1] + ((i & 0x01) << 16);
        m68k.memory_map[i].read8   = NULL;
        m68k.memory_map[i].read16  = NULL;
        m68k.memory_map[i].write8  = NULL;
        m68k.memory_map[i].write16 = NULL;
        zbank_memory_map[i].read   = NULL;
        zbank_memory_map[i].write  = NULL;
      }

      for (i=scd.cartridge.boot+0x22; i<scd.cartridge.boot+0x24; i++)
      {
        /* VRAM cell image mapped at $220000-$23FFFF */
        m68k.memory_map[i].read8   = cell_ram_1_read8;
        m68k.memory_map[i].read16  = cell_ram_1_read16;
        m68k.memory_map[i].write8  = cell_ram_1_write8;
        m68k.memory_map[i].write16 = cell_ram_1_write16;
        zbank_memory_map[i].read   = cell_ram_1_read8;
        zbank_memory_map[i].write  = cell_ram_1_write8;
      }

      /* Word-RAM 0 assigned to SUB-CPU */
      for (i=0x08; i<0x0c; i++)
      {
        /* DOT image mapped at $080000-$0BFFFF */
        s68k.memory_map[i].read8   = dot_ram_0_read8;
        s68k.memory_map[i].read16  = dot_ram_0_read16;
        s68k.memory_map[i].write8  = dot_ram_0_write8;
        s68k.memory_map[i].write16 = dot_ram_0_write16;
      }

      for (i=0x0c; i<0x0e; i++)
      {
        /* Word-RAM 0 data mapped at $0C0000-$0DFFFF */
        s68k.memory_map[i].base    = scd.word_ram[0] + ((i & 0x01) << 16);
        s68k.memory_map[i].read8   = NULL;
        s68k.memory_map[i].read16  = NULL;
        s68k.memory_map[i].write8  = NULL;
        s68k.memory_map[i].write16 = NULL;
      }
    }
    else
    {
      /* Word-RAM 0 assigned to MAIN-CPU */
      for (i=scd.cartridge.boot+0x20; i<scd.cartridge.boot+0x22; i++)
      {
        /* Word-RAM 0 data mapped at $200000-$21FFFF */
        m68k.memory_map[i].base = scd.word_ram[0] + ((i & 0x01) << 16);
        m68k.memory_map[i].read8   = NULL;
        m68k.memory_map[i].read16  = NULL;
        m68k.memory_map[i].write8  = NULL;
        m68k.memory_map[i].write16 = NULL;
        zbank_memory_map[i].read   = NULL;
        zbank_memory_map[i].write  = NULL;
      }

      for (i=scd.cartridge.boot+0x22; i<scd.cartridge.boot+0x24; i++)
      {
        /* VRAM cell image mapped at $220000-$23FFFF */
        m68k.memory_map[i].read8   = cell_ram_0_read8;
        m68k.memory_map[i].read16  = cell_ram_0_read16;
        m68k.memory_map[i].write8  = cell_ram_0_write8;
        m68k.memory_map[i].write16 = cell_ram_0_write16;
        zbank_memory_map[i].read   = cell_ram_0_read8;
        zbank_memory_map[i].write  = cell_ram_0_write8;
      }

      /* Word-RAM 1 assigned to SUB-CPU */
      for (i=0x08; i<0x0c; i++)
      {
        /* DOT image mapped at $080000-$0BFFFF */
        s68k.memory_map[i].read8   = dot_ram_1_read8;
        s68k.memory_map[i].read16  = dot_ram_1_read16;
        s68k.memory_map[i].write8  = dot_ram_1_write8;
        s68k.memory_map[i].write16 = dot_ram_1_write16;
      }

      for (i=0x0c; i<0x0e; i++)
      {
        /* Word-RAM 1 data mapped at $0C0000-$0DFFFF */
        s68k.memory_map[i].base    = scd.word_ram[1] + ((i & 0x01) << 16);
        s68k.memory_map[i].read8   = NULL;
        s68k.memory_map[i].read16  = NULL;
        s68k.memory_map[i].write8  = NULL;
        s68k.memory_map[i].write16 = NULL;
      }
    }
  }
  else
  {
    /* 2M mode */
    load_param(scd.word_ram_2M, sizeof(scd.word_ram_2M));

    /* check RET bit */
    if (scd.regs[0x03>>1].byte.l & 0x01)
    {
      /* MAIN-CPU: $200000-$23FFFF is mapped to 256K Word-RAM */
      for (i=scd.cartridge.boot+0x20; i<scd.cartridge.boot+0x24; i++)
      {
        m68k.memory_map[i].base    = scd.word_ram_2M + ((i & 0x03) << 16);
        m68k.memory_map[i].read8   = NULL;
        m68k.memory_map[i].read16  = NULL;
        m68k.memory_map[i].write8  = NULL;
        m68k.memory_map[i].write16 = NULL;
        zbank_memory_map[i].read   = NULL;
        zbank_memory_map[i].write  = NULL;
      }

      /* SUB-CPU: access to Word-RAM at $080000-$0BFFFF is locked (/DTACK not asserted)  */
      for (i=0x08; i<0x0c; i++)
      {
        s68k.memory_map[i].read8   = s68k_lockup_r_8;
        s68k.memory_map[i].read16  = s68k_lockup_r_16;
        s68k.memory_map[i].write8  = s68k_lockup_w_8;
        s68k.memory_map[i].write16 = s68k_lockup_w_16;
      }
    }
    else
    {
      /* MAIN-CPU: $200000-$23FFFF is unmapped */
      for (i=scd.cartridge.boot+0x20; i<scd.cartridge.boot+0x24; i++)
      {
        m68k.memory_map[i].base    = scd.word_ram_2M + ((i & 0x03) << 16);
        m68k.memory_map[i].read8   = m68k_read_bus_8;
        m68k.memory_map[i].read16  = m68k_read_bus_16;
        m68k.memory_map[i].write8  = m68k_unused_8_w;
        m68k.memory_map[i].write16 = m68k_unused_16_w;
        zbank_memory_map[i].read   = zbank_unused_r;
        zbank_memory_map[i].write  = zbank_unused_w;
      }

      /* SUB-CPU: access to Word-RAM at $080000-$0BFFFF is unlocked (/DTACK asserted) */
      for (i=0x08; i<0x0c; i++)
      {
        s68k.memory_map[i].read8   = NULL;
        s68k.memory_map[i].read16  = NULL;
        s68k.memory_map[i].write8  = NULL;
        s68k.memory_map[i].write16 = NULL;
      }
    }

    /* SUB-CPU: $0C0000-$0DFFFF is unmapped */
    for (i=0x0c; i<0x0e; i++)
    {
      s68k.memory_map[i].read8   = s68k_read_bus_8;
      s68k.memory_map[i].read16  = s68k_read_bus_16;
      s68k.memory_map[i].write8  = s68k_unused_8_w;
      s68k.memory_map[i].write16 = s68k_unused_16_w;
    }
  }

  /* MAIN-CPU & SUB-CPU polling */
  load_param(&m68k.poll, sizeof(m68k.poll));
  load_param(&s68k.poll, sizeof(s68k.poll));

  /* H-INT default vector */
  load_param(&tmp16, 2);
  *(uint16 *)(m68k.memory_map[scd.cartridge.boot].base + 0x72) = tmp16;

  /* SUB-CPU registers */
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D0, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D1, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D2, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D3, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D4, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D5, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D6, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_D7, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A0, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A1, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A2, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A3, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A4, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A5, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A6, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_A7, tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_PC, tmp32);  
  load_param(&tmp16, 2); s68k_set_reg(M68K_REG_SR, tmp16);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_USP,tmp32);
  load_param(&tmp32, 4); s68k_set_reg(M68K_REG_ISP,tmp32);

  /* SUB-CPU internal state */
  load_param(&s68k.cycles, sizeof(s68k.cycles));
  load_param(&s68k.int_level, sizeof(s68k.int_level));
  load_param(&s68k.stopped, sizeof(s68k.stopped));

  /* bootable MD cartridge hardware */
  if (scd.cartridge.boot)
  {
    bufferptr += md_cart_context_load(&state[bufferptr]);
  }

  return bufferptr;
}

int vdp_context_save(uint8 *state)
{
  int bufferptr = 0;

  save_param(sat, sizeof(sat));
  save_param(vram, sizeof(vram));
  save_param(cram, sizeof(cram));
  save_param(vsram, sizeof(vsram));
  save_param(reg, sizeof(reg));
  save_param(&addr, sizeof(addr));
  save_param(&addr_latch, sizeof(addr_latch));
  save_param(&code, sizeof(code));
  save_param(&pending, sizeof(pending));
  save_param(&status, sizeof(status));
  save_param(&dmafill, sizeof(dmafill));
  save_param(&fifo_idx, sizeof(fifo_idx));
  save_param(&fifo, sizeof(fifo));
  save_param(&h_counter, sizeof(h_counter));
  save_param(&hint_pending, sizeof(hint_pending));
  save_param(&vint_pending, sizeof(vint_pending));
  save_param(&dma_length, sizeof(dma_length));
  save_param(&dma_type, sizeof(dma_type));
  save_param(&dma_src, sizeof(dma_src));
  save_param(&cached_write, sizeof(cached_write));
  return bufferptr;
}

int vdp_context_load(uint8 *state)
{
  int i, bufferptr = 0;
  uint8 temp_reg[0x20];

  load_param(sat, sizeof(sat));
  load_param(vram, sizeof(vram));
  load_param(cram, sizeof(cram));
  load_param(vsram, sizeof(vsram));
  load_param(temp_reg, sizeof(temp_reg));

  /* restore VDP registers */
  if (system_hw < SYSTEM_MD)
  {
    if (system_hw >= SYSTEM_MARKIII)
    {
      for (i=0;i<0x10;i++) 
      {
        pending = 1;
        addr_latch = temp_reg[i];
        vdp_sms_ctrl_w(0x80 | i);
      }
    }
    else
    {
      /* TMS-99xx registers are updated directly to prevent spurious 4K->16K VRAM switching */
      for (i=0;i<0x08;i++) 
      {
        reg[i] = temp_reg[i];
      }

      /* Rendering mode */
      render_bg = render_bg_modes[((reg[0] & 0x02) | (reg[1] & 0x18)) >> 1];
    }
  }
  else
  {
    for (i=0;i<0x20;i++) 
    {
      vdp_reg_w(i, temp_reg[i], 0);
    }
  }

  load_param(&addr, sizeof(addr));
  load_param(&addr_latch, sizeof(addr_latch));
  load_param(&code, sizeof(code));
  load_param(&pending, sizeof(pending));
  load_param(&status, sizeof(status));
  load_param(&dmafill, sizeof(dmafill));
  load_param(&fifo_idx, sizeof(fifo_idx));
  load_param(&fifo, sizeof(fifo));
  load_param(&h_counter, sizeof(h_counter));
  load_param(&hint_pending, sizeof(hint_pending));
  load_param(&vint_pending, sizeof(vint_pending));
  load_param(&dma_length, sizeof(dma_length));
  load_param(&dma_type, sizeof(dma_type));
  load_param(&dma_src, sizeof(dma_src));
  load_param(&cached_write, sizeof(cached_write));

  /* restore FIFO byte access flag */
  fifo_byte_access = ((code & 0x0F) < 0x03);

  /* restore current NTSC/PAL mode */
  if (system_hw & SYSTEM_MD)
  {
    status = (status & ~1) | vdp_pal;
  }

  if (reg[1] & 0x04)
  {
    /* Mode 5 */
    bg_list_index = 0x800;

    /* reinitialize palette */
    color_update_m5(0, *(uint16 *)&cram[border << 1]);
    for(i = 1; i < 0x40; i++)
    {
      color_update_m5(i, *(uint16 *)&cram[i << 1]);
    }
  }
  else
  {
    /* Modes 0,1,2,3,4 */
    bg_list_index = 0x200;

    /* reinitialize palette */
    for(i = 0; i < 0x20; i ++)
    {
      color_update_m4(i, *(uint16 *)&cram[i << 1]);
    }
    color_update_m4(0x40, *(uint16 *)&cram[(0x10 | (border & 0x0F)) << 1]);
  }

  /* invalidate tile cache */
  for (i=0;i<bg_list_index;i++) 
  {
    bg_name_list[i]=i;
    bg_name_dirty[i]=0xFF;
  }

  return bufferptr;
}

int sms_cart_context_save(uint8 *state)
{
  int bufferptr = 0;

  /* check if cartridge ROM is disabled */
  if (io_reg[0x0E] & 0x40)
  {
    /* save Boot ROM mapper settings */
    save_param(bios_rom.fcr, 4);
  }
  else
  {
    /* save cartridge mapper settings */
    save_param(cart_rom.fcr, 4);
  }

  /* support for SG-1000 games with extra RAM */
  if ((cart_rom.mapper == MAPPER_RAM_8K) || (cart_rom.mapper == MAPPER_RAM_8K_EXT1))
  {
    /* 8KB extra RAM */
    save_param(work_ram + 0x2000, 0x2000);
  }
  else if (cart_rom.mapper == MAPPER_RAM_2K)
  {
    /* 2KB extra RAM */
    save_param(work_ram + 0x2000, 0x800);
  }

  return bufferptr;
}


int cdc_context_save(uint8 *state)
{
  uint8 tmp8;
  int bufferptr = 0;

  if (cdc_dma_w == pcm_ram_dma_w)
  {
    tmp8 = 1;
  }
  else if (cdc_dma_w == prg_ram_dma_w)
  {
    tmp8 = 2;
  }
  else if (cdc_dma_w == word_ram_0_dma_w)
  {
    tmp8 = 3;
  }
  else if (cdc_dma_w == word_ram_1_dma_w)
  {
    tmp8 = 4;
  }
  else if (cdc_dma_w == word_ram_2M_dma_w)
  {
    tmp8 = 5;
  }
  else if (cdc_halted_dma_w == prg_ram_dma_w)
  {
    tmp8 = 6;
  }
  else if (cdc_halted_dma_w == word_ram_2M_dma_w)
  {
    tmp8 = 7;
  }
  else
  {
    tmp8 = 0;
  }

  save_param(&cdc.ifstat, sizeof(cdc.ifstat));
  save_param(&cdc.ifctrl, sizeof(cdc.ifctrl));
  save_param(&cdc.dbc, sizeof(cdc.dbc));
  save_param(&cdc.dac, sizeof(cdc.dac));
  save_param(&cdc.pt, sizeof(cdc.pt));
  save_param(&cdc.wa, sizeof(cdc.wa));
  save_param(&cdc.ctrl, sizeof(cdc.ctrl));
  save_param(&cdc.head, sizeof(cdc.head));
  save_param(&cdc.stat, sizeof(cdc.stat));
  save_param(&cdc.cycles, sizeof(cdc.cycles));
  save_param(&cdc.ram, sizeof(cdc.ram));
  save_param(&tmp8, 1);

  return bufferptr;
}



int YM2612LoadContext(unsigned char *state)
{
  int c,s;
  uint8 index;
  int bufferptr = 0;

  /* restore YM2612 context */
  load_param(&ym2612, sizeof(ym2612));

  /* restore DT table address pointer for each channel slots */
  for (c=0; c<6; c++)
  {
    for (s=0; s<4; s++)
    {
      load_param(&index,sizeof(index));
      bufferptr += sizeof(index);
      ym2612.CH[c].SLOT[s].DT = ym2612.OPN.ST.dt_tab[index&7];
    }
  }

  /* restore outputs connections */
  setup_connection(&ym2612.CH[0],0);
  setup_connection(&ym2612.CH[1],1);
  setup_connection(&ym2612.CH[2],2);
  setup_connection(&ym2612.CH[3],3);
  setup_connection(&ym2612.CH[4],4);
  setup_connection(&ym2612.CH[5],5);

  return bufferptr;
}

int YM2612SaveContext(unsigned char *state)
{
  int c,s;
  uint8 index;
  int bufferptr = 0;

  /* save YM2612 context */
  save_param(&ym2612, sizeof(ym2612));

  /* save DT table index for each channel slots */
  for (c=0; c<6; c++)
  {
    for (s=0; s<4; s++)
    {
      index = (ym2612.CH[c].SLOT[s].DT - ym2612.OPN.ST.dt_tab[0]) >> 5;
      save_param(&index,sizeof(index));
      bufferptr += sizeof(index);
    }
  }

  return bufferptr;
}

int psg_context_save(uint8 *state)
{
  int bufferptr = 0;

  save_param(&psg.clocks,sizeof(psg.clocks));
  save_param(&psg.latch,sizeof(psg.latch));
  save_param(&psg.noiseShiftValue,sizeof(psg.noiseShiftValue));
  save_param(psg.regs,sizeof(psg.regs));
  save_param(psg.freqInc,sizeof(psg.freqInc));
  save_param(psg.freqCounter,sizeof(psg.freqCounter));
  save_param(psg.polarity,sizeof(psg.polarity));
  save_param(psg.chanOut,sizeof(psg.chanOut));

  return bufferptr;
}

int psg_context_load(uint8 *state)
{
  int delta[2];
  int i, bufferptr = 0;

  /* initialize delta with current noise channel output */
  if (psg.noiseShiftValue & 1)
  {
    delta[0] = -psg.chanOut[3][0];
    delta[1] = -psg.chanOut[3][1];
  }
  else
  {
    delta[0] = 0;
    delta[1] = 0;
  }

  /* add current tone channels output */
  for (i=0; i<3; i++)
  {
    if (psg.polarity[i] > 0)
    {
      delta[0] -= psg.chanOut[i][0];
      delta[1] -= psg.chanOut[i][1];
    }
  }

  load_param(&psg.clocks,sizeof(psg.clocks));
  load_param(&psg.latch,sizeof(psg.latch));
  load_param(&psg.noiseShiftValue,sizeof(psg.noiseShiftValue));
  load_param(psg.regs,sizeof(psg.regs));
  load_param(psg.freqInc,sizeof(psg.freqInc));
  load_param(psg.freqCounter,sizeof(psg.freqCounter));
  load_param(psg.polarity,sizeof(psg.polarity));
  load_param(psg.chanOut,sizeof(psg.chanOut));

  /* add noise channel output variation */
  if (psg.noiseShiftValue & 1)
  {
    delta[0] += psg.chanOut[3][0];
    delta[1] += psg.chanOut[3][1];
  }

  /* add tone channels output variation */
  for (i=0; i<3; i++)
  {
    if (psg.polarity[i] > 0)
    {
      delta[0] += psg.chanOut[i][0];
      delta[1] += psg.chanOut[i][1];
    }
  }

  /* update mixed channels output */
  if (config.hq_psg)
  {
    blip_add_delta(snd.blips[0], psg.clocks, delta[0], delta[1]);
  }
  else
  {
    blip_add_delta_fast(snd.blips[0], psg.clocks, delta[0], delta[1]);
  }

  return bufferptr;
}

int sound_context_save(uint8 *state)
{
  int bufferptr = 0;
  
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
#ifdef HAVE_YM3438_CORE
    save_param(&config.ym3438, sizeof(config.ym3438));
    if (config.ym3438)
    {
      save_param(&ym3438, sizeof(ym3438));
      save_param(&ym3438_accm, sizeof(ym3438_accm));
      save_param(&ym3438_sample, sizeof(ym3438_sample));
      save_param(&ym3438_cycles, sizeof(ym3438_cycles));
    }
    else
    {
      bufferptr += YM2612SaveContext(state + sizeof(config.ym3438));
    }
#else
    bufferptr = YM2612SaveContext(state);
#endif
  }
  else
  {
#ifdef HAVE_OPLL_CORE
    save_param(&config.opll, sizeof(config.opll));
    if (config.opll)
    {
      save_param(&opll, sizeof(opll));
      save_param(&opll_accm, sizeof(opll_accm));
      save_param(&opll_sample, sizeof(opll_sample));
      save_param(&opll_cycles, sizeof(opll_cycles));
      save_param(&opll_status, sizeof(opll_status));
    }
    else
#endif
    {
      save_param(YM2413GetContextPtr(),YM2413GetContextSize());
    }
  }

  bufferptr += psg_context_save(&state[bufferptr]);

  save_param(&fm_cycles_start,sizeof(fm_cycles_start));

  return bufferptr;
}

int sound_context_load(uint8 *state)
{
  int bufferptr = 0;

  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
#ifdef HAVE_YM3438_CORE
    uint8 config_ym3438;
    load_param(&config_ym3438, sizeof(config_ym3438));
    if (config_ym3438)
    {
      load_param(&ym3438, sizeof(ym3438));
      load_param(&ym3438_accm, sizeof(ym3438_accm));
      load_param(&ym3438_sample, sizeof(ym3438_sample));
      load_param(&ym3438_cycles, sizeof(ym3438_cycles));
    }
    else
    {
      bufferptr += YM2612LoadContext(state + sizeof(config_ym3438));
    }
#else
    bufferptr = YM2612LoadContext(state);
#endif
  }
  else
  {
#ifdef HAVE_OPLL_CORE
    uint8 config_opll;
    load_param(&config_opll, sizeof(config_opll));
    if (config_opll)
    {
      load_param(&opll, sizeof(opll));
      load_param(&opll_accm, sizeof(opll_accm));
      load_param(&opll_sample, sizeof(opll_sample));
      load_param(&opll_cycles, sizeof(opll_cycles));
      load_param(&opll_status, sizeof(opll_status));
    }
    else
#endif
    {
      load_param(YM2413GetContextPtr(),YM2413GetContextSize());
    }
  }

  bufferptr += psg_context_load(&state[bufferptr]);

  load_param(&fm_cycles_start,sizeof(fm_cycles_start));
  fm_cycles_count = fm_cycles_start;

  return bufferptr;
}

int pcm_context_save(uint8 *state)
{
  uint8 tmp8;
  int bufferptr = 0;

  tmp8 = (pcm.bank - pcm.ram) >> 12;

  save_param(pcm.chan, sizeof(pcm.chan));
  save_param(pcm.out, sizeof(pcm.out));
  save_param(&tmp8, 1);
  save_param(&pcm.enabled, sizeof(pcm.enabled));
  save_param(&pcm.status, sizeof(pcm.status));
  save_param(&pcm.index, sizeof(pcm.index));
  save_param(pcm.ram, sizeof(pcm.ram));

  return bufferptr;
}

int pcm_context_load(uint8 *state)
{
  uint8 tmp8;
  int bufferptr = 0;

  load_param(pcm.chan, sizeof(pcm.chan));
  load_param(pcm.out, sizeof(pcm.out));

  load_param(&tmp8, 1);
  pcm.bank = &pcm.ram[(tmp8 & 0x0f) << 12];

  load_param(&pcm.enabled, sizeof(pcm.enabled));
  load_param(&pcm.status, sizeof(pcm.status));
  load_param(&pcm.index, sizeof(pcm.index));
  load_param(pcm.ram, sizeof(pcm.ram));

  return bufferptr;
}


int gfx_context_save(uint8 *state)
{
  uint32 tmp32;
  int bufferptr = 0;

  save_param(&gfx.cycles, sizeof(gfx.cycles));
  save_param(&gfx.cyclesPerLine, sizeof(gfx.cyclesPerLine));
  save_param(&gfx.dotMask, sizeof(gfx.dotMask));
  save_param(&gfx.stampShift, sizeof(gfx.stampShift));
  save_param(&gfx.mapShift, sizeof(gfx.mapShift));
  save_param(&gfx.bufferOffset, sizeof(gfx.bufferOffset));
  save_param(&gfx.bufferStart, sizeof(gfx.bufferStart));

  tmp32 = (uint8 *)(gfx.tracePtr) - scd.word_ram_2M;
  save_param(&tmp32, 4);

  tmp32 = (uint8 *)(gfx.mapPtr) - scd.word_ram_2M;
  save_param(&tmp32, 4);

  return bufferptr;
}

int gfx_context_load(uint8 *state)
{
  uint32 tmp32;
  int bufferptr = 0;

  load_param(&gfx.cycles, sizeof(gfx.cycles));
  load_param(&gfx.cyclesPerLine, sizeof(gfx.cyclesPerLine));
  load_param(&gfx.dotMask, sizeof(gfx.dotMask));
  load_param(&gfx.stampShift, sizeof(gfx.stampShift));
  load_param(&gfx.mapShift, sizeof(gfx.mapShift));
  load_param(&gfx.bufferOffset, sizeof(gfx.bufferOffset));
  load_param(&gfx.bufferStart, sizeof(gfx.bufferStart));

  load_param(&tmp32, 4);
  gfx.tracePtr = (uint16 *)(scd.word_ram_2M + tmp32);

  load_param(&tmp32, 4);
  gfx.mapPtr = (uint16 *)(scd.word_ram_2M + tmp32);

  return bufferptr;
}

int cdd_context_save(uint8 *state)
{
  int bufferptr = 0;
  unsigned int offset = 0;

  save_param(&cdd.cycles, sizeof(cdd.cycles));
  save_param(&cdd.latency, sizeof(cdd.latency));
  save_param(&cdd.index, sizeof(cdd.index));
  save_param(&cdd.lba, sizeof(cdd.lba));
  save_param(&cdd.scanOffset, sizeof(cdd.scanOffset));
  save_param(&cdd.fader, sizeof(cdd.fader));
  save_param(&cdd.status, sizeof(cdd.status));

  /* current track is an audio track ? */
  if (cdd.toc.tracks[cdd.index].type == TYPE_AUDIO)
  {
    /* get file read offset */
#if defined(USE_LIBCHDR)
    if (libCHDRfile)
    {
      /* CHD file offset */
      offset = cdd.chd.hunkofs;
    }
    else
#endif
#if defined(USE_LIBTREMOR) || defined(USE_LIBVORBIS)
    if (cdd.toc.tracks[cdd.index].vf.seekable)
    {
      /* VORBIS file sample offset */
      offset = ov_pcm_tell(&cdd.toc.tracks[cdd.index].vf);
    }
    else
#endif 
    if (cdTrackStreams[cdd.index])
    {
      /* PCM file offset */
      offset = cdStreamTell(cdTrackStreams[cdd.index]);
    }
  }

  save_param(&offset, sizeof(offset));
  save_param(&cdd.audio, sizeof(cdd.audio));

  return bufferptr;
}

int cdd_context_load(uint8 *state, char *version)
{
  unsigned int offset, lba, index;
  int bufferptr = 0;

  load_param(&cdd.cycles, sizeof(cdd.cycles));
  load_param(&cdd.latency, sizeof(cdd.latency));
  load_param(&index, sizeof(cdd.index));
  load_param(&lba, sizeof(cdd.lba));
  load_param(&cdd.scanOffset, sizeof(cdd.scanOffset));
  load_param(&cdd.fader, sizeof(cdd.fader));
  load_param(&cdd.status, sizeof(cdd.status));

  /* update current sector */
  cdd.lba = lba;

  /* support for previous state version (1.7.5) */
  if ((version[11] == 0x31) && (version[13] == 0x37) && (version[15] == 0x35))
  {
    /* current track is an audio track ? */
    if (cdd.toc.tracks[index].type == TYPE_AUDIO)
    {
      /* stay within track limits when seeking files */
      if (lba < cdd.toc.tracks[index].start)
      {
        lba = cdd.toc.tracks[index].start;
      }

      /* seek to current track sector */
      cdd_seek_audio(index, lba);
    }
  }
  else
  {
    load_param(&offset, sizeof(offset));
    load_param(&cdd.audio, sizeof(cdd.audio));

    /* current track is an audio track ? */
    if (cdd.toc.tracks[index].type == TYPE_AUDIO)
    {
#if defined(USE_LIBTREMOR) || defined(USE_LIBVORBIS)
#ifdef DISABLE_MANY_OGG_OPEN_FILES
      /* check if track index has changed */
      if (index != cdd.index)
      {
        /* close previous track VORBIS file structure to save memory */
        if (cdd.toc.tracks[cdd.index].vf.datasource)
        {
          ogg_free(cdd.index);
        }

        /* open current track VORBIS file */
        if (cdd.toc.tracks[index].vf.seekable)
        {
          ov_open_callbacks(cdTrackStreams[index],&cdd.toc.tracks[index].vf,0,0,cb);
        }
      }
#endif
#endif
      /* seek to current file read offset */
#if defined(USE_LIBCHDR)
      if (libCHDRfile)
      {
        /* CHD file offset */
        cdd.chd.hunkofs = offset;
      }
      else
#endif
#if defined(USE_LIBTREMOR) || defined(USE_LIBVORBIS)
      if (cdd.toc.tracks[index].vf.seekable)
      {
        /* VORBIS file sample offset */
        ov_pcm_seek(&cdd.toc.tracks[index].vf, offset);
      }
      else
#endif 
      if (cdTrackStreams[index])
      {
        /* PCM file offset */
        cdStreamSeek(cdTrackStreams[index], offset, SEEK_SET);
      }
    }
  }

  /* seek to current subcode position */
  if (cdTocStream)
  {
    /* 96 bytes per sector */
    cdStreamSeek(cdTocStream, lba * 96, SEEK_SET);
  }

  /* update current track index */
  cdd.index = index;

  return bufferptr;
}

int cdc_context_load(uint8 *state)
{
  uint8 tmp8;
  int bufferptr = 0;

  load_param(&cdc.ifstat, sizeof(cdc.ifstat));
  load_param(&cdc.ifctrl, sizeof(cdc.ifctrl));
  load_param(&cdc.dbc, sizeof(cdc.dbc));
  load_param(&cdc.dac, sizeof(cdc.dac));
  load_param(&cdc.pt, sizeof(cdc.pt));
  load_param(&cdc.wa, sizeof(cdc.wa));
  load_param(&cdc.ctrl, sizeof(cdc.ctrl));
  load_param(&cdc.head, sizeof(cdc.head));
  load_param(&cdc.stat, sizeof(cdc.stat));
  load_param(&cdc.cycles, sizeof(cdc.cycles));
  load_param(&cdc.ram, sizeof(cdc.ram));

  load_param(&tmp8, 1);

  switch (tmp8)
  {
    case 1:
      cdc_dma_w = pcm_ram_dma_w;
      cdc_halted_dma_w = 0;
      break;
    case 2:
      cdc_dma_w = prg_ram_dma_w;
      cdc_halted_dma_w = 0;
      break;
    case 3:
      cdc_dma_w = word_ram_0_dma_w;
      cdc_halted_dma_w = 0;
      break;
    case 4:
      cdc_dma_w = word_ram_1_dma_w;
      cdc_halted_dma_w = 0;
      break;
    case 5:
      cdc_dma_w = word_ram_2M_dma_w;
      cdc_halted_dma_w = 0;
      break;
    case 6:
      cdc_dma_w = 0;
      cdc_halted_dma_w = prg_ram_dma_w;
      break;
    case 7:
      cdc_dma_w = 0;
      cdc_halted_dma_w = word_ram_2M_dma_w;
      break;
    default:
      cdc_dma_w = 0;
      cdc_halted_dma_w = 0;
      break;
  }

  cdc.irq = ~cdc.ifstat & cdc.ifctrl & (BIT_DTEIEN | BIT_DECIEN);

  return bufferptr;
}

int sms_cart_context_load(uint8 *state)
{
  int bufferptr = 0;

  /* check if cartridge ROM is disabled */
  if (io_reg[0x0E] & 0x40)
  {
    /* load Boot ROM mapper settings */
    load_param(bios_rom.fcr, 4);

    /* set default cartridge ROM paging */
    switch (cart_rom.mapper)
    {
      case MAPPER_SEGA:
      case MAPPER_SEGA_X:
        cart_rom.fcr[0] = 0;
        cart_rom.fcr[1] = 0;
        cart_rom.fcr[2] = 1;
        cart_rom.fcr[3] = 2;
        break;

      case MAPPER_ZEMINA_16K_32K:
        cart_rom.fcr[0] = 0;
        cart_rom.fcr[1] = 0;
        cart_rom.fcr[2] = 1;
        cart_rom.fcr[3] = 1;
        break;

      case MAPPER_ZEMINA_4x8K:
        cart_rom.fcr[0] = 3;
        cart_rom.fcr[1] = 2;
        cart_rom.fcr[2] = 1;
        cart_rom.fcr[3] = 0;
        break;

      case MAPPER_KOREA_8K:
      case MAPPER_MSX:
      case MAPPER_MSX_NEMESIS:
      case MAPPER_MULTI_4x8K:
      case MAPPER_MULTI_8K:
        cart_rom.fcr[0] = 0;
        cart_rom.fcr[1] = 0;
        cart_rom.fcr[2] = 0;
        cart_rom.fcr[3] = 0;
        break;

      default:
        cart_rom.fcr[0] = 0;
        cart_rom.fcr[1] = 0;
        cart_rom.fcr[2] = 1;
        cart_rom.fcr[3] = 0;
        break;
    }
  }
  else
  {
    /* load cartridge mapper settings */
    load_param(cart_rom.fcr, 4);

    /* set default BIOS ROM paging (SEGA mapper by default) */
    bios_rom.fcr[0] = 0;
    bios_rom.fcr[1] = 0;
    bios_rom.fcr[2] = 1;
    bios_rom.fcr[3] = 2;
  }

  /* support for SG-1000 games with extra RAM */
  if ((cart_rom.mapper == MAPPER_RAM_8K) || (cart_rom.mapper == MAPPER_RAM_8K_EXT1))
  {
    /* 8KB extra RAM */
    load_param(work_ram + 0x2000, 0x2000);
  }
  else if (cart_rom.mapper == MAPPER_RAM_2K)
  {
    /* 2KB extra RAM */
    load_param(work_ram + 0x2000, 0x800);
  }

  return bufferptr;
}

int megasd_context_save(uint8 *state)
{
  int bufferptr = 0;

  save_param(&megasd_hw, sizeof(megasd_hw));

  /* save needed CD hardware state (only if not already saved) */
  if (system_hw != SYSTEM_MCD)
  {
    bufferptr += cdd_context_save(&state[bufferptr]);
    bufferptr += pcm_context_save(&state[bufferptr]);
    save_param(&scd.regs[0x36>>1].byte.h, 1);
  }

  return bufferptr;
}

int megasd_context_load(uint8 *state)
{
  int bufferptr = 0;

  load_param(&megasd_hw, sizeof(megasd_hw));

  /* load needed CD hardware state (only if not already loaded) */
  if (system_hw != SYSTEM_MCD)
  {
    bufferptr += cdd_context_load(&state[bufferptr], STATE_VERSION);
    bufferptr += pcm_context_load(&state[bufferptr]);
    load_param(&scd.regs[0x36>>1].byte.h, 1);
  }

  return bufferptr;
}

int md_cart_context_save(uint8 *state)
{
  int i;
  int bufferptr = 0;
  uint8 *base;

  // /* cartridge mapping */
  // for (i=0; i<0x40; i++)
  // {
  //   /* get base address */
  //   base = m68k.memory_map[i].base;

  //   if (base == sram.sram)
  //   {
  //     /* SRAM */
  //     state[bufferptr++] = 0xff;
  //   }
  //   else if (base == boot_rom)
  //   {
  //     /* Boot ROM */
  //     state[bufferptr++] = 0xfe;
  //   }
  //   else
  //   {
  //     /* Cartridge ROM */
  //     state[bufferptr++] = ((base - cart.rom) >> 16) & 0xff;
  //   }
  // }

  for (i = 0; i < 255; i++)
  {
    // Getting target base pointer
    void* basePtr = 0;
    switch (m68k.memory_map[i].target)
    {
      case  MM_TARGET_NULL: break;
      case  MM_TARGET_SCD_WORD_RAM_0: basePtr = scd.word_ram[0]; break;
      case  MM_TARGET_SCD_WORD_RAM_1: basePtr = scd.word_ram[1]; break;
      case  MM_TARGET_SCD_WORD_RAM_2M: basePtr = scd.word_ram_2M; break;
      case  MM_TARGET_SCD_PRG_RAM: basePtr = scd.prg_ram; break;
      case  MM_TARGET_SCD_BOOT_ROM: basePtr = scd.bootrom; break;
      case  MM_TARGET_SRAM: basePtr = sram.sram;
      case  MM_TARGET_BOOT_ROM: basePtr = boot_rom;
      case  MM_TARGET_WORK_RAM: basePtr = work_ram;
      case  MM_TARGET_CART_ROM: basePtr = cart.rom;
      case  MM_TARGET_CART_LOCK_ROM: basePtr = cart.lockrom;
      case  MM_TARGET_SVP_DRAM: basePtr = svp->dram;
      case  MM_TARGET_ACTION_REPLAY_RAM: basePtr = action_replay.ram;
    }
    
    // Getting offset with respect to target
    size_t offset = (size_t)m68k.memory_map[i].base - (size_t)basePtr;

    // Storing target and offset
    save_param(&m68k.memory_map[i].target, sizeof(m68k.memory_map[i].target));
    save_param(&offset, sizeof(offset));
  }

  /* hardware registers */
  save_param(cart.hw.regs, sizeof(cart.hw.regs));

  /* SVP */
  if (svp)
  {
    save_param(svp->iram_rom, 0x800);
    save_param(svp->dram,sizeof(svp->dram));
    save_param(&svp->ssp1601,sizeof(ssp1601_t));
  }

  /* MegaSD hardware */
  if (cart.special & HW_MEGASD)
  {
    bufferptr += megasd_context_save(&state[bufferptr]);
  }

  return bufferptr;
}

int md_cart_context_load(uint8 *state)
{
  int i;
  int bufferptr = 0;
  uint8 offset;

  // /* cartridge mapping */
  // for (i=0; i<0x40; i++)
  // {
  //   /* get offset */
  //   offset = state[bufferptr++];

  //   if (offset == 0xff)
  //   {
  //     /* SRAM */
  //     m68k.memory_map[i].base     = sram.sram;
  //     m68k.memory_map[i].read8    = sram_read_byte;
  //     m68k.memory_map[i].read16   = sram_read_word;
  //     m68k.memory_map[i].write8   = sram_write_byte;
  //     m68k.memory_map[i].write16  = sram_write_word;
  //     zbank_memory_map[i].read    = sram_read_byte;
  //     zbank_memory_map[i].write   = sram_write_byte;

  //   }
  //   else
  //   {
  //     /* check if SRAM was mapped there before loading state */
  //     if (m68k.memory_map[i].base == sram.sram)
  //     {
  //       m68k.memory_map[i].read8    = NULL;
  //       m68k.memory_map[i].read16   = NULL;
  //       m68k.memory_map[i].write8   = m68k_unused_8_w;
  //       m68k.memory_map[i].write16  = m68k_unused_16_w;
  //       zbank_memory_map[i].read    = NULL;
  //       zbank_memory_map[i].write   = zbank_unused_w;
  //     }

  //     /* ROM */
  //     m68k.memory_map[i].base = (offset == 0xfe) ? boot_rom : (cart.rom + (offset << 16));
  //   }
  // }

  
  for (i = 0; i < 255; i++)
  {
    // Getting target base pointer
    uint8_t* basePtr = 0;
    size_t offset = 0;

    // Loading target and offset
    load_param(&m68k.memory_map[i].target, sizeof(m68k.memory_map[i].target));
    load_param(&offset, sizeof(offset));

    switch (m68k.memory_map[i].target)
    {
      case  MM_TARGET_NULL: break;
      case  MM_TARGET_SCD_WORD_RAM_0: basePtr = scd.word_ram[0]; break;
      case  MM_TARGET_SCD_WORD_RAM_1: basePtr = scd.word_ram[1]; break;
      case  MM_TARGET_SCD_WORD_RAM_2M: basePtr = scd.word_ram_2M; break;
      case  MM_TARGET_SCD_PRG_RAM: basePtr = scd.prg_ram; break;
      case  MM_TARGET_SCD_BOOT_ROM: basePtr = scd.bootrom; break;
      case  MM_TARGET_SRAM: basePtr = sram.sram;
      case  MM_TARGET_BOOT_ROM: basePtr = boot_rom;
      case  MM_TARGET_WORK_RAM: basePtr = work_ram;
      case  MM_TARGET_CART_ROM: basePtr = cart.rom;
      case  MM_TARGET_CART_LOCK_ROM: basePtr = cart.lockrom;
      case  MM_TARGET_SVP_DRAM: basePtr = svp->dram;
      case  MM_TARGET_ACTION_REPLAY_RAM: basePtr = action_replay.ram;
    }
    
    // Getting offset with respect to target
    m68k.memory_map[i].base = &basePtr[offset];
  }

  /* hardware registers */
  load_param(cart.hw.regs, sizeof(cart.hw.regs));

  /* SVP */
  if (svp)
  {
    load_param(svp->iram_rom, 0x800);
    load_param(svp->dram,sizeof(svp->dram));
    load_param(&svp->ssp1601,sizeof(ssp1601_t));
  }

  /* MegaSD hardware */
  if (cart.special & HW_MEGASD)
  {
    bufferptr += megasd_context_load(&state[bufferptr]);
  }

  return bufferptr;
}



int state_save(unsigned char *state)
{
  /* bufferptr size */
  int bufferptr = 0;

  /* version string */
  char version[16];
  memcpy(version,STATE_VERSION,16);
  save_param(version, 16);

  /* GENESIS */
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    save_param(work_ram, sizeof(work_ram));
    save_param(zram, sizeof(zram));
    save_param(&zstate, sizeof(zstate));
    save_param(&zbank, sizeof(zbank));
  }
  else
  {
    save_param(work_ram, 0x2000);
  }

  /* IO */
  save_param(io_reg, sizeof(io_reg));

  /* VDP */
  bufferptr += vdp_context_save(&state[bufferptr]);

  /* SOUND */
  bufferptr += sound_context_save(&state[bufferptr]);

  /* 68000 */ 
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    uint16 tmp16;
    uint32 tmp32;
    tmp32 = m68k_get_reg(M68K_REG_D0);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D1);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D2);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D3);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D4);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D5);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D6);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_D7);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A0);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A1);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A2);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A3);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A4);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A5);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A6);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_A7);  save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_PC);  save_param(&tmp32, 4);
    tmp16 = m68k_get_reg(M68K_REG_SR);  save_param(&tmp16, 2); 
    tmp32 = m68k_get_reg(M68K_REG_USP); save_param(&tmp32, 4);
    tmp32 = m68k_get_reg(M68K_REG_ISP); save_param(&tmp32, 4);

    save_param(&m68k.cycles, sizeof(m68k.cycles));
    save_param(&m68k.int_level, sizeof(m68k.int_level));
    save_param(&m68k.stopped, sizeof(m68k.stopped));

    save_param(&m68k.poll              , sizeof(m68k.poll            ));   
    save_param(&m68k.cycles            , sizeof(m68k.cycles          )); 
    save_param(&m68k.refresh_cycles    , sizeof(m68k.refresh_cycles  )); 
    save_param(&m68k.cycle_end         , sizeof(m68k.cycle_end       )); 
    // save_param(&m68k.dar               , sizeof(m68k.dar             )); 
    // save_param(&m68k.pc                , sizeof(m68k.pc              )); 
    save_param(&m68k.prev_pc           , sizeof(m68k.prev_pc         )); 
    save_param(&m68k.prev_ar           , sizeof(m68k.prev_ar         )); 
    save_param(&m68k.sp                , sizeof(m68k.sp              )); 
    save_param(&m68k.ir                , sizeof(m68k.ir              )); 
    save_param(&m68k.t1_flag           , sizeof(m68k.t1_flag         )); 
    save_param(&m68k.s_flag            , sizeof(m68k.s_flag          )); 
    save_param(&m68k.x_flag            , sizeof(m68k.x_flag          )); 
    save_param(&m68k.n_flag            , sizeof(m68k.n_flag          )); 
    save_param(&m68k.not_z_flag        , sizeof(m68k.not_z_flag      )); 
    save_param(&m68k.v_flag            , sizeof(m68k.v_flag          )); 
    save_param(&m68k.c_flag            , sizeof(m68k.c_flag          )); 
    save_param(&m68k.int_mask          , sizeof(m68k.int_mask        )); 
    save_param(&m68k.int_level         , sizeof(m68k.int_level       )); 
    save_param(&m68k.stopped           , sizeof(m68k.stopped         )); 
    save_param(&m68k.pref_addr         , sizeof(m68k.pref_addr       )); 
    save_param(&m68k.pref_data         , sizeof(m68k.pref_data       )); 
    save_param(&m68k.instr_mode        , sizeof(m68k.instr_mode      )); 
    save_param(&m68k.run_mode          , sizeof(m68k.run_mode        )); 
    save_param(&m68k.aerr_enabled      , sizeof(m68k.aerr_enabled    )); 
    save_param(&m68k.aerr_trap         , sizeof(m68k.aerr_trap       )); 
    save_param(&m68k.aerr_address      , sizeof(m68k.aerr_address    )); 
    save_param(&m68k.aerr_write_mode   , sizeof(m68k.aerr_write_mode )); 
    save_param(&m68k.aerr_fc           , sizeof(m68k.aerr_fc         )); 
    save_param(&m68k.tracing           , sizeof(m68k.tracing         )); 
    save_param(&m68k.address_space     , sizeof(m68k.address_space   )); 
  }

  /* Z80 */ 
  save_param(&Z80, sizeof(Z80_Regs));

  /* External HW */
  if (system_hw == SYSTEM_MCD)
  {
    /* CD hardware ID flag */
    char id[4];
    memcpy(id,"SCD!",4);
    save_param(id, 4);

    /* CD hardware */
    bufferptr += scd_context_save(&state[bufferptr]);
  }
  else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    /* MD cartridge hardware */
    bufferptr += md_cart_context_save(&state[bufferptr]);
  }
  else
  {
    /* MS cartridge hardware */
    bufferptr += sms_cart_context_save(&state[bufferptr]);
  }


  /* return total size */
  return bufferptr;
}

int state_load(unsigned char *state)
{
  int i, bufferptr = 0;

  /* signature check (GENPLUS-GX x.x.x) */
  char version[17];
  load_param(version,16);
  version[16] = 0;
  if (memcmp(version,STATE_VERSION,11))
  {
    return 0;
  }

  /* version check */
  if ((version[11] < 0x31) || (version[13] < 0x37) || (version[15] < 0x35))
  {
    return 0;
  }

  /* reset system */
  system_reset();

  /* enable VDP access for TMSS systems */
  for (i=0xc0; i<0xe0; i+=8)
  {
    m68k.memory_map[i].read8    = vdp_read_byte;
    m68k.memory_map[i].read16   = vdp_read_word;
    m68k.memory_map[i].write8   = vdp_write_byte;
    m68k.memory_map[i].write16  = vdp_write_word;
    zbank_memory_map[i].read    = zbank_read_vdp;
    zbank_memory_map[i].write   = zbank_write_vdp;
  }

  /* GENESIS */
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    load_param(work_ram, sizeof(work_ram));
    load_param(zram, sizeof(zram));
    load_param(&zstate, sizeof(zstate));
    load_param(&zbank, sizeof(zbank));
    if (zstate == 3)
    {
      m68k.memory_map[0xa0].read8   = z80_read_byte;
      m68k.memory_map[0xa0].read16  = z80_read_word;
      m68k.memory_map[0xa0].write8  = z80_write_byte;
      m68k.memory_map[0xa0].write16 = z80_write_word;
    }
    else
    {
      m68k.memory_map[0xa0].read8   = m68k_read_bus_8;
      m68k.memory_map[0xa0].read16  = m68k_read_bus_16;
      m68k.memory_map[0xa0].write8  = m68k_unused_8_w;
      m68k.memory_map[0xa0].write16 = m68k_unused_16_w;
    }
  }
  else
  {
    load_param(work_ram, 0x2000);
  }

  /* IO */
  load_param(io_reg, sizeof(io_reg));
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    io_reg[0] = region_code | 0x20 | (config.bios & 1);
  }
  else
  {
    io_reg[0] = 0x80 | (region_code >> 1);
  }

  /* VDP */
  bufferptr += vdp_context_load(&state[bufferptr]);

  /* SOUND */
  bufferptr += sound_context_load(&state[bufferptr]);
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    psg_config(0, config.psg_preamp, 0xff);
  }
  else
  {
    psg_config(0, config.psg_preamp, io_reg[6]);
  }

  /* 68000 */
  if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {
    uint16 tmp16;
    uint32 tmp32;
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D0, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D1, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D2, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D3, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D4, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D5, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D6, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_D7, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A0, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A1, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A2, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A3, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A4, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A5, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A6, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_A7, tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_PC, tmp32);  
    load_param(&tmp16, 2); m68k_set_reg(M68K_REG_SR, tmp16);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_USP,tmp32);
    load_param(&tmp32, 4); m68k_set_reg(M68K_REG_ISP,tmp32);

    load_param(&m68k.cycles, sizeof(m68k.cycles));
    load_param(&m68k.int_level, sizeof(m68k.int_level));
    load_param(&m68k.stopped, sizeof(m68k.stopped));

    load_param(&m68k.poll              , sizeof(m68k.poll            ));   
    load_param(&m68k.cycles            , sizeof(m68k.cycles          )); 
    load_param(&m68k.refresh_cycles    , sizeof(m68k.refresh_cycles  )); 
    load_param(&m68k.cycle_end         , sizeof(m68k.cycle_end       )); 
    // load_param(&m68k.dar               , sizeof(m68k.dar             )); 
    // load_param(&m68k.pc                , sizeof(m68k.pc              )); 
    load_param(&m68k.prev_pc           , sizeof(m68k.prev_pc         )); 
    load_param(&m68k.prev_ar           , sizeof(m68k.prev_ar         )); 
    load_param(&m68k.sp                , sizeof(m68k.sp              )); 
    load_param(&m68k.ir                , sizeof(m68k.ir              )); 
    load_param(&m68k.t1_flag           , sizeof(m68k.t1_flag         )); 
    load_param(&m68k.s_flag            , sizeof(m68k.s_flag          )); 
    load_param(&m68k.x_flag            , sizeof(m68k.x_flag          )); 
    load_param(&m68k.n_flag            , sizeof(m68k.n_flag          )); 
    load_param(&m68k.not_z_flag        , sizeof(m68k.not_z_flag      )); 
    load_param(&m68k.v_flag            , sizeof(m68k.v_flag          )); 
    load_param(&m68k.c_flag            , sizeof(m68k.c_flag          )); 
    load_param(&m68k.int_mask          , sizeof(m68k.int_mask        )); 
    load_param(&m68k.int_level         , sizeof(m68k.int_level       )); 
    load_param(&m68k.stopped           , sizeof(m68k.stopped         )); 
    load_param(&m68k.pref_addr         , sizeof(m68k.pref_addr       )); 
    load_param(&m68k.pref_data         , sizeof(m68k.pref_data       )); 
    load_param(&m68k.instr_mode        , sizeof(m68k.instr_mode      )); 
    load_param(&m68k.run_mode          , sizeof(m68k.run_mode        )); 
    load_param(&m68k.aerr_enabled      , sizeof(m68k.aerr_enabled    )); 
    load_param(&m68k.aerr_trap         , sizeof(m68k.aerr_trap       )); 
    load_param(&m68k.aerr_address      , sizeof(m68k.aerr_address    )); 
    load_param(&m68k.aerr_write_mode   , sizeof(m68k.aerr_write_mode )); 
    load_param(&m68k.aerr_fc           , sizeof(m68k.aerr_fc         )); 
    load_param(&m68k.tracing           , sizeof(m68k.tracing         )); 
    load_param(&m68k.address_space     , sizeof(m68k.address_space   )); 
  }

  /* Z80 */ 
  load_param(&Z80, sizeof(Z80_Regs));
  Z80.irq_callback = z80_irq_callback;

  /* Extra HW */
  if (system_hw == SYSTEM_MCD)
  {
    /* handle case of MD cartridge using or not CD hardware */
    char id[5];
    load_param(id,4);
    id[4] = 0;

    /* check if CD hardware was enabled before attempting to restore */
    if (memcmp(id,"SCD!",4))
    {
       return 0;
    }

    /* CD hardware */
    bufferptr += scd_context_load(&state[bufferptr], version);
  }
  else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
  {  
    /* MD cartridge hardware */
    bufferptr += md_cart_context_load(&state[bufferptr]);
  }
  else
  {
    /* MS cartridge hardware */
    bufferptr += sms_cart_context_load(&state[bufferptr]);
    sms_cart_switch(~io_reg[0x0E]);
  }

  return bufferptr;
}

void printMemoryMapChecksum()
{
  uint8_t* ptr = m68k.memory_map;
  size_t size = sizeof(m68k.memory_map);
  uint64_t checksum = 0;
  for (size_t i = 0; i < size; i++)
  {
    checksum += ptr[i];
  }
  printf("Checksum: 0x%lX\n", checksum);
}