/***************************************************************************************
 *  Genesis Plus
 *  Input peripherals support
 *
 *  Copyright (C) 1998-2003  Charles Mac Donald (original code)
 *  Copyright (C) 2007-2016  Eke-Eke (Genesis Plus GX)
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

#include "../io_ctrl.h"

/* Max. number of devices */
#define MAX_DEVICES (8)

/* Ports configuration */
#define NO_SYSTEM              (0) /* unconnected port*/
#define SYSTEM_GAMEPAD         (1) /* 2-buttons, 3-buttons or 6-buttons Control Pad */
#define SYSTEM_MOUSE           (2) /* Sega Mouse (only supported in either port A or port B) */
#define SYSTEM_MENACER         (3) /* Sega Menacer (only supported in port B) */
#define SYSTEM_JUSTIFIER       (4) /* Konami Justifiers (only supported in port B) */
#define SYSTEM_XE_1AP          (5) /* XE-1AP analog controller */
#define SYSTEM_ACTIVATOR       (6) /* Sega Activator */
#define SYSTEM_LIGHTPHASER     (7) /* Sega Light Phaser */
#define SYSTEM_PADDLE          (8) /* Sega Paddle Control */
#define SYSTEM_SPORTSPAD       (9) /* Sega Sports Pad */
#define SYSTEM_GRAPHIC_BOARD  (10) /* Sega Graphic Board */
#define SYSTEM_MASTERTAP      (11) /* Multi Tap -- Furrtek's Master Tap (unofficial) */
#define SYSTEM_TEAMPLAYER     (12) /* Multi Tap -- Sega TeamPlayer */
#define SYSTEM_WAYPLAY        (13) /* Multi Tap -- EA 4-Way Play (use both ports) */

/* Device type */
#define NO_DEVICE             (0xff) /* unconnected device (fixed ID for Team Player) */
#define DEVICE_PAD3B          (0x00) /* 3-buttons Control Pad (fixed ID for Team Player)*/
#define DEVICE_PAD6B          (0x01) /* 6-buttons Control Pad (fixed ID for Team Player) */
#define DEVICE_PAD2B          (0x02) /* 2-buttons Control Pad */
#define DEVICE_MOUSE          (0x03) /* Sega Mouse */
#define DEVICE_LIGHTGUN       (0x04) /* Sega Light Phaser, Menacer or Konami Justifiers */
#define DEVICE_PADDLE         (0x05) /* Sega Paddle Control */
#define DEVICE_SPORTSPAD      (0x06) /* Sega Sports Pad */
#define DEVICE_GRAPHIC_BOARD  (0x07) /* Sega Graphic Board */
#define DEVICE_PICO           (0x08) /* PICO tablet */
#define DEVICE_TEREBI         (0x09) /* Terebi Oekaki tablet */
#define DEVICE_XE_1AP         (0x0a) /* XE-1AP analog controller */
#define DEVICE_ACTIVATOR      (0x0b) /* Activator */

/* Default Input bitmasks */
#define INPUT_MODE         (0x0800)
#define INPUT_X            (0x0400)
#define INPUT_Y            (0x0200)
#define INPUT_Z            (0x0100)
#define INPUT_START        (0x0080)
#define INPUT_A            (0x0040)
#define INPUT_C            (0x0020)
#define INPUT_B            (0x0010)
#define INPUT_RIGHT        (0x0008)
#define INPUT_LEFT         (0x0004)
#define INPUT_DOWN         (0x0002)
#define INPUT_UP           (0x0001)

/* Master System specific bitmasks */
#define INPUT_BUTTON2      (0x0020)
#define INPUT_BUTTON1      (0x0010)

/* Mega Mouse specific bitmask */
#define INPUT_MOUSE_CENTER (0x0040)
#define INPUT_MOUSE_RIGHT  (0x0020)
#define INPUT_MOUSE_LEFT   (0x0010)

/* Pico hardware specific bitmask */
#define INPUT_PICO_PEN     (0x0080)
#define INPUT_PICO_RED     (0x0010)

/* XE-1AP specific bitmask */
#define INPUT_XE_E1        (0x2000)
#define INPUT_XE_E2        (0x1000)
#define INPUT_XE_START     (0x0800)
#define INPUT_XE_SELECT    (0x0400)
#define INPUT_XE_A         (0x0200)
#define INPUT_XE_B         (0x0100)
#define INPUT_XE_A2        (0x0080)
#define INPUT_XE_B2        (0x0040)
#define INPUT_XE_C         (0x0020)
#define INPUT_XE_D         (0x0010)

/* Activator specific bitmasks */
#define INPUT_ACTIVATOR_8U (0x8000)
#define INPUT_ACTIVATOR_8L (0x4000)
#define INPUT_ACTIVATOR_7U (0x2000)
#define INPUT_ACTIVATOR_7L (0x1000)
#define INPUT_ACTIVATOR_6U (0x0800)
#define INPUT_ACTIVATOR_6L (0x0400)
#define INPUT_ACTIVATOR_5U (0x0200)
#define INPUT_ACTIVATOR_5L (0x0100)
#define INPUT_ACTIVATOR_4U (0x0080)
#define INPUT_ACTIVATOR_4L (0x0040)
#define INPUT_ACTIVATOR_3U (0x0020)
#define INPUT_ACTIVATOR_3L (0x0010)
#define INPUT_ACTIVATOR_2U (0x0008)
#define INPUT_ACTIVATOR_2L (0x0004)
#define INPUT_ACTIVATOR_1U (0x0002)
#define INPUT_ACTIVATOR_1L (0x0001)

/* Graphic Board specific bitmasks */
#define INPUT_GRAPHIC_PEN  (0x0004)
#define INPUT_GRAPHIC_DO   (0x0002)
#define INPUT_GRAPHIC_MENU (0x0001)

typedef struct
{
  uint8_t system[2];              /* can be one of the SYSTEM_* values */
  uint8_t dev[MAX_DEVICES];       /* can be one of the DEVICE_* values */
  uint16_t pad[MAX_DEVICES];      /* digital inputs (any of INPUT_* values)  */
  int16_t analog[MAX_DEVICES][2]; /* analog inputs (x/y) */
  int x_offset;                 /* gun horizontal offset */
  int y_offset;                 /* gun vertical offset */
} t_input;

/* Function prototypes */
extern void input_init(void);
extern void input_reset(void);
extern void input_refresh(void);
extern void input_end_frame(unsigned int cycles);

