/*
   basic, incomplete SSP160x (SSP1601?) interpreter
   with SVP memory controller emu

   (c) Copyright 2008, Grazvydas "notaz" Ignotas
   Free for non-commercial use.

   For commercial use, separate licencing terms must be obtained.

   Modified for Genesis Plus GX (Eke-Eke): added BIG ENDIAN support, fixed addr/code inversion
*/

#pragma once

#include "ssp16.h"

typedef struct {
  unsigned char iram_rom[0x20000]; /* IRAM (0-0x7ff) and program ROM (0x800-0x1ffff) */
  unsigned char dram[0x20000];
  ssp1601_t ssp1601;
} svp_t;

extern svp_t *svp;

extern void svp_init(void);
extern void svp_reset(void);
extern void svp_set_mappings(void);
