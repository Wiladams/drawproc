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


#include "dproc_types.h"


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
#define DPFONT_SYSTEM_VAR	"System"	/* winFreeSansSerif 11x13 (ansi)*/
#define DPFONT_SYSTEM_FIXED	"SystemFixed"	/* X6x13 (should be ansi)*/
#define DPFONT_GUI_VAR		"System"	/* deprecated (was "Helvetica")*/
#define DPFONT_OEM_FIXED	"SystemFixed"	/* deprecated (was "Terminal")*/

/* Text/GetTextSize encoding flags*/
#define DPTF_ASCII	0x00000000L	/* 8 bit packing, ascii*/
#define DPTF_UTF8	0x00000001L	/* 8 bit packing, utf8*/
#define DPTF_UC16	0x00000002L	/* 16 bit packing, unicode 16*/
#define DPTF_UC32	0x00000004L	/* 32 bit packing, unicode 32*/
#define DPTF_XCHAR2B	0x00000008L	/* 16 bit packing, X11 big endian PCF*/
#define DPTF_PACKMASK	0x0000000FL	/* packing bits mask*/

/* asian double-byte encodings*/
#define DPTF_DBCS_BIG5	0x00000100L	/* chinese big5*/
#define DPTF_DBCS_EUCCN	0x00000200L	/* chinese EUCCN (gb2312+0x80)*/
#define DPTF_DBCS_EUCKR	0x00000300L	/* korean EUCKR (ksc5601+0x80)*/
#define DPTF_DBCS_EUCJP	0x00000400L	/* japanese EUCJP*/
#define DPTF_DBCS_JIS	0x00000500L	/* japanese JISX0213*/
#define DPTF_DBCSMASK	0x00000700L	/* DBCS encodings bitmask*/

/* Text alignment flags*/
#define DPTF_TOP	0x01000000L	/* align on top*/
#define DPTF_BASELINE	0x02000000L	/* align on baseline*/
#define DPTF_BOTTOM	0x04000000L	/* align on bottom*/

/* SetFontAttr and capabilities flags (not used with DPTF_ above)*/
#define DPTF_KERNING	0x0001		/* font kerning*/
#define DPTF_ANTIALIAS	0x0002		/* antialiased output*/
#define DPTF_UNDERLINE	0x0004		/* draw underline*/
#define DPTF_BOLD		0x0008		/* draw bold glyph (not present on all renderers)*/

#define DPTF_CMAP_DEFAULT 0x0010	/* use default (unicode) charset in truetype font (not required)*/
#define DPTF_CMAP_0		  0x0020	/* use charmap 0 in truetype font*/
#define DPTF_CMAP_1       0x0040	/* use charmap 1 in truetype font*/

#define DPTF_FREETYPE	0x1000		/* FIXME: remove*/
#define DPTF_SCALEHEIGHT 0x2000		/* font can scale height seperately*/
#define DPTF_SCALEWIDTH	0x4000		/* font can scale width seperately*/

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




/* logical font descriptor*/

/* font classes - used to specify a particular renderer*/
#define DPLF_CLASS_ANY		0	/* any font*/
#define DPLF_CLASS_BUILTIN	1	/* builtin fonts*/
#define DPLF_CLASS_FNT		2	/* FNT native fonts*/
#define DPLF_CLASS_PCF		3	/* X11 PCF/PCF.GZ fonts*/
#define DPLF_CLASS_FREETYPE	4	/* FreeType 1 or 2 fonts in TT format*/
#define DPLF_CLASS_T1LIB	5	/* T1LIB outlined Adobe Type 1 fonts*/
#define DPLF_CLASS_MGL		6	/* MGL (EUCJP) fonts*/
#define DPLF_CLASS_HZK		7	/* chinese HZK fonts*/


/* font type selection - lfOutPrecision*/
#define DPLF_TYPE_DEFAULT	0	/* any font*/
#define DPLF_TYPE_SCALED	4	/* outlined font (tt or adobe)*/
#define DPLF_TYPE_RASTER	5	/* raster only*/
#define DPLF_TYPE_TRUETYPE	7	/* truetype only*/
#define DPLF_TYPE_ADOBE		10	/* adobe type 1 only*/

/* font weights - lfWeight*/
#define DPLF_WEIGHT_DEFAULT	0	/* any weight*/
#define DPLF_WEIGHT_THIN	100	/* thin*/
#define DPLF_WEIGHT_EXTRALIGHT	200
#define DPLF_WEIGHT_LIGHT	300	/* light */
#define DPLF_WEIGHT_NORMAL	400	/* regular*/
#define DPLF_WEIGHT_REGULAR	400
#define DPLF_WEIGHT_MEDIUM	500	/* medium */
#define DPLF_WEIGHT_DEMIBOLD	600
#define DPLF_WEIGHT_BOLD	700	/* bold*/
#define DPLF_WEIGTH_EXTRABOLD	800
#define DPLF_WEIGHT_BLACK	900	/* black */

/* font charset - lfCharSet*/
#define DPLF_CHARSET_ANSI	0	/* win32 ansi*/
#define DPLF_CHARSET_DEFAULT	1	/* any charset*/
#define DPLF_CHARSET_UNICODE	254	/* unicode*/
#define DPLF_CHARSET_OEM	255	/* local hw*/

/* font pitch - lfPitch */
#define DPLF_PITCH_DEFAULT		0	/* any pitch */
#define DPLF_PITCH_ULTRACONDENSED	10
#define DPLF_PITCH_EXTRACONDENSED	20
#define DPLF_PITCH_CONDENSED		30
#define DPLF_PITCH_SEMICONDENSED	40
#define DPLF_PITCH_NORMAL		50
#define DPLF_PITCH_SEMIEXPANDED		60
#define DPLF_PITCH_EXPANDED		70
#define DPLF_PITCH_EXTRAEXPANDED	80
#define DPLF_PITCH_ULTRAEXPANDED	90

/* flags for the GdAddFont function */
#define DPLF_FLAGS_ALIAS	1



/*
* Macros to initialize the MWLOGFONT structure to the most common defaults
* needed by application programs and the nano-X server program.
*/

#define DPLF_Clear(lf)					\
	do {						\
		(lf)->lfHeight = 0;			\
		(lf)->lfWidth = 0;			\
		(lf)->lfEscapement = 0;			\
		(lf)->lfOrientation = 0;		\
		(lf)->lfWeight = DPLF_WEIGHT_REGULAR;	\
		(lf)->lfPitch = 0;			\
		(lf)->lfClass = DPLF_CLASS_ANY;		\
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

#define DPLF_SetBold(lf)				\
	do {						\
		(lf)->lfWeight = DPLF_WEIGHT_BOLD;	\
	} while (0)

#define DPLF_SetRegular(lf)				\
	do {						\
		(lf)->lfWeight = DPLF_WEIGHT_REGULAR;	\
	} while (0)

#define DPLF_SetItalics(lf)				\
	do {						\
		(lf)->lfItalic = 1;			\
		(lf)->lfOblique = 0;			\
		(lf)->lfRoman = 0;			\
	} while (0)

#define DPLF_SetRoman(lf)				\
	do {						\
		(lf)->lfItalic = 0;			\
		(lf)->lfOblique = 0;			\
		(lf)->lfRoman = 1;			\
	} while (0)



/* region types */
#define DPREGION_ERROR		0
#define DPREGION_NULL		1
#define DPREGION_SIMPLE		2
#define DPREGION_COMPLEX	3

/* GdRectInRegion return codes*/
#define DPRECT_OUT	0	/* rectangle not in region*/
#define DPRECT_ALLIN	1	/* rectangle all in region*/
#define DPRECT_PARTIN	2	/* rectangle partly in region*/

/* GdAllocPolyRegion types*/
#define DPPOLY_EVENODD		1
#define DPPOLY_WINDING		2





/* outline and filled arc and pie types*/
#define DPARC		0x0001	/* arc*/
#define DPOUTLINE	0x0002
#define DPARCOUTLINE	0x0003	/* arc + outline*/
#define DPPIE		0x0004	/* pie (filled)*/
#define DPELLIPSE	0x0008	/* ellipse outline*/
#define DPELLIPSEFILL	0x0010	/* ellipse filled*/

/* create DPCOLORVAL (0xAABBGGRR format)*/
#define DPARGB(a,r,g,b)	((DPCOLORVAL)(((unsigned char)(r)|\
				(((uint32_t)(unsigned char)(g))<<8))|\
				(((uint32_t)(unsigned char)(b))<<16)|\
				(((uint32_t)(unsigned char)(a))<<24)))
#define DPRGB(r,g,b)	DPARGB(255,(r),(g),(b))		/* argb 255 alpha*/
#define DP0RGB(r,g,b)	DPARGB(0,(r),(g),(b))		/* rgb 0 alpha*/

/* no color, used for transparency, should not be 0, -1 or any MWRGB color*/
#define DPNOCOLOR	0x01000000L			/* MWRGBA(1, 0, 0, 0)*/


// Color and pixel handling
#include "dp_pixmanip.h"

/* Mouse button bits*/
#define DPBUTTON_L	04
#define DPBUTTON_M	02
#define DPBUTTON_R	01


#include "dp_keymouse.h"

#endif /* _MWTYPES_H*/
