/***************************************************************************************
 *  Genesis Plus
 *  2-Buttons, 3-Buttons & 6-Buttons controller support
 *  with support for J-Cart, 4-Way Play & Master Tap adapters
 *
 *  Copyright (C) 2007-2019  Eke-Eke (Genesis Plus GX)
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

#include <stdint.h>

struct gamepad_t
{
  uint8_t State;
  uint8_t Counter;
  uint8_t Timeout;
  uint32_t Latency;
};

struct flipflop_t
{
  uint8_t Latch;
  uint8_t Counter;
};

/* Function prototypes */
extern void gamepad_reset(int port);
extern void gamepad_refresh(int port);
extern void gamepad_end_frame(int port, unsigned int cycles);
extern unsigned char gamepad_1_read(void);
extern unsigned char gamepad_2_read(void);
extern void gamepad_1_write(unsigned char data, unsigned char mask);
extern void gamepad_2_write(unsigned char data, unsigned char mask);
extern unsigned char wayplay_1_read(void);
extern unsigned char wayplay_2_read(void);
extern void wayplay_1_write(unsigned char data, unsigned char mask);
extern void wayplay_2_write(unsigned char data, unsigned char mask);
extern unsigned int jcart_read(unsigned int address);
extern void jcart_write(unsigned int address, unsigned int data);
extern unsigned char mastertap_1_read(void);
extern unsigned char mastertap_2_read(void);
extern void mastertap_1_write(unsigned char data, unsigned char mask);
extern void mastertap_2_write(unsigned char data, unsigned char mask);

