#pragma once
#include "dproc_config.h"
#include "dpdevice.h"

#ifdef __cplusplus
extern "C" {
#endif

int		DPROC_EXPORT GdCalcMemGCAlloc(PSD psd, int width, int height, int planes, int bpp,
	unsigned int *size, unsigned int *ppitch);


#ifdef __cplusplus
}
#endif
