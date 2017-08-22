/*
* Copyright (c) 1999, 2000, 2001, 2010 Greg Haerr <greg@censoft.com>
*
* 32bpp Linear Video Driver for Microwindows (BGRA or RGBA byte order)
* Writes memory image: |B|G|R|A| LE 0xARGB BE 0xBGRA MWPF_TRUECOLOR8888
* Writes memory image: |R|G|B|A| LE 0xABGR BE 0xRGBA MWPF_TRUECOLORABGR
*
* Inspired from Ben Pfaff's BOGL <pfaffben@debian.org>
*/
/*#define NDEBUG*/
#include <assert.h>
#include <string.h>
#include "dpdevice.h"
#include "convblit.h"
#include "fb.h"
#include "fblin32.h"
#include "genmem.h"


// Some helpful macros
#define lerp255(bg, fg, a) ((uint8_t)div255((fg*a+bg*(255-a))))

#define blend_color(bg, fg) RGBA(				\
	lerp255(GET_R(bg), GET_R(fg), GET_A(fg)), \
	lerp255(GET_G(bg), GET_G(fg), GET_A(fg)), \
	lerp255(GET_B(bg), GET_B(fg), GET_A(fg)), 255)



/* Set pixel at x, y, to pixelval c*/
void
linear32_drawpixel(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c)
{
	register unsigned char *addr = psd->addr + y * psd->pitch + (x << 2);
#if DEBUG
	assert(x >= 0 && x < psd->xres);
	assert(y >= 0 && y < psd->yres);
#endif
	DRAWON;
	if (gr_mode == DPROP_COPY)
		*((ADDR32)addr) = c;
	else
		APPLYOP(gr_mode, 1, (uint32_t), c, *(ADDR32), addr, 0, 0);
	DRAWOFF;

	if (psd->Update)
		psd->Update(psd, x, y, 1, 1);
}

/* Read pixel at x, y*/
DPPIXELVAL
linear32_readpixel(PSD psd, DPCOORD x, DPCOORD y)
{
	register unsigned char *addr = psd->addr + y * psd->pitch + (x << 2);
#if DEBUG
	assert(x >= 0 && x < psd->xres);
	assert(y >= 0 && y < psd->yres);
#endif
	return *((ADDR32)addr);
}

/* Draw horizontal line from x1,y to x2,y including final point*/
void
linear32_drawhorzline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c)
{
	register uint8_t *addr = psd->addr + y * psd->pitch + (x1 << 2);
	int lwidth = x2 - x1 + 1;
#if DEBUG
	assert(x1 >= 0 && x1 < psd->xres);
	assert(x2 >= 0 && x2 < psd->xres);
	assert(x2 >= x1);
	assert(y >= 0 && y < psd->yres);
#endif
	DRAWON;
	switch (gr_mode) 
	{
		case DPROP_COPY:
		{
			int w = lwidth;
			while (--w >= 0)
			{
				*((ADDR32)addr) = c;
				addr += 4;
			}
		}
		break;

		case DPROP_SRC_OVER:
		{
			DPPIXELVAL fg = c;

			int w = lwidth;
			while (--w >= 0)
			{
				DPPIXELVAL bg = *addr;
				*((ADDR32)addr) = blend_color(bg, fg);
				addr += 4;
			}
		}
		break;

		default:
			APPLYOP(gr_mode, lwidth, (uint32_t), c, *(ADDR32), addr, 0, 4);

	}
	DRAWOFF;

	if (psd->Update)
		psd->Update(psd, x1, y, lwidth, 1);
}

/* Draw a vertical line from x,y1 to x,y2 including final point*/
void
linear32_drawvertline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c)
{
	int	pitch = psd->pitch;
	register unsigned char *addr = psd->addr + y1 * pitch + (x << 2);
	int height = y2 - y1 + 1;
#if DEBUG
	assert(x >= 0 && x < psd->xres);
	assert(y1 >= 0 && y1 < psd->yres);
	assert(y2 >= 0 && y2 < psd->yres);
	assert(y2 >= y1);
#endif
	DRAWON;
	if (gr_mode == DPROP_COPY)
	{
		int h = height;
		while (--h >= 0)
		{
			*((ADDR32)addr) = c;
			addr += pitch;
		}
	}
	else
		APPLYOP(gr_mode, height, (uint32_t), c, *(ADDR32), addr, 0, pitch);
	DRAWOFF;

	if (psd->Update)
		psd->Update(psd, x, y1, 1, height);
}

/* BGRA subdriver*/
SUBDRIVER fblinear32bgra_none = {
	linear32_drawpixel,
	linear32_readpixel,
	linear32_drawhorzline,
	linear32_drawvertline,
	gen_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_bgra,		/* ft2 non-alias*/
	convblit_copy_mask_mono_byte_lsb_bgra,		/* t1 non-alias*/
	convblit_copy_mask_mono_word_msb_bgra,		/* core/pcf non-alias*/
	convblit_blend_mask_alpha_byte_bgra,		/* ft2/t1 antialias*/
	convblit_copy_rgba8888_bgra8888,			/* RGBA image copy (GdArea MWPF_RGB)*/
	convblit_srcover_rgba8888_bgra8888,			/* RGBA images w/alpha*/
	convblit_copy_rgb888_bgra8888,				/* RGB images no alpha*/
	frameblit_stretch_rgba8888_bgra8888			/* RGBA stretchblit*/
};

SUBDRIVER fblinear32bgra_left = {
	fbportrait_left_drawpixel,
	fbportrait_left_readpixel,
	fbportrait_left_drawhorzline,
	fbportrait_left_drawvertline,
	fbportrait_left_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_bgra,
	convblit_copy_mask_mono_byte_lsb_bgra,
	convblit_copy_mask_mono_word_msb_bgra,
	convblit_blend_mask_alpha_byte_bgra,
	convblit_copy_rgba8888_bgra8888,
	convblit_srcover_rgba8888_bgra8888,
	convblit_copy_rgb888_bgra8888,
	frameblit_stretch_rgba8888_bgra8888
};

SUBDRIVER fblinear32bgra_right = {
	fbportrait_right_drawpixel,
	fbportrait_right_readpixel,
	fbportrait_right_drawhorzline,
	fbportrait_right_drawvertline,
	fbportrait_right_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_bgra,
	convblit_copy_mask_mono_byte_lsb_bgra,
	convblit_copy_mask_mono_word_msb_bgra,
	convblit_blend_mask_alpha_byte_bgra,
	convblit_copy_rgba8888_bgra8888,
	convblit_srcover_rgba8888_bgra8888,
	convblit_copy_rgb888_bgra8888,
	frameblit_stretch_rgba8888_bgra8888
};

SUBDRIVER fblinear32bgra_down = {
	fbportrait_down_drawpixel,
	fbportrait_down_readpixel,
	fbportrait_down_drawhorzline,
	fbportrait_down_drawvertline,
	fbportrait_down_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_bgra,
	convblit_copy_mask_mono_byte_lsb_bgra,
	convblit_copy_mask_mono_word_msb_bgra,
	convblit_blend_mask_alpha_byte_bgra,
	convblit_copy_rgba8888_bgra8888,
	convblit_srcover_rgba8888_bgra8888,
	convblit_copy_rgb888_bgra8888,
	frameblit_stretch_rgba8888_bgra8888
};

PSUBDRIVER fblinear32bgra[4] = {
	&fblinear32bgra_none, &fblinear32bgra_left, &fblinear32bgra_right, &fblinear32bgra_down
};

/* RGBA subdriver*/
SUBDRIVER fblinear32rgba_none = {
	linear32_drawpixel,
	linear32_readpixel,
	linear32_drawhorzline,
	linear32_drawvertline,
	gen_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_rgba,		/* ft2 non-alias*/
	convblit_copy_mask_mono_byte_lsb_rgba,		/* t1 non-alias*/
	convblit_copy_mask_mono_word_msb_rgba,		/* core/pcf non-alias*/
	convblit_blend_mask_alpha_byte_rgba,		/* ft2/t1 antialias*/
	convblit_copy_rgba8888_rgba8888,			/* RGBA image copy (GdArea MWPF_RGB)*/
	convblit_srcover_rgba8888_rgba8888,			/* RGBA images w/alpha*/
	convblit_copy_rgb888_rgba8888,				/* RGB images no alpha*/
	frameblit_stretch_xxxa8888					/* RGBA -> RGBA stretchblit*/
};

SUBDRIVER fblinear32rgba_left = {
	fbportrait_left_drawpixel,
	fbportrait_left_readpixel,
	fbportrait_left_drawhorzline,
	fbportrait_left_drawvertline,
	fbportrait_left_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_rgba,
	convblit_copy_mask_mono_byte_lsb_rgba,
	convblit_copy_mask_mono_word_msb_rgba,
	convblit_blend_mask_alpha_byte_rgba,
	convblit_copy_rgba8888_rgba8888,
	convblit_srcover_rgba8888_rgba8888,
	convblit_copy_rgb888_rgba8888,
	frameblit_stretch_xxxa8888
};

SUBDRIVER fblinear32rgba_right = {
	fbportrait_right_drawpixel,
	fbportrait_right_readpixel,
	fbportrait_right_drawhorzline,
	fbportrait_right_drawvertline,
	fbportrait_right_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_rgba,
	convblit_copy_mask_mono_byte_lsb_rgba,
	convblit_copy_mask_mono_word_msb_rgba,
	convblit_blend_mask_alpha_byte_rgba,
	convblit_copy_rgba8888_rgba8888,
	convblit_srcover_rgba8888_rgba8888,
	convblit_copy_rgb888_rgba8888,
	frameblit_stretch_xxxa8888
};

SUBDRIVER fblinear32rgba_down = {
	fbportrait_down_drawpixel,
	fbportrait_down_readpixel,
	fbportrait_down_drawhorzline,
	fbportrait_down_drawvertline,
	fbportrait_down_fillrect,
	NULL,			/* no fallback Blit - uses BlitFrameBlit*/
	frameblit_xxxa8888,
	frameblit_stretch_xxxa8888,
	convblit_copy_mask_mono_byte_msb_rgba,
	convblit_copy_mask_mono_byte_lsb_rgba,
	convblit_copy_mask_mono_word_msb_rgba,
	convblit_blend_mask_alpha_byte_rgba,
	convblit_copy_rgba8888_rgba8888,
	convblit_srcover_rgba8888_rgba8888,
	convblit_copy_rgb888_rgba8888,
	frameblit_stretch_xxxa8888
};

PSUBDRIVER fblinear32rgba[4] = {
	&fblinear32rgba_none, &fblinear32rgba_left, &fblinear32rgba_right, &fblinear32rgba_down
};
