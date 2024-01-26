
// #ifndef _CONFIG_H_
// #define _CONFIG_H_

// #include "shared.h"

// /****************************************************************************
//  * Config Option 
//  *
//  ****************************************************************************/
// typedef struct
// {
//   int8 device;
//   uint8 port;
//   uint8 padtype;
// } t_input_config;

// struct
// {
//   char version[16];
//   uint8 hq_fm;
//   uint8 filter;
//   uint8 psgBoostNoise;
//   uint8 dac_bits;
//   uint8 ym2413;
//   uint8 mono;
//   int16 psg_preamp;
//   int16 fm_preamp;
//   uint16 lp_range;
//   int16 low_freq;
//   int16 high_freq;
//   int16 lg;
//   int16 mg;
//   int16 hg;
//   uint8 system;
//   uint8 region_detect;
//   uint8 master_clock;
//   uint8 vdp_mode;
//   uint8 force_dtack;
//   uint8 addr_error;
//   uint8 bios;
//   uint8 lock_on;
//   uint8 overscan;
//   uint8 ntsc;
//   uint8 gg_extra;
//   uint8 render;
//   t_input_config input[MAX_INPUTS];
//   uint8 no_sprite_limit;
// } config;

// /* Global variables */
// extern __thread t_config config;

// #ifdef __cplusplus
// extern "C" {
// #else
// extern
// #endif
// void set_config_defaults(void);
// #ifdef __cplusplus
// }
// #endif

// #endif /* _CONFIG_H_ */

