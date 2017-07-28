#pragma once

//#ifdef MWINCLUDECOLORS
/*
* Alpha blending evolution
*
* Blending r,g,b pixels w/src alpha
* unoptimized two mult one div		 	bg = (a*fg+(255-a)*bg)/255
* optimized one mult one div			bg = (a*(fg-bg))/255 + bg
* optimized /255 replaced with +1/>>8	bg = (((a+1)*(fg-bg))>>8) + bg
* optimized +=							bg +=(((a+1)*(fg-bg))>>8)
* macro +=								bg +=muldiv255(a,fg-bg)
* macro =								bg  =muldiv255(a,fg-bg) + bg
* -or-
* macro = (src/dst reversed)			bg  =muldiv255(255-a,bg-fg) + fg
*
* Updating dst alpha from src alpha
* original routine						d =   (255-d)*a/255 + d
* rearrange							d =   a*(255-d)/255 + d
* replace multiply by fast +1>>8		d = (((a+1)*(255-d)) >> 8) + d
* macro =								d =  muldiv255(a, 255 - d) + d
* macro +=								d += muldiv255(a, 255 - d)
* -or- src/dst reversed (method used in 0.91, not quite correct)
* mathematical correct  routine		d =  (d * (255 - a)/255 + a
* rearrange							d = ((d * (255 - a + 1)) >> 8) + a
* alternate (used in v0.91)			d = ((d * (256 - a)) >> 8) + a
* macro = (to duplicate 0.91 code)		d = muldiv255(255 - a, d) + a
* correct macro =						d = muldiv255(d, 255 - a) + a
*/
/* single byte color macros for 24/32bpp*/
//#define muldiv255(a,b)	(((a)*(b))/255)		/* slow divide, exact*/
#define muldiv255(a,b)		((((a)+1)*(b))>>8)		/* very fast, 92% accurate*/
//#define muldiv255(a,b)	((((a)+((a)>>7))*(b))>>8)	/* fast, 35% accurate*/

/* pixel to pixel blend for 16bpp*/
#define mulscale(a,b,n)		((((a)+1)*(b))>>(n))	/* very fast, always shift for 16bpp*/

/* single byte color macros for 15/16bpp macros - FIXME endian specific*/
/* d = muldiv255(255-a, d-s) + s*/
#define muldiv255_rgb565(d, sr, sg, sb, as) \
						  (((((((d) & 0xF800) - (sr)) * as) >> 8) + (sr)) & 0xF800)\
						| (((((((d) & 0x07E0) - (sg)) * as) >> 8) + (sg)) & 0x07E0)\
						| (((((((d) & 0x001F) - (sb)) * as) >> 8) + (sb)) & 0x001F)
#define muldiv255_rgb555(d, sr, sg, sb, as) \
						  (((((((d) & 0x7C00) - (sr)) * as) >> 8) + (sr)) & 0x7C00)\
						| (((((((d) & 0x03E0) - (sg)) * as) >> 8) + (sg)) & 0x03E0)\
						| (((((((d) & 0x001F) - (sb)) * as) >> 8) + (sb)) & 0x001F)
#define muldiv255_rgb1555(d, sr, sg, sb, as) \
						  (((((((d) & 0x001F) - (sr)) * as) >> 8) + (sr)) & 0x001F)\
						| (((((((d) & 0x03E0) - (sg)) * as) >> 8) + (sg)) & 0x03E0)\
						| (((((((d) & 0x7C00) - (sb)) * as) >> 8) + (sb)) & 0x7C00)


/* palette color definition*/
#define RGBDEF(r,g,b)	{r, g, b}

/* return palette entry as DPCOLORVAL (0xAABBGGRR)*/
#define GETPALENTRY(pal,index) ((DPCOLORVAL)(pal[index].r | (pal[index].g << 8) |\
									    (pal[index].b << 16) | (255 << 24)))

/* extract DPCOLORVAL (0xAABBGGRR) values*/
#define REDVALUE(rgb)	((rgb) & 0xff)
#define GREENVALUE(rgb) (((rgb) >> 8) & 0xff)
#define BLUEVALUE(rgb)	(((rgb) >> 16) & 0xff)
#define ALPHAVALUE(rgb)	(((rgb) >> 24) & 0xff)

/* Mask values for DPIF_ image data formats*/
/* DPIF_BGR233*/
#define RMASK233	0x07
#define GMASK233	0x38
#define BMASK233	0xc0
#define AMASK233	0x00

/* DPIF_RGB332*/
#define RMASK332	0xe0
#define GMASK332	0x1c
#define BMASK332	0x03
#define AMASK332	0x00

/* DPIF_RGB555*/
#define RMASK555	0x7c00
#define GMASK555	0x03e0
#define BMASK555	0x001f
#define AMASK555	0x8000

/* DPIF_RGB565*/
#define RMASK565	0xf800
#define GMASK565	0x07e0
#define BMASK565	0x001f
#define AMASK565	0x0000

/* DPIF_BGR888*/
#define RMASKBGR	0xff0000
#define GMASKBGR	0x00ff00
#define BMASKBGR	0x0000ff
#define AMASKBGR	0x000000

/* DPIF_BGRA8888*/
#define RMASKBGRA	0x00ff0000
#define GMASKBGRA	0x0000ff00
#define BMASKBGRA	0x000000ff
#define AMASKBGRA	0xff000000

/* DPIF_RGBA8888*/
#define RMASKRGBA	0x000000ff
#define GMASKRGBA	0x0000ff00
#define BMASKRGBA	0x00ff0000
#define AMASKRGBA	0xff000000

/* Truecolor color conversion and extraction macros*/
/*
* Conversion from 8-bit RGB components to MWPIXELVAL
*/
/* create 32 bit 8/8/8/8 format pixel (0xAARRGGBB) from RGB triplet*/
#define RGB2PIXEL8888(r,g,b)	\
	(0xFF000000UL | ((r) << 16) | ((g) << 8) | (b))

/* create 32 bit 8/8/8/8 format pixel (0xAABBGGRR) from RGB triplet*/
#define RGB2PIXELABGR(r,g,b)	\
	(0xFF000000UL | ((b) << 16) | ((g) << 8) | (r))

/* create 32 bit 8/8/8/8 format pixel (0xAARRGGBB) from ARGB quad*/
#define ARGB2PIXEL8888(a,r,g,b)	\
	(((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

/* create 32 bit 8/8/8/8 format pixel (0xAABBGGRR) from ARGB quad*/
#define ARGB2PIXELABGR(a,r,g,b)	\
	(((a) << 24) | ((b) << 16) | ((g) << 8) | (r))

/* create 24 bit 8/8/8 format pixel (0x00RRGGBB) from RGB triplet*/
#define RGB2PIXEL888(r,g,b)	\
	(((r) << 16) | ((g) << 8) | (b))

/* create 16 bit 5/6/5 format pixel from RGB triplet */
#define RGB2PIXEL565(r,g,b)	\
	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

/* create 16 bit 5/5/5 format pixel from RGB triplet */
#define RGB2PIXEL555(r,g,b)	\
	((((r) & 0xf8) << 7) | (((g) & 0xf8) << 2) | (((b) & 0xf8) >> 3))

/* create 16 bit b/g/r 5/5/5 format pixel from RGB triplet */
#define RGB2PIXEL1555(r,g,b)	\
	((((b) & 0xf8) << 7) | (((g) & 0xf8) << 2) | (((r) & 0xf8) >> 3) | 0x8000)

/* create 8 bit 3/3/2 format pixel from RGB triplet*/
#define RGB2PIXEL332(r,g,b)	\
	(((r) & 0xe0) | (((g) & 0xe0) >> 3) | (((b) & 0xc0) >> 6))

/* create 8 bit 2/3/3 format pixel from RGB triplet*/
#define RGB2PIXEL233(r,g,b)	\
	((((r) & 0xe0) >> 5) | (((g) & 0xe0) >> 2) | (((b) & 0xc0) >> 0))

/* create single component of 5/6/5format pixel from color byte*/
#define RED2PIXEL565(byte)		(((byte) & 0xf8) << 8)
#define GREEN2PIXEL565(byte)	(((byte) & 0xfc) << 3)
#define BLUE2PIXEL565(byte)		(((byte) & 0xf8) >> 3)

/* create single component of 5/5/5format pixel from color byte*/
#define RED2PIXEL555(byte)		(((byte) & 0xf8) << 7)
#define GREEN2PIXEL555(byte)	(((byte) & 0xf8) << 2)
#define BLUE2PIXEL555(byte)		(((byte) & 0xf8) >> 3)

/* create single component of 1/5/5/5format pixel from color byte*/
#define RED2PIXEL1555(byte)		(((byte) & 0xf8) >> 3)
#define GREEN2PIXEL1555(byte)	(((byte) & 0xf8) << 2)
#define BLUE2PIXEL1555(byte)		(((byte) & 0xf8) << 7)

/* these defines used in convblits, must be available regardless of DPPIXEL_FORMAT, default 565*/
#if DPPIXEL_FORMAT == DPPF_TRUECOLOR555
#define muldiv255_16bpp		muldiv255_rgb555
#define RED2PIXEL(byte)		RED2PIXEL555(byte)
#define GREEN2PIXEL(byte)	GREEN2PIXEL555(byte)
#define BLUE2PIXEL(byte)	BLUE2PIXEL555(byte)
#define REDMASK(pixel)		((pixel) & 0x7c00)
#define GREENMASK(pixel)	((pixel) & 0x03e0)
#define BLUEMASK(pixel)		((pixel) & 0x001f)
#else
#if DPPIXEL_FORMAT == DPPF_TRUECOLOR1555
#define muldiv255_16bpp		muldiv255_rgb1555
#define RED2PIXEL(byte)		RED2PIXEL1555(byte)
#define GREEN2PIXEL(byte)	GREEN2PIXEL1555(byte)
#define BLUE2PIXEL(byte)	BLUE2PIXEL1555(byte)
#define REDMASK(pixel)		((pixel) & 0x001f)
#define GREENMASK(pixel)	((pixel) & 0x03e0)
#define BLUEMASK(pixel)		((pixel) & 0x7c00)
#else
#define muldiv255_16bpp		muldiv255_rgb565
#define RED2PIXEL(byte)		RED2PIXEL565(byte)
#define GREEN2PIXEL(byte)	GREEN2PIXEL565(byte)
#define BLUE2PIXEL(byte)	BLUE2PIXEL565(byte)
#define REDMASK(pixel)		((pixel) & 0xf800)
#define GREENMASK(pixel)	((pixel) & 0x07e0)
#define BLUEMASK(pixel)		((pixel) & 0x001f)
#endif
#endif

/*
* Conversion from DPCOLORVAL to MWPIXELVAL
*/
/* create 32 bit 8/8/8/8 format pixel from ABGR colorval (0xAABBGGRR)*/
/* In this format, alpha is preserved. */
#define COLOR2PIXEL8888(c)	\
	((((c) & 0xff) << 16) | ((c) & 0xff00ff00ul) | (((c) & 0xff0000) >> 16))

/* convert ABGR pixel to ARGB pixel (RGBA image pixel to BGRA image pixel)*/
#define PIXELABGR2PIXEL8888(c)	COLOR2PIXEL8888(c)

/* create 32 bit 8/8/8/8 format pixel from ABGR colorval (0xAABBGGRR)*/
/* In this format, alpha is preserved. */
#define COLOR2PIXELABGR(c)		(c)

/* create 24 bit 8/8/8 format pixel from 0BGR colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL888(c)	\
	((((c) & 0xff) << 16) | ((c) & 0xff00) | (((c) & 0xff0000) >> 16))

/* create 16 bit 5/6/5 format pixel from 0BGR colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL565(c)	\
	((((c) & 0xf8) << 8) | (((c) & 0xfc00) >> 5) | (((c) & 0xf80000) >> 19))

/* create 16 bit 5/5/5 format pixel from 0BGR colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL555(c)	\
	((((c) & 0xf8) << 7) | (((c) & 0xf800) >> 6) | (((c) & 0xf80000) >> 19))

/* create 16 bit b/g/r 5/5/5 format pixel from 0BGR colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL1555(c)	\
	((((c) & 0xf8) >> 3) | (((c) & 0xf800) >> 6) | (((c) & 0xf80000) >> 9) | 0x8000)

/* create 8 bit 3/3/2 format pixel from 0BGR colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL332(c)	\
	(((c) & 0xe0) | (((c) & 0xe000) >> 11) | (((c) & 0xc00000) >> 22))

/* create 8 bit 2/3/3 format pixel from 0BGR colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL233(c)	\
        ((((c) & 0xC00000) >> 16) | (((c) & 0x00E000) >> 10) | (((c) & 0xE0) >> 5))

/*
* Conversion from MWPIXELVAL to red, green or blue components
*/
/* return 8/8/8/8 bit a, r, g or b component of 32 bit pixelval*/
#define PIXEL8888ALPHA(pixelval)	(((pixelval) >> 24) & 0xff)
#define PIXEL8888RED(pixelval)  	(((pixelval) >> 16) & 0xff)
#define PIXEL8888GREEN(pixelval)	(((pixelval) >> 8) & 0xff)
#define PIXEL8888BLUE(pixelval) 	((pixelval) & 0xff)

/* return 8/8/8/8 bit a, r, g or b component of 32 bit pixelval*/
#define PIXELABGRALPHA(pixelval)	(((pixelval) >> 24) & 0xff)
#define PIXELABGRBLUE(pixelval)  	(((pixelval) >> 16) & 0xff)
#define PIXELABGRGREEN(pixelval)	(((pixelval) >> 8) & 0xff)
#define PIXELABGRRED(pixelval)		((pixelval) & 0xff)

/* return 8/8/8 bit r, g or b component of 24 bit pixelval*/
#define PIXEL888RED(pixelval)		(((pixelval) >> 16) & 0xff)
#define PIXEL888GREEN(pixelval)		(((pixelval) >> 8) & 0xff)
#define PIXEL888BLUE(pixelval)		((pixelval) & 0xff)

/* return 5/6/5 bit r, g or b component of 16 bit pixelval*/
#define PIXEL565RED(pixelval)		(((pixelval) >> 11) & 0x1f)
#define PIXEL565GREEN(pixelval)		(((pixelval) >> 5) & 0x3f)
#define PIXEL565BLUE(pixelval)		((pixelval) & 0x1f)

/* return 5/5/5 bit r, g or b component of 16 bit pixelval*/
#define PIXEL555RED(pixelval)		(((pixelval) >> 10) & 0x1f)
#define PIXEL555GREEN(pixelval)		(((pixelval) >> 5) & 0x1f)
#define PIXEL555BLUE(pixelval)		((pixelval) & 0x1f)

/* return b/g/r 5/5/5 bit r, g or b component of 16 bit pixelval*/
#define PIXEL1555BLUE(pixelval)		(((pixelval) >> 10) & 0x1f)
#define PIXEL1555GREEN(pixelval)	(((pixelval) >> 5) & 0x1f)
#define PIXEL1555RED(pixelval)		((pixelval) & 0x1f)

/* return 3/3/2 bit r, g or b component of 8 bit pixelval*/
#define PIXEL332RED(pixelval)		(((pixelval) >> 5) & 0x07)
#define PIXEL332GREEN(pixelval)		(((pixelval) >> 2) & 0x07)
#define PIXEL332BLUE(pixelval)		((pixelval) & 0x03)

/* return 2/3/3 bit b, g, r component of 8 bit pixelval*/
#define PIXEL233RED(pixelval)		((pixelval) & 0x07)
#define PIXEL233GREEN(pixelval)		(((pixelval) >> 3) & 0x07)
#define PIXEL233BLUE(pixelval)		(((pixelval) >> 6) & 0x03)

/*
* Conversion from MWPIXELVAL to normal 8-bit red, green or blue components
*/
/* return 8/8/8/8 bit a, r, g or b component of 32 bit pixelval*/
#define PIXEL8888ALPHA8(pixelval)		(((pixelval) >> 24) & 0xff)
#define PIXEL8888RED8(pixelval)			(((pixelval) >> 16) & 0xff)
#define PIXEL8888GREEN8(pixelval)		(((pixelval) >> 8) & 0xff)
#define PIXEL8888BLUE8(pixelval)		((pixelval) & 0xff)

/* return 8/8/8/8 bit a, r, g or b component of 32 bit pixelval*/
#define PIXELABGRALPHA8(pixelval)		(((pixelval) >> 24) & 0xff)
#define PIXELABGRBLUE8(pixelval)		(((pixelval) >> 16) & 0xff)
#define PIXELABGRGREEN8(pixelval)		(((pixelval) >> 8) & 0xff)
#define PIXELABGRRED8(pixelval)			((pixelval) & 0xff)

/* return 8 bit r, g or b component of 8/8/8 24 bit pixelval*/
#define PIXEL888RED8(pixelval)          (((pixelval) >> 16) & 0xff)
#define PIXEL888GREEN8(pixelval)        (((pixelval) >> 8) & 0xff)
#define PIXEL888BLUE8(pixelval)         ((pixelval) & 0xff)

/* return 8 bit r, g or b component of 5/6/5 16 bit pixelval*/
#define PIXEL565RED8(pixelval)          (((pixelval) >> 8) & 0xf8)
#define PIXEL565GREEN8(pixelval)        (((pixelval) >> 3) & 0xfc)
#define PIXEL565BLUE8(pixelval)         (((pixelval) << 3) & 0xf8)

/* return 8 bit r, g or b component of 5/5/5 16 bit pixelval*/
#define PIXEL555RED8(pixelval)          (((pixelval) >> 7) & 0xf8)
#define PIXEL555GREEN8(pixelval)        (((pixelval) >> 2) & 0xf8)
#define PIXEL555BLUE8(pixelval)         (((pixelval) << 3) & 0xf8)

/* return 8 bit r, g or b component of b/g/r 5/5/5 16 bit pixelval*/
#define PIXEL1555BLUE8(pixelval)		(((pixelval) >> 7) & 0xf8)
#define PIXEL1555GREEN8(pixelval)		(((pixelval) >> 2) & 0xf8)
#define PIXEL1555RED8(pixelval)			(((pixelval) << 3) & 0xf8)

/* return 8 bit r, g or b component of 3/3/2 8 bit pixelval*/
#define PIXEL332RED8(pixelval)          ( (pixelval)       & 0xe0)
#define PIXEL332GREEN8(pixelval)        (((pixelval) << 3) & 0xe0)
#define PIXEL332BLUE8(pixelval)         (((pixelval) << 6) & 0xc0)

/* return 8 bit r, g or b component of 2/3/3 8 bit pixelval*/
#define PIXEL233RED8(pixelval)          (((pixelval) << 5) & 0xe0)
#define PIXEL233GREEN8(pixelval)        (((pixelval) << 2) & 0xe0)
#define PIXEL233BLUE8(pixelval)         ( (pixelval)       & 0xc0)

/*
* Conversion from MWPIXELVAL to *32-bit* red, green or blue components
* (i.e. PIXEL888RED32(x) == (PIXEL888RED8(x) << 24).  These macros optimize
* out the extra shift.)
*/
/* return 32 bit a, r, g or b component of 8/8/8/8 32 bit pixelval*/
#define PIXEL8888ALPHA32(pixelval)        ( ((uint32_t)(pixelval))        & 0xff000000UL)
#define PIXEL8888RED32(pixelval)          ((((uint32_t)(pixelval)) <<  8) & 0xff000000UL)
#define PIXEL8888GREEN32(pixelval)        ((((uint32_t)(pixelval)) << 16) & 0xff000000UL)
#define PIXEL8888BLUE32(pixelval)         ((((uint32_t)(pixelval)) << 24) & 0xff000000UL)

/* return 32 bit a, r, g or b component of 8/8/8/8 32 bit pixelval*/
#define PIXELABGRALPHA32(pixelval)        ( ((uint32_t)(pixelval))        & 0xff000000UL)
#define PIXELABGRBLUE32(pixelval)         ((((uint32_t)(pixelval)) <<  8) & 0xff000000UL)
#define PIXELABGRGREEN32(pixelval)        ((((uint32_t)(pixelval)) << 16) & 0xff000000UL)
#define PIXELABGRRED32(pixelval)          ((((uint32_t)(pixelval)) << 24) & 0xff000000UL)

/* return 32 bit r, g or b component of 8/8/8 24 bit pixelval*/
#define PIXEL888RED32(pixelval)          ((((uint32_t)(pixelval)) <<  8) & 0xff000000UL)
#define PIXEL888GREEN32(pixelval)        ((((uint32_t)(pixelval)) << 16) & 0xff000000UL)
#define PIXEL888BLUE32(pixelval)         ((((uint32_t)(pixelval)) << 24) & 0xff000000UL)

/* return 32 bit r, g or b component of 5/6/5 16 bit pixelval*/
#define PIXEL565RED32(pixelval)          ((((uint32_t)(pixelval)) << 16) & 0xf8000000UL)
#define PIXEL565GREEN32(pixelval)        ((((uint32_t)(pixelval)) << 21) & 0xfc000000UL)
#define PIXEL565BLUE32(pixelval)         ((((uint32_t)(pixelval)) << 27) & 0xf8000000UL)

/* return 32 bit r, g or b component of 5/5/5 16 bit pixelval*/
#define PIXEL555RED32(pixelval)          ((((uint32_t)(pixelval)) << 17) & 0xf8000000UL)
#define PIXEL555GREEN32(pixelval)        ((((uint32_t)(pixelval)) << 22) & 0xf8000000UL)
#define PIXEL555BLUE32(pixelval)         ((((uint32_t)(pixelval)) << 27) & 0xf8000000UL)

/* return 32 bit r, g or b component of 3/3/2 8 bit pixelval*/
#define PIXEL332RED32(pixelval)          ((((uint32_t)(pixelval)) << 24) & 0xe0000000UL)
#define PIXEL332GREEN32(pixelval)        ((((uint32_t)(pixelval)) << 27) & 0xe0000000UL)
#define PIXEL332BLUE32(pixelval)         ((((uint32_t)(pixelval)) << 30) & 0xc0000000UL)

/* return 32 bit r, g or b component of 2/3/3 8 bit pixelval*/
#define PIXEL233RED32(pixelval)          ((((uint32_t)(pixelval)) << 29) & 0xe0000000UL)
#define PIXEL233GREEN32(pixelval)        ((((uint32_t)(pixelval)) << 26) & 0xe0000000UL)
#define PIXEL233BLUE32(pixelval)         ((((uint32_t)(pixelval)) << 24) & 0xc0000000UL)

/*
* Conversion from MWPIXELVAL to DPCOLORVAL
*/
/* create ABGR colorval (0xAABBGGRR) from 8/8/8/8 ARGB (0xAARRGGBB) format pixel*/
#define PIXEL8888TOCOLORVAL(p)	\
	((((p) & 0xff0000ul) >> 16) | ((p) & 0xff00ff00ul) | (((p) & 0xffu) << 16) | 0xff000000ul)

/* create ABGR colorval (0xAABBGGRR) from 8/8/8/8 ABGR (0xAABBGGRR) format pixel*/
#define PIXELABGRTOCOLORVAL(p)	\
	((p) | 0xff000000ul)

/* create ABGR colorval (0xFFBBGGRR) from 8/8/8 RGB (0x00RRGGBB) format pixel*/
#define PIXEL888TOCOLORVAL(p)	\
	(0xff000000ul | (((p) & 0xff0000ul) >> 16) | ((p) & 0xff00u) | (((p) & 0xffu) << 16) | 0xff000000ul)

/* create ABGR colorval (0xFFBBGGRR) from 5/6/5 format pixel*/
#define PIXEL565TOCOLORVAL(p)	\
	(0xff000000ul | (((p) & 0xf800u) >> 8) | (((p) & 0x07e0u) << 5) | (((p) & 0x1ful) << 19) | 0xff000000ul)

/* create ABGR colorval (0xFFBBGGRR) from 5/5/5 format pixel*/
#define PIXEL555TOCOLORVAL(p)	\
	(0xff000000ul | (((p) & 0x7c00u) >> 7) | (((p) & 0x03e0u) << 6) | (((p) & 0x1ful) << 19) | 0xff000000ul)

/* create ABGR colorval (0xFFBBGGRR) from 1/5/5/5 format pixel*/
#define PIXEL1555TOCOLORVAL(p)  \
        (0xff000000ul | (((p) & 0x7c00u) << 9) | (((p) & 0x03e0u) << 6) | (((p) & 0x1ful) << 3) | 0xff000000ul)

/* create ABGR colorval (0xFFBBGGRR) from 3/3/2 format pixel*/
#define PIXEL332TOCOLORVAL(p)	\
	(0xff000000ul | (((p) & 0xe0u)) | (((p) & 0x1cu) << 11) | (((p) & 0x03ul) << 19) | 0xff000000ul)

/* create ABGR colorval (0x00BBGGRR) from 2/3/3 format pixel*/
#define PIXEL233TOCOLORVAL(p)	\
	(((p) & 0x07) | (((p) & 0x38) << 5) | (((p) & 0xC0) << 14))

/*
* Conversion from ARGB values
*/
/* create 16 bit 5/6/5 format pixel from ARGB value (0xAARRGGBB)*/
#define ARGB2PIXEL565(c) \
	((((c) & 0x0000f8) >> 3) | (((c) & 0x00fc00) >> 5) | (((c) & 0xf80000) >> 8))

/* convert ARGB (0xAARRGGBB) to ABGR colorval (0xAABBGGRR)*/
#define ARGB2COLORVAL(c) \
	(((c) & 0xFF00FF00UL) | (((c) & 0x00FF0000UL) >> 16) | (((c) & 0x000000FFUL) << 16))

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR8888
#define RGB2PIXEL(r,g,b)	RGB2PIXEL8888(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL8888(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL8888TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL8888RED(p)
#define PIXEL2GREEN(p)		PIXEL8888GREEN(p)
#define PIXEL2BLUE(p)		PIXEL8888BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLORABGR
#define RGB2PIXEL(r,g,b)	RGB2PIXELABGR(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXELABGR(c)
#define PIXELVALTOCOLORVAL(p)	PIXELABGRTOCOLORVAL(p)
#define PIXEL2RED(p)		PIXELABGRRED(p)
#define PIXEL2GREEN(p)		PIXELABGRGREEN(p)
#define PIXEL2BLUE(p)		PIXELABGRBLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR888
#define RGB2PIXEL(r,g,b)	RGB2PIXEL888(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL888(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL888TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL888RED(p)
#define PIXEL2GREEN(p)		PIXEL888GREEN(p)
#define PIXEL2BLUE(p)		PIXEL888BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR565
#define RGB2PIXEL(r,g,b)	RGB2PIXEL565(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL565(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL565TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL565RED(p)
#define PIXEL2GREEN(p)		PIXEL565GREEN(p)
#define PIXEL2BLUE(p)		PIXEL565BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR555
#define RGB2PIXEL(r,g,b)	RGB2PIXEL555(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL555(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL555TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL555RED(p)
#define PIXEL2GREEN(p)		PIXEL555GREEN(p)
#define PIXEL2BLUE(p)		PIXEL555BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR1555
#define RGB2PIXEL(r,g,b)	RGB2PIXEL1555(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL1555(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL1555TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL1555RED(p)
#define PIXEL2GREEN(p)		PIXEL1555GREEN(p)
#define PIXEL2BLUE(p)		PIXEL1555BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR332
#define RGB2PIXEL(r,g,b)	RGB2PIXEL332(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL332(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL332TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL332RED(p)
#define PIXEL2GREEN(p)		PIXEL332GREEN(p)
#define PIXEL2BLUE(p)		PIXEL332BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_TRUECOLOR233
#define RGB2PIXEL(r,g,b)	RGB2PIXEL233(r,g,b)
#define COLORVALTOPIXELVAL(c)	COLOR2PIXEL233(c)
#define PIXELVALTOCOLORVAL(p)	PIXEL233TOCOLORVAL(p)
#define PIXEL2RED(p)		PIXEL233RED(p)
#define PIXEL2GREEN(p)		PIXEL233GREEN(p)
#define PIXEL2BLUE(p)		PIXEL233BLUE(p)
#endif

#if DPPIXEL_FORMAT == DPPF_PALETTE
//only required for compiling in palette pixel size, not supported in convblits
//extern MWPALENTRY	gr_palette[256];
//extern int32_t	gr_ncolors;
//#define RGB2PIXEL(r,g,b)	GdFindNearestColor(gr_palette, gr_ncolors, DPRGB(r,g,b))
#define RGB2PIXEL(r,g,b)	0
#endif

/*
* Common colors - note any color including these may not be
* available on palettized systems, and the system will
* then use the nearest color already in the system palette,
* or allocate a new color entry.
* These colors are the first 16 entries in the std palette,
* and are written to the system palette if writable.
*/
#define BLACK		DPRGB( 0  , 0  , 0   )
#define BLUE		DPRGB( 0  , 0  , 128 )
#define GREEN		DPRGB( 0  , 128, 0   )
#define CYAN		DPRGB( 0  , 128, 128 )
#define RED			DPRGB( 128, 0  , 0   )
#define MAGENTA		DPRGB( 128, 0  , 128 )
#define BROWN		DPRGB( 128, 64 , 0   )
#define LTGRAY		DPRGB( 192, 192, 192 )
#define GRAY		DPRGB( 128, 128, 128 )
#define LTBLUE		DPRGB( 0  , 0  , 255 )
#define LTGREEN		DPRGB( 0  , 255, 0   )
#define LTCYAN		DPRGB( 0  , 255, 255 )
#define LTRED		DPRGB( 255, 0  , 0   )
#define LTMAGENTA	DPRGB( 255, 0  , 255 )
#define YELLOW		DPRGB( 255, 255, 0   )
#define WHITE		DPRGB( 255, 255, 255 )
/* other common colors*/
#define DKGRAY		DPRGB( 32,  32,  32)

#if 0000
/* colors assumed in first 16 palette entries*/
/* note: don't use palette indices if the palette may
* be reloaded.  Use the RGB values instead.
*/
#define BLACK		PALINDEX(0)		/*   0,   0,   0*/
#define BLUE		PALINDEX(1)
#define GREEN		PALINDEX(2)
#define CYAN		PALINDEX(3)
#define RED			PALINDEX(4)
#define MAGENTA		PALINDEX(5)
#define BROWN		PALINDEX(6)
#define LTGRAY		PALINDEX(7)		/* 192, 192, 192*/
#define GRAY		PALINDEX(8)		/* 128, 128, 128*/
#define LTBLUE		PALINDEX(9)
#define LTGREEN		PALINDEX(10)
#define LTCYAN		PALINDEX(11)
#define LTRED		PALINDEX(12)
#define LTMAGENTA	PALINDEX(13)
#define YELLOW		PALINDEX(14)
#define WHITE		PALINDEX(15)	/* 255, 255, 255*/
#endif

//#endif /* MWINCLUDECOLORS*/
