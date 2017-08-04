#pragma once
#include "dproc_types.h"

DPROC_API extern DPPIXELVAL linear32_readpixel(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern void linear32_drawpixel(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c);
DPROC_API extern void linear32_drawhorzline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c);
DPROC_API extern void linear32_drawvertline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c);

DPROC_API extern SUBDRIVER fblinear32rgba_none;

