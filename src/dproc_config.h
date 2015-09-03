#pragma once

#if defined(BUILD_AS_DLL)
  #define DPROC_API		__declspec(dllexport)
#else
  #define DPROC_API		__declspec(dllimport)
#endif

#define DPROC_EXPORT		__declspec(dllexport)




#ifdef _MSC_VER
#include <intrin.h>
#endif

typedef float	float32;
typedef double	float64;
typedef float32 real;
typedef float coord;

typedef real real2[2];
typedef real real3[3];
typedef real real4[4];
