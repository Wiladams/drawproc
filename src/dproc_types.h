#pragma once

#include "dproc_config.h"

#include <stdint.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif


typedef float	float32;
typedef double	float64;
typedef float32 real;
typedef float coord;

typedef real real2[2];
typedef real real3[3];
typedef real real4[4];

/*
* Type definitions
*/
typedef int				DPCOORD;	/* device coordinates*/
									//typedef int				DPBOOL;		/* boolean value*/
typedef unsigned char	DPUCHAR;	/* unsigned char*/
typedef uint32_t		DPCOLORVAL;	/* device-independent color value (0xAABBGGRR)*/
typedef uint32_t		DPPIXELVAL;	/* pixel value parameter type, not for packing*/
typedef unsigned short	DPIMAGEBITS;/* bitmap image unit size*/
typedef uint32_t		DPTIMEOUT;	/* timeout value */
typedef uint32_t		DPTEXTFLAGS;/* DPTF_ text flag*/

/* Keyboard values*/
// Keyboard state modifiers
typedef unsigned int	MWKEYMOD;
typedef unsigned short	MWKEY;
typedef unsigned short	MWSCANCODE;



/* Interface to Keyboard Device Driver*/
typedef struct _kbddevice {
	int(*Open)(struct _kbddevice *pkd);
	void(*Close)(void);
	void(*GetModifierInfo)(MWKEYMOD *modifiers, MWKEYMOD *curmodifiers);
	int(*Read)(MWKEY *buf, MWKEYMOD *modifiers, MWSCANCODE *scancode);
	int(*Poll)(void);		/* not required if have select()*/
} KBDDEVICE;




typedef enum {
	COLOR_MODE_RGB,
	COLOR_MODE_HSB
} COLORMODE;

typedef enum {
	CORNER,
	CORNERS,
	RADIUS,
	CENTER
} RECTMODE;

typedef enum {
	TX_CENTER = 0x00,
	TX_LEFT = 0x01,
	TX_RIGHT = 0x04,
	TX_TOP = 0x10,
	TX_BOTTOM = 0x40

} ALIGNMENT;

typedef enum {
	GR_POINTS,
	GR_LINES,
	GR_LINE_STRIP,
	GR_LINE_LOOP,
	GR_POLYGON,
	GR_QUADS,
	GR_QUAD_STRIP,
	GR_TRIANGLES,
	GR_TRIANGLE_STRIP,
	GR_TRIANGLE_FAN
} GEOMETRY;

typedef enum {
	STROKE,
	CLOSE
} KindOfClose;


/* draw procs associated with fonts.  Strings are [re]packed using defencoding*/
typedef struct _mwscreendevice *PSD;
typedef struct _mwfont *		PDPFONT;
typedef struct _mwfontinfo *	PDPFONTINFO;


typedef struct {
	DPCOORD		width;
	DPCOORD		height;
	DPIMAGEBITS *	bitmap;
} DPSTIPPLE;


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
typedef struct  {
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


#define DPLF_FACESIZE 64	// max facename size




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
	char	lfFaceName[DPLF_FACESIZE];/* font name, may be aliased*/

} DPLOGFONT, *PDPLOGFONT;

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
	bool fixed;

	/**
	* Table of character advance widths for characters 0-255.
	* Note that fonts can contain characters with codes >255 - in that
	* case this table contains the advance widths for some but not all
	* characters.  Also note that if the font contains kerning
	* information, the advance width of the string "AV" may differ from
	* the sum of the advance widths for the characters 'A' and 'V'.
	*/
	DPUCHAR widths[256];
} DPFONTINFO;


/* GetFontList structure */
typedef struct {
	char *ttname;		/* TrueType name, eg "Times New Roman Bold" */
	char *mwname;		/* microwin name, eg "timesb" */
} DPFONTLIST, *PDPFONTLIST;


typedef struct {
	int		capabilities;		/* flags for font subdriver capabilities*/
	DPTEXTFLAGS	encoding;	/* routines expect this encoding*/
	bool(*Init)(PSD psd);
	PDPFONT(*CreateFont)(const char *name, DPCOORD height, DPCOORD width, int attr);
	bool(*GetFontInfo)(PDPFONT pfont, PDPFONTINFO pfontinfo);
	void(*GetTextSize)(PDPFONT pfont, const void *text, int cc,
		DPTEXTFLAGS flags, DPCOORD *pwidth, DPCOORD *pheight,
		DPCOORD *pbase);
	void(*GetTextBits)(PDPFONT pfont, int ch, const DPIMAGEBITS **retmap,
		DPCOORD *pwidth, DPCOORD *pheight, DPCOORD *pbase);
	void(*DestroyFont)(PDPFONT pfont);
	void(*DrawText)(PDPFONT pfont, PSD psd, DPCOORD x, DPCOORD y,
		const void *str, int count, DPTEXTFLAGS flags);
	int(*SetFontSize)(PDPFONT pfont, DPCOORD height, DPCOORD width);
	void(*SetFontRotation)(PDPFONT pfont, int tenthdegrees);
	int(*SetFontAttr)(PDPFONT pfont, int setflags, int clrflags);
	PDPFONT(*Duplicate) (PDPFONT psrcfont, DPCOORD height, DPCOORD width);
} DPFONTPROCS, *PDPFONTPROCS;

/* new multi-renderer font struct*/
typedef struct _mwfont {		/* common hdr for all font structures*/
	PDPFONTPROCS	fontprocs;	/* font-specific rendering routines*/
	DPCOORD			fontsize;	/* font height in pixels*/
	DPCOORD			fontwidth;	/* font width in pixels*/
	int				fontrotation; /* font rotation*/
	int				fontattr;	/* font attributes: kerning/antialias*/
								/* font-specific rendering data here*/
} DPFONT;


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
} DPCFONT, *PDPCFONT; 

/* builtin core font struct*/
typedef struct {
	/* common hdr*/
	PDPFONTPROCS	fontprocs;
	DPCOORD			fontsize;	/* font height in pixels*/
	DPCOORD			fontwidth;	/* font width in pixels*/
	int				fontrotation;
	int				fontattr;
	/* core font specific data*/
	char *		name;			/* Microwindows font name*/
	PDPCFONT	cfont;			/* builtin font data*/
} DPCOREFONT, *PDPCOREFONT;





// GetScreenInfo structure
typedef struct {
	DPCOORD	rows;		/* number of rows on screen */
	DPCOORD cols;		/* number of columns on screen */
	int 	xdpcm;		/* dots/centimeter in x direction */
	int 	ydpcm;		/* dots/centimeter in y direction */
	int	 	planes;		/* hw # planes*/
	int	 	bpp;		/* hw bpp*/
	int		data_format;/* DPIF_ image data format*/
	size_t	ncolors;	/* hw number of colors supported*/
	size_t 	fonts;		/* number of built-in fonts */
	int 	buttons;	/* buttons which are implemented */
	MWKEYMOD modifiers;	/* modifiers which are implemented */
	int	 	pixtype;	/* format of pixel value*/
	int	 	portrait;	/* current portrait mode*/
	bool	fbdriver;	/* true if running mwin fb screen driver*/
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
	size_t pitch;	/* bytes per scan line for window (=fb pitch)*/
	DPCOORD	x, y;		/* absolute window coordinates*/
	int	portrait_mode;	/* current portrait mode*/
	DPCOORD	xres;		/* real framebuffer resolution*/
	DPCOORD	yres;
	DPCOORD	xvirtres;	/* virtual framebuffer resolution*/
	DPCOORD	yvirtres;
} DPWINDOWFBINFO;





// In-core color palette structure
typedef struct {
	DPUCHAR	r;
	DPUCHAR	g;
	DPUCHAR	b;
	DPUCHAR _padding;
} DPPALENTRY;

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
	bool		usebg;			/* set =1 to draw background*/
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
	DPPALENTRY *palette;		// palette for image
	uint32_t	transcolor;		// transparent color in image

								//	PSD			alphachan;		/* alpha chan for DPROP_BLENDCHANNEL*/
} DPBLITPARMS, *PDPBLITPARMS;


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
	DPPALENTRY *palette;/* palette*/
	uint32_t transcolor;/* transparent color or MWNOCOLOR if none*/
						/* end of shared header*/
} DPIMAGEHDR, *PDPIMAGEHDR;

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
	DPPALENTRY 	palette[256];	/* palette*/
} DPIMAGEINFO, *PDPIMAGEINFO;


// DPIMAGEBITS macros
#define DPIMAGE_WORDS(x)	(((x)+15)/16)
#define DPIMAGE_BYTES(x)	(DPIMAGE_WORDS(x)*sizeof(DPIMAGEBITS))
// size of image in words
#define	DPIMAGE_SIZE(width, height)  	\
	((height) * (((width) + DPIMAGE_BITSPERIMAGE - 1) / DPIMAGE_BITSPERIMAGE))
#define	DPIMAGE_BITSPERIMAGE	(sizeof(DPIMAGEBITS) * 8)
#define	DPIMAGE_BITVALUE(n)	((DPIMAGEBITS) (((DPIMAGEBITS) 1) << (n)))
#define	DPIMAGE_FIRSTBIT	(DPIMAGE_BITVALUE(DPIMAGE_BITSPERIMAGE - 1))
#define	DPIMAGE_NEXTBIT(m)	((DPIMAGEBITS) ((m) >> 1))
#define	DPIMAGE_TESTBIT(m)	((m) & DPIMAGE_FIRSTBIT)	// use with shiftbit
#define	DPIMAGE_SHIFTBIT(m)	((DPIMAGEBITS) ((m) << 1))  // for testbit

#define	DPMAX_CURSOR_SIZE	32		/* maximum cursor x and y size*/
#define	DPMAX_CURSOR_BUFLEN	DPIMAGE_SIZE(DPMAX_CURSOR_SIZE,DPMAX_CURSOR_SIZE)

/* In-core software cursor structure*/
typedef struct {
	int			width;			/* cursor width in pixels*/
	int			height;			/* cursor height in pixels*/
	DPCOORD		hotx;			/* relative x pos of hot spot*/
	DPCOORD		hoty;			/* relative y pos of hot spot*/
	DPCOLORVAL	fgcolor;		/* foreground color*/
	DPCOLORVAL	bgcolor;		/* background color*/
	DPIMAGEBITS	image[DPMAX_CURSOR_SIZE * 2];/* cursor image bits*/
	DPIMAGEBITS	mask[DPMAX_CURSOR_SIZE * 2];/* cursor mask bits*/
} DPCURSOR, *PDPCURSOR;

/** touchscreen device transform coefficients for GdSetTransform*/
typedef struct {
	int	a, b, c;	/* xpos = (a*jitx + b*jity + c)/denom */
	int	d, e, f;	/* ypos = (d*jitx + e*jity + f)/denom */
	int	s;			/* denom*/
} DPTRANSFORM;


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
	DPPALENTRY *palette;/* palette*/
	int32_t	transcolor;	/* not used*/
						/* end of shared header*/

	DPCOORD	xres;		/* X screen res (real) */
	DPCOORD	yres;		/* Y screen res (real) */
	unsigned int size;	/* size of memory allocated*/
	int32_t	ncolors;	/* # screen colors*/
	int	pixtype;		/* format of pixel value*/

						/* driver entry points*/
	PDPCOREFONT builtin_fonts;
	PSD(*Open)(PSD psd);
	void(*Close)(PSD psd);
	void(*SetPalette)(PSD psd, int first, int count, DPPALENTRY *pal);
	void(*GetScreenInfo)(PSD psd, PDPSCREENINFO psi);
	PSD(*AllocateMemGC)(PSD psd);
	bool(*MapMemGC)(PSD mempsd, DPCOORD w, DPCOORD h, int planes, int bpp,
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






struct COLOR
{
	COLOR() :value(0) {}
	//COLOR(const COLOR &other) :value(other.value){}
	COLOR(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) :r(r), g(g), b(b), a(a) {}

	union {
		struct {
			uint8_t b, g, r, a;
		};
		uint32_t value;
	};
};



struct Vector2d
{
	Vector2d(coord x, coord y)
		:_x(x), _y(y)
	{};

	coord _x;
	coord _y;
};

typedef struct {
	float x;
	float y;
	float z;
} Pt3;




/*
A pixel buffer is an array of pixels.

The pixelformat determines the layout of the
individual elements.

data		- pointer to the beginning of the data
bitStride	- number of bits between rows
pixelpitch	- number of pixels between rows
*/

typedef struct _pb_rgba {
	uint8_t *		data;			// pointer to the actual data
	unsigned int	pixelpitch;		// how many pixels per row, if alias
	int				bitStride;		// how many bits per row
	int				owndata;		// does this struct own the data pointer
	DPCLIPRECT		frame;			// if alias, what portion of parent
} pb_rgba;

struct PImage
{
	pb_rgba fb;
};


// Font information
typedef uint8_t cover_type;    //----cover_type
enum cover_scale_e
{
	cover_none = 0,                 //----cover_none 
	cover_shift = 8,                 //----cover_shift
	cover_size = 1 << cover_shift,  //----cover_size 
	cover_mask = cover_size - 1,    //----cover_mask 
	cover_full = cover_mask         //----cover_full 
};




typedef struct _font {
	uint8_t	height;			// height in pixels of all characters
	uint8_t baseline;		// baseline offset of character
	uint8_t start_char;		// ordinal of first character in the set
	uint8_t num_chars;		// number of characters in the font

	bool bigendian;			// endianness of current machine
	uint8_t *charbits;		// pointer to where the bits for the chars start

} font_t;

// Rectangle representing the boundary of a glyph
struct glyph_rect
{
	int x1, y1, x2, y2;
	double dx, dy;
};

typedef struct _glyph {
	size_t width;
	size_t byte_width;

	uint8_t *data;
} glyph_t;


// Linear Algebra types
typedef struct _mat2 {
	real m11, m12;
	real m21, m22;
} mat2;

typedef struct _mat3 {
	real m11, m12, m13;
	real m21, m22, m23;
	real m31, m32, m33;
} mat3;


typedef struct _mat4 {
	real m11, m12, m13, m14;
	real m21, m22, m23, m24;
	real m31, m32, m33, m34;
	real m41, m42, m43, m44;
} mat4;

#ifdef _MSC_VER
#pragma warning(pop)
#endif


DPROC_API extern bool containsPoint(DPCLIPRECT &aRect, const int x, const int y);
DPROC_API extern DPRECT intersection(const DPCLIPRECT &a, const DPCLIPRECT &b);
