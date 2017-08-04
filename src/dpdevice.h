#pragma once
/*
	Very basic low level definitions of drawing, keyboard and mouse devices

*/


#include "dptypes.h"			// public export typedefs
#include "dpsystem.h"
#include "global_state.h"




// determine compiler capability for handling EPRINTF/DPRINTF macros
#if (defined(GCC_VERSION) && (GCC_VERSION >= 2093)) || (defined(__GNUC__) && (((__GNUC__ >= 2) && (__GNUC_MINOR__ >= 95)) || (__GNUC__ > 2)))
#define HAVE_VARARG_MACROS	1
#else
#define HAVE_VARARG_MACROS	0
#endif

/* see if can use GCC compiler-only macro magic to save space */
#if HAVE_VARARG_MACROS && HAVE_FPRINTF
#include <stdio.h>    				/* For stderr */
#define EPRINTF(str, args...)   fprintf(stderr, str, ##args)  /* error output*/
#if DEBUG
#define DPRINTF(str, args...)   fprintf(stderr, str, ##args)  /* debug output*/
#else
#define DPRINTF(str, ...)			/* no debug output*/
#endif

#else	/* must call GdError*/

#define EPRINTF			GdError		/* error output*/
#if DEBUG
# define DPRINTF		GdError		/* debug output*/
#else
# if HAVE_VARARG_MACROS
# define DPRINTF(str, ...)			/* no debug output*/
# else
# define DPRINTF		GdErrorNull	/* no debug output*/
# endif
#endif

#endif /* HAVE_VARARG_MACROS && HAVE_FPRINTF*/

/* Sanity check: VTSWITCH involves a timer. */
#if VTSWITCH && !MW_FEATURE_TIMERS
#error VTSWITCH depends on MW_FEATURE_TIMERS - disable VTSWITCH in config or enable MW_FEATURE_TIMERS in Arch.rules
#endif



/* PSD flags*/
#define	PSF_SCREEN			0x0001	/* screen device*/
#define PSF_MEMORY			0x0002	/* memory device*/
#define PSF_ADDRMALLOC		0x0010	/* psd->addr was malloc'd*/
#define PSF_ADDRSHAREDMEM	0x0020	/* psd->addr is shared memory*/
#define PSF_IMAGEHDR		0x0040	/* psd is actually MWIMAGEHDR*/


#define MOUSE_NORMAL		0x0000	/* mouse in normal mode*/
#define MOUSE_RAW			0x0001	/* mouse in raw mode*/
#define MOUSE_TRANSFORM		0x0002	/* perform transform*/



/* Clip areas*/
#define CLIP_VISIBLE		0
#define CLIP_INVISIBLE		1
#define CLIP_PARTIAL		2

#define	MAX_CLIPRECTS 	200			/* max clip rects (obsolete)*/

/* GdMakePaletteConversionTable bLoadType types*/
#define LOADPALETTE	1	/* load image palette into system palette*/
#define MERGEPALETTE	2	/* merge image palette into system palette*/


#include "dp_gddevice.h"

										   /* no assert() in MSDOS or ELKS...*/
#if MSDOS | ELKS | PSP
#undef assert
#define assert(x)
#endif

#if RTEMS
										   /* RTEMS requires rtems_main()*/
int rtems_main(int, char **);
#define main	rtems_main
#endif
