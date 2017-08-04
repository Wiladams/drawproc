#pragma once

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


// the following defines are unset in Arch.rules based on ARCH= setting
#ifndef HAVE_SELECT
#define HAVE_SELECT		1		/* =1 has select system call*/
#endif

#ifndef HAVE_SIGNAL
#define HAVE_SIGNAL		1		/* =1 has signal system call*/
#endif

#ifndef HAVE_MMAP
#define HAVE_MMAP       1       /* =1 has mmap system call*/
#endif

// control whether printf/fprintf required in server and demo programs and C library 
// if this is set to =0 in Arch.rules, fprintf/printf will be no-op in all demo programs, 
// and in the server GdError will be called, which calls write() if HAVE_FILEIO=Y in config 
#ifndef HAVE_FPRINTF
#define HAVE_FPRINTF	1		// =1 EPRINTF/DPRINTF uses fprintf/printf, otherwise GdError or no-op
#endif

#ifndef HAVE_FLOAT
#define HAVE_FLOAT		1		// =1 incl float, GdArcAngle
#endif

// the following enable Microwindows features, also unset in Arch.rules
#ifndef MW_FEATURE_IMAGES
#define MW_FEATURE_IMAGES 1		// =1 to enable GdLoadImage/GdDrawImage etc
#endif

#ifndef MW_FEATURE_TIMERS
#define MW_FEATURE_TIMERS 1		// =1 to include MWTIMER support
#endif

#ifndef MW_FEATURE_TWO_KEYBOARDS
#define MW_FEATURE_TWO_KEYBOARDS 0	// =1 to include multiple keybioard support
#endif

#ifndef DYNAMICREGIONS
#define DYNAMICREGIONS	1		// =1 to use DPCLIPREGIONS
#endif


// Changeable limits and options
#define UNIFORMPALETTE	1		/* =1 for 256 entry uniform palette (required for palette alpha blending)*/
#define POLYREGIONS		1		/* =1 includes polygon regions*/
#define ANIMATEPALETTE	0		/* =1 animated palette test*/
#define FT_MINAA_HEIGHT	0		/* min height for FT antialias with win32 plogfont*/
#define TRANSLATE_ESCAPE_SEQUENCES  1		/* =1 to parse fnkeys w/tty driver*/
#define DEBUG_EXPOSE	0		/* =1 to flash yellow before painting expose areas*/
#define DEBUG_BLIT		0		/* =1 to flash brown before painting areas with convblit*/


