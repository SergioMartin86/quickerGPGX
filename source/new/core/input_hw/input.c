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

#include "../loadrom.h"
#include "../system.h"
#include "../genesis.h"
#include "gamepad.h"
#include "lightgun.h"
#include "mouse.h"
#include "activator.h"
#include "xe_1ap.h"
#include "teamplayer.h"
#include "paddle.h"
#include "sportspad.h"
#include "terebi_oekaki.h"
#include "graphic_board.h"
#include "input.h"
#include "state.h"

void input_init(void)
{
  int i, padtype;
  int player = 0;

  for (i=0; i<MAX_DEVICES; i++)
  {
    input.dev[i] = NO_DEVICE;
    input.pad[i] = 0;
  }

  /* PICO tablet */
  if (system_hw == SYSTEM_PICO)
  {
    input.dev[0] = DEVICE_PICO;
    return;
  }

  /* Terebi Oekaki tablet */
  if (cart.special & HW_TEREBI_OEKAKI)
  {
    input.dev[0] = DEVICE_TEREBI;
    return;
  }

  /* default gamepad type */
  if (romtype & SYSTEM_MD)
  {
    /* 3-buttons or 6-buttons */
    padtype = (rominfo.peripherals & 2) ? DEVICE_PAD6B : DEVICE_PAD3B;
  }
  else
  {
    /* 2-buttons */
    padtype = DEVICE_PAD2B;
  }

  switch (input.system[0])
  {
    case SYSTEM_GAMEPAD:
    {
      /* 2-buttons, 3-buttons or 6-buttons control pad */
      if (config.input[player].padtype != (DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B))
      {
        input.dev[0] = config.input[player].padtype;
      }
      else
      {
        input.dev[0] = padtype;
      }
      player++;
      break;
    }

    case SYSTEM_MOUSE:
    {
      input.dev[0] = DEVICE_MOUSE;
      player++;
      break;
    }

    case SYSTEM_ACTIVATOR:
    {
      input.dev[0] = DEVICE_ACTIVATOR;
      player++;
      break;
    }

    case SYSTEM_XE_1AP:
    {
      input.dev[0] = DEVICE_XE_1AP;
      player++;
      break;
    }

    case SYSTEM_WAYPLAY:
    {
      for (i=0; i< 4; i++)
      {
        if (player < MAX_INPUTS)
        {
          /* only allow 3-buttons or 6-buttons control pad */
          if (config.input[player].padtype != (DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B))
          {
            input.dev[i] = config.input[player].padtype & DEVICE_PAD6B;
          }
          else
          {
            input.dev[i] = padtype & DEVICE_PAD6B;
          }
          player++;
        }
      }
      break;
    }

    case SYSTEM_TEAMPLAYER:
    {
      for (i=0; i<4; i++)
      {
        if (player < MAX_INPUTS)
        {
          /* only allow 3-buttons or 6-buttons control pad */
          if (config.input[player].padtype != (DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B))
          {
            input.dev[i] = config.input[player].padtype & DEVICE_PAD6B;
          }
          else
          {
            input.dev[i] = padtype & DEVICE_PAD6B;
          }
          player++;
        }
      }
      teamplayer_init(0);
      break;
    }

    case SYSTEM_MASTERTAP:
    {
      for (i=0; i<4; i++)
      {
        if (player < MAX_INPUTS)
        {
          input.dev[i] = DEVICE_PAD2B;
          player++;
        }
      }
      break;
    }

    case SYSTEM_LIGHTPHASER:
    {
      input.dev[0] = DEVICE_LIGHTGUN;
      player++;
      break;
    }

    case SYSTEM_PADDLE:
    {
      input.dev[0] = DEVICE_PADDLE;
      player++;
      break;
    }

    case SYSTEM_SPORTSPAD:
    {
      input.dev[0] = DEVICE_SPORTSPAD;
      player++;
      break;
    }

    case SYSTEM_GRAPHIC_BOARD:
    {
      input.dev[0] = DEVICE_GRAPHIC_BOARD;
      player++;
      break;
    }
  }

  if (player == MAX_INPUTS)
  {
    return;
  }

  switch (input.system[1])
  {
    case SYSTEM_GAMEPAD:
    {
      if (config.input[player].padtype != (DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B))
      {
        input.dev[4] = config.input[player].padtype;
      }
      else
      {
        input.dev[4] = padtype;
      }
      player++;
      break;
    }

    case SYSTEM_MOUSE:
    {
      input.dev[4] = DEVICE_MOUSE;
      player++;
      break;
    }

    case SYSTEM_ACTIVATOR:
    {
      input.dev[4] = DEVICE_ACTIVATOR;
      player++;
      break;
    }

    case SYSTEM_XE_1AP:
    {
      input.dev[4] = DEVICE_XE_1AP;
      player++;
      break;
    }

    case SYSTEM_MENACER:
    {
      input.dev[4] = DEVICE_LIGHTGUN;
      player++;
      break;
    }

    case SYSTEM_JUSTIFIER:
    {
      for (i=4; i<6; i++)
      {
        if (player < MAX_INPUTS)
        {
          input.dev[i] = DEVICE_LIGHTGUN;
          player++;
        }
      }
      break;
    }

    case SYSTEM_TEAMPLAYER:
    {
      for (i=4; i<8; i++)
      {
        if (player < MAX_INPUTS)
        {
          /* only allow 3-buttons or 6-buttons control pad */
          if (config.input[player].padtype != (DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B))
          {
            input.dev[i] = config.input[player].padtype & DEVICE_PAD6B;
          }
          else
          {
            input.dev[i] = padtype & DEVICE_PAD6B;
          }
          player++;
        }
      }
      teamplayer_init(1);
      break;
    }

    case SYSTEM_MASTERTAP:
    {
      for (i=4; i<8; i++)
      {
        if (player < MAX_INPUTS)
        {
          input.dev[i] = DEVICE_PAD2B;
          player++;
        }
      }
      break;
    }

    case SYSTEM_LIGHTPHASER:
    {
      input.dev[4] = DEVICE_LIGHTGUN;
      player++;
      break;
    }

    case SYSTEM_PADDLE:
    {
      input.dev[4] = DEVICE_PADDLE;
      player++;
      break;
    }

    case SYSTEM_SPORTSPAD:
    {
      input.dev[4] = DEVICE_SPORTSPAD;
      player++;
      break;
    }

    case SYSTEM_GRAPHIC_BOARD:
    {
      input.dev[4] = DEVICE_GRAPHIC_BOARD;
      player++;
      break;
    }
  }

  /* J-CART */
  if (cart.special & HW_J_CART)
  {
    /* two additional gamepads */
    for (i=5; i<7; i++)
    {
      if (player < MAX_INPUTS)
      {
        /* only allow 3-buttons or 6-buttons control pad */
        if (config.input[player].padtype != (DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B))
        {
          input.dev[i] = config.input[player].padtype & DEVICE_PAD6B;
        }
        else
        {
          input.dev[i] = padtype & DEVICE_PAD6B;
        }
        player ++;
      }
    }
  }
}

void input_reset(void)
{
  /* Reset input devices */
  int i;
  for (i=0; i<MAX_DEVICES; i++)
  {
    switch (input.dev[i])
    {
      case DEVICE_PAD2B:
      case DEVICE_PAD3B:
      case DEVICE_PAD6B:
      {
        gamepad_reset(i);
        break;
      }

      case DEVICE_LIGHTGUN:
      {
        lightgun_reset(i);
        break;
      }

      case DEVICE_MOUSE:
      {
        mouse_reset(i);
        break;
      }

      case DEVICE_ACTIVATOR:
      {
        activator_reset(i >> 2);
        break;
      }

      case DEVICE_XE_1AP:
      {
        xe_1ap_reset(i);
        break;
      }

      case DEVICE_PADDLE:
      {
        paddle_reset(i);
        break;
      }

      case DEVICE_SPORTSPAD:
      {
        sportspad_reset(i);
        break;
      }

      case DEVICE_TEREBI:
      {
        terebi_oekaki_reset();
        break;
      }

      case DEVICE_GRAPHIC_BOARD:
      {
        graphic_board_reset(i);
        break;
      }

      default:
      {
        break;
      }
    }
  }

  /* Team Player */
  for (i=0; i<2; i++)
  {
    if (input.system[i] == SYSTEM_TEAMPLAYER)
    {
      teamplayer_reset(i);
    }
  }
}

void input_refresh(void)
{
  int i;
  for (i=0; i<MAX_DEVICES; i++)
  {
    switch (input.dev[i])
    {
      case DEVICE_PAD6B:
      {
        gamepad_refresh(i);
        break;
      }

      case DEVICE_LIGHTGUN:
      {
        lightgun_refresh(i);
        break;
      }
    }
  }
}

void input_end_frame(unsigned int cycles)
{
  int i;
  for (i=0; i<MAX_DEVICES; i++)
  {
    switch (input.dev[i])
    {
      case DEVICE_PAD3B:
      case DEVICE_PAD6B:
      {
        gamepad_end_frame(i, cycles);
        break;
      }
    }
  }
}
