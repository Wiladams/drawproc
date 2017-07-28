#pragma once
/*
* Copyright (c) 2000 Greg Haerr <greg@censoft.com>
*
* Screen Driver Utilities
*
* Microwindows memory device routines header file
*/
#include "drawproc.h"

/* entry points*/

/* genmem.c*/
PSD		GdCreatePixmap(PSD rootpsd, DPCOORD width, DPCOORD height, int format, void *pixels,
	int palsize);
void	GdFreePixmap(PSD pmd);

PSD 	gen_allocatememgc(PSD psd);
bool	gen_mapmemgc(PSD mempsd, DPCOORD w, DPCOORD h, int planes, int bpp, int data_format,
	unsigned int pitch, int size, void *addr);
void	gen_freememgc(PSD mempsd);

int		GdCalcMemGCAlloc(PSD psd, int width, int height, int planes, int bpp,
	unsigned int *size, unsigned int *ppitch);

void	gen_fillrect(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c);

void	gen_setportrait(PSD psd, int portraitmode);
void	set_portrait_subdriver(PSD psd);

void	set_subdriver(PSD psd, PSUBDRIVER subdriver);
void	get_subdriver(PSD psd, PSUBDRIVER subdriver);
