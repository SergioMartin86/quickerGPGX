#include <string.h>
#include "state.h"

uint8_t VRAMBlockEnabled = 1;
void enableVRAMBlock() { VRAMBlockEnabled = 1; };
void disableVRAMBlock() { VRAMBlockEnabled = 0; };

uint8_t SATMBlockEnabled = 1;
void enableSATMBlock() { SATMBlockEnabled = 1; };
void disableSATMBlock() { SATMBlockEnabled = 0; };

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

__thread romhw_t cart_rom;
__thread romhw_t bios_rom;

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

int scd_context_save(uint8 *state);
int scd_context_load(uint8 *state, char *version);
int vdp_context_save(uint8 *state);
int vdp_context_load(uint8 *state);
int sms_cart_context_save(uint8 *state);
int cdc_context_save(uint8 *state);
int cdc_context_load(uint8 *state);
int sound_context_save(uint8 *state);
int sound_context_load(uint8 *state);
int pcm_context_save(uint8 *state);
int pcm_context_load(uint8 *state);
int gfx_context_save(uint8 *state);
int gfx_context_load(uint8 *state);
int cdd_context_save(uint8 *state);
int cdd_context_load(uint8 *state, char *version);
int sms_cart_context_load(uint8 *state);
int megasd_context_save(uint8 *state);
int megasd_context_load(uint8 *state);
int md_cart_context_save(uint8 *state);
int md_cart_context_load(uint8 *state);


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

  if (SATMBlockEnabled == 1) { save_param(sat, sizeof(sat)); }
  if (VRAMBlockEnabled == 1) { save_param(vram, sizeof(vram)); }
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

  if (SATMBlockEnabled == 1) { load_param(sat, sizeof(sat)); }
  if (VRAMBlockEnabled == 1) { load_param(vram, sizeof(vram)); }
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
  // save_param(&cdc.ram, sizeof(cdc.ram));
  save_param(&tmp8, 1);

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
  // load_param(&cdc.ram, sizeof(cdc.ram));

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

int sound_context_save(uint8 *state)
{
  int bufferptr = 0;
  
  save_param(&fm_cycles_start,sizeof(fm_cycles_start));

  return bufferptr;
}

int sound_context_load(uint8 *state)
{
  int bufferptr = 0;


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
  // save_param(pcm.ram, sizeof(pcm.ram));

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
  // load_param(pcm.ram, sizeof(pcm.ram));

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

  /* cartridge mapping */
  for (i=0; i<0x40; i++)
  {
    /* get base address */
    base = m68k.memory_map[i].base;

    if (base == sram.sram)
    {
      /* SRAM */
      state[bufferptr++] = 0xff;
    }
    else if (base == boot_rom)
    {
      /* Boot ROM */
      state[bufferptr++] = 0xfe;
    }
    else
    {
      /* Cartridge ROM */
      state[bufferptr++] = ((base - cart.rom) >> 16) & 0xff;
    }
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

  /* cartridge mapping */
  for (i=0; i<0x40; i++)
  {
    /* get offset */
    offset = state[bufferptr++];

    if (offset == 0xff)
    {
      /* SRAM */
      m68k.memory_map[i].base     = sram.sram;
      m68k.memory_map[i].read8    = sram_read_byte;
      m68k.memory_map[i].read16   = sram_read_word;
      m68k.memory_map[i].write8   = sram_write_byte;
      m68k.memory_map[i].write16  = sram_write_word;
      zbank_memory_map[i].read    = sram_read_byte;
      zbank_memory_map[i].write   = sram_write_byte;

    }
    else
    {
      /* check if SRAM was mapped there before loading state */
      if (m68k.memory_map[i].base == sram.sram)
      {
        m68k.memory_map[i].read8    = NULL;
        m68k.memory_map[i].read16   = NULL;
        m68k.memory_map[i].write8   = m68k_unused_8_w;
        m68k.memory_map[i].write16  = m68k_unused_16_w;
        zbank_memory_map[i].read    = NULL;
        zbank_memory_map[i].write   = zbank_unused_w;
      }

      /* ROM */
      m68k.memory_map[i].base = (offset == 0xfe) ? boot_rom : (cart.rom + (offset << 16));
    }
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

  /* SYSTEM */
  load_param(&pause_b, sizeof(pause_b));

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

  /* CONTROLLERS */
  load_param(gamepad, sizeof(gamepad));

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
    load_param(&m68k.refresh_cycles, sizeof(m68k.refresh_cycles));
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

int state_save(unsigned char *state)
{
  /* buffer size */
  int bufferptr = 0;

  /* version string */
  char version[16];
  memcpy(version,STATE_VERSION,16);
  save_param(version, 16);

  /* SYSTEM */
  save_param(&pause_b, sizeof(pause_b));

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
  
  /* CONTROLLERS */
  save_param(gamepad, sizeof(gamepad));
  
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
    save_param(&m68k.refresh_cycles, sizeof(m68k.refresh_cycles));
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