#pragma once
#include "dproc_config.h"
#include "dpdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

DPROC_API extern int	GdCalcMemGCAlloc(PSD psd, int width, int height, int planes, int bpp,
	unsigned int *size, unsigned int *ppitch);

DPROC_API extern PSD	GdCreatePixmap(PSD rootpsd, DPCOORD width, DPCOORD height, int format,
	void *pixels,int palsize);

DPROC_API void			GdFreePixmap(PSD pmd);

#ifdef __cplusplus
}
#endif
