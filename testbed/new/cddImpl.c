#include "shared.h"
#include "megasd.h"

static cdStream *trackStream[CD_MAX_TRACKS];
static int trackOffset[CD_MAX_TRACKS];
static cdStream *tocStream;

void cdd_reset(void)
{
  /* reset drive access latency */
  cdd.latency = 0;

  /* reset track index */
  cdd.index = 0;

  /* reset logical block address */
  cdd.lba = 0;

  /* reset status */
  cdd.status = cdd.loaded ? CD_TOC : NO_DISC;
  
  /* reset CD-DA fader (full volume) */
  cdd.fader[0] = cdd.fader[1] = 0x400;

  /* clear CD-DA output */
  cdd.audio[0] = cdd.audio[1] = 0;
}

int cdd_load(const char *filename, char *header)
{
  char fname[256+10];
  char line[128];
  char *ptr, *lptr;
  cdStream *fd;
  
  /* assume normal CD image file by default */
  int isCDfile = 1;
  int isMSDfile = 0;

  /* first unmount any loaded disc */
  cdd_unload();

  /* open file */
  fd = cdStreamOpen(filename);
  if (!fd)
  {
    /* do not return an error as this could be a ROM loaded in memory */
    /* which should be handled by load_archive function */
    return (0);
  }

  /* save a copy of base filename (max. 255 characters) */
  strncpy(fname, filename, 255);
  fname[256] = 0;

  /* check loaded file extension */
  if (memcmp("cue", &filename[strlen(filename) - 3], 3) && memcmp("CUE", &filename[strlen(filename) - 3], 3))
  {
    int len;

    static const uint8 sync[12] = {0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00};

    /* read first 16 bytes */
    cdStreamRead(header, 0x10, 1, fd);

    /* auto-detect valid Sega CD image */
    if (!memcmp("SEGADISCSYSTEM", header, 14))
    {
      /* COOKED CD-ROM image (2048 bytes data blocks) */
      cdd.sectorSize = 2048;

      /* CD-ROM Mode 1 by default */
      cdd.toc.tracks[0].type = TYPE_MODE1;
    }

    /* auto-detect CD-ROM synchro pattern */
    else if (!memcmp(sync, header, 12))
    {
      /* RAW CD-ROM image (2352 bytes data blocks) */
      cdd.sectorSize = 2352;

      /* auto-detect CD-ROM mode from block header (byte 15) */
      cdd.toc.tracks[0].type = header[15];

      /* read next 16 bytes (start of user data) */
      cdStreamRead(header, 0x10, 1, fd);
    }

    /* supported CD-ROM image file ? */
    if ((cdd.toc.tracks[0].type == TYPE_MODE1) || (cdd.toc.tracks[0].type == TYPE_MODE2))
    {
      /* read Sega CD image header + security code */
      cdStreamRead(header + 0x10, 0x200, 1, fd);

      /* initialize first track file descriptor */
      trackStream[0] = fd;

      /* DATA track end LBA (based on DATA file length) */
      cdStreamSeek(fd, 0, SEEK_END);
      cdd.toc.tracks[0].end = cdStreamTell(fd) / cdd.sectorSize;

      /* DATA track length should be at least 2s (BIOS requirement) */
      if (cdd.toc.tracks[0].end < 150)
      {
        cdd.toc.tracks[0].end = 150;
      }
        
      /* DATA track start LBA (logical block 0) */
      cdStreamSeek(fd, 0, SEEK_SET);
      cdd.toc.tracks[0].start = 0;

      /* initialize TOC */
      cdd.toc.end = cdd.toc.tracks[0].end;
      cdd.toc.last = 1;
    }
    else
    {
      /* this is not a supported CD-ROM image file */
      isCDfile = 0;

      /* close file */
      cdStreamClose(fd);
    }

    /* automatically try to mount CD associated CUE file */
    len = strlen(fname);
    while ((len && (fname[len] != '.')) || (len > 251)) len--;
    strcpy(&fname[len], ".cue");
    fd = cdStreamOpen(fname);
  }

  /* parse CUE file */
  if (fd)
  {
    int mm, ss, bb, pregap = 0;
    int index = 0;

    /* DATA track already loaded ? */
    if (cdd.toc.last)
    {
      /* skip first track */
      while (cdStreamGets(line, 128, fd))
      {
        if (strstr(line, "INDEX 01") && !strstr(line, "INDEX 1"))
          break;
      }
    }

    /* read lines until end of file */
    while (cdStreamGets(line, 128, fd))
    {
      /* skip any SPACE characters */
      lptr = line;
      while (*lptr == 0x20) lptr++;

      /* decode FILE commands */
      if (!(memcmp(lptr, "FILE", 4)))
      {
        /* retrieve current path */
        ptr = fname + strlen(fname) - 1;
        while ((ptr - fname) && (*ptr != '/') && (*ptr != '\\')) ptr--;
        if (ptr - fname) ptr++;

        /* skip "FILE" attribute */
        lptr += 4;

        /* skip SPACE characters */
        while (*lptr == 0x20) lptr++;

        /* retrieve full filename */
        if (*lptr == '\"')
        {
          /* skip first DOUBLE QUOTE character */
          lptr++;
          while ((*lptr != '\"') && (lptr <= (line + 128)) && (ptr < (fname + 255)))
            *ptr++ = *lptr++;
        }
        else
        {
          /* no DOUBLE QUOTE used */
          while ((*lptr != 0x20) && (lptr <= (line + 128)) && (ptr < (fname + 255)))
            *ptr++ = *lptr++;
        }
        *ptr = 0;

        /* open current track file descriptor */
        trackStream[cdd.toc.last] = cdStreamOpen(fname);
        if (!trackStream[cdd.toc.last])
        {
          /* error opening file */
          break;
        }

        /* reset current file PREGAP length */
        pregap = 0;

        /* reset current track file read offset */
        trackOffset[cdd.toc.last] = 0;

        /* check supported audio file types */
        if (!strstr(lptr,"BINARY") && !strstr(lptr,"MOTOROLA"))
        {
          /* read file header */
          unsigned char head[12];
          cdStreamRead(head, 12, 1, trackStream[cdd.toc.last]);
          cdStreamSeek(trackStream[cdd.toc.last], 0, SEEK_SET);

          /* autodetect WAVE file */
          if (!memcmp(head, "RIFF", 4) && !memcmp(head + 8, "WAVE", 4))
          {
            /* look for 'data' chunk */
            int chunkSize, dataOffset = 0;
            cdStreamSeek(trackStream[cdd.toc.last], 12, SEEK_SET);
            while (cdStreamRead(head, 8, 1, trackStream[cdd.toc.last]))
            {
              if (!memcmp(head, "data", 4))
              {
                dataOffset = cdStreamTell(trackStream[cdd.toc.last]);
                cdStreamSeek(trackStream[cdd.toc.last], 0, SEEK_SET);
                break;
              }
              chunkSize = head[4] + (head[5] << 8) + (head[6] << 16) + (head[7] << 24);
              cdStreamSeek(trackStream[cdd.toc.last], chunkSize, SEEK_CUR);
            }

            /* check if 'data' chunk has not been found */
            if (!dataOffset)
            {
              /* invalid WAVE file */
              cdStreamClose(trackStream[cdd.toc.last]);
              trackStream[cdd.toc.last] = 0;
              break;
            }

            /* adjust current track file read offset with WAVE header length */
            trackOffset[cdd.toc.last] -= dataOffset;
          }
          else
          {
            /* unsupported audio file */
            cdStreamClose(trackStream[cdd.toc.last]);
            trackStream[cdd.toc.last] = 0;
            break;
          }
        }
      }

      /* decode TRACK commands */
      else if ((sscanf(lptr, "TRACK %02d %*s", &bb)) || (sscanf(lptr, "TRACK %d %*s", &bb)))
      {
        /* autodetect DATA track type (first track only) */
        if (!cdd.toc.last)
        {
          if (strstr(lptr,"MODE1/2048"))
          {
            /* Mode 1 COOKED format (2048 bytes / block) */
            cdd.sectorSize = 2048;
            cdd.toc.tracks[0].type = TYPE_MODE1;
          }
          else if (strstr(lptr,"MODE1/2352"))
          {
            /* Mode 1 RAW format (2352 bytes / block) */
            cdd.sectorSize = 2352;
            cdd.toc.tracks[0].type = TYPE_MODE1;
          }
          else if (strstr(lptr,"MODE2/2352"))
          {
            /* Mode 2 RAW format (2352 bytes / block) */
            cdd.sectorSize = 2352;
            cdd.toc.tracks[0].type = TYPE_MODE2;
          }

          if (cdd.sectorSize)
          {
            if (cdd.sectorSize == 2352)
            {
              /* skip 16-byte header */
              cdStreamSeek(trackStream[0], 0x10, SEEK_SET);
            }

            /* read CD image header + security code */
            cdStreamRead(header, 0x210, 1, trackStream[0]);
            cdStreamSeek(trackStream[0], 0, SEEK_SET);
          }
        }
        else
        {
          /* check if same file is used for consecutive tracks */
          if (!trackStream[cdd.toc.last])
          {
            /* clear previous track end time */
            cdd.toc.tracks[cdd.toc.last - 1].end = 0;
          }
        }

        /* save current track index */
        index = cdd.toc.last;
      }

      /* decode REM LOOP xxx command (MegaSD specific command) */
      else if (sscanf(lptr, "REM LOOP %d", &bb) == 1)
      {
        cdd.toc.tracks[index].loopEnabled = 1;
        cdd.toc.tracks[index].loopOffset = bb;
        isMSDfile = 1;
      }

      /* decode REM LOOP command (MegaSD specific command) */
      else if (strstr(lptr,"REM LOOP"))
      {
        cdd.toc.tracks[index].loopEnabled = 1;
        isMSDfile = 1;
      }

      /* decode REM NOLOOP command (MegaSD specific command) */
      else if (strstr(lptr,"REM NOLOOP"))
      {
        cdd.toc.tracks[index].loopEnabled = -1;
        isMSDfile = 1;
      }

      /* decode PREGAP commands */
      else if (sscanf(lptr, "PREGAP %02d:%02d:%02d", &mm, &ss, &bb) == 3)
      {
        /* increment current file PREGAP length */
        pregap += bb + ss*75 + mm*60*75;
      }

      /* decode INDEX commands */
      else if ((sscanf(lptr, "INDEX 00 %02d:%02d:%02d", &mm, &ss, &bb) == 3) ||
               (sscanf(lptr, "INDEX 0 %02d:%02d:%02d", &mm, &ss, &bb) == 3))
      {
        /* check if previous track end time needs to be set */
        if (cdd.toc.last && !cdd.toc.tracks[cdd.toc.last - 1].end)
        {
          /* set previous track end time (current file absolute time + PREGAP length) */
          cdd.toc.tracks[cdd.toc.last - 1].end = bb + ss*75 + mm*60*75 + pregap;
        }
      }
      else if ((sscanf(lptr, "INDEX 01 %02d:%02d:%02d", &mm, &ss, &bb) == 3) ||
               (sscanf(lptr, "INDEX 1 %02d:%02d:%02d", &mm, &ss, &bb) == 3))
      {
        /* adjust current track file read offset with current file PREGAP length (only used for AUDIO track) */
        trackOffset[cdd.toc.last] += pregap * 2352;

        /* check if a single file is used for consecutive tracks */
        if (!trackStream[cdd.toc.last] && cdd.toc.last)
        {
          /* use common file descriptor */
          trackStream[cdd.toc.last] = trackStream[cdd.toc.last - 1];

          /* current track start time (based on current file absolute time + PREGAP length) */
          cdd.toc.tracks[cdd.toc.last].start = bb + ss*75 + mm*60*75 + pregap;

          /* check if previous track end time needs to be set */
          if (!cdd.toc.tracks[cdd.toc.last - 1].end)
          {
            /* set previous track end time (based on current track start time, ignoring any "PREGAP"-type pause if no INDEX00) */
            cdd.toc.tracks[cdd.toc.last - 1].end = cdd.toc.tracks[cdd.toc.last].start;
          }
        }
        else
        {
          /* current file start time (based on previous track end time + PREGAP length) */
          cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end + pregap;

          /* adjust current track file read offset with previous track end time (only used for AUDIO track) */
          trackOffset[cdd.toc.last] += cdd.toc.end * 2352;

          {
            /* current track end time */
            cdStreamSeek(trackStream[cdd.toc.last], 0, SEEK_END);
            if (cdd.toc.tracks[cdd.toc.last].type)
            {
              /* DATA track length */
              cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + (cdStreamTell(trackStream[cdd.toc.last]) / cdd.sectorSize);
            }
            else
            {
              /* AUDIO track length */
              cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + (cdStreamTell(trackStream[cdd.toc.last]) / 2352);
            }
            cdStreamSeek(trackStream[cdd.toc.last], 0, SEEK_SET);
          }

          /* adjust track start time (based on current file start time + index absolute time) */
          cdd.toc.tracks[cdd.toc.last].start += (bb + ss*75 + mm*60*75);

          /* update TOC end */
          cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
        }

        /* increment track number */
        cdd.toc.last++;
        
        /* max. 99 tracks */
        if (cdd.toc.last == 99) break;
      }
    }

    /* check if last track end time needs to be set */
    if (cdd.toc.last && !cdd.toc.tracks[cdd.toc.last - 1].end)
    {
      /* adjust TOC end with current file PREGAP length */
      cdd.toc.end += pregap;

      /* last track end time */
      cdd.toc.tracks[cdd.toc.last - 1].end = cdd.toc.end;
    }

    /* close any incomplete track file */
    if (trackStream[cdd.toc.last])
    {
      cdStreamClose(trackStream[cdd.toc.last]);
    }

    /* close CUE file */
    cdStreamClose(fd);
  }
  else if (cdd.toc.last)
  {
    int i, offset = 1;

    /* set pointer at the end of filename */
    ptr = fname + strlen(fname) - 4; 

    /* autodetect audio track file extensions */
    for (i=0; i<SUPPORTED_EXT; i++)
    {
      /* auto-detect wrong initial track index */
      sprintf(ptr, extensions[i], cdd.toc.last);
      fd = cdStreamOpen(fname);
      if (fd)
      {
        offset = 0;
        break;
      }

      sprintf(ptr, extensions[i], cdd.toc.last + 1);
      fd = cdStreamOpen(fname);
      if (fd) break;
    }

    /* repeat until no more valid track files can be found */
    while (fd)
    {
      /* read file header */
      unsigned char head[12];
      cdStreamRead(head, 12, 1, fd);
      cdStreamSeek(fd, 0, SEEK_SET);

      /* autodetect WAVE file */
      if (!memcmp(head, "RIFF", 4) && !memcmp(head + 8, "WAVE", 4))
      {
        /* look for 'data' chunk */
        int chunkSize, dataOffset = 0;
        cdStreamSeek(fd, 12, SEEK_SET);
        while (cdStreamRead(head, 8, 1, fd))
        {
          if (!memcmp(head, "data", 4))
          {
            dataOffset = cdStreamTell(fd);
            cdStreamSeek(fd, 0, SEEK_SET);
            break;
          }
          chunkSize = head[4] + (head[5] << 8) + (head[6] << 16) + (head[7] << 24);
          cdStreamSeek(fd, chunkSize, SEEK_CUR);
        }

        /* check if 'data' chunk has not been found */
        if (!dataOffset)
        {
          /* invalid WAVE file */
          cdStreamClose(fd);
          break;
        }

        /* initialize current track file descriptor */
        trackStream[cdd.toc.last] = fd;

        /* initialize current track start time (based on previous track end time) */
        cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;

        /* add default 2s PAUSE between tracks */
        cdd.toc.tracks[cdd.toc.last].start += 150;

        /* current track end time */
        cdStreamSeek(fd, 0, SEEK_END);
        cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + ((cdStreamTell(fd) - dataOffset + 2351) / 2352);

        /* initialize file read offset for current track */
        trackOffset[cdd.toc.last] = cdd.toc.tracks[cdd.toc.last].start * 2352;

        /* auto-detect PAUSE within audio files */
        cdStreamSeek(fd, 100 * 2352, SEEK_SET);
        cdStreamRead(head, 4, 1, fd);
        cdStreamSeek(fd, 0, SEEK_SET);
        if (*(int32 *)head == 0)
        {
          /* assume 2s PAUSE is included at the beginning of the file */
          trackOffset[cdd.toc.last] -= 150 * 2352;
          cdd.toc.tracks[cdd.toc.last].end -= 150;
        }

        /* update TOC end */
        cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;

        /* adjust file read offset for current track with WAVE header length */
        trackOffset[cdd.toc.last] -= dataOffset;

        /* increment track number */
        cdd.toc.last++;
      }
      else
      {
        /* unsupported audio file format */
        cdStreamClose(fd);
        break;
      }

      /* max. 99 tracks */
      if (cdd.toc.last == 99)  break;

      /* try to open next audio track file */
      sprintf(ptr, extensions[i], cdd.toc.last + offset);
      fd = cdStreamOpen(fname);
    }

    /* Valid CD-ROM Mode 1 track found ? */
    if (cdd.toc.tracks[0].type == TYPE_MODE1)
    {
      /* simulate audio tracks if none found */
      if (cdd.toc.last == 1)
      {
        /* some games require exact TOC infos */
        if (strstr(header + 0x180,"T-95035") != NULL)
        {
          /* Snatcher */
          cdd.toc.last = cdd.toc.end = 0;
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + toc_snatcher[cdd.toc.last];
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while (cdd.toc.last < 21);
        }
        else if (strstr(header + 0x180,"T-127015") != NULL)
        {
          /* Lunar - The Silver Star */
          cdd.toc.last = cdd.toc.end = 0;
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + toc_lunar[cdd.toc.last];
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while (cdd.toc.last < 52);
        }
        else if (strstr(header + 0x180,"T-113045") != NULL)
        {
          /* Shadow of the Beast II */
          cdd.toc.last = cdd.toc.end = 0;
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + toc_shadow[cdd.toc.last];
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while (cdd.toc.last < 15);
        }
        else if (strstr(header + 0x180,"T-143025") != NULL)
        {
          /* Dungeon Explorer */
          cdd.toc.last = cdd.toc.end = 0;
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + toc_dungeon[cdd.toc.last];
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while (cdd.toc.last < 13);
        }
        else if (strstr(header + 0x180,"MK-4410") != NULL)
        {
          /* Final Fight CD (USA, Europe) */
          cdd.toc.last = cdd.toc.end = 0;
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + toc_ffight[cdd.toc.last];
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while (cdd.toc.last < 26);
        }
        else if (strstr(header + 0x180,"G-6013") != NULL)
        {
          /* Final Fight CD (Japan) */
          cdd.toc.last = cdd.toc.end = 0;
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + toc_ffightj[cdd.toc.last];
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while (cdd.toc.last < 29);
        }
        else if (strstr(header + 0x180,"T-06201-01") != NULL)
        {
          /* Sewer Shark (USA) (REV1) */
          /* no audio track */
        }
        else
        {
          /* default TOC (99 tracks & 2s per audio tracks) */
          do
          {
            cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end + 2*75;
            cdd.toc.tracks[cdd.toc.last].end = cdd.toc.tracks[cdd.toc.last].start + 2*75;
            cdd.toc.end = cdd.toc.tracks[cdd.toc.last].end;
            cdd.toc.last++;
          }
          while ((cdd.toc.last < 99) && (cdd.toc.end < 56*60*75));
        }
      }
    }

  }

  /* CD tracks found ? */
  if (cdd.toc.last)
  {
    /* Lead-out */
    cdd.toc.tracks[cdd.toc.last].start = cdd.toc.end;

    /* CD mounted */
    cdd.loaded = isMSDfile ? HW_ADDON_MEGASD : HW_ADDON_MEGACD;

    /* Automatically try to open associated subcode data file */
    memcpy(&fname[strlen(fname) - 4], ".sub", 4);
    tocStream = cdStreamOpen(fname);

    /* return 1 if loaded file is CD image file */
    return (isCDfile);
  }

  /* no CD image file loaded */
  return 0;
}

void cdd_unload(void)
{
  if (cdd.loaded)
  {
    int i;

#if defined(USE_LIBCHDR)
    chd_close(cdd.chd.file);
    if (cdd.chd.hunk)
      free(cdd.chd.hunk);
#endif

    /* close CD tracks */
    for (i=0; i<cdd.toc.last; i++)
    {
      if (trackStream[i])
      {
        /* check if single file is used for consecutive tracks */
        if ((i > 0) && (trackStream[i] == trackStream[i-1]))
        {
          /* skip track */
          i++;
        }
        else
        {
          /* close file */
          cdStreamClose(trackStream[i]);
        }
      }
    }

    /* close any opened subcode file */
    if (tocStream)
      cdStreamClose(tocStream);

    /* CD unloaded */
    cdd.loaded = 0;
  }

  /* reset TOC */
  memset(&cdd.toc, 0x00, sizeof(cdd.toc));

#if defined(USE_LIBCHDR)
  /* reset CHD data */
  memset(&cdd.chd, 0x00, sizeof(cdd.chd));
#endif

  /* no CD-ROM track */
  cdd.sectorSize = 0;
}

void cdd_update(void)
{  
#ifdef LOG_CDD
  error("LBA = %d (track %d)(latency=%d)\n", cdd.lba, cdd.index, cdd.latency);
#endif

  /* drive latency */
  if (cdd.latency > 0)
  {
    cdd.latency--;
    return;
  }

  /* reading disc */
  if (cdd.status == CD_PLAY)
  {
    /* end of disc detection */
    if (cdd.index >= cdd.toc.last)
    {
      cdd.status = CD_END;
      return;
    }

    /* subcode data processing */
    cdd_read_subcode();

    /* track type */
    if (cdd.toc.tracks[cdd.index].type)
    {
      /* CD-ROM sector header */
      uint8 header[4];
      uint32 msf = cdd.lba + 150;
      header[0] = lut_BCD_8[(msf / 75) / 60];
      header[1] = lut_BCD_8[(msf / 75) % 60];
      header[2] = lut_BCD_8[(msf % 75)];
      header[3] = cdd.toc.tracks[cdd.index].type;

      /* decode CD-ROM track sector */
      cdc_decoder_update(*(uint32 *)(header));
    }
    else
    {
      /* check against audio track start index */
      if (cdd.lba >= cdd.toc.tracks[cdd.index].start)
      {
        /* audio track playing */
        scd.regs[0x36>>1].byte.h = 0x00;
      }

      /* audio blocks are still sent to CDC as well as CD DAC/Fader */
      cdc_decoder_update(0);
    }

    /* read next sector */
    cdd.lba++;

    /* check end of current track */
    if (cdd.lba >= cdd.toc.tracks[cdd.index].end)
    {
      /* seek to next track start (assuming it can only be an audio track) */
      cdd_seek_audio(cdd.index + 1, cdd.toc.tracks[cdd.index + 1].start);

      /* increment current track index */
      cdd.index++;

      /* PAUSE between tracks */
      scd.regs[0x36>>1].byte.h = 0x01;
    }
  }
  else
  {
    /* CDC decoder is still running while disc is not being read (fixes MCD-verificator CDC Flags Test #30) */
    cdc_decoder_update(0);
  }

  /* scanning disc */
  if (cdd.status == CD_SCAN)
  {
    /* current track index */
    int index = cdd.index;

    /* fast-forward or fast-rewind */
    cdd.lba += cdd.scanOffset;

    /* check current track limits */
    if (cdd.lba >= cdd.toc.tracks[index].end)
    {
      /* next track */
      index++;

      /* check disc limits */
      if (index < cdd.toc.last)
      {
        /* skip directly to next track start position */
        cdd.lba = cdd.toc.tracks[index].start;
      }
      else
      {
        /* end of disc */
        cdd.lba = cdd.toc.end;
        cdd.index = cdd.toc.last;
        cdd.status = CD_END;

        /* no audio track playing */
        scd.regs[0x36>>1].byte.h = 0x01;
        return;
      }
    }
    else if (cdd.lba < cdd.toc.tracks[index].start)
    {
      /* check disc limits */
      if (index > 0)
      {
        /* previous track */
        index--;

        /* skip directly to previous track end position */
        cdd.lba = cdd.toc.tracks[index].end;
      }
      else
      {
        /* start of first track */
        cdd.lba = 0;
      }
    }

    /* seek to current subcode position */
	cdd_seek_toc(cdd.lba * 96);

    /* current track is an audio track ? */
    if (cdd.toc.tracks[index].type == TYPE_AUDIO)
    {
      /* seek to current track sector */
      cdd_seek_audio(index, cdd.lba);

      /* audio track playing */
      scd.regs[0x36>>1].byte.h = 0x00;
    }
    else
    {
      /* no audio track playing */
      scd.regs[0x36>>1].byte.h = 0x01;
    }

    /* udpate current track index */
    cdd.index = index;
  }
}


void cdd_read_data(uint8 *dst, uint8 *subheader)
{
  /* only allow reading (first) CD-ROM track sectors */
  if (cdd.toc.tracks[cdd.index].type && (cdd.lba >= 0))
  {
    /* check sector size */
    if (cdd.sectorSize == 2048)
    {
      /* read Mode 1 user data (2048 bytes) */
      cdStreamSeek(trackStream[0], cdd.lba * 2048, SEEK_SET);
      cdStreamRead(dst, 2048, 1, trackStream[0]);
    }
    else
    {
      /* check if sub-header is required (Mode 2 sector only) */
      if (!subheader)
      {
        /* skip block sync pattern (12 bytes) + block header (4 bytes) then read Mode 1 user data (2048 bytes) */
        cdStreamSeek(trackStream[0], (cdd.lba * 2352) + 12 + 4, SEEK_SET);
        cdStreamRead(dst, 2048, 1, trackStream[0]);
      }
      else
      {
        /* skip block sync pattern (12 bytes) + block header (4 bytes) + Mode 2 sub-header (first 4 bytes) then read Mode 2 sub-header (last 4 bytes) */
        cdStreamSeek(trackStream[0], (cdd.lba * 2352) + 12 + 4 + 4, SEEK_SET);
        cdStreamRead(subheader, 4, 1, trackStream[0]);

        /* read Mode 2 user data (max 2328 bytes) */
        cdStreamRead(dst, 2328, 1, trackStream[0]);
      }
    }
  }
}

void cdd_seek_audio(int index, int lba)
{
  /* seek to track position */
  if (trackStream[index])
  {
    /* PCM AUDIO track */
    cdStreamSeek(trackStream[index], (lba * 2352) - trackOffset[index], SEEK_SET);
  }
}

void cdd_read_audio(unsigned int samples)
{
  /* previous audio outputs */
  int prev_l = cdd.audio[0];
  int prev_r = cdd.audio[1];

  /* audio track playing ? */
  if (!scd.regs[0x36>>1].byte.h && trackStream[cdd.index])
  {
    int i, mul, l, r;

    /* current CD-DA fader volume */
    int curVol = cdd.fader[0];

    /* CD-DA fader volume setup (0-1024) */
    int endVol = cdd.fader[1];

    /* read samples from current block */
    {
#ifdef LSB_FIRST
      int16 *ptr = (int16 *) (cdc.ram);
#else
      uint8 *ptr = cdc.ram;
#endif
      cdStreamRead(cdc.ram, 1, samples * 4, trackStream[cdd.index]);

      /* process 16-bit (little-endian) stereo samples */
      for (i=0; i<samples; i++)
      {
        /* CD-DA fader multiplier (cf. LC7883 datasheet) */
        /* (MIN) 0,1,2,3,4,8,12,16,20...,1020,1024 (MAX) */
        mul = (curVol & 0x7fc) ? (curVol & 0x7fc) : (curVol & 0x03);

        /* left & right channels */
#ifdef LSB_FIRST
        l = ((ptr[0] * mul) / 1024);
        r = ((ptr[1] * mul) / 1024);
        ptr+=2;
#else
        l = (((int16)((ptr[0] + ptr[1]*256)) * mul) / 1024);
        r = (((int16)((ptr[2] + ptr[3]*256)) * mul) / 1024);
        ptr+=4;
#endif

        /* CD-DA output mixing volume (0-100%) */
        l = (l * config.cdda_volume) / 100;
        r = (r * config.cdda_volume) / 100;

        /* update blip buffer */
        blip_add_delta_fast(snd.blips[2], i, l-prev_l, r-prev_r);
        prev_l = l;
        prev_r = r;

        /* update CD-DA fader volume (one step/sample) */
        if (curVol < endVol)
        {
          /* fade-in */
          curVol++;
        }
        else if (curVol > endVol)
        {
          /* fade-out */
          curVol--;
        }
        else if (!curVol)
        {
          /* audio will remain muted until next setup */
          break;
        }
      }
    }

    /* save current CD-DA fader volume */
    cdd.fader[0] = curVol;

    /* save last audio output for next frame */
    cdd.audio[0] = prev_l;
    cdd.audio[1] = prev_r;
  }
  else
  {
    /* no audio output */
    if (prev_l | prev_r)
    {
      /* update blip buffer */
      blip_add_delta_fast(snd.blips[2], 0, -prev_l, -prev_r);

      /* save audio output for next frame */
      cdd.audio[0] = 0;
      cdd.audio[1] = 0;
    }
  }

  /* end of blip buffer timeframe */
  blip_end_frame(snd.blips[2], samples);
}

void cdd_seek_toc(int lba)
{
  if (tocStream == NULL) return;
  cdStreamSeek(tocStream, cdd.lba * 96, SEEK_SET);
}

void cdd_read_toc(uint8 *dst, size_t size)
{
  if (tocStream == NULL) return;
  cdStreamRead(dst, 1, size, tocStream);
}

int cdd_context_save(uint8 *state)
{
  int bufferptr = 0;
  unsigned int offset = 0;

  save_param(&cdd.cycles, sizeof(cdd.cycles));
  save_param(&cdd.latency, sizeof(cdd.latency));
  save_param(&cdd.index, sizeof(cdd.index));
  save_param(&cdd.lba, sizeof(cdd.lba));
  save_param(&cdd.scanOffset, sizeof(cdd.scanOffset));
  save_param(&cdd.fader, sizeof(cdd.fader));
  save_param(&cdd.status, sizeof(cdd.status));

  /* current track is an audio track ? */
  if (cdd.toc.tracks[cdd.index].type == TYPE_AUDIO)
  {
    /* get file read offset */
#if defined(USE_LIBCHDR)
    if (cdd.chd.file)
    {
      /* CHD file offset */
      offset = cdd.chd.hunkofs;
    }
    else
#endif
#if defined(USE_LIBTREMOR) || defined(USE_LIBVORBIS)
    if (cdd.toc.tracks[cdd.index].vf.seekable)
    {
      /* VORBIS file sample offset */
      offset = ov_pcm_tell(&cdd.toc.tracks[cdd.index].vf);
    }
    else
#endif 
    if (trackStream[cdd.index])
    {
      /* PCM file offset */
      offset = cdStreamTell(trackStream[cdd.index]);
    }
  }

  save_param(&offset, sizeof(offset));
  save_param(&cdd.audio, sizeof(cdd.audio));

  return bufferptr;
}

int cdd_context_load(uint8 *state, char *version)
{
  unsigned int offset, lba, index;
  int bufferptr = 0;

  load_param(&cdd.cycles, sizeof(cdd.cycles));
  load_param(&cdd.latency, sizeof(cdd.latency));
  load_param(&index, sizeof(cdd.index));
  load_param(&lba, sizeof(cdd.lba));
  load_param(&cdd.scanOffset, sizeof(cdd.scanOffset));
  load_param(&cdd.fader, sizeof(cdd.fader));
  load_param(&cdd.status, sizeof(cdd.status));

  /* update current sector */
  cdd.lba = lba;

  /* support for previous state version (1.7.5) */
  if ((version[11] == 0x31) && (version[13] == 0x37) && (version[15] == 0x35))
  {
    /* current track is an audio track ? */
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
  }
  else
  {
    load_param(&offset, sizeof(offset));
    load_param(&cdd.audio, sizeof(cdd.audio));

    /* current track is an audio track ? */
    if (cdd.toc.tracks[index].type == TYPE_AUDIO)
    {
#if defined(USE_LIBTREMOR) || defined(USE_LIBVORBIS)
#ifdef DISABLE_MANY_OGG_OPEN_FILES
      /* check if track index has changed */
      if (index != cdd.index)
      {
        /* close previous track VORBIS file structure to save memory */
        if (cdd.toc.tracks[cdd.index].vf.datasource)
        {
          ogg_free(cdd.index);
        }

        /* open current track VORBIS file */
        if (cdd.toc.tracks[index].vf.seekable)
        {
          ov_open_callbacks(trackStream[index],&cdd.toc.tracks[index].vf,0,0,cb);
        }
      }
#endif
#endif
      /* seek to current file read offset */
#if defined(USE_LIBCHDR)
      if (cdd.chd.file)
      {
        /* CHD file offset */
        cdd.chd.hunkofs = offset;
      }
      else
#endif
#if defined(USE_LIBTREMOR) || defined(USE_LIBVORBIS)
      if (cdd.toc.tracks[index].vf.seekable)
      {
        /* VORBIS file sample offset */
        ov_pcm_seek(&cdd.toc.tracks[index].vf, offset);
      }
      else
#endif 
      if (trackStream[index])
      {
        /* PCM file offset */
        cdStreamSeek(trackStream[index], offset, SEEK_SET);
      }
    }
  }

  /* seek to current subcode position */
  if (tocStream)
  {
    /* 96 bytes per sector */
    cdStreamSeek(tocStream, lba * 96, SEEK_SET);
  }

  /* update current track index */
  cdd.index = index;

  return bufferptr;
}