#pragma once
/*
* Conversion blits - public header file
*/
#include "drawproc.h"

/* for convenience in specifying inline parms*/
#define R		0		/* RGBA parms*/
#define G		1
#define B		2
#define A		3

#define NONE	DPPORTRAIT_NONE
#define LEFT	DPPORTRAIT_LEFT
#define RIGHT	DPPORTRAIT_RIGHT
#define DOWN	DPPORTRAIT_DOWN

#define COPY	0		/* mode parm*/
#define SRCOVER	1

/* convblit_8888.c*/

/* ----- 32bpp output -----*/
void convblit_srcover_rgba8888_rgba8888(PSD psd, PDPBLITPARMS gc);	// png/tiff 32bpp RGBA srcover
void convblit_copy_rgba8888_rgba8888(PSD psd, PDPBLITPARMS gc);		// 32bpp RGBA to 32bpp RGBA copy
void convblit_copy_rgb888_rgba8888(PSD psd, PDPBLITPARMS gc);		// png/jpg 24bpp RGB copy

void convblit_srcover_rgba8888_bgra8888(PSD psd, PDPBLITPARMS gc);	// png/tiff 32bpp RGBA srcover
void convblit_copy_rgba8888_bgra8888(PSD psd, PDPBLITPARMS gc);		// 32bpp RGBA to 32bpp BGRA copy
void convblit_copy_rgb888_bgra8888(PSD psd, PDPBLITPARMS gc);		// png/jpg 24bpp RGB copy

void convblit_copy_8888_8888(PSD psd, PDPBLITPARMS gc);				// 32bpp to 32bpp copy

																	/* ----- 24bpp output -----*/
void convblit_srcover_rgba8888_bgr888(PSD psd, PDPBLITPARMS gc);
void convblit_copy_rgba8888_bgr888(PSD psd, PDPBLITPARMS gc);		// 32bpp RGBA to 24bpp BGR copy
void convblit_copy_rgb888_bgr888(PSD psd, PDPBLITPARMS gc);

void convblit_copy_888_888(PSD psd, PDPBLITPARMS gc);				// 24bpp to 24bpp copy

void convblit_copy_bgra8888_bgr888(PSD psd, PDPBLITPARMS gc);		// 32bpp BGRA to 24bpp BGR copy

																	/* ----- 16bpp output -----*/
void convblit_srcover_rgba8888_16bpp(PSD psd, PDPBLITPARMS gc);
void convblit_copy_rgba8888_16bpp(PSD psd, PDPBLITPARMS gc);		// 32bpp RGBA to 16bpp copy
void convblit_copy_rgb888_16bpp(PSD psd, PDPBLITPARMS gc);

void convblit_copy_16bpp_16bpp(PSD psd, PDPBLITPARMS gc);			// 16bpp to 16bpp copy

																	/* convblit_mask.c*/
																	/* 1bpp and 8bpp (alphablend) mask conversion blits - for font display*/

																	/* ----- 32bpp output -----*/
void convblit_copy_mask_mono_byte_msb_rgba(PSD psd, PDPBLITPARMS gc);
void convblit_copy_mask_mono_byte_lsb_rgba(PSD psd, PDPBLITPARMS gc);
void convblit_copy_mask_mono_word_msb_rgba(PSD psd, PDPBLITPARMS gc);
void convblit_blend_mask_alpha_byte_rgba(PSD psd, PDPBLITPARMS gc);

void convblit_copy_mask_mono_byte_msb_bgra(PSD psd, PDPBLITPARMS gc);		/* ft2 non-alias*/
void convblit_copy_mask_mono_byte_lsb_bgra(PSD psd, PDPBLITPARMS gc);		/* t1lib non-alias*/
void convblit_copy_mask_mono_word_msb_bgra(PSD psd, PDPBLITPARMS gc);		/* pcf non-alias*/
void convblit_blend_mask_alpha_byte_bgra(PSD psd, PDPBLITPARMS gc);			/* ft2/t1lib alias*/

																			/* ----- 24bpp output -----*/
void convblit_copy_mask_mono_byte_msb_bgr(PSD psd, PDPBLITPARMS gc);
void convblit_copy_mask_mono_byte_lsb_bgr(PSD psd, PDPBLITPARMS gc);
void convblit_copy_mask_mono_word_msb_bgr(PSD psd, PDPBLITPARMS gc);
void convblit_blend_mask_alpha_byte_bgr(PSD psd, PDPBLITPARMS gc);

/* ----- 16bpp output -----*/
void convblit_copy_mask_mono_byte_msb_16bpp(PSD psd, PDPBLITPARMS gc);
void convblit_copy_mask_mono_byte_lsb_16bpp(PSD psd, PDPBLITPARMS gc);
void convblit_copy_mask_mono_word_msb_16bpp(PSD psd, PDPBLITPARMS gc);
void convblit_blend_mask_alpha_byte_16bpp(PSD psd, PDPBLITPARMS gc);

#if LATER
void convblit_copy_mask_mono_byte_msb_bgra_large(PSD psd, PDPBLITPARMS gc);	/* ft2 non-alias*/
void convblit_copy_mask_mono_byte_lsb_bgra_large(PSD psd, PDPBLITPARMS gc);	/* t1lib non-alias*/
#endif


																			/* convblit_frameb.c*/
																			/* framebuffer pixel format blits - must handle backwards copy, different rotation code*/
void frameblit_xxxa8888(PSD psd, PDPBLITPARMS gc);		/* 32bpp*/
void frameblit_24bpp(PSD psd, PDPBLITPARMS gc);			/* 24bpp*/
void frameblit_16bpp(PSD psd, PDPBLITPARMS gc);			/* 16bpp*/
void frameblit_8bpp(PSD psd, PDPBLITPARMS gc);			/* 8bpp*/

														/* framebuffer pixel format stretch blits - different rotation code, no backwards copy*/
void frameblit_stretch_xxxa8888(PSD dstpsd, PDPBLITPARMS gc);	/* 32bpp, alpha in byte 4*/
void frameblit_stretch_24bpp(PSD psd, PDPBLITPARMS gc);			/* 24 bpp*/
void frameblit_stretch_16bpp(PSD psd, PDPBLITPARMS gc);			/* 16 bpp*/
void frameblit_stretch_8bpp(PSD psd, PDPBLITPARMS gc);			/* 8 bpp*/

																/* these work for src_over and copy only*/
void frameblit_stretch_rgba8888_bgra8888(PSD psd, PDPBLITPARMS gc);	/* RGBA -> BGRA*/
void frameblit_stretch_rgba8888_bgr888(PSD psd, PDPBLITPARMS gc);	/* RGBA -> BGR*/
void frameblit_stretch_rgba8888_16bpp(PSD psd, PDPBLITPARMS gc);	/* RGBA -> 16bpp*/

																	/* devimage.c*/
void convblit_pal8_rgba8888(PDPBLITPARMS gc);
void convblit_pal4_msb_rgba8888(PDPBLITPARMS gc);
void convblit_pal1_byte_msb_rgba8888(PDPBLITPARMS gc);

/* image_bmp.c*/

/* Conversion blit 24bpp BGR to 24bpp RGB*/
void convblit_bgr888_rgb888(unsigned char *data, int width, int height, int pitch);
/* Conversion blit 32bpp BGRX to 32bpp RGBA 255 alpha*/
void convblit_bgrx8888_rgba8888(unsigned char *data, int width, int height, int pitch);

/* image_tiff.c*/

/* Conversion blit flip y direction 32bpp (upside-down)*/
void convblit_flipy_8888(PDPBLITPARMS gc);
