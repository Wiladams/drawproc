#pragma once
#pragma once
/*
* Copyright (c) 1999, 2000, 2001, 2002, 2010 Greg Haerr <greg@censoft.com>
*
* Framebuffer drivers header file for Microwindows Screen Drivers
*
* Portions contributed by Koninklijke Philips Electronics N.V.
* These portions are Copyright 2002 Koninklijke Philips Electronics
* N.V.  All Rights Reserved.  These portions are licensed under the
* terms of the Mozilla Public License, version 1.1, or, at your
* option, the GNU General Public License version 2.0.  Please see
* the file "ChangeLog" for documentation regarding these
* contributions.
*/

#include "drawproc.h"

/* Linux framebuffer critical sections*/
#if VTSWITCH
extern volatile int mwdrawing;
#define DRAWON		++mwdrawing
#define DRAWOFF		--mwdrawing
#else
#define DRAWON
#define DRAWOFF
#endif

typedef unsigned char *		ADDR8;
typedef unsigned short *	ADDR16;
typedef uint32_t *			ADDR32;

/* Note that the following APPLYOP macro implements the
* Porter-Duff rules assuming that source and destination
* both have an alpha of 1.0.  Drivers that support alpha
* should provide a better implementation of these rules.
*
* The following are not handled yet:
*		DPROP_SRC_IN
*		DPROP_SRC_ATOP
*		DPROP_DST_OVER
*		DPROP_DST_IN
*		DPROP_DST_ATOP
*		DPROP_SRC_OUT
*		DPROP_DST_OUT
*		DPROP_PORTERDUFF_XOR
*
* Arguments:
*	op		- MWROP code
*	width	- repeat count
*	STYPE	- src 'type' e.g. (ADDR32) or (DPPIXELVAL)
*	s		- src pointer or value
*	DTYPE	- dst 'type' e.g. *(ADDR32)
*	d		- dst pointer
*	ssz		- src pointer increment
*	dsz		- dst pointer increment
*/

/* applyOp with count, src ptr, ssz/dsz increment*/
#define	APPLYOP(op, width, STYPE, s, DTYPE, d, ssz, dsz)	\
	{											\
		int  count = width;						\
		switch (op) {							\
		case DPROP_COPY:						\
		case DPROP_SRC_OVER:					\
		case DPROP_SRC_IN:						\
		case DPROP_SRC_ATOP:					\
			while(--count >= 0) {				\
				DTYPE d = STYPE s;				\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_XOR_FGBG:					\
		case DPROP_PORTERDUFF_XOR:				\
			while(--count >= 0) {				\
				DTYPE d ^= (STYPE s) ^ gr_background; \
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_SRCTRANSCOPY:				\
			while(--count >= 0) {				\
				DTYPE d = (DTYPE d)? DTYPE d: STYPE s; \
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_XOR:							\
			while(--count >= 0) {				\
				DTYPE d ^= STYPE s;				\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_AND:							\
			while(--count >= 0) {				\
				DTYPE d &= STYPE s;				\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_OR:							\
			while(--count >= 0) {				\
				DTYPE d |= STYPE s;				\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_SRC_OUT:						\
		case DPROP_DST_OUT:						\
		case DPROP_CLEAR:						\
			while(--count >= 0) {				\
				DTYPE d = 0;					\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_SET:							\
			while(--count >= 0) {				\
				DTYPE d = ~0;					\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_EQUIV:						\
			while(--count >= 0) {				\
				DTYPE d = ~(DTYPE d ^ STYPE s); \
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_NOR:							\
			while(--count >= 0) {				\
				DTYPE d = ~(DTYPE d | STYPE s); \
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_NAND:						\
			while(--count >= 0) {				\
				DTYPE d = ~(DTYPE d & STYPE s); \
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_INVERT:						\
			while(--count >= 0) {				\
				DTYPE d = ~DTYPE d;				\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_COPYINVERTED:				\
			while(--count >= 0) {				\
				DTYPE d = ~STYPE s;				\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_ORINVERTED:					\
			while(--count >= 0) {				\
				DTYPE d |= ~STYPE s;			\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_ANDINVERTED:					\
			while(--count >= 0) {				\
				DTYPE d &= ~STYPE s;			\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_ORREVERSE:					\
			while(--count >= 0) {				\
				DTYPE d = ~DTYPE d | STYPE s; 	\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_ANDREVERSE:					\
			while(--count >= 0) {				\
				DTYPE d = ~DTYPE d & STYPE s; 	\
				d += dsz; s += ssz; }			\
			break;								\
		case DPROP_NOOP:						\
		case DPROP_DST_OVER:					\
		case DPROP_DST_IN:						\
		case DPROP_DST_ATOP:					\
			break;								\
		}										\
	}

/* APPLYOP w/return value - used only in fblin4.c*/
#define DEFINE_applyOpR				\
static inline int applyOpR(int op, int src, int dst)	\
{						\
	switch (op) {				\
	case DPROP_XOR:			\
		return (src) ^ (dst);		\
	case DPROP_AND:			\
		return (src) & (dst);		\
	case DPROP_OR:				\
		return (src) | (dst);		\
	case DPROP_SRC_OUT:		\
	case DPROP_DST_OUT:		\
	case DPROP_PORTERDUFF_XOR:		\
	case DPROP_CLEAR:			\
		return 0;			\
	case DPROP_SET:			\
		return ~0;			\
	case DPROP_EQUIV:			\
		return ~((src) ^ (dst));	\
	case DPROP_NOR:			\
		return ~((src) | (dst));	\
	case DPROP_NAND:			\
		return ~((src) & (dst));	\
	case DPROP_INVERT:			\
		return ~(dst);			\
	case DPROP_COPYINVERTED:		\
		return ~(src);			\
	case DPROP_ORINVERTED:			\
		return ~(src) | (dst);		\
	case DPROP_ANDINVERTED:		\
		return ~(src) & (dst);		\
	case DPROP_ORREVERSE:			\
		return (src) | ~(dst);		\
	case DPROP_ANDREVERSE:			\
		return (src) & ~(dst);		\
	case DPROP_SRC_OVER:		\
	case DPROP_SRC_IN:			\
	case DPROP_SRC_ATOP:		\
	case DPROP_COPY:			\
		return (src);			\
	case DPROP_XOR_FGBG:		\
		return (src) ^ (dst) ^ gr_background;	\
	case DPROP_DST_OVER:		\
	case DPROP_DST_IN:			\
	case DPROP_DST_ATOP:		\
	case DPROP_NOOP:			\
	default:				\
		return (dst);			\
	}					\
}

/* global vars*/
extern int 	gr_mode;	/* temp kluge*/

						/* entry points*/
						/* scr_fb.c*/
void ioctl_getpalette(int start, int len, short *red, short *green, short *blue);
void ioctl_setpalette(int start, int len, short *red, short *green, short *blue);
void setfadelevel(PSD psd, int f);

/* fb.c*/
DPROC_API extern	 PSUBDRIVER select_fb_subdriver(PSD psd);
DPROC_API extern	 int		set_data_format(PSD psd);
DPROC_API extern	 void	gen_getscreeninfo(PSD psd, PDPSCREENINFO psi);

/* fbportrait_xxx.c*/
extern SUBDRIVER fbportrait_left;
extern SUBDRIVER fbportrait_right;
extern SUBDRIVER fbportrait_down;

DPROC_API extern	 void fbportrait_left_drawpixel(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c);
DPROC_API extern	 DPPIXELVAL fbportrait_left_readpixel(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern	 void fbportrait_left_drawhorzline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_left_drawvertline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_left_fillrect(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_left_blit(PSD dstpsd, DPCOORD destx, DPCOORD desty, DPCOORD w, DPCOORD h,
	PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int op);
DPROC_API extern	 void fbportrait_left_convblit_blend_mask_alpha_byte(PSD dstpsd, PDPBLITPARMS gc);
DPROC_API extern	 void fbportrait_left_convblit_copy_mask_mono_byte_msb(PSD psd, PDPBLITPARMS gc);
DPROC_API extern	 void fbportrait_left_convblit_copy_mask_mono_byte_lsb(PSD psd, PDPBLITPARMS gc);

DPROC_API extern	 void fbportrait_right_drawpixel(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c);
DPROC_API extern	 DPPIXELVAL fbportrait_right_readpixel(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern	 void fbportrait_right_drawhorzline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_right_drawvertline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_right_fillrect(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_right_blit(PSD dstpsd, DPCOORD destx, DPCOORD desty, DPCOORD w, DPCOORD h,
	PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int op);
DPROC_API extern	 void fbportrait_right_convblit_blend_mask_alpha_byte(PSD dstpsd, PDPBLITPARMS gc);
DPROC_API extern	 void fbportrait_right_convblit_copy_mask_mono_byte_msb(PSD psd, PDPBLITPARMS gc);
DPROC_API extern	 void fbportrait_right_convblit_copy_mask_mono_byte_lsb(PSD psd, PDPBLITPARMS gc);

DPROC_API extern	 void fbportrait_down_drawpixel(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c);
DPROC_API extern	 DPPIXELVAL fbportrait_down_readpixel(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern	 void fbportrait_down_drawhorzline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_down_drawvertline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_down_fillrect(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c);
DPROC_API extern	 void fbportrait_down_blit(PSD dstpsd, DPCOORD destx, DPCOORD desty, DPCOORD w, DPCOORD h,
	PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int op);
DPROC_API extern	 void fbportrait_down_convblit_blend_mask_alpha_byte(PSD dstpsd, PDPBLITPARMS gc);
DPROC_API extern	 void fbportrait_down_convblit_copy_mask_mono_byte_msb(PSD psd, PDPBLITPARMS gc);
DPROC_API extern	 void fbportrait_down_convblit_copy_mask_mono_byte_lsb(PSD psd, PDPBLITPARMS gc);

/* rasterops.c*/
DPROC_API extern	 void GdRasterOp(PDPIMAGEHDR pixd, DPCOORD dx, DPCOORD dy, DPCOORD dw, DPCOORD dh, int op, PDPIMAGEHDR pixs, DPCOORD sx, DPCOORD sy);
