#ifndef _DEVFONT_H
#define _DEVFONT_H
/*
 * Copyright (c) 2002, 2003 Greg Haerr <greg@censoft.com>
 * Portions Copyright (c) 2002 by Koninklijke Philips Electronics N.V.
 *
 * Font engine header file
 */

/* font engine entry points*/
PDPFONT fnt_createfont(const char *name, DPCOORD height, DPCOORD width, int attr);

#if HAVE_T1LIB_SUPPORT
PDPFONT t1lib_createfont(const char *name, DPCOORD height, DPCOORD width, int attr);
#endif

#if HAVE_FREETYPE_2_SUPPORT
PDPFONT freetype2_createfont(const char *name,DPCOORD height,DPCOORD width,int attr);
PDPFONT freetype2_createfontfrombuffer(const unsigned char *buffer, unsigned length,
	DPCOORD height, DPCOORD width);
#endif

#if HAVE_PCF_SUPPORT
PDPFONT pcf_createfont(const char *name, DPCOORD height, DPCOORD width, int attr);
#endif

#if HAVE_HZK_SUPPORT
PDPFONT hzk_createfont(const char *name, DPCOORD height, DPCOORD width, int fontattr);
int UC16_to_GB(const unsigned char *uc16, int cc, unsigned char *ascii);
#endif

#if HAVE_EUCJP_SUPPORT
PDPFONT eucjp_createfont(const char *name, DPCOORD height, DPCOORD width, int attr);
#endif

#if FONTMAPPER
/* entry point for font selection*/
int select_font(const PMWLOGFONT plogfont, const char **physname);
#endif

/* DBCS routines*/
void dbcs_gettextbits(PDPFONT pfont, int ch, DPTEXTFLAGS flags,
	const DPIMAGEBITS **retmap, DPCOORD *pwidth, DPCOORD *pheight,
	DPCOORD *pbase);
void dbcs_gettextsize(PDPFONT pfont, const unsigned short *str, int cc, DPTEXTFLAGS flags,
	DPCOORD *pwidth, DPCOORD *pheight, DPCOORD *pbase);

#endif /* _DEVFONT_H*/
