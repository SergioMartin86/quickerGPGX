/***************************************************************************************
 *  Genesis Plus
 *  CD drive processor & CD-DA fader
 *
 *  Copyright (C) 2012-2024  Eke-Eke (Genesis Plus GX)
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
#include "shared.h"
#include "megasd.h"

/* BCD conversion lookup tables */
const uint8 lut_BCD_8[100] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 
};

const uint16 lut_BCD_16[100] =
{
  0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 
  0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0106, 0x0107, 0x0108, 0x0109, 
  0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205, 0x0206, 0x0207, 0x0208, 0x0209, 
  0x0300, 0x0301, 0x0302, 0x0303, 0x0304, 0x0305, 0x0306, 0x0307, 0x0308, 0x0309, 
  0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408, 0x0409, 
  0x0500, 0x0501, 0x0502, 0x0503, 0x0504, 0x0505, 0x0506, 0x0507, 0x0508, 0x0509, 
  0x0600, 0x0601, 0x0602, 0x0603, 0x0604, 0x0605, 0x0606, 0x0607, 0x0608, 0x0609, 
  0x0700, 0x0701, 0x0702, 0x0703, 0x0704, 0x0705, 0x0706, 0x0707, 0x0708, 0x0709, 
  0x0800, 0x0801, 0x0802, 0x0803, 0x0804, 0x0805, 0x0806, 0x0807, 0x0808, 0x0809, 
  0x0900, 0x0901, 0x0902, 0x0903, 0x0904, 0x0905, 0x0906, 0x0907, 0x0908, 0x0909, 
};

/* pre-build TOC */
const uint16 toc_snatcher[21] =
{
  56014,   495, 10120, 20555, 1580, 5417, 12502, 16090,  6553, 9681,
   8148, 20228,  8622,  6142, 5858, 1287,  7424,  3535, 31697, 2485,
  31380
};

const uint16 toc_lunar[52] =
{
  5422, 1057, 7932, 5401, 6380, 6592, 5862,  5937, 5478, 5870,
  6673, 6613, 6429, 4996, 4977, 5657, 3720,  5892, 3140, 3263,
  6351, 5187, 3249, 1464, 1596, 1750, 1751,  6599, 4578, 5205,
  1550, 1827, 2328, 1346, 1569, 1613, 7199,  4928, 1656, 2549,
  1875, 3901, 1850, 2399, 2028, 1724, 4889, 14551, 1184, 2132,
  685, 3167
};

const uint32 toc_shadow[15] =
{
  10226, 70054, 11100, 12532, 12444, 11923, 10059, 10167, 10138, 13792,
  11637,  2547,  2521,  3856, 900
};

const uint32 toc_dungeon[13] =
{
  2250, 22950, 16350, 24900, 13875, 19950, 13800, 15375, 17400, 17100,
  3325,  6825, 25275
};

const uint32 toc_ffight[26] =
{
  11994, 9742, 10136, 9685, 9553, 14588, 9430, 8721, 9975, 9764,
  9704, 12796, 585, 754, 951, 624, 9047, 1068, 817, 9191, 1024,
  14562, 10320, 8627, 3795, 3047
};

const uint32 toc_ffightj[29] =
{
  11994, 9752, 10119, 9690, 9567, 14575, 9431, 8731, 9965, 9763,
  9716, 12791, 579, 751, 958, 630, 9050, 1052, 825, 9193, 1026,
  14553, 9834, 10542, 1699, 1792, 1781, 3783, 3052
};


/* supported audio file extensions */
const char extensions[SUPPORTED_EXT][16] =
{
  "%02d.wav",
  " %02d.wav",
  "-%02d.wav",
  "_%02d.wav",
  " - %02d.wav",
  "%d.wav",
  " %d.wav",
  "-%d.wav",
  "_%d.wav",
  " - %d.wav"
};

void cdd_init(int samplerate)
{
  /* CD-DA is running by default at 44100 Hz */
  /* Audio stream is resampled to desired rate using Blip Buffer */
  blip_set_rates(snd.blips[2], 44100, samplerate);
}

void cdd_update_audio(unsigned int samples)
{
  /* get number of internal clocks (CD-DA samples) needed */
  samples = blip_clocks_needed(snd.blips[2], samples);

  if (cart.special & HW_MEGASD)
  {
    /* MegaSD add-on specific CD-DA processing */
    megasd_update_cdda(samples);
  }
  else
  {
    /* read needed CD-DA samples */
    cdd_read_audio(samples);
  }
}

void cdd_read_subcode(void)
{
  uint8 subc[96];
  int i,j,index;

  /* update subcode buffer pointer address */
  scd.regs[0x68>>1].byte.l = (scd.regs[0x68>>1].byte.l + 98) & 0x7e;
  
  /* 16-bit register index */
  index = (scd.regs[0x68>>1].byte.l + 0x100) >> 1;

  /* read interleaved subcode data from .sub file (12 x 8-bit of P subchannel first, then Q subchannel, etc) */
  cdd_read_toc(subc, 96);

  /* convert back to raw subcode format (96 bytes with 8 x P-W subchannel bits per byte) */
  for (i=0; i<96; i+=2)
  {
    int code = 0;
    for (j=0; j<8; j++)
    {
      int bits = (subc[(j*12)+(i/8)] >> (6 - (i & 6))) & 3;
      code |= ((bits & 1) << (7 - j));
      code |= ((bits >> 1) << (15 - j));
    }

    /* subcode buffer is accessed as 16-bit words */
    scd.regs[index].w = code;

    /* subcode buffer is limited to 64 x 16-bit words */
    index = (index + 1) & 0xbf;
  }

  /* level 6 interrupt enabled ? */
  if (scd.regs[0x32>>1].byte.l & 0x40)
  {
    /* trigger level 6 interrupt */
    scd.pending |= (1 << 6);

    /* update IRQ level */
    s68k_update_irq((scd.pending & scd.regs[0x32>>1].byte.l) >> 1);
  }
}


void cdd_process(void)
{
  /* Process CDD command */
  switch (scd.regs[0x42>>1].byte.h & 0x0f)
  {
    case 0x00:  /* Get Drive Status */
    {
      /* RS0-RS1 are normally unchanged unless reported drive status needs to be updated (i.e previous drive command has been processed) */
      /* Note: this function is called one 75hz frame ahead of CDD update so latency counter is always one step ahead of upcoming status */
      /* Radical Rex and Annet Futatabi both need at least respectively 2 and 3 interrupts with 'playing' status returned before sectors start getting incremented */
      if (cdd.latency <= 3)
      {
        /* update reported drive status */
        scd.regs[0x38>>1].byte.h = cdd.status;

        /* do not update RS1-RS8 if disc is stopped */
        if ((cdd.status == CD_STOP) || (cdd.status > CD_PAUSE))
          break;

        /* check if RS1 indicated invalid track infos (during seeking) */
        if (scd.regs[0x38>>1].byte.l == 0x0f)
        {
          /* seeking has ended so we return valid track infos, e.g current absolute time by default (fixes Lunar - The Silver Star) */
          int lba = cdd.lba + 150;
          scd.regs[0x38>>1].byte.l = 0x00;
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x40>>1].byte.h = cdd.toc.tracks[cdd.index].type ? 0x04 : 0x00; /* Current block flags in RS8 (bit0 = mute status, bit1: pre-emphasis status, bit2: track type) */
        }

        /* otherwise, check if RS2-RS8 need to be updated */
        else if (scd.regs[0x38>>1].byte.l == 0x00)
        {
          /* current absolute time */
          int lba = cdd.lba + 150;
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x40>>1].byte.h = cdd.toc.tracks[cdd.index].type ? 0x04 : 0x00; /* Current block flags in RS8 (bit0 = mute status, bit1: pre-emphasis status, bit2: track type) */
        }
        else if (scd.regs[0x38>>1].byte.l == 0x01)
        {
          /* current track relative time */
          int lba = abs(cdd.lba - cdd.toc.tracks[cdd.index].start);
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x40>>1].byte.h = cdd.toc.tracks[cdd.index].type ? 0x04 : 0x00; /* Current block flags in RS8 (bit0 = mute status, bit1: pre-emphasis status, bit2: track type) */
        }
        else if (scd.regs[0x38>>1].byte.l == 0x02)
        {
          /* current track number */
          scd.regs[0x3a>>1].w = (cdd.index < cdd.toc.last) ? lut_BCD_16[cdd.index + 1] : 0x0A0A;
        }
      }
      break;
    }

    case 0x01:  /* Stop Drive */
    {
      /* update status */
      cdd.status = cdd.loaded ? CD_TOC : NO_DISC;

      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* RS1-RS8 ignored, expects 0x0 (CD_STOP) in RS0 once */
      scd.regs[0x38>>1].w = (CD_STOP << 8) | 0x0f;
      scd.regs[0x3a>>1].w = 0x0000;
      scd.regs[0x3c>>1].w = 0x0000;
      scd.regs[0x3e>>1].w = 0x0000;
      scd.regs[0x40>>1].w = ~(CD_STOP + 0x0f) & 0x0f;

      /* reset drive position */
      cdd.index = cdd.lba = 0;
      return;
    }

    case 0x02:  /* Report TOC infos */
    {
      /* Infos automatically retrieved by CDD processor from Q-Channel */
      /* commands 0x00-0x02 (current block) and 0x03-0x05 (Lead-In) */
      switch (scd.regs[0x44>>1].byte.l)
      {
        case 0x00:  /* Current Absolute Time (MM:SS:FF) */
        {
          int lba = cdd.lba + 150;
          scd.regs[0x38>>1].w = cdd.status << 8;
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x40>>1].byte.h = cdd.toc.tracks[cdd.index].type ? 0x04 : 0x00; /* Current block flags in RS8 (bit0 = mute status, bit1: pre-emphasis status, bit2: track type) */
          break;
        }

        case 0x01:  /* Current Track Relative Time (MM:SS:FF) */
        {
          int lba = abs(cdd.lba - cdd.toc.tracks[cdd.index].start);
          scd.regs[0x38>>1].w = (cdd.status << 8) | 0x01;
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x40>>1].byte.h = cdd.toc.tracks[cdd.index].type ? 0x04 : 0x00; /* Current block flags in RS8 (bit0 = mute status, bit1: pre-emphasis status, bit2: track type) */
          break;
        }

        case 0x02:  /* Current Track Number */
        {
          scd.regs[0x38>>1].w = (cdd.status << 8) | 0x02;
          scd.regs[0x3a>>1].w = (cdd.index < cdd.toc.last) ? lut_BCD_16[cdd.index + 1] : 0x0A0A;
          scd.regs[0x3c>>1].w = 0x0000;
          scd.regs[0x3e>>1].w = 0x0000; /* Disk Control Code (?) in RS6 */
          scd.regs[0x40>>1].byte.h = 0x00;
          break;
        }

        case 0x03:  /* Total length (MM:SS:FF) */
        {
          int lba = cdd.toc.end + 150;
          scd.regs[0x38>>1].w = (cdd.status << 8) | 0x03;
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x40>>1].byte.h = 0x00;
          break;
        }

        case 0x04:  /* First & Last Track Numbers */
        {
          scd.regs[0x38>>1].w = (cdd.status << 8) | 0x04;
          scd.regs[0x3a>>1].w = 0x0001;
          scd.regs[0x3c>>1].w = lut_BCD_16[cdd.toc.last];
          scd.regs[0x3e>>1].w = 0x0000; /* Drive Version (?) in RS6-RS7 */
          scd.regs[0x40>>1].byte.h = 0x00;  /* Lead-In flags in RS8 (bit0 = mute status, bit1: pre-emphasis status, bit2: track type) */
          break;
        }

        case 0x05:  /* Track Start Time (MM:SS:FF) */
        {
          int track = scd.regs[0x46>>1].byte.h * 10 + scd.regs[0x46>>1].byte.l;
          int lba = cdd.toc.tracks[track-1].start + 150;
          scd.regs[0x38>>1].w = (cdd.status << 8) | 0x05;
          scd.regs[0x3a>>1].w = lut_BCD_16[(lba/75)/60];
          scd.regs[0x3c>>1].w = lut_BCD_16[(lba/75)%60];
          scd.regs[0x3e>>1].w = lut_BCD_16[(lba%75)];
          scd.regs[0x3e>>1].byte.h |= cdd.toc.tracks[track-1].type ? 0x08 : 0x00; /* RS6 bit 3 is set for CD-ROM track */
          scd.regs[0x40>>1].byte.h = track % 10;  /* Track Number (low digit) */
          break;
        }

        case 0x06:  /* Latest Error Information */
        {
          scd.regs[0x38>>1].w = (cdd.status << 8) | 0x06;
          scd.regs[0x3a>>1].w = 0x0000; /* no error */
          scd.regs[0x3c>>1].w = 0x0000;
          scd.regs[0x3e>>1].w = 0x0000;
          scd.regs[0x40>>1].byte.h = 0x00;
          break;
        }

        default:
        {
#ifdef LOG_ERROR
          error("Invalid CDD request code %02X (%X)\n", scd.regs[0x44>>1].byte.l, s68k.pc);
#endif
          return;
        }
      }
      break;
    }

    case 0x03:  /* Play */
    {
      /* reset track index */
      int index = 0;

      /* new LBA position */
      int lba = ((scd.regs[0x44>>1].byte.h * 10 + scd.regs[0x44>>1].byte.l) * 60 + 
                 (scd.regs[0x46>>1].byte.h * 10 + scd.regs[0x46>>1].byte.l)) * 75 +
                 (scd.regs[0x48>>1].byte.h * 10 + scd.regs[0x48>>1].byte.l) - 150;

      /* CD drive latency */
      if (!cdd.latency)
      {
        /* Fixes a few games hanging because they expect data to be read with some delay */
        /* Wolf Team games (Annet Futatabi, Aisle Lord, Cobra Command, Earnest Evans, Road Avenger & Time Gal) need at least 11 interrupts delay  */
        /* Space Adventure Cobra (2nd morgue scene) needs at least 13 interrupts delay (incl. seek time, so 11 is OK) */
        /* By default, at least two interrupts latency is required by current emulation model (BIOS hangs otherwise) */
        cdd.latency = 2 + 9*config.cd_latency;
      }

      /* CD drive seek time */
      /* max. seek time = 1.5 s = 1.5 x 75 = 112.5 CDD interrupts (rounded to 120) for 270000 sectors max on disc. */
      /* Note: This is only a rough approximation since, on real hardware, seek time is much likely not linear and */
      /* latency much larger than above value, but this model works fine for Sonic CD (track 26 playback needs to  */
      /* be enough delayed to start in sync with intro sequence, as compared with real hardware recording).        */
      if (lba > cdd.lba)
      {
        cdd.latency += (((lba - cdd.lba) * 120 * config.cd_latency) / 270000);
      }
      else 
      {
        cdd.latency += (((cdd.lba - lba) * 120 * config.cd_latency) / 270000);
      }

      /* update current LBA */
      cdd.lba = lba;

      /* get track index */
      while ((cdd.toc.tracks[index].end <= lba) && (index < cdd.toc.last)) index++;

      /* audio track ? */
      if (cdd.toc.tracks[index].type == TYPE_AUDIO)
      {
        /* stay within track limits when seeking files */
        if (lba < cdd.toc.tracks[index].start) 
        {
          lba = cdd.toc.tracks[index].start;
        }

        /* seek to current track sector */
        cdd_seek_audio(index, lba);
      }

      /* update current track index */
      cdd.index = index;

      /* seek to current subcode position */
	  cdd_seek_toc(lba * 96);

      /* no audio track playing (yet) */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* update status (reported to host once seeking has ended) */
      cdd.status = CD_PLAY;

      /* RS0 should indicates seeking until drive is ready (fixes audio delay in Bari Arm) */
      /* RS1=0xf to invalidate track infos in RS2-RS8 until drive is ready (fixes Snatcher Act 2 start cutscene) */
      scd.regs[0x38>>1].w = (CD_SEEK << 8) | 0x0f;
      scd.regs[0x3a>>1].w = 0x0000;
      scd.regs[0x3c>>1].w = 0x0000;
      scd.regs[0x3e>>1].w = 0x0000;
      scd.regs[0x40>>1].w = ~(CD_SEEK + 0x0f) & 0x0f;
      return;
    }

    case 0x04:  /* Seek */
    {
      /* reset track index */
      int index = 0;

      /* new LBA position */
      int lba = ((scd.regs[0x44>>1].byte.h * 10 + scd.regs[0x44>>1].byte.l) * 60 + 
                 (scd.regs[0x46>>1].byte.h * 10 + scd.regs[0x46>>1].byte.l)) * 75 +
                 (scd.regs[0x48>>1].byte.h * 10 + scd.regs[0x48>>1].byte.l) - 150;

      /* CD drive seek time  */
      /* We are using similar linear model as above, although still not exactly accurate, */
      /* it works fine for Switch/Panic! intro (Switch needs at least 30 interrupts while */
      /* seeking from 00:05:63 to 24:03:19, Panic! when seeking from 00:05:60 to 24:06:07) */
      if (lba > cdd.lba)
      {
        cdd.latency = ((lba - cdd.lba) * 120 * config.cd_latency) / 270000;
      }
      else
      {
        cdd.latency = ((cdd.lba - lba) * 120 * config.cd_latency) / 270000;
      }

      /* update current LBA */
      cdd.lba = lba;

      /* get current track index */
      while ((cdd.toc.tracks[index].end <= lba) && (index < cdd.toc.last)) index++;

      /* audio track ? */
      if (cdd.toc.tracks[index].type == TYPE_AUDIO)
      {
        /* stay within track limits when seeking files */
        if (lba < cdd.toc.tracks[index].start) 
        {
          lba = cdd.toc.tracks[index].start;
        }

        /* seek to current track sector */
        cdd_seek_audio(index, lba);
      }

      /* update current track index */
      cdd.index = index;

      /* seek to current subcode position */
	  cdd_seek_toc(lba * 96);

      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* update status (reported to host once seeking has ended) */
      cdd.status = CD_PAUSE;

      /* RS1=0xf to invalidate track infos in RS2-RS8 while seeking (fixes Final Fight CD intro when seek time is emulated) */
      scd.regs[0x38>>1].w = (CD_SEEK << 8) | 0x0f;
      scd.regs[0x3a>>1].w = 0x0000;
      scd.regs[0x3c>>1].w = 0x0000;
      scd.regs[0x3e>>1].w = 0x0000;
      scd.regs[0x40>>1].w = ~(CD_SEEK + 0x0f) & 0x0f;
      return;
    }

    case 0x06:  /* Pause */
    {
      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* update status (RS1-RS8 unchanged) */
      cdd.status = scd.regs[0x38>>1].byte.h = CD_PAUSE;
      break;
    }

    case 0x07:  /* Resume */
    {
      /* update status (RS1-RS8 unchanged) */
      cdd.status = scd.regs[0x38>>1].byte.h = CD_PLAY;
      break;
    }

    case 0x08:  /* Forward Scan */
    {
      /* reset scanning direction / speed */
      cdd.scanOffset = CD_SCAN_SPEED;

      /* update status (RS1-RS8 unchanged) */
      cdd.status = scd.regs[0x38>>1].byte.h = CD_SCAN;
      break;
    }

    case 0x09:  /* Rewind Scan */
    {
      /* reset scanning direction / speed */
      cdd.scanOffset = -CD_SCAN_SPEED;

      /* update status (RS1-RS8 unchanged) */
      cdd.status = scd.regs[0x38>>1].byte.h = CD_SCAN;
      break;
    }

    case 0x0a:  /* N-Track Jump Control ? (usually sent before CD_SEEK or CD_PLAY commands) */
    {
      /* TC3 corresponds to seek direction (00=forward, FF=reverse) */
      /* TC4-TC7 are related to seek length (4x4 bits i.e parameter values are between -65535 and +65535) */
      /* Maybe related to number of auto-sequenced track jumps/moves for CD DSP (cf. CXD2500BQ datasheet) */
      /* also see US Patent nr. 5222054 for a detailled description of seeking operation using Track Jump */

      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* update status (RS1-RS8 unchanged) */
      cdd.status = scd.regs[0x38>>1].byte.h = CD_PAUSE;
      break;
    }

    case 0x0c:  /* Close Tray */
    {
      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* update status */
      cdd.status = cdd.loaded ? CD_TOC : NO_DISC;

      /* RS1-RS8 ignored, expects CD_STOP in RS0 once */
      scd.regs[0x38>>1].w = (CD_STOP << 8) | 0x0f;
      scd.regs[0x3a>>1].w = 0x0000;
      scd.regs[0x3c>>1].w = 0x0000;
      scd.regs[0x3e>>1].w = 0x0000;
      scd.regs[0x40>>1].w = ~(CD_STOP + 0x0f) & 0x0f;

      /* reset drive position */
      cdd.index = cdd.lba = 0;

#ifdef CD_TRAY_CALLBACK
      CD_TRAY_CALLBACK
#endif
      return;
    }

    case 0x0d:  /* Open Tray */
    {
      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;

      /* update status (RS1-RS8 ignored) */
      cdd.status = CD_OPEN;
      scd.regs[0x38>>1].w = (CD_OPEN << 8) | 0x0f;
      scd.regs[0x3a>>1].w = 0x0000;
      scd.regs[0x3c>>1].w = 0x0000;
      scd.regs[0x3e>>1].w = 0x0000;
      scd.regs[0x40>>1].w = ~(CD_OPEN + 0x0f) & 0x0f;

      /* reset drive position */
      cdd.index = cdd.lba = 0;

#ifdef CD_TRAY_CALLBACK
      CD_TRAY_CALLBACK
#endif
      return;
    }

    default:  /* Unknown command */
#ifdef LOG_ERROR
      error("Unsupported CDD command %02X (%X)\n", scd.regs[0x42>>1].byte.h & 0x0f, s68k.pc);
#endif
      scd.regs[0x38>>1].byte.h = cdd.status;
      break;
  }

  /* only compute checksum when necessary */
  scd.regs[0x40>>1].byte.l = ~(scd.regs[0x38>>1].byte.h + scd.regs[0x38>>1].byte.l +
                               scd.regs[0x3a>>1].byte.h + scd.regs[0x3a>>1].byte.l +
                               scd.regs[0x3c>>1].byte.h + scd.regs[0x3c>>1].byte.l +
                               scd.regs[0x3e>>1].byte.h + scd.regs[0x3e>>1].byte.l +
                               scd.regs[0x40>>1].byte.h) & 0x0f;
}

void cdd_hotswap(const toc_t *toc)
{
	if (toc)
	{
		cdd.loaded = 1;
		memcpy(&cdd.toc, toc, sizeof(cdd.toc));
	}
	else
	{
		cdd.loaded = 0;
		memset(&cdd.toc, 0x00, sizeof(cdd.toc));
	}

	cdd_reset();
}