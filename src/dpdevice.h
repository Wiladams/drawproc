#pragma once
/*
	Very basic low level definitions of drawing, keyboard and mouse devices

*/


#include "dptypes.h"			// public export typedefs
//#include "mwsystem.h"

// Changeable limits and options
#define UNIFORMPALETTE	1		/* =1 for 256 entry uniform palette (required for palette alpha blending)*/
#define POLYREGIONS		1		/* =1 includes polygon regions*/
#define ANIMATEPALETTE	0		/* =1 animated palette test*/
#define FT_MINAA_HEIGHT	0		/* min height for FT antialias with win32 plogfont*/
#define TRANSLATE_ESCAPE_SEQUENCES  1		/* =1 to parse fnkeys w/tty driver*/
#define DEBUG_EXPOSE	0		/* =1 to flash yellow before painting expose areas*/
#define DEBUG_BLIT		0		/* =1 to flash brown before painting areas with convblit*/


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

typedef void(*DPBLITFUNC)(PSD, PDPBLITPARMS);		/* proto for blitter functions*/

													/* screen subdriver entry points: one required for each draw function*/
typedef struct {
	void(*DrawPixel)(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c);
	DPPIXELVAL(*ReadPixel)(PSD psd, DPCOORD x, DPCOORD y);
	void(*DrawHorzLine)(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c);
	void(*DrawVertLine)(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c);
	void(*FillRect)(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c);
	/* fallback blit - used only for 1,2,4bpp drivers*/
	void(*BlitFallback)(PSD destpsd, DPCOORD destx, DPCOORD desty, DPCOORD w, DPCOORD h,
		PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int op);
	/* endian neutral hw pixel format blits*/
	DPBLITFUNC FrameBlit;
	DPBLITFUNC FrameStretchBlit;
	/* fast conversion blits for text and images*/
	DPBLITFUNC BlitCopyMaskMonoByteMSB;				/* ft non-alias*/
	DPBLITFUNC BlitCopyMaskMonoByteLSB;				/* t1 non-alias*/
	DPBLITFUNC BlitCopyMaskMonoWordMSB;				/* core/pcf non-alias*/
	DPBLITFUNC BlitBlendMaskAlphaByte;				/* ft2/t1 antialias*/
	DPBLITFUNC BlitCopyRGBA8888;					/* GdArea RGBA MWPF_RGB image copy*/
	DPBLITFUNC BlitSrcOverRGBA8888;					/* png RGBA image w/alpha*/
	DPBLITFUNC BlitCopyRGB888;						/* png RGB image no alpha*/
	DPBLITFUNC BlitStretchRGBA8888;					/* conversion stretch blit for RGBA src*/
} SUBDRIVER, *PSUBDRIVER;

/*
* Interface to Screen Device Driver
* This structure is also allocated for memory (offscreen) drawing and blitting.
*/
typedef struct _mwscreendevice {
	/* shared header with MWIMAGEHDR*/
	int		flags;		/* PSF_SCREEN or PSF_MEMORY*/
	DPCOORD	xvirtres;	/* X drawing res (will be flipped in portrait mode) */
	DPCOORD	yvirtres;	/* Y drawing res (will be flipped in portrait mode) */
	int		planes;		/* # planes*/
	int		bpp;		/* # bpp*/
	int 	data_format;/* MWIF_ image data format*/
	size_t	pitch;		/* row length in bytes*/
	uint8_t *addr;		/* address of memory allocated (memdc or fb)*/
	int		palsize;	/* palette size*/
	MWPALENTRY *palette;/* palette*/
	int32_t	transcolor;	/* not used*/
						/* end of shared header*/

	DPCOORD	xres;		/* X screen res (real) */
	DPCOORD	yres;		/* Y screen res (real) */
	unsigned int size;	/* size of memory allocated*/
	int32_t	ncolors;	/* # screen colors*/
	int	pixtype;		/* format of pixel value*/

						/* driver entry points*/
	PMWCOREFONT builtin_fonts;
	PSD(*Open)(PSD psd);
	void(*Close)(PSD psd);
	void(*SetPalette)(PSD psd, int first, int count, MWPALENTRY *pal);
	void(*GetScreenInfo)(PSD psd, PDPSCREENINFO psi);
	PSD(*AllocateMemGC)(PSD psd);
	DPBOOL(*MapMemGC)(PSD mempsd, DPCOORD w, DPCOORD h, int planes, int bpp,
		int data_format, unsigned int pitch, int size, void *addr);
	void(*FreeMemGC)(PSD mempsd);
	void(*SetPortrait)(PSD psd, int portraitmode);
	void(*Update)(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height);
	void(*PreSelect)(PSD psd);
	int	portrait;	 /* screen portrait mode*/
	PSUBDRIVER orgsubdriver; /* original subdriver for portrait modes*/
	PSUBDRIVER left_subdriver;
	PSUBDRIVER right_subdriver;
	PSUBDRIVER down_subdriver;
	/* SUBDRIVER functions*/
	void(*DrawPixel)(PSD psd, DPCOORD x, DPCOORD y, DPPIXELVAL c);
	DPPIXELVAL(*ReadPixel)(PSD psd, DPCOORD x, DPCOORD y);
	void(*DrawHorzLine)(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, DPPIXELVAL c);
	void(*DrawVertLine)(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, DPPIXELVAL c);
	void(*FillRect)(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPPIXELVAL c);
	/* fallback blit - used only for 1,2,4bpp drivers*/
	void(*BlitFallback)(PSD destpsd, DPCOORD destx, DPCOORD desty, DPCOORD w, DPCOORD h,
		PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int op);
	/* endian neutral hw pixel format blits*/
	DPBLITFUNC FrameBlit;
	DPBLITFUNC FrameStretchBlit;
	/* fast conversion blits for text and images*/
	DPBLITFUNC BlitCopyMaskMonoByteMSB;				/* ft non-alias*/
	DPBLITFUNC BlitCopyMaskMonoByteLSB;				/* t1 non-alias*/
	DPBLITFUNC BlitCopyMaskMonoWordMSB;				/* core/pcf non-alias*/
	DPBLITFUNC BlitBlendMaskAlphaByte;				/* ft2/t1 antialias*/
	DPBLITFUNC BlitCopyRGBA8888;					/* GdArea RGBA MWPF_RGB image copy*/
	DPBLITFUNC BlitSrcOverRGBA8888;					/* png RGBA image w/alpha*/
	DPBLITFUNC BlitCopyRGB888;						/* png RGB image no alpha*/
	DPBLITFUNC BlitStretchRGBA8888;					/* conversion stretch blit for RGBA src*/
} SCREENDEVICE;

/* PSD flags*/
#define	PSF_SCREEN			0x0001	/* screen device*/
#define PSF_MEMORY			0x0002	/* memory device*/
#define PSF_ADDRMALLOC		0x0010	/* psd->addr was malloc'd*/
#define PSF_ADDRSHAREDMEM	0x0020	/* psd->addr is shared memory*/
#define PSF_IMAGEHDR		0x0040	/* psd is actually MWIMAGEHDR*/

/* Interface to Mouse Device Driver*/
typedef struct _mousedevice {
	int(*Open)(struct _mousedevice *);
	void(*Close)(void);
	int(*GetButtonInfo)(void);
	void(*GetDefaultAccel)(int *pscale, int *pthresh);
	int(*Read)(DPCOORD *dx, DPCOORD *dy, DPCOORD *dz, int *bp);
	int(*Poll)(void);	/* not required if have select()*/
	int     flags;			/* raw, normal, transform flags*/
} MOUSEDEVICE;

#define MOUSE_NORMAL		0x0000	/* mouse in normal mode*/
#define MOUSE_RAW			0x0001	/* mouse in raw mode*/
#define MOUSE_TRANSFORM		0x0002	/* perform transform*/

/* Interface to Keyboard Device Driver*/
typedef struct _kbddevice {
	int(*Open)(struct _kbddevice *pkd);
	void(*Close)(void);
	void(*GetModifierInfo)(MWKEYMOD *modifiers, MWKEYMOD *curmodifiers);
	int(*Read)(MWKEY *buf, MWKEYMOD *modifiers, MWSCANCODE *scancode);
	int(*Poll)(void);		/* not required if have select()*/
} KBDDEVICE;

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

