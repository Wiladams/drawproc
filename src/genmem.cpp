/*
* Copyright (c) 2000, 2001, 2010 Greg Haerr <greg@censoft.com>
* Portions Copyright (c) 2002 by Koninklijke Philips Electronics N.V.
*
* Screen Driver Utilities
*
* Microwindows memory device routines
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "dpdevice.h"
#include "dp_win32.h"
#include "fb.h"
#include "genmem.h"





/* allocate a memory offscreen screen device (pixmap)*/
PSD
gen_allocatememgc(PSD psd)
{
	PSD	mempsd;

	assert(psd == &scrdev);

	mempsd = (PSD)malloc(sizeof(SCREENDEVICE));
	if (!mempsd)
		return NULL;

	/* copy passed device get initial values*/
	*mempsd = *psd;

	/* initialize*/
	mempsd->flags = PSF_MEMORY;			/* reset PSF_SCREEN or PSF_ADDRMALLOC flags*/
	mempsd->portrait = DPPORTRAIT_NONE; /* don't rotate offscreen pixmaps*/
	mempsd->addr = NULL;
	mempsd->Update = NULL;				/* no external updates required for mem device*/
	mempsd->palette = NULL;				/* don't copy any palette*/
	mempsd->palsize = 0;
	mempsd->transcolor = DPNOCOLOR;		/* no transparent colors unless set by image loader*/

	return mempsd;
}

/*
* Initialize memory device with passed parms,
* select suitable framebuffer subdriver,
* and set subdriver in memory device.
*
* Pixmaps are always drawn using linear fb drivers
* in non-portrait mode.
*/
bool
gen_mapmemgc(PSD mempsd, DPCOORD w, DPCOORD h, int planes, int bpp, int data_format,
	unsigned int pitch, int size, void *addr)
{
	PSUBDRIVER subdriver;

	assert(mempsd->flags & PSF_MEMORY);

	/* pixmaps are always in non-portrait orientation*/
	mempsd->xres = w;
	mempsd->yres = h;

	mempsd->xvirtres = w;
	mempsd->yvirtres = h;
	mempsd->planes = planes;
	mempsd->bpp = bpp;
	mempsd->data_format = data_format;
	mempsd->pitch = pitch;
	mempsd->size = size;
	mempsd->addr = (uint8_t *)addr;

	/* select and init hw compatible framebuffer subdriver for pixmap drawing*/
	subdriver = select_fb_subdriver(mempsd);
	if (!subdriver)
		return 0;

	/* assign portrait subdriver or regular fb driver for pixmap drawing*/
	set_portrait_subdriver(mempsd);

	return 1;
}

void
gen_freememgc(PSD mempsd)
{
	assert(mempsd->flags & PSF_MEMORY);
	//	/* may be an image header, just return*/
	//	if (!(mempsd->flags & PSF_MEMORY))
	//		return;

	if (mempsd->addr && (mempsd->flags & PSF_ADDRMALLOC))
		free(mempsd->addr);

	if (mempsd->palette)
		free(mempsd->palette);
	free(mempsd);
}

void
gen_setportrait(PSD psd, int portraitmode)
{
	psd->portrait = portraitmode;

	/* swap x and y in left or right portrait modes*/
	if (portraitmode & (DPPORTRAIT_LEFT | DPPORTRAIT_RIGHT)) {
		/* swap x, y*/
		psd->xvirtres = psd->yres;
		psd->yvirtres = psd->xres;
	}
	else {
		/* normal x, y*/
		psd->xvirtres = psd->xres;
		psd->yvirtres = psd->yres;
	}

	/* assign portrait subdriver or original driver*/
	set_portrait_subdriver(psd);
}

/*
* Set portrait subdriver or original subdriver according
* to current portrait mode.
*/
void
set_portrait_subdriver(PSD psd)
{
	PSUBDRIVER subdriver;

	switch (psd->portrait) {
	case DPPORTRAIT_NONE:
	default:
		subdriver = psd->orgsubdriver;
		break;
	case DPPORTRAIT_LEFT:
		subdriver = psd->left_subdriver;
		break;
	case DPPORTRAIT_RIGHT:
		subdriver = psd->right_subdriver;
		break;
	case DPPORTRAIT_DOWN:
		subdriver = psd->down_subdriver;
		break;
	}
	set_subdriver(psd, subdriver);
}

void
gen_fillrect(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c)
{
	/* temporarily stop updates for speed*/
	void(*Update)(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height) = psd->Update;
	int X1 = x1;
	int Y1 = y1;
	psd->Update = NULL;

	if (psd->portrait & (DPPORTRAIT_LEFT | DPPORTRAIT_RIGHT))
		while (x1 <= x2)
			psd->DrawVertLine(psd, x1++, y1, y2, c);
	else
		while (y1 <= y2)
			psd->DrawHorzLine(psd, x1, x2, y1++, c);

	/* now redraw once if external update required*/
	if (Update) {
		Update(psd, X1, Y1, x2 - X1 + 1, y2 - Y1 + 1);
		psd->Update = Update;
	}
}

/*
* Set subdriver entry points in screen device
*/
void
set_subdriver(PSD psd, PSUBDRIVER subdriver)
{
	/* set subdriver entry points in screen driver*/
	psd->DrawPixel = subdriver->DrawPixel;
	psd->ReadPixel = subdriver->ReadPixel;
	psd->DrawHorzLine = subdriver->DrawHorzLine;
	psd->DrawVertLine = subdriver->DrawVertLine;
	psd->FillRect = subdriver->FillRect;
	psd->BlitFallback = subdriver->BlitFallback;
	psd->FrameBlit = subdriver->FrameBlit;
	psd->FrameStretchBlit = subdriver->FrameStretchBlit;
	psd->BlitCopyMaskMonoByteMSB = subdriver->BlitCopyMaskMonoByteMSB;
	psd->BlitCopyMaskMonoByteLSB = subdriver->BlitCopyMaskMonoByteLSB;
	psd->BlitCopyMaskMonoWordMSB = subdriver->BlitCopyMaskMonoWordMSB;
	psd->BlitBlendMaskAlphaByte = subdriver->BlitBlendMaskAlphaByte;
	psd->BlitCopyRGBA8888 = subdriver->BlitCopyRGBA8888;
	psd->BlitSrcOverRGBA8888 = subdriver->BlitSrcOverRGBA8888;
	psd->BlitCopyRGB888 = subdriver->BlitCopyRGB888;
	psd->BlitStretchRGBA8888 = subdriver->BlitStretchRGBA8888;
}

/* fill in a subdriver struct from passed screen device*/
void
get_subdriver(PSD psd, PSUBDRIVER subdriver)
{
	/* set subdriver entry points in screen driver*/
	subdriver->DrawPixel = psd->DrawPixel;
	subdriver->ReadPixel = psd->ReadPixel;
	subdriver->DrawHorzLine = psd->DrawHorzLine;
	subdriver->DrawVertLine = psd->DrawVertLine;
	subdriver->FillRect = psd->FillRect;
	subdriver->BlitFallback = psd->BlitFallback;
	subdriver->FrameBlit = psd->FrameBlit;
	subdriver->FrameStretchBlit = psd->FrameStretchBlit;
	subdriver->BlitCopyMaskMonoByteMSB = psd->BlitCopyMaskMonoByteMSB;
	subdriver->BlitCopyMaskMonoByteLSB = psd->BlitCopyMaskMonoByteLSB;
	subdriver->BlitCopyMaskMonoWordMSB = psd->BlitCopyMaskMonoWordMSB;
	subdriver->BlitBlendMaskAlphaByte = psd->BlitBlendMaskAlphaByte;
	subdriver->BlitCopyRGBA8888 = psd->BlitCopyRGBA8888;
	subdriver->BlitSrcOverRGBA8888 = psd->BlitSrcOverRGBA8888;
	subdriver->BlitCopyRGB888 = psd->BlitCopyRGB888;
	subdriver->BlitStretchRGBA8888 = psd->BlitStretchRGBA8888;
}
