#pragma once
#include "dproc_config.h"
#include "dpdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

int		DPROC_EXPORT GdCalcMemGCAlloc(PSD psd, int width, int height, int planes, int bpp,
	unsigned int *size, unsigned int *ppitch);

PSD		DPROC_EXPORT GdCreatePixmap(PSD rootpsd, DPCOORD width, DPCOORD height, int format, 
	void *pixels,int palsize);

void	DPROC_EXPORT GdFreePixmap(PSD pmd);

#ifdef __cplusplus
}
#endif
