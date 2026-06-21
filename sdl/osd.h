
#ifndef _OSD_H_
#define _OSD_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <stdlib.h>

#include "main.h"
#include "config.h"
#include "error.h"
#include "unzip.h"
#include "fileio.h"

#define osd_input_update sdl_input_update

#define GG_ROM      "./ggenie.bin"
#define AR_ROM      "./areplay.bin"
#define SK_ROM      "./sk.bin"
#define SK_UPMEM    "./sk2chip.bin"
/* Settable at runtime (see setBiosFile) so the harness can point at a provided BIOS;
 * defaults preserved in main.c. */
extern char CD_BIOS_US[256];
extern char CD_BIOS_EU[256];
extern char CD_BIOS_JP[256];
#define MD_BIOS     "./bios_MD.bin"
#define MS_BIOS_US  "./bios_U.sms"
#define MS_BIOS_EU  "./bios_E.sms"
#define MS_BIOS_JP  "./bios_J.sms"
#define GG_BIOS     "./bios.gg"

#endif /* _OSD_H_ */
