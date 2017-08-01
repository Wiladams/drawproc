/*
* Copyright (c) 1999-2001, 2010 Greg Haerr <greg@censoft.com>
*
* 1bpp Packed Linear Video Driver for Microwindows
* Writes MWIF_MONOBYTEMSB data format (MSB first bit order)
*/
/*#define NDEBUG*/
#include <assert.h>
#include <string.h>
#include "dpdevice.h"
#include "fb.h"
#include "genmem.h"

/* This driver doesn't have full drawing mode functionality using
* the applyOp() macro from fb.h
*/

static const unsigned char notmask[8] = {
	0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe };

/* Set pixel at x, y, to pixelval c*/
static void
linear1_drawpixel(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c)
{
	register unsigned char *addr = psd->addr + y * psd->pitch + (x >> 3);
#if DEBUG
	assert(x >= 0 && x < psd->xres);
	assert(y >= 0 && y < psd->yres);
	assert(c < psd->ncolors);
#endif
	DRAWON;
	if (gr_mode == DPROP_XOR)
		*addr ^= c << (7 - (x & 7));
	else
		*addr = (*addr & notmask[x & 7]) | (c << (7 - (x & 7)));
	DRAWOFF;
}

/* Read pixel at x, y*/
static DPPIXELVAL
linear1_readpixel(PSD psd, DPCOORD x, DPCOORD y)
{
	register unsigned char *addr = psd->addr + y * psd->pitch + (x >> 3);
#if DEBUG
	assert(x >= 0 && x < psd->xres);
	assert(y >= 0 && y < psd->yres);
#endif
	return (*addr >> (7 - (x & 7))) & 0x01;
}

/* Draw horizontal line from x1,y to x2,y including final point*/
static void
linear1_drawhorzline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c)
{
	register unsigned char *addr = psd->addr + y * psd->pitch + (x1 >> 3);
#if DEBUG
	assert(x1 >= 0 && x1 < psd->xres);
	assert(x2 >= 0 && x2 < psd->xres);
	assert(x2 >= x1);
	assert(y >= 0 && y < psd->yres);
	assert(c < psd->ncolors);
#endif
	DRAWON;
	if (gr_mode == DPROP_XOR) {
		while (x1 <= x2) {
			*addr ^= c << (7 - (x1 & 7));
			if ((++x1 & 7) == 0)
				++addr;
		}
	}
	else {
		while (x1 <= x2) {
			*addr = (*addr & notmask[x1 & 7]) | (c << (7 - (x1 & 7)));
			if ((++x1 & 7) == 0)
				++addr;
		}
	}
	DRAWOFF;
}

/* Draw a vertical line from x,y1 to x,y2 including final point*/
static void
linear1_drawvertline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c)
{
	int	pitch = psd->pitch;
	register unsigned char *addr = psd->addr + y1 * pitch + (x >> 3);
#if DEBUG
	assert(x >= 0 && x < psd->xres);
	assert(y1 >= 0 && y1 < psd->yres);
	assert(y2 >= 0 && y2 < psd->yres);
	assert(y2 >= y1);
	assert(c < psd->ncolors);
#endif
	DRAWON;
	if (gr_mode == DPROP_XOR)
		while (y1++ <= y2) {
			*addr ^= c << (7 - (x & 7));
			addr += pitch;
		}
	else
		while (y1++ <= y2) {
			*addr = (*addr & notmask[x & 7]) | (c << (7 - (x & 7)));
			addr += pitch;
		}
	DRAWOFF;
}

/* general frameblit, all base rops supported*/
static void
linear1_frameblit(PSD psd, PDPBLITPARMS gc)
{
	/* GdRasterOp doesn't work yet in portrait modes, requires normal src/dst*/
	//assert(psd->portrait == MWPORTRAIT_NONE);

	GdRasterOp((PDPIMAGEHDR)psd, gc->dstx, gc->dsty, gc->width, gc->height, gc->op,
		(PDPIMAGEHDR)gc->srcpsd, gc->srcx, gc->srcy);
}



/*
* Routine to draw mono 1bpp MSBFirst bitmap to MSB 1bpp
* Bitmap is byte array.
*
* Used to draw FT2 non-antialiased glyphs.
*/
static void
linear1_convblit_copy_mask_mono_byte_msb(PSD psd, PDPBLITPARMS gc)
{
	int		i;
	int		dpitch = gc->dst_pitch;
	int		spitch = gc->src_pitch;
	/* src is MSB 1bpp, dst is MSB 1bpp*/
	ADDR8 dst = ((ADDR8)gc->data_out) + (gc->dstx >> 3) + gc->dsty * dpitch;
	ADDR8 src = ((ADDR8)gc->data) + (gc->srcx >> 3) + gc->srcy * spitch;
	DPCOORD	h = gc->height;
	DPCOORD	w = gc->width;
	DPPIXELVAL fg = gc->fg_pixelval;
	DPPIXELVAL bg = gc->bg_pixelval;
#if DEBUG
	assert(gc->dstx >= 0 && gc->dstx < psd->xres);
	assert(gc->dsty >= 0 && gc->dsty < psd->yres);
	assert(w > 0);
	assert(h > 0);
	assert(gc->dstx + w <= psd->xres);
	assert(gc->dsty + h <= psd->yres);
#endif
	DRAWON;
	while (--h >= 0) {
		ADDR8	d = dst;
		ADDR8	s = src;
		DPCOORD	dx = gc->dstx;
		DPCOORD	sx = gc->srcx;

		if (gc->usebg) {
			for (i = 0; i<w; ++i) {
				if ((*s >> (7 - (sx & 7))) & 01)
					*d = (*d & notmask[dx & 7]) | (fg << (7 - (dx & 7)));
				else
					*d = (*d & notmask[dx & 7]) | (bg << (7 - (dx & 7)));
				if ((++dx & 7) == 0)
					++d;
				if ((++sx & 7) == 0)
					++s;
			}
		}
		else {
			for (i = 0; i<w; ++i) {
				if ((*s >> (7 - (sx & 7))) & 01)
					*d = (*d & notmask[dx & 7]) | (fg << (7 - (dx & 7)));
				if ((++dx & 7) == 0)
					++d;
				if ((++sx & 7) == 0)
					++s;
			}
		}
		dst += dpitch;
		src += spitch;
	}
	DRAWOFF;
}

/*
* Routine to draw mono 1bpp LSBFirst bitmap to MSB 1bpp
* Bitmap is byte array.
*
* Used to draw T1LIB non-antialiased glyphs.
*/
static void
linear1_convblit_copy_mask_mono_byte_lsb(PSD psd, PDPBLITPARMS gc)
{
	int		i;
	int		dpitch = gc->dst_pitch;
	int		spitch = gc->src_pitch;
	/* src is LSB 1bpp, dst is MSB 1bpp*/
	ADDR8 dst = ((ADDR8)gc->data_out) + (gc->dstx >> 3) + gc->dsty * dpitch;
	ADDR8 src = ((ADDR8)gc->data) + (gc->srcx >> 3) + gc->srcy * spitch;
	DPCOORD	h = gc->height;
	DPCOORD	w = gc->width;
	DPPIXELVAL fg = gc->fg_pixelval;
	DPPIXELVAL bg = gc->bg_pixelval;
#if DEBUG
	assert(gc->dstx >= 0 && gc->dstx < psd->xres);
	assert(gc->dsty >= 0 && gc->dsty < psd->yres);
	assert(w > 0);
	assert(h > 0);
	assert(gc->dstx + w <= psd->xres);
	assert(gc->dsty + h <= psd->yres);
#endif
	DRAWON;
	while (--h >= 0) {
		ADDR8	d = dst;
		ADDR8	s = src;
		DPCOORD	dx = gc->dstx;
		DPCOORD	sx = gc->srcx;

		if (gc->usebg) {
			for (i = 0; i<w; ++i) {
				if ((*s >> (sx & 7)) & 01)
					*d = (*d & notmask[dx & 7]) | (fg << (7 - (dx & 7)));
				else
					*d = (*d & notmask[dx & 7]) | (bg << (7 - (dx & 7)));
				if ((++dx & 7) == 0)
					++d;
				if ((++sx & 7) == 0)
					++s;
			}
		}
		else {
			for (i = 0; i<w; ++i) {
				if ((*s >> (sx & 7)) & 01)
					*d = (*d & notmask[dx & 7]) | (fg << (7 - (dx & 7)));
				if ((++dx & 7) == 0)
					++d;
				if ((++sx & 7) == 0)
					++s;
			}
		}
		dst += dpitch;
		src += spitch;
	}
	DRAWOFF;
}

/*
* Routine to draw mono 1bpp MSBFirst bitmap to MSB 1bpp
* Bitmap is little endian word array.
*
* Used to draw PCF/core glyphs.
*/
static void
linear1_convblit_copy_mask_mono_word_msb(PSD psd, PDPBLITPARMS gc)
{
	int		i;
	int		dpitch = gc->dst_pitch;
	int		spitch = gc->src_pitch;
	/* src is word MSB 1bpp, dst is byte MSB 1bpp*/
	ADDR8 dst = ((ADDR8)gc->data_out) + (gc->dstx >> 3) + gc->dsty * dpitch;
	ADDR8 src = ((ADDR8)gc->data) + ((gc->srcx >> 4) << 1) + gc->srcy * spitch;
	DPCOORD	h = gc->height;
	DPCOORD	w = gc->width;
	DPPIXELVAL fg = gc->fg_pixelval;
	DPPIXELVAL bg = gc->bg_pixelval;
#if DEBUG
	assert(gc->dstx >= 0 && gc->dstx < psd->xres);
	assert(gc->dsty >= 0 && gc->dsty < psd->yres);
	assert(w > 0);
	assert(h > 0);
	assert(gc->dstx + w <= psd->xres);
	assert(gc->dsty + h <= psd->yres);
#endif
	DRAWON;
	while (--h >= 0) {
		ADDR8	d = dst;
		ADDR16	s = (ADDR16)src;
		DPCOORD	dx = gc->dstx;
		DPCOORD	sx = gc->srcx;

		if (gc->usebg) {
			for (i = 0; i<w; ++i) {
				if ((*s >> (15 - (sx & 15))) & 01)
					*d = (*d & notmask[dx & 7]) | (fg << (7 - (dx & 7)));
				else
					*d = (*d & notmask[dx & 7]) | (bg << (7 - (dx & 7)));
				if ((++dx & 7) == 0)
					++d;
				if ((++sx & 15) == 0)
					++s;
			}
		}
		else {
			for (i = 0; i<w; ++i) {
				if ((*s >> (15 - (sx & 15))) & 01)
					*d = (*d & notmask[dx & 7]) | (fg << (7 - (dx & 7)));
				if ((++dx & 7) == 0)
					++d;
				if ((++sx & 15) == 0)
					++s;
			}
		}
		dst += dpitch;
		src += spitch;
	}
	DRAWOFF;
}

static SUBDRIVER fblinear1_none = {
	linear1_drawpixel,
	linear1_readpixel,
	linear1_drawhorzline,
	linear1_drawvertline,
	gen_fillrect,
	NULL,		/* fallback blit*/
	linear1_frameblit,
	NULL,		/* FrameStretchBlit*/
	linear1_convblit_copy_mask_mono_byte_msb,
	linear1_convblit_copy_mask_mono_byte_lsb,
	linear1_convblit_copy_mask_mono_word_msb,
	NULL,		/* BlitBlendMaskAlphaByte*/
	NULL,		/* BlitCopyRGBA8888*/
	NULL,		/* BlitSrcOverRGBA8888*/
	NULL		/* BlitCopyRGB888*/
};

PSUBDRIVER fblinear1[4] = {
	&fblinear1_none, &fbportrait_left, &fbportrait_right, &fbportrait_down
};
