#pragma once
/*
* Copyright (c) 1999, 2005, 2010 Greg Haerr <greg@censoft.com>
*
* Screen Driver Utilities
*
* Microwindows Proportional Font Routine Header (proportional font format)
*
* These routines are screen driver entry points.
*/
#include "dpdevice.h"

#define NUMBER_FONTS	4	/* number of compiled-in fonts*/

/* entry points*/
void	gen_setfontproc(DPCOREFONT *pf);
bool	gen_getfontinfo(PDPFONT pfont, PDPFONTINFO pfontinfo);
void	gen_gettextsize(PDPFONT pfont, const void *text, int cc,
	DPTEXTFLAGS flags, DPCOORD *pwidth, DPCOORD *pheight,
	DPCOORD *pbase);
void	gen_gettextbits(PDPFONT pfont, int ch, const DPIMAGEBITS **retmap,
	DPCOORD *pwidth, DPCOORD *pheight, DPCOORD *pbase);
void	gen_unloadfont(PDPFONT pfont);

void 	gen_drawtext(PDPFONT pfont, PSD psd, DPCOORD x, DPCOORD y,
	const void *text, int cc, DPTEXTFLAGS flags);
void	gen16_gettextsize(PDPFONT pfont, const void *text, int cc,
	DPTEXTFLAGS flags, DPCOORD *pwidth, DPCOORD *pheight,
	DPCOORD *pbase);

/* local data*/
extern DPCOREFONT gen_fonts[NUMBER_FONTS];
extern DPFONTPROCS mwfontprocs;	/* builtin fontprocs - for special DBCS handling*/
