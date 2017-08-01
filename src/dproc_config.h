#pragma once

#define BUILD_AS_DLL false

#if BUILD_AS_DLL
  #define DPROC_API		__declspec(dllexport)
#else
  #define DPROC_API		__declspec(dllimport)
#endif

#define DPROC_EXPORT		__declspec(dllexport)


#ifdef _MSC_VER
#include <intrin.h>
#endif

#include <stdint.h>



