#include "dpdevice.h"

#if DYNAMICREGIONS
#include "./devclip2.cpp"
#else
#include "devclip1.c"
#endif