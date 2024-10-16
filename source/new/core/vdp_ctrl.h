/***************************************************************************************
 *  Genesis Plus
 *  Video Display Processor (68k & Z80 CPU interface)
 *
 *  Support for SG-1000 (TMS99xx & 315-5066), Master System (315-5124 & 315-5246), Game Gear & Mega Drive VDP
 *
 *  Copyright (C) 1998-2003  Charles Mac Donald (original code)
 *  Copyright (C) 2007-2024  Eke-Eke (Genesis Plus GX)
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

/* Function pointers */
extern void (*vdp_68k_data_w)(unsigned int data);
extern void (*vdp_z80_data_w)(unsigned int data);
extern unsigned int (*vdp_68k_data_r)(void);
extern unsigned int (*vdp_z80_data_r)(void);

/* Function prototypes */
extern void vdp_init(void);
extern void vdp_reset(void);
extern int vdp_context_save(uint8_t *state);
extern int vdp_context_load(uint8_t *state);
extern void vdp_dma_update(unsigned int cycles);
extern void vdp_68k_ctrl_w(unsigned int data);
extern void vdp_z80_ctrl_w(unsigned int data);
extern void vdp_sms_ctrl_w(unsigned int data);
extern void vdp_tms_ctrl_w(unsigned int data);
extern unsigned int vdp_68k_ctrl_r(unsigned int cycles);
extern unsigned int vdp_z80_ctrl_r(unsigned int cycles);
extern unsigned int vdp_hvc_r(unsigned int cycles);
extern void vdp_test_w(unsigned int data);
extern int vdp_68k_irq_ack(int int_level);

extern void (*const render_bg_modes[16])(int line);

extern void vdp_68k_data_w_m4(unsigned int data);
extern void vdp_68k_data_w_m5(unsigned int data);
extern unsigned int vdp_68k_data_r_m4(void);
extern unsigned int vdp_68k_data_r_m5(void);
extern void vdp_z80_data_w_m4(unsigned int data);
extern void vdp_z80_data_w_m5(unsigned int data);
extern unsigned int vdp_z80_data_r_m4(void);
extern unsigned int vdp_z80_data_r_m5(void);
extern void vdp_z80_data_w_ms(unsigned int data);
extern void vdp_z80_data_w_gg(unsigned int data);
extern void vdp_z80_data_w_sg(unsigned int data);
extern void vdp_bus_w(unsigned int data);
extern void vdp_reg_w(unsigned int r, unsigned int d, unsigned int cycles);
extern void vdp_dma_68k_ext(unsigned int length);
extern void vdp_dma_68k_ram(unsigned int length);
extern void vdp_dma_68k_io(unsigned int length);
extern void vdp_dma_copy(unsigned int length);
extern void vdp_dma_fill(unsigned int length);