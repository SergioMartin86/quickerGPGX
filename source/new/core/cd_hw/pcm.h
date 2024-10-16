/***************************************************************************************
 *  Genesis Plus
 *  PCM sound chip (315-5476A) (RF5C164 compatible)
 *
 *  Copyright (C) 2012-2023  Eke-Eke (Genesis Plus GX)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/
#pragma once

#include "../sound/blip_buf.h"

#define pcm scd.pcm_hw

/* PCM channel */
typedef struct
{
  uint32_t addr;  /* current Wave RAM address (16.11 fixed point) */
  uint32_t st;    /* Wave RAM start address (16.11 fixed point) */
  reg16_t ls;   /* Wave RAM loop address ($0000-$ffff) */
  reg16_t fd;   /* Wave RAM address increment (5.11 fixed point) */
  uint8_t env;    /* enveloppe multiplier */
  uint8_t pan;    /* stereo panning */
} chan_t;

/* PCM sound chip */
typedef struct
{
  chan_t chan[8];     /* PCM channels 1-8 */
  int16_t out[2];       /* previous PCM stereo output */
  uint8_t *bank;        /* external RAM bank pointer */
  uint8_t enabled;      /* PCM chip ON/OFF status */
  uint8_t status;       /* channels ON/OFF status */
  uint8_t index;        /* current channel index */
  uint8_t ram[0x10000]; /* 64k external RAM */
  uint32_t cycles;
} pcm_t;

/* Function prototypes */
extern void pcm_init(double clock, int rate);
extern void pcm_reset(void);
extern int pcm_context_save(uint8_t *state);
extern int pcm_context_load(uint8_t *state);
extern void pcm_update(unsigned int samples);
extern void pcm_write(unsigned int address, unsigned char data, unsigned int cycles);
extern unsigned char pcm_read(unsigned int address, unsigned int cycles);
extern void pcm_ram_dma_w(unsigned int length);

