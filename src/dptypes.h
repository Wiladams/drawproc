#pragma once
#ifndef _DPTYPES_H
#define _DPTYPES_H
/*
* Copyright (c) 1999, 2000, 2001, 2002, 2003, 2005, 2010, 2011 Greg Haerr <greg@censoft.com>
* Portions Copyright (c) 2002 by Koninklijke Philips Electronics N.V.
*
* Exported Microwindows engine typedefs and defines
*/
#include <stdint.h> 		/* for uint32_t, int32_t*/


/* configurable options*/
#define USE_ALLOCA	1			/* alloca() is available */


/* force byte-packed structures and inlining*/
#if defined(__GNUC__)
#define MWPACKED			__attribute__ ((aligned(1), packed))	/* for structs*/
#define PACKEDDATA			__attribute__ ((__packed__))			/* for data members*/
#define ALWAYS_INLINE		__attribute__ ((always_inline))			/* force inline functions*/
#else
#define MWPACKED
#define PACKEDDATA			/* FIXME for MSVC #pragma pack(1) equiv*/
#define ALWAYS_INLINE
#endif

#if USE_ALLOCA
/* alloca() is available, so use it for better performance */
#define ALLOCA(size)	alloca(size)
#define FREEA(pmem)
#else
/* no alloca(), so use malloc()/free() instead */
#define ALLOCA(size)	malloc(size)
#define FREEA(pmem)	free(pmem)
#endif

/* builtin font std names*/
#define MWFONT_SYSTEM_VAR	"System"	/* winFreeSansSerif 11x13 (ansi)*/
#define MWFONT_SYSTEM_FIXED	"SystemFixed"	/* X6x13 (should be ansi)*/
#define MWFONT_GUI_VAR		"System"	/* deprecated (was "Helvetica")*/
#define MWFONT_OEM_FIXED	"SystemFixed"	/* deprecated (was "Terminal")*/

/* Text/GetTextSize encoding flags*/
#define MWTF_ASCII	0x00000000L	/* 8 bit packing, ascii*/
#define MWTF_UTF8	0x00000001L	/* 8 bit packing, utf8*/
#define MWTF_UC16	0x00000002L	/* 16 bit packing, unicode 16*/
#define MWTF_UC32	0x00000004L	/* 32 bit packing, unicode 32*/
#define MWTF_XCHAR2B	0x00000008L	/* 16 bit packing, X11 big endian PCF*/
#define MWTF_PACKMASK	0x0000000FL	/* packing bits mask*/

/* asian double-byte encodings*/
#define MWTF_DBCS_BIG5	0x00000100L	/* chinese big5*/
#define MWTF_DBCS_EUCCN	0x00000200L	/* chinese EUCCN (gb2312+0x80)*/
#define MWTF_DBCS_EUCKR	0x00000300L	/* korean EUCKR (ksc5601+0x80)*/
#define MWTF_DBCS_EUCJP	0x00000400L	/* japanese EUCJP*/
#define MWTF_DBCS_JIS	0x00000500L	/* japanese JISX0213*/
#define MWTF_DBCSMASK	0x00000700L	/* DBCS encodings bitmask*/

/* Text alignment flags*/
#define MWTF_TOP	0x01000000L	/* align on top*/
#define MWTF_BASELINE	0x02000000L	/* align on baseline*/
#define MWTF_BOTTOM	0x04000000L	/* align on bottom*/

/* SetFontAttr and capabilities flags (not used with MWTF_ above)*/
#define MWTF_KERNING	0x0001		/* font kerning*/
#define MWTF_ANTIALIAS	0x0002		/* antialiased output*/
#define MWTF_UNDERLINE	0x0004		/* draw underline*/
#define MWTF_BOLD		0x0008		/* draw bold glyph (not present on all renderers)*/

#define MWTF_CMAP_DEFAULT 0x0010	/* use default (unicode) charset in truetype font (not required)*/
#define MWTF_CMAP_0		  0x0020	/* use charmap 0 in truetype font*/
#define MWTF_CMAP_1       0x0040	/* use charmap 1 in truetype font*/

#define MWTF_FREETYPE	0x1000		/* FIXME: remove*/
#define MWTF_SCALEHEIGHT 0x2000		/* font can scale height seperately*/
#define MWTF_SCALEWIDTH	0x4000		/* font can scale width seperately*/

/* Image data formats, used by GdConversionBlit*/

/* bits per pixel*/
#define DPIF_1BPP			0x00000001L
#define DPIF_8BPP			0x00000008L
#define DPIF_15BPP			0x0000000FL
#define DPIF_16BPP			0x00000010L
#define DPIF_24BPP			0x00000018L
#define DPIF_32BPP			0x00000020L
#define DPIF_BPPMASK		0x0000003FL

/* monochrome bitmap formats*/
#define DPIF_MONO			0x00000040L
#define DPIF_HASALPHA		0x00000080L
#define DPIF_BYTEDATA		0x00000100L
#define DPIF_LEWORDDATA		0x00000200L		/* 16-bit little endian format (retrofit format)*/
//#define DPIF_BEQUADDATA	0x00000400L		/* 32-bit big endian format*/
#define DPIF_MSBFIRST		0x00000800L		/* highest bit displayed leftmost*/
#define DPIF_LSBFIRST		0x00001000L		/* lowest bit displayed leftmost*/
#define DPIF_MONOBYTEMSB	(DPIF_1BPP | DPIF_MONO | DPIF_BYTEDATA | DPIF_MSBFIRST)
#define DPIF_MONOBYTELSB	(DPIF_1BPP | DPIF_MONO | DPIF_BYTEDATA | DPIF_LSBFIRST)
#define DPIF_MONOWORDMSB	(DPIF_1BPP | DPIF_MONO | DPIF_LEWORDDATA | DPIF_MSBFIRST)
//#define DPIF_MONOQUADMSB	(DPIF_1BPP | DPIF_MONO | DPIF_BEQUADDATA | DPIF_MSBFIRST)
#define DPIF_ALPHABYTE		(DPIF_8BPP | DPIF_HASALPHA| DPIF_BYTEDATA)

/* framebuffer and image data formats - yet unsupported formats commented out*/
#define DPIF_BGRA8888	(0x00010000L|DPIF_HASALPHA) /* 32bpp BGRA image byte order (old TRUECOLOR8888)*/
#define DPIF_RGBA8888	(0x00020000L|DPIF_HASALPHA)	/* 32bpp RGBA image byte order (old TRUECOLORABGR)*/
//#define DPIF_ARGB8888	(0x00030000L|DPIF_HASALPHA)	/* 32bpp ARGB image byte order (new)*/
//#define DPIF_ABGR8888	(0x00040000L|DPIF_HASALPHA)	/* 32bpp ABGR image byte order (new)*/
//#define DPIF_BGRX8888	(0x00050000L|DPIF_HASALPHA)	/* 32bpp BGRX image order no alpha (new)*/
#define DPIF_BGR888		 0x00060000L		/* 24bpp BGR image byte order  (old TRUECOLOR888)*/
#define DPIF_RGB888		 0x00070000L		/* 24bpp RGB image byte order  (png no alpha)*/
#define DPIF_RGB565		 0x00080000L		/* 16bpp 5/6/5 RGB packed l.endian (old TRUECOLOR565)*/
//#define DPIF_RGB565_BE 0x00090000L		/* 16bpp 5/6/5 RGB packed b.endian (new)*/
#define DPIF_RGB555		 0x000A0000L		/* 16bpp 5/5/5 RGB packed l.endian (old TRUECOLOR555)*/
//#define DPIF_RGB555_BE 0x000B0000L		/* 16bpp 5/5/5 RGB packed b.endian (new)*/
#define DPIF_RGB1555	 0x000C0000L		        /* 16bpp 1/5/5/5 NDS color format */
//#define DPIF_BGR555_BE 0x000D0000L		/* 16bpp 5/5/5 BGR packed b.endian (new)*/
#define DPIF_RGB332		 0x000E0000L		/*  8bpp 3/3/2 RGB packed (old TRUECOLOR332)*/
#define DPIF_BGR233		 0x000F0000L		/*  8bpp 2/3/3 BGR packed (old TRUECOLOR233)*/
#define DPIF_PAL1		 DPIF_MONOBYTEMSB	/*  1bpp palette (old DPPF_PALETTE)*/
#define DPIF_PAL2		 0x00200000L		/*  2bpp palette (old DPPF_PALETTE)*/
#define DPIF_PAL4		 0x00400000L		/*  4bpp palette (old DPPF_PALETTE)*/
#define DPIF_PAL8		 0x00800000L		/*  8bpp palette (old DPPF_PALETTE)*/
//#define DPIF_PALETTE	 0x00F00000L		/* requires palette*/

/* Line modes */
#define DPLINE_SOLID      0
#define DPLINE_ONOFF_DASH 1
/* FUTURE: MWLINE_DOUBLE_DASH */

/* Fill mode  */
#define DPFILL_SOLID          0  
#define DPFILL_STIPPLE        1  
#define DPFILL_OPAQUE_STIPPLE 2  
#define DPFILL_TILE           3

/* Drawing modes (raster ops)*/
#define	DPROP_COPY			0	/* src*/
#define	DPROP_XOR			1	/* src ^ dst*/
#define	DPROP_OR			2	/* src | dst (PAINT)*/
#define	DPROP_AND			3	/* src & dst (MASK)*/
#define	DPROP_CLEAR			4	/* 0*/
#define	DPROP_SET			5	/* ~0*/
#define	DPROP_EQUIV			6	/* ~(src ^ dst)*/
#define	DPROP_NOR			7	/* ~(src | dst)*/
#define	DPROP_NAND			8	/* ~(src & dst)*/
#define	DPROP_INVERT		9	/* ~dst*/
#define	DPROP_COPYINVERTED	10	/* ~src*/
#define	DPROP_ORINVERTED	11	/* ~src | dst*/
#define	DPROP_ANDINVERTED	12	/* ~src & dst (SUBTRACT)*/
#define DPROP_ORREVERSE		13	/* src | ~dst*/
#define	DPROP_ANDREVERSE	14	/* src & ~dst*/
#define	DPROP_NOOP			15	/* dst*/
#define	DPROP_XOR_FGBG		16	/* src ^ background ^ dst (Java XOR mode)*/
#define DPROP_SIMPLE_MAX 	16	/* last non-compositing rop*/

/* Porter-Duff compositing operations.  Only SRC, CLEAR and SRC_OVER are commonly used*/
#define	DPROP_SRC			DPROP_COPY
#define	DPROP_DST			DPROP_NOOP
//#define DPROP_CLEAR		DPROP_CLEAR
#define	DPROP_SRC_OVER		17	/* dst = alphablend src,dst*/
#define	DPROP_DST_OVER		18
#define	DPROP_SRC_IN		19
#define	DPROP_DST_IN		20
#define	DPROP_SRC_OUT		21
#define	DPROP_DST_OUT		22
#define	DPROP_SRC_ATOP		23
#define	DPROP_DST_ATOP		24
#define	DPROP_PORTERDUFF_XOR 25
#define DPROP_SRCTRANSCOPY	26	/* copy src -> dst except for transparent color in src*/
#define	DPROP_MAX			26	/* last non-blit rop*/

/* blit ROP modes in addtion to DPROP_xxx */
#define DPROP_BLENDCONSTANT		32	/* alpha blend src -> dst with constant alpha*/
#define DPROP_BLENDFGBG			33	/* alpha blend fg/bg color -> dst with src alpha channel*/
//#define DPROP_BLENDCHANNEL	35	/* alpha blend src -> dst with separate per pixel alpha chan*/
//#define DPROP_STRETCH			36	/* stretch src -> dst*/
#define DPROP_USE_GC_MODE		255 /* use current GC mode for ROP.  Nano-X CopyArea only*/

//#define DPROP_SRCCOPY		DPROP_COPY	/* obsolete*/
//#define DPROP_SRCAND		DPROP_AND	/* obsolete*/
//#define DPROP_SRCINVERT	DPROP_XOR	/* obsolete*/
//#define DPROP_BLACKNESS   DPROP_CLEAR	/* obsolete*/

/*
* Pixel formats
* Note the two pseudo pixel formats are never returned by display drivers,
* but rather used as a data structure type in GrArea.  The other
* types are both returned by display drivers and used as pixel packing
* specifiers.
*/
#define DPPF_RGB	   0	/* pseudo, convert from packed 32 bit RGB*/
#define DPPF_PIXELVAL	   1	/* pseudo, no convert from packed PIXELVAL*/
#define DPPF_PALETTE	   2	/* pixel is packed 8 bits 1, 4 or 8 pal index*/
#define DPPF_TRUECOLOR888  4	/* pixel is packed 24 bits R/G/B RGB truecolor*/
#define DPPF_TRUECOLOR565  5	/* pixel is packed 16 bits 5/6/5 RGB truecolor*/
#define DPPF_TRUECOLOR555  6	/* pixel is packed 16 bits 5/5/5 RGB truecolor*/
#define DPPF_TRUECOLOR332  7	/* pixel is packed  8 bits 3/3/2 RGB truecolor*/
#define DPPF_TRUECOLOR8888 8	/* pixel is packed 32 bits A/R/G/B ARGB truecolor with alpha */
#define DPPF_TRUECOLOR0888 8	/* deprecated*/
#define DPPF_TRUECOLOR233  9	/* pixel is packed  8 bits 2/3/3 BGR truecolor*/
#define DPPF_HWPIXELVAL   10	/* pseudo, no convert, pixels are in hw format*/
#define DPPF_TRUECOLORABGR 11	/* pixel is packed 32 bits A/B/G/R ABGR truecolor with alpha */
#define DPPF_TRUECOLOR1555 12   /* pixel is packed 16 bits 1/5/5/5 NDS truecolor */

/*
* MWPIXELVALHW definition: changes based on target system
* Set using -DDPPIXEL_FORMAT=DPPF_XXX
*
* For the Nano-X server, it is important to use the correct DPPF_* value
* for the DPPIXEL_FORMAT macro in order to match the hardware,
* while the Nano-X clients that includes this file can get away with
* a default pixel format of 32-bit color as the client will either:
*    1) Use the DPPF_PIXELVAL native format when calling GrReadArea, in
*       which case we have to have enough spare room to hold 32-bit
*       pixel values (hence the default DPPF_TRUECOLOR8888 format), or
*    2) Will use some other PF_* format, in which case the application
*       is well aware of which pixel-format it uses and can avoid the
*       device specific RGB2PIXEL and use RGB2PIXEL565 etc. instead,
*       and specifiy the pixel format as DPPF_TRUECOLOR565 etc. when
*       calling the GrArea function(s).
*/
#ifndef DPPIXEL_FORMAT
#define DPPIXEL_FORMAT	DPPF_TRUECOLOR8888
#endif


#if (DPPIXEL_FORMAT == DPPF_TRUECOLOR565) || (DPPIXEL_FORMAT == DPPF_TRUECOLOR555)
typedef unsigned short MWPIXELVALHW;
#else
#if (DPPIXEL_FORMAT == DPPF_TRUECOLOR332) || (DPPIXEL_FORMAT == DPPF_TRUECOLOR233)
typedef unsigned char MWPIXELVALHW;
#else
#if DPPIXEL_FORMAT == DPPF_PALETTE
typedef unsigned char MWPIXELVALHW;
#else
typedef uint32_t DPPIXELVALHW;
#endif
#endif
#endif

// portrait modes
#define DPPORTRAIT_NONE		0x00	/* hw framebuffer, no rotation*/
#define DPPORTRAIT_LEFT		0x01	/* rotate left*/
#define	DPPORTRAIT_RIGHT	0x02	/* rotate right*/
#define DPPORTRAIT_DOWN		0x04	/* upside down*/

/*
* Type definitions
*/
typedef int				DPCOORD;	/* device coordinates*/
typedef int				DPBOOL;		/* boolean value*/
typedef unsigned char	DPUCHAR;	/* unsigned char*/
typedef uint32_t		DPCOLORVAL;	/* device-independent color value (0xAABBGGRR)*/
typedef uint32_t		DPPIXELVAL;	/* pixel value parameter type, not for packing*/
typedef unsigned short	DPIMAGEBITS;/* bitmap image unit size*/
typedef uint32_t		DPTIMEOUT;	/* timeout value */
typedef uint32_t		DPTEXTFLAGS;/* MWTF_ text flag*/

#define DPCOORD_MAX	0x7fff		/* maximum coordinate value*/
#define DPCOORD_MIN	(-DPCOORD_MAX)	/* minimum coordinate value*/

									/* max char height/width must be >= 16 and a multiple of sizeof(DPIMAGEBITS)*/
#define MAX_CHAR_HEIGHT	128			/* maximum text bitmap height*/
#define MAX_CHAR_WIDTH	128			/* maximum text bitmap width*/
#define	MIN_DPCOORD	((DPCOORD) -32768)	/* minimum coordinate value */
#define	MAX_DPCOORD	((DPCOORD) 32767)	/* maximum coordinate value */


#define	DPMIN(a,b)		((a) < (b) ? (a) : (b))
#define	DPMAX(a,b) 		((a) > (b) ? (a) : (b))
#define DPABS(x)		((x) < 0 ? -(x) : (x))
#define DPSIGN(x)		(((x) > 0) ? 1 : (((x) == 0) ? 0 : -1))
#define DPCLAMP(x,a,b)	((x) > (b) ? (b) : ((x) < (a) ? (a) : (x)))

// DPIMAGEBITS macros
#define DPIMAGE_WORDS(x)	(((x)+15)/16)
#define DPIMAGE_BYTES(x)	(MWIMAGE_WORDS(x)*sizeof(DPIMAGEBITS))
									// size of image in words
#define	DPIMAGE_SIZE(width, height)  	\
	((height) * (((width) + MWIMAGE_BITSPERIMAGE - 1) / MWIMAGE_BITSPERIMAGE))
#define	DPIMAGE_BITSPERIMAGE	(sizeof(DPIMAGEBITS) * 8)
#define	DPIMAGE_BITVALUE(n)	((DPIMAGEBITS) (((DPIMAGEBITS) 1) << (n)))
#define	DPIMAGE_FIRSTBIT	(MWIMAGE_BITVALUE(MWIMAGE_BITSPERIMAGE - 1))
#define	DPIMAGE_NEXTBIT(m)	((DPIMAGEBITS) ((m) >> 1))
#define	DPIMAGE_TESTBIT(m)	((m) & MWIMAGE_FIRSTBIT)	// use with shiftbit
#define	DPIMAGE_SHIFTBIT(m)	((DPIMAGEBITS) ((m) << 1))  // for testbit

// dbl linked list data structure
// LIST must be first decl in struct
typedef struct _mwlist {		
	struct _mwlist *next;		// next item
	struct _mwlist *prev;		// previous item
} MWLIST, *PMWLIST;

// dbl linked list head data structure
typedef struct _mwlisthead {
	struct _mwlist *head;		// first item
	struct _mwlist *tail;		// last item
} MWLISTHEAD, *PMWLISTHEAD;

// Keyboard state modifiers
typedef unsigned int	MWKEYMOD;

// GetScreenInfo structure
typedef struct {
	DPCOORD	rows;		/* number of rows on screen */
	DPCOORD cols;		/* number of columns on screen */
	int 	xdpcm;		/* dots/centimeter in x direction */
	int 	ydpcm;		/* dots/centimeter in y direction */
	int	 	planes;		/* hw # planes*/
	int	 	bpp;		/* hw bpp*/
	int		data_format;/* DPIF_ image data format*/
	int32_t	ncolors;	/* hw number of colors supported*/
	int 	fonts;		/* number of built-in fonts */
	int 	buttons;	/* buttons which are implemented */
	MWKEYMOD modifiers;	/* modifiers which are implemented */
	int	 	pixtype;	/* format of pixel value*/
	int	 	portrait;	/* current portrait mode*/
	DPBOOL	fbdriver;	/* true if running mwin fb screen driver*/
	uint32_t rmask;		/* red mask bits in pixel*/
	uint32_t gmask;		/* green mask bits in pixel*/
	uint32_t bmask;		/* blue mask bits in pixel*/
	uint32_t amask;		/* alpha mask bits in pixel*/
	DPCOORD	xpos;		/* current x mouse position*/
	DPCOORD	ypos;		/* current y mouse position*/

						/* items below are get/set by the window manager and not used internally*/
	int	vs_width;	/* virtual screen width/height*/
	int	vs_height;
	int	ws_width;	/* workspace width/height*/
	int	ws_height;
} DPSCREENINFO, *PDPSCREENINFO;

// client side window framebuffer info
// This is a 'sub-window'
typedef struct {
	unsigned char *	physpixels;	/* address of real framebuffer*/
								/* note winpixels is only correct in non-portrait modes*/
	unsigned char *	winpixels;	/* address of 0,0 this window in fb*/
	int	pixtype;		/* DPPF_ pixel type*/
	int	bpp;			/* bits per pixel*/
	int	bytespp;		/* bytes per pixel*/
	unsigned int pitch;	/* bytes per scan line for window (=fb pitch)*/
	DPCOORD	x, y;		/* absolute window coordinates*/
	int	portrait_mode;	/* current portrait mode*/
	DPCOORD	xres;		/* real framebuffer resolution*/
	DPCOORD	yres;
	DPCOORD	xvirtres;	/* virtual framebuffer resolution*/
	DPCOORD	yvirtres;
} MWWINDOWFBINFO;

/* builtin C-based proportional/fixed font structure*/
typedef struct {
	char *			name;		/* font name*/
	int				maxwidth;	/* max width in pixels*/
	unsigned int	height;		/* height in pixels*/
	int				ascent;		/* ascent (baseline) height*/
	int				firstchar;	/* first character in bitmap*/
	int				size;		/* font size in characters*/
	const DPIMAGEBITS *bits;	/* 16-bit right-padded bitmap data*/
	const uint32_t 	*offset;	/* offsets into bitmap data*/
	const unsigned char *width;	/* character widths or 0 if fixed*/
	int				defaultchar;/* default char (not glyph index)*/
	int32_t			bits_size;	/* # words of DPIMAGEBITS bits*/
} MWCFONT, *PMWCFONT;

/* draw procs associated with fonts.  Strings are [re]packed using defencoding*/
typedef struct _mwscreendevice *PSD;
typedef struct _mwfont *		PMWFONT;
typedef struct _mwfontinfo *	PMWFONTINFO;

typedef struct {
	int		capabilities;		/* flags for font subdriver capabilities*/
	DPTEXTFLAGS	encoding;	/* routines expect this encoding*/
	DPBOOL(*Init)(PSD psd);
	PMWFONT(*CreateFont)(const char *name, DPCOORD height, DPCOORD width, int attr);
	DPBOOL(*GetFontInfo)(PMWFONT pfont, PMWFONTINFO pfontinfo);
	void(*GetTextSize)(PMWFONT pfont, const void *text, int cc,
		DPTEXTFLAGS flags, DPCOORD *pwidth, DPCOORD *pheight,
		DPCOORD *pbase);
	void(*GetTextBits)(PMWFONT pfont, int ch, const DPIMAGEBITS **retmap,
		DPCOORD *pwidth, DPCOORD *pheight, DPCOORD *pbase);
	void(*DestroyFont)(PMWFONT pfont);
	void(*DrawText)(PMWFONT pfont, PSD psd, DPCOORD x, DPCOORD y,
		const void *str, int count, DPTEXTFLAGS flags);
	int(*SetFontSize)(PMWFONT pfont, DPCOORD height, DPCOORD width);
	void(*SetFontRotation)(PMWFONT pfont, int tenthdegrees);
	int(*SetFontAttr)(PMWFONT pfont, int setflags, int clrflags);
	PMWFONT(*Duplicate) (PMWFONT psrcfont, DPCOORD height, DPCOORD width);
} MWFONTPROCS, *PMWFONTPROCS;

/* new multi-renderer font struct*/
typedef struct _mwfont {		/* common hdr for all font structures*/
	PMWFONTPROCS	fontprocs;	/* font-specific rendering routines*/
	DPCOORD			fontsize;	/* font height in pixels*/
	DPCOORD			fontwidth;	/* font width in pixels*/
	int				fontrotation; /* font rotation*/
	int				fontattr;	/* font attributes: kerning/antialias*/
								/* font-specific rendering data here*/
} MWFONT;

/* builtin core font struct*/
typedef struct {
	/* common hdr*/
	PMWFONTPROCS	fontprocs;
	DPCOORD			fontsize;	/* font height in pixels*/
	DPCOORD			fontwidth;	/* font width in pixels*/
	int				fontrotation;
	int				fontattr;
	/* core font specific data*/
	char *		name;			/* Microwindows font name*/
	PMWCFONT	cfont;			/* builtin font data*/
} MWCOREFONT, *PMWCOREFONT;

// In-core color palette structure
typedef struct {
	DPUCHAR	r;
	DPUCHAR	g;
	DPUCHAR	b;
	DPUCHAR _padding;
} MWPALENTRY;

// GdConversionBlit parameter structure
typedef struct {
	int			op;				/* MWROP operation requested*/
	int			data_format;	/* DPIF_ image data format*/
	DPCOORD		width, height;	/* width and height for src and dest*/
	DPCOORD		dstx, dsty;		/* dest x, y*/
	DPCOORD		srcx, srcy;		/* source x, y*/
	unsigned int src_pitch;		/* source row length in bytes*/
	DPCOLORVAL	fg_colorval;	/* fg color, DPCOLORVAL 0xAARRGGBB format*/
	DPCOLORVAL	bg_colorval;
	uint32_t	fg_pixelval;	/* fg color, hw pixel format*/
	uint32_t	bg_pixelval;
	DPBOOL		usebg;			/* set =1 to draw background*/
	void *		data;			/* input image data GdConversionBlit*/

								/* these items filled in by GdConversionBlit*/
	void *		data_out;		/* output image from conversion blits subroutines*/
	unsigned int dst_pitch;		/* dest row length in bytes*/

								/* used by GdBlit and GdStretchBlit for GdCheckCursor and fallback blit*/
	PSD			srcpsd;			/* source psd for psd->psd blits*/

								/* used by frameblits only*/
	DPCOORD		src_xvirtres;	/* srcpsd->x/y virtres, used in frameblit for src coord rotation*/
	DPCOORD		src_yvirtres;

	// used in stretch blits only
	int			src_x_step;		/* normal steps in source image*/
	int			src_y_step;
	int			src_x_step_one;	/* 1-unit steps in source image*/
	int			src_y_step_one;
	int			err_x_step;		/* 1-unit error steps in source image*/
	int			err_y_step;
	int			err_y;			/* source coordinate error tracking*/
	int			err_x;
	int			x_denominator;	/* denominator fraction*/
	int			y_denominator;

	// used in palette conversions only
	MWPALENTRY *palette;		// palette for image
	uint32_t	transcolor;		// transparent color in image

								//	PSD			alphachan;		/* alpha chan for DPROP_BLENDCHANNEL*/
} DPBLITPARMS, *PDPBLITPARMS;

/**
* Structure returned by GetFontInfo.
*
* All sizes are in pixels.
*
* Some of the sizes are limits for "most characters".  With built-in bitmap
* fonts, "most characters" means "all characters".  Otherwise, the
* definition of "most characters" depends on the person who designed the
* font.  Typically it is the alphanumeric characters, and it may or may not
* include accented characters.
*/
typedef struct _mwfontinfo {
	/**
	* Maximum advance width of any character.
	*/
	int maxwidth;

	/**
	* Height of "most characters" in the font. This does not include any
	* leading (blank space between lines of text).
	* Always equal to (baseline+descent).
	*/
	int height;

	/**
	* The ascent (height above the baseline) of "most characters" in
	* the font.
	*
	* Note: This member variable should be called "ascent", to be
	* consistent with FreeType 2, and also to be internally consistent
	* with the "descent" member.  It has not been renamed because that
	* would break backwards compatibility.  FIXME
	*/
	int baseline;

	/**
	* The descent (height below the baseline) of "most characters" in
	* the font.
	*
	* Should be a POSITIVE number.
	*/
	int descent;

	/**
	* Maximum height of any character above the baseline.
	*/
	int maxascent;

	/**
	* Maximum height of any character below the baseline.
	*
	* Should be a POSITIVE number.
	*/
	int maxdescent;

	/**
	* The distance between the baselines of two consecutive lines of text.
	* This is usually height plus some font-specific "leading" value.
	*/
	int linespacing;

	/**
	* First character in the font.
	*/
	int firstchar;

	/**
	* Last character in the font.
	*/
	int lastchar;

	/**
	* True (nonzero) if font is fixed width.  In that case, maxwidth
	* gives the width for every character in the font.
	*/
	DPBOOL fixed;

	/**
	* Table of character advance widths for characters 0-255.
	* Note that fonts can contain characters with codes >255 - in that
	* case this table contains the advance widths for some but not all
	* characters.  Also note that if the font contains kerning
	* information, the advance width of the string "AV" may differ from
	* the sum of the advance widths for the characters 'A' and 'V'.
	*/
	DPUCHAR widths[256];
} MWFONTINFO;


/* GetFontList structure */
typedef struct {
	char *ttname;		/* TrueType name, eg "Times New Roman Bold" */
	char *mwname;		/* microwin name, eg "timesb" */
} MWFONTLIST, *PMWFONTLIST;

/* logical font descriptor*/

/* font classes - used to specify a particular renderer*/
#define MWLF_CLASS_ANY		0	/* any font*/
#define MWLF_CLASS_BUILTIN	1	/* builtin fonts*/
#define MWLF_CLASS_FNT		2	/* FNT native fonts*/
#define MWLF_CLASS_PCF		3	/* X11 PCF/PCF.GZ fonts*/
#define MWLF_CLASS_FREETYPE	4	/* FreeType 1 or 2 fonts in TT format*/
#define MWLF_CLASS_T1LIB	5	/* T1LIB outlined Adobe Type 1 fonts*/
#define MWLF_CLASS_MGL		6	/* MGL (EUCJP) fonts*/
#define MWLF_CLASS_HZK		7	/* chinese HZK fonts*/

#define MWLF_FACESIZE		64	/* max facename size*/

/* font type selection - lfOutPrecision*/
#define MWLF_TYPE_DEFAULT	0	/* any font*/
#define MWLF_TYPE_SCALED	4	/* outlined font (tt or adobe)*/
#define MWLF_TYPE_RASTER	5	/* raster only*/
#define MWLF_TYPE_TRUETYPE	7	/* truetype only*/
#define MWLF_TYPE_ADOBE		10	/* adobe type 1 only*/

/* font weights - lfWeight*/
#define MWLF_WEIGHT_DEFAULT	0	/* any weight*/
#define MWLF_WEIGHT_THIN	100	/* thin*/
#define MWLF_WEIGHT_EXTRALIGHT	200
#define MWLF_WEIGHT_LIGHT	300	/* light */
#define MWLF_WEIGHT_NORMAL	400	/* regular*/
#define MWLF_WEIGHT_REGULAR	400
#define MWLF_WEIGHT_MEDIUM	500	/* medium */
#define MWLF_WEIGHT_DEMIBOLD	600
#define MWLF_WEIGHT_BOLD	700	/* bold*/
#define MWLF_WEIGTH_EXTRABOLD	800
#define MWLF_WEIGHT_BLACK	900	/* black */

/* font charset - lfCharSet*/
#define MWLF_CHARSET_ANSI	0	/* win32 ansi*/
#define MWLF_CHARSET_DEFAULT	1	/* any charset*/
#define MWLF_CHARSET_UNICODE	254	/* unicode*/
#define MWLF_CHARSET_OEM	255	/* local hw*/

/* font pitch - lfPitch */
#define MWLF_PITCH_DEFAULT		0	/* any pitch */
#define MWLF_PITCH_ULTRACONDENSED	10
#define MWLF_PITCH_EXTRACONDENSED	20
#define MWLF_PITCH_CONDENSED		30
#define MWLF_PITCH_SEMICONDENSED	40
#define MWLF_PITCH_NORMAL		50
#define MWLF_PITCH_SEMIEXPANDED		60
#define MWLF_PITCH_EXPANDED		70
#define MWLF_PITCH_EXTRAEXPANDED	80
#define MWLF_PITCH_ULTRAEXPANDED	90

/* flags for the GdAddFont function */
#define MWLF_FLAGS_ALIAS	1

/* windows-compatible MWLOGFONT structure*/
typedef struct {
	int32_t	lfHeight;		/* desired height in pixels*/
	int32_t	lfWidth;		/* desired width in pixels or 0*/
	int32_t	lfEscapement;	/* rotation in tenths of degree*/
	int32_t	lfOrientation;	/* not used*/
	int32_t	lfWeight;		/* font weight*/
	DPUCHAR	lfItalic;		/* =1 for italic */
	DPUCHAR	lfUnderline;	/* =1 for underline */
	DPUCHAR	lfStrikeOut;	/* not used*/
	DPUCHAR	lfCharSet;		/* font character set*/
	DPUCHAR	lfOutPrecision;	/* font type selection*/
	DPUCHAR	lfClipPrecision;/* not used*/
	DPUCHAR	lfQuality;		/* not used*/
	DPUCHAR lfPitchAndFamily;/* not used*/
							 /* end of windows-compatibility*/

	DPUCHAR lfClass;		/* font class (renderer) */

							/* Following only used by (the legacy) FONTMAPPER when enabled.
							* They are only kept around to stay source and binary
							* compatible to previous microwindows releases.
							*/
	DPUCHAR	lfPitch;		/* font pitch */
	DPUCHAR	lfRoman;		/* =1 for Roman letters (upright) */
	DPUCHAR	lfSerif;		/* =1 for Serifed font */
	DPUCHAR	lfSansSerif;	/* =1 for Sans-serif font */
	DPUCHAR	lfModern;		/* =1 for Modern font */
	DPUCHAR	lfMonospace;	/* =1 for Monospaced font */
	DPUCHAR	lfProportional;	/* =1 for Proportional font */
	DPUCHAR	lfOblique;		/* =1 for Oblique (kind of Italic) */
	DPUCHAR	lfSmallCaps;	/* =1 for small caps */
							/* End of fontmapper-only variables */

							/* render-dependent full path or facename here*/
	char	lfFaceName[MWLF_FACESIZE];/* font name, may be aliased*/

} MWLOGFONT, *PMWLOGFONT;

/*
* Macros to initialize the MWLOGFONT structure to the most common defaults
* needed by application programs and the nano-X server program.
*/

#define MWLF_Clear(lf)					\
	do {						\
		(lf)->lfHeight = 0;			\
		(lf)->lfWidth = 0;			\
		(lf)->lfEscapement = 0;			\
		(lf)->lfOrientation = 0;		\
		(lf)->lfWeight = MWLF_WEIGHT_REGULAR;	\
		(lf)->lfPitch = 0;			\
		(lf)->lfClass = MWLF_CLASS_ANY;		\
		(lf)->lfItalic = 0;			\
		(lf)->lfOblique = 0;			\
		(lf)->lfRoman = 0;			\
		(lf)->lfSerif = 0;			\
		(lf)->lfSansSerif = 0;			\
		(lf)->lfModern = 0;			\
		(lf)->lfMonospace = 0;			\
		(lf)->lfProportional = 0;		\
		(lf)->lfSmallCaps = 0;			\
		(lf)->lfUnderline = 0;			\
		(lf)->lfStrikeOut = 0;			\
		(lf)->lfCharSet = 0;			\
		(lf)->lfOutPrecision = 0;		\
		(lf)->lfClipPrecision = 0;		\
		(lf)->lfQuality = 0;			\
		(lf)->lfPitchAndFamily = 0;		\
		(lf)->lfFaceName[0] = '\0';		\
	} while (0)

#define MWLF_SetBold(lf)				\
	do {						\
		(lf)->lfWeight = MWLF_WEIGHT_BOLD;	\
	} while (0)

#define MWLF_SetRegular(lf)				\
	do {						\
		(lf)->lfWeight = MWLF_WEIGHT_REGULAR;	\
	} while (0)

#define MWLF_SetItalics(lf)				\
	do {						\
		(lf)->lfItalic = 1;			\
		(lf)->lfOblique = 0;			\
		(lf)->lfRoman = 0;			\
	} while (0)

#define MWLF_SetRoman(lf)				\
	do {						\
		(lf)->lfItalic = 0;			\
		(lf)->lfOblique = 0;			\
		(lf)->lfRoman = 1;			\
	} while (0)

/*
* Rectangle and point structures.
* These structures are "inherited" in wingdi.h for
* the Win32 RECT and POINT structures, so they must match
* Microsoft's definition.
*/

/* MWPOINT used in GdPoly, GdFillPoly*/
typedef struct {
	DPCOORD x;
	DPCOORD y;
} DPPOINT;

/* MWRECT used in region routines*/
typedef struct {
	DPCOORD	left;
	DPCOORD	top;
	DPCOORD	right;
	DPCOORD	bottom;
} DPRECT;

/* static clip rectangle: drawing allowed if point within rectangle*/
typedef struct {
	DPCOORD 	x;		/* x coordinate of top left corner */
	DPCOORD 	y;		/* y coordinate of top left corner */
	DPCOORD 	width;		/* width of rectangle */
	DPCOORD 	height;		/* height of rectangle */
} DPCLIPRECT;

/* dynamically allocated multi-rectangle clipping region*/
typedef struct {
	int	size;		/* malloc'd # of rectangles*/
	int	numRects;	/* # rectangles in use*/
	int	type; 		/* region type*/
	DPRECT *rects;		/* rectangle array*/
	DPRECT	extents;	/* bounding box of region*/
} DPCLIPREGION;

typedef struct {
	DPCOORD	width;
	DPCOORD	height;
	PSD	psd;
} DPTILE;

/* region types */
#define MWREGION_ERROR		0
#define MWREGION_NULL		1
#define MWREGION_SIMPLE		2
#define MWREGION_COMPLEX	3

/* GdRectInRegion return codes*/
#define MWRECT_OUT	0	/* rectangle not in region*/
#define MWRECT_ALLIN	1	/* rectangle all in region*/
#define MWRECT_PARTIN	2	/* rectangle partly in region*/

/* GdAllocPolyRegion types*/
#define MWPOLY_EVENODD		1
#define MWPOLY_WINDING		2

typedef struct {
	DPCOORD		width;
	DPCOORD		height;
	DPIMAGEBITS *	bitmap;
} MWSTIPPLE;

/* image structure - if changed, convbmp.c and client.c::GrDrawImageBits needs updating*/
typedef struct {
	/* shared header with SCREENDEVICE*/
	int		flags;		/* PSF_IMAGEHDR*/
	int		width;		/* image width in pixels*/
	int		height;		/* image height in pixels*/
	int		planes;		/* # image planes*/
	int		bpp;		/* bits per pixel*/
	int		data_format;/* DPIF_ image data format*/
	unsigned int pitch;	/* bytes per line*/
	DPUCHAR *imagebits;	/* image bits (dword padded)*/
	int		palsize;	/* palette size*/
	MWPALENTRY *palette;/* palette*/
	uint32_t transcolor;/* transparent color or MWNOCOLOR if none*/
						/* end of shared header*/
} MWIMAGEHDR, *PMWIMAGEHDR;

/* image information structure - returned by GdGetImageInfo*/
typedef struct {
	int		id;			/* image id*/
	int 	width;		/* image width in pixels*/
	int 	height;		/* image height in pixels*/
	int		planes;		/* # image planes*/
	int		bpp;		/* bits per pixel (1, 4 or 8)*/
	int		data_format;/* MWIF image data format*/
	unsigned int pitch;	/* bytes per line*/
	uint32_t transcolor;/* transparent color or MWNOCOLOR if none*/
	int		palsize;	/* palette size*/
	MWPALENTRY 	palette[256];	/* palette*/
} MWIMAGEINFO, *PMWIMAGEINFO;

#define	MWMAX_CURSOR_SIZE	32		/* maximum cursor x and y size*/
#define	MWMAX_CURSOR_BUFLEN	MWIMAGE_SIZE(MWMAX_CURSOR_SIZE,MWMAX_CURSOR_SIZE)

/* In-core software cursor structure*/
typedef struct {
	int			width;			/* cursor width in pixels*/
	int			height;			/* cursor height in pixels*/
	DPCOORD		hotx;			/* relative x pos of hot spot*/
	DPCOORD		hoty;			/* relative y pos of hot spot*/
	DPCOLORVAL	fgcolor;		/* foreground color*/
	DPCOLORVAL	bgcolor;		/* background color*/
	DPIMAGEBITS	image[MWMAX_CURSOR_SIZE * 2];/* cursor image bits*/
	DPIMAGEBITS	mask[MWMAX_CURSOR_SIZE * 2];/* cursor mask bits*/
} DPCURSOR, *PDPCURSOR;

/** touchscreen device transform coefficients for GdSetTransform*/
typedef struct {
	int	a, b, c;	/* xpos = (a*jitx + b*jity + c)/denom */
	int	d, e, f;	/* ypos = (d*jitx + e*jity + f)/denom */
	int	s;			/* denom*/
} MWTRANSFORM;

/* outline and filled arc and pie types*/
#define MWARC		0x0001	/* arc*/
#define MWOUTLINE	0x0002
#define MWARCOUTLINE	0x0003	/* arc + outline*/
#define MWPIE		0x0004	/* pie (filled)*/
#define MWELLIPSE	0x0008	/* ellipse outline*/
#define MWELLIPSEFILL	0x0010	/* ellipse filled*/

/* create DPCOLORVAL (0xAABBGGRR format)*/
#define MWARGB(a,r,g,b)	((DPCOLORVAL)(((unsigned char)(r)|\
				(((uint32_t)(unsigned char)(g))<<8))|\
				(((uint32_t)(unsigned char)(b))<<16)|\
				(((uint32_t)(unsigned char)(a))<<24)))
#define MWRGB(r,g,b)	MWARGB(255,(r),(g),(b))		/* argb 255 alpha*/
#define MW0RGB(r,g,b)	MWARGB(0,(r),(g),(b))		/* rgb 0 alpha*/

/* no color, used for transparency, should not be 0, -1 or any MWRGB color*/
#define MWNOCOLOR	0x01000000L			/* MWRGBA(1, 0, 0, 0)*/


// Color and pixel handling
#include "dp_pixmanip.h"

/* Mouse button bits*/
#define MWBUTTON_L	04
#define MWBUTTON_M	02
#define MWBUTTON_R	01

/* Keyboard values*/
typedef unsigned short	MWKEY;
typedef unsigned short	MWSCANCODE;

#include "dp_keymouse.h"

#endif /* _MWTYPES_H*/
