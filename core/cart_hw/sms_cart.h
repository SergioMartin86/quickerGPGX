/****************************************************************************
 *  Genesis Plus
 *  SG-1000, Master System & Game Gear cartridge hardware support
 *
 *  Copyright (C) 2007-2023  Eke-Eke (Genesis Plus GX)
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

/* Special hardware */
#define HW_3D_GLASSES     0x01
#define HW_TEREBI_OEKAKI  0x02

#define MAPPER_NONE           (0x00)
#define MAPPER_TEREBI         (0x01)
#define MAPPER_RAM_2K         (0x02)
#define MAPPER_RAM_8K         (0x03)
#define MAPPER_RAM_8K_EXT1    (0x04)
#define MAPPER_SEGA           (0x10)
#define MAPPER_SEGA_X         (0x11)
#define MAPPER_93C46          (0x12)
#define MAPPER_CODIES         (0x13)
#define MAPPER_MULTI_16K      (0x14)
#define MAPPER_KOREA          (0x15)
#define MAPPER_KOREA_16K      (0x16)
#define MAPPER_MULTI_2x16K_V1 (0x17)
#define MAPPER_MULTI_2x16K_V2 (0x18)
#define MAPPER_MULTI_16K_32K  (0x19)
#define MAPPER_ZEMINA_16K_32K (0x1A)
#define MAPPER_HWASUNG        (0x1B)
#define MAPPER_KOREA_8K       (0x20)
#define MAPPER_MSX            (0x21)
#define MAPPER_MSX_NEMESIS    (0x22)
#define MAPPER_MULTI_8K       (0x23)
#define MAPPER_MULTI_4x8K     (0x24)
#define MAPPER_ZEMINA_4x8K    (0x25)
#define MAPPER_MULTI_32K      (0x40)
#define MAPPER_MULTI_32K_16K  (0x41)
#define MAPPER_HICOM          (0x42)

typedef struct
{
  uint32_t crc;
  uint8_t g_3d;
  uint8_t fm;
  uint8_t peripheral;
  uint8_t mapper;
  uint8_t system;
  uint8_t region;
} rominfo_t;

typedef struct
{
  uint8_t fcr[4];
  uint8_t mapper;
  uint16_t pages;
} romhw_t;

/* Function prototypes */
extern void sms_cart_init(void);
extern void sms_cart_reset(void);
extern void sms_cart_switch(uint8_t mode);
extern int sms_cart_ram_size(void);
extern int sms_cart_region_detect(void);
extern int sms_cart_context_save(uint8_t *state);
extern int sms_cart_context_load(uint8_t *state);



