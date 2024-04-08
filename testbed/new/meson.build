# newGPGX Core sources

newGPGXSrc =  [
   'main.c',
   'fileio.c',
   'unzip.c',
   '../../core/sound/sound.c',
   '../../core/sound/ym3438.c',
   '../../core/sound/opll.c',
   '../../core/sound/eq.c',
   '../../core/sound/ym2413.c',
   '../../core/sound/blip_buf.c',
   '../../core/sound/psg.c',
   '../../core/sound/ym2612.c',
   '../../core/membnk.c',
   '../../core/vdp_ctrl.c',
   '../../core/z80/z80.c',
   '../../core/io_ctrl.c',
   '../../core/ntsc/sms_ntsc.c',
   '../../core/ntsc/md_ntsc.c',
   '../../core/m68k/s68kcpu.c',
   '../../core/m68k/m68kcpu.c',
   '../../core/memz80.c',
   '../../core/genesis.c',
   '../../core/vdp_render.c',
   '../../core/system.c',
   '../../core/cd_hw/cd_cart.c',
   '../../core/cd_hw/cdc.c',
   '../../core/cd_hw/pcm.c',
   '../../core/cd_hw/gfx.c',
   '../../core/cd_hw/scd.c',
   '../../core/cd_hw/cdd.c',
   '../../core/input_hw/sportspad.c',
   '../../core/input_hw/activator.c',
   '../../core/input_hw/mouse.c',
   '../../core/input_hw/paddle.c',
   '../../core/input_hw/gamepad.c',
   '../../core/input_hw/input.c',
   '../../core/input_hw/terebi_oekaki.c',
   '../../core/input_hw/teamplayer.c',
   '../../core/input_hw/graphic_board.c',
   '../../core/input_hw/lightgun.c',
   '../../core/input_hw/xe_1ap.c',
   '../../core/mem68k.c',
   '../../core/cart_hw/svp/ssp16.c',
   '../../core/cart_hw/svp/svp.c',
   '../../core/cart_hw/areplay.c',
   '../../core/cart_hw/eeprom_spi.c',
   '../../core/cart_hw/sram.c',
   '../../core/cart_hw/eeprom_93c.c',
   '../../core/cart_hw/sms_cart.c',
   '../../core/cart_hw/eeprom_i2c.c',
   '../../core/cart_hw/ggenie.c',
   '../../core/cart_hw/md_cart.c',
   '../../core/cart_hw/megasd.c',
   '../../core/loadrom.c',
]

newGPGXIncludeDirs = [
  '.',
  '../../core',
  '../../core/cart_hw',
  '../../core/cart_hw/svp',
  '../../core/cd_hw',
  '../../core/input_hw',
  '../../core/m68k',
  '../../core/ntsc',
  '../../core/sound',
  '../../core/z80',
]

newGPGXCompileArgs = [
   '-DLSB_FIRST',
   '-DUSE_32BPP_RENDERING',
   '-DHAVE_YM3438_CORE',
   '-DHAVE_OPLL_CORE',
   '-Wno-unused-result'
]

# newGPGX Core Configuration

 newGPGXDependency = declare_dependency(
  compile_args        : [  newGPGXCompileArgs ],
  include_directories : include_directories(newGPGXIncludeDirs),
  sources             : [ newGPGXSrc ],
  dependencies        : [  
                          dependency('sdl2',  required : true),
                          dependency('zlib',  required : true)
                        ]
 )
