# gpgx Core sources

gpgxSrc =  [
  './core/cinterface/cinterface.c',
  './core/util/scrc32.c',
  './core/core/vdp_ctrl.c',
  './core/core/io_ctrl.c',
  './core/core/m68k/s68kcpu.c',
  './core/core/m68k/m68kcpu.c',
  './core/core/membnk.c',
  './core/core/cd_hw/pcm.c',
  './core/core/cd_hw/gfx.c',
  './core/core/cd_hw/scd.c',
  './core/core/cd_hw/cd_cart.c',
  './core/core/cd_hw/cdd.c',
  './core/core/cd_hw/cdc.c',
  './core/core/genesis.c',
  './core/core/input_hw/gamepad.c',
  './core/core/input_hw/terebi_oekaki.c',
  './core/core/input_hw/lightgun.c',
  './core/core/input_hw/xe_a1p.c',
  './core/core/input_hw/input.c',
  './core/core/input_hw/mouse.c',
  './core/core/input_hw/sportspad.c',
  './core/core/input_hw/teamplayer.c',
  './core/core/input_hw/paddle.c',
  './core/core/input_hw/activator.c',
  './core/core/z80/z80.c',
  './core/core/loadrom.c',
  './core/core/memz80.c',
  './core/core/mem68k.c',
  './core/core/cart_hw/sms_cart.c',
  './core/core/cart_hw/areplay.c',
  './core/core/cart_hw/eeprom_spi.c',
  './core/core/cart_hw/eeprom_i2c.c',
  './core/core/cart_hw/svp/ssp16.c',
  './core/core/cart_hw/svp/svp.c',
  './core/core/cart_hw/ggenie.c',
  './core/core/cart_hw/sram.c',
  './core/core/cart_hw/eeprom_93c.c',
  './core/core/cart_hw/md_cart.c',
  './core/core/sound/blip_buf.c',
  './core/core/sound/ym2413.c',
  './core/core/sound/sn76489.c',
  './core/core/sound/ym2612.c',
  './core/core/sound/eq.c',
  './core/core/sound/sound.c',
  './core/core/ntsc/md_ntsc.c',
  './core/core/ntsc/sms_ntsc.c',
  './core/core/vdp_render.c',
  './core/core/system.c',
]

gpgxIncludeDirs = [
  '.',
  'core/cinterface',
  'core/util',
  'core/core',
  'core/core/cart_hw',
  'core/core/cd_hw',
  'core/core/input_hw',
  'core/core/m68k',
  'core/core/ntsc',
  'core/core/sound',
  'core/core/z80',
]

gpgxCompileFlags = [
  '-DLSB_FIRST',
  '-DUSE_32BPP_RENDERING'
]

# gpgx Core Configuration

 gpgxDependency = declare_dependency(
  compile_args        : [  gpgxCompileFlags ],
  include_directories : include_directories(gpgxIncludeDirs),
  sources             : [ gpgxSrc ]
 )


