#pragma once



/* entry points*/

/* devdraw.c*/
PSD		GdOpenScreen(void);
void	GdCloseScreen(PSD psd);
int		GdSetPortraitMode(PSD psd, int portraitmode);
int		GdSetMode(int mode);
DPBOOL	GdSetUseBackground(DPBOOL flag);
DPPIXELVAL GdSetForegroundPixelVal(PSD psd, DPPIXELVAL fg);
DPPIXELVAL GdSetBackgroundPixelVal(PSD psd, DPPIXELVAL bg);
DPPIXELVAL GdSetForegroundColor(PSD psd, DPCOLORVAL fg);
DPPIXELVAL GdSetBackgroundColor(PSD psd, DPCOLORVAL bg);
void	GdResetPalette(void);
void	GdSetPalette(PSD psd, int first, int count, MWPALENTRY *palette);
int		GdGetPalette(PSD psd, int first, int count, MWPALENTRY *palette);
DPCOLORVAL GdGetColorRGB(PSD psd, DPPIXELVAL pixel);
DPPIXELVAL GdFindColor(PSD psd, DPCOLORVAL c);
DPPIXELVAL GdFindNearestColor(MWPALENTRY *pal, int size, DPCOLORVAL cr);
int		GdCaptureScreen(PSD psd, char *pathname);	/* debug only*/
void	GdPrintBitmap(PDPBLITPARMS gc, int SSZ);	/* debug only*/
void	GdGetScreenInfo(PSD psd, PDPSCREENINFO psi);
void	GdPoint(PSD psd, DPCOORD x, DPCOORD y);
void	GdLine(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, DPBOOL bDrawLastPoint);
void	GdRect(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height);
void	GdFillRect(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height);
DPBOOL	GdColorInPalette(DPCOLORVAL cr, MWPALENTRY *palette, int palsize);
void	GdMakePaletteConversionTable(PSD psd, MWPALENTRY *palette, int palsize,
	DPPIXELVALHW *convtable, int fLoadType);
void	GdDrawImage(PSD psd, DPCOORD x, DPCOORD y, PMWIMAGEHDR pimage);
void	GdBitmap(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height, const DPIMAGEBITS *imagebits);
void	GdBitmapByPoint(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height,
	const DPIMAGEBITS *imagebits, int clipresult);
void	GdPoly(PSD psd, int count, DPPOINT *points);
void	GdFillPoly(PSD psd, int count, DPPOINT *points);
void	GdReadArea(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height, DPPIXELVALHW *pixels);
void	GdArea(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height, void *pixels, int pixtype);
void	GdTranslateArea(DPCOORD width, DPCOORD height, void *in, int inpixtype,
	DPCOORD inpitch, void *out, int outpixtype, int outpitch);
void	drawpoint(PSD psd, DPCOORD x, DPCOORD y);
void	drawrow(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y);
extern SCREENDEVICE scrdev;
extern DPPIXELVAL gr_foreground;		/* current foreground color */
extern DPPIXELVAL gr_background;		/* current background color */
extern DPBOOL 	  gr_usebg;			/* TRUE if background drawn in pixmaps */
extern DPCOLORVAL gr_foreground_rgb;/* current fg color in 0xAARRGGBB format*/
extern DPCOLORVAL gr_background_rgb;

/* devblit.c*/
DPBLITFUNC GdFindConvBlit(PSD psd, int data_format, int op);
void	GdConversionBlit(PSD psd, PDPBLITPARMS parms);
void	GdConvBlitInternal(PSD psd, PDPBLITPARMS gc, DPBLITFUNC convblit);
void	GdBlit(PSD dstpsd, DPCOORD dstx, DPCOORD dsty, DPCOORD width, DPCOORD height,
	PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int rop);
void	GdStretchBlit(PSD dstpsd, DPCOORD dx1, DPCOORD dy1, DPCOORD dx2,
	DPCOORD dy2, PSD srcpsd, DPCOORD sx1, DPCOORD sy1, DPCOORD sx2, DPCOORD sy2, int rop);

/* devarc.c*/
/* requires float*/
void	GdArcAngle(PSD psd, DPCOORD x0, DPCOORD y0, DPCOORD rx, DPCOORD ry,
	DPCOORD angle1, DPCOORD angle2, int type);
/* integer only*/
void	GdArc(PSD psd, DPCOORD x0, DPCOORD y0, DPCOORD rx, DPCOORD ry,
	DPCOORD ax, DPCOORD ay, DPCOORD bx, DPCOORD by, int type);
void	GdEllipse(PSD psd, DPCOORD x, DPCOORD y, DPCOORD rx, DPCOORD ry,
	DPBOOL fill);

/* devfont.c*/
void	GdClearFontList(void);
int		GdAddFont(char *fndry, char *family, char *fontname, PMWLOGFONT lf, unsigned int flags);
PMWFONT GdCreateFont(PSD psd, const char *name, DPCOORD height, DPCOORD width,
	const PMWLOGFONT plogfont);
DPCOORD	GdSetFontSize(PMWFONT pfont, DPCOORD height, DPCOORD width);
void 	GdGetFontList(MWFONTLIST ***list, int *num);
void 	GdFreeFontList(MWFONTLIST ***list, int num);
int		GdSetFontRotation(PMWFONT pfont, int tenthdegrees);
int		GdSetFontAttr(PMWFONT pfont, int setflags, int clrflags);
void	GdDestroyFont(PMWFONT pfont);
DPBOOL	GdGetFontInfo(PMWFONT pfont, PMWFONTINFO pfontinfo);
int		GdConvertEncoding(const void *istr, DPTEXTFLAGS iflags, int cc, void *ostr, DPTEXTFLAGS oflags);
void	GdGetTextSize(PMWFONT pfont, const void *str, int cc, DPCOORD *pwidth,
	DPCOORD *pheight, DPCOORD *pbase, DPTEXTFLAGS flags);
int		GdGetTextSizeEx(PMWFONT pfont, const void *str, int cc, int nMaxExtent,
	int *lpnFit, int *alpDx, DPCOORD *pwidth,
	DPCOORD *pheight, DPCOORD *pbase, DPTEXTFLAGS flags);
void	GdText(PSD psd, PMWFONT pfont, DPCOORD x, DPCOORD y, const void *str, int count, DPTEXTFLAGS flags);
PMWFONT	GdCreateFontFromBuffer(PSD psd, const unsigned char *buffer,
	unsigned length, const char *format, DPCOORD height, DPCOORD width);
PMWFONT	GdDuplicateFont(PSD psd, PMWFONT psrcfont, DPCOORD height, DPCOORD width);


/* both devclip1.c and devclip2.c */
DPBOOL	GdClipPoint(PSD psd, DPCOORD x, DPCOORD y);
int		GdClipArea(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2);
extern DPCOORD clipminx, clipminy, clipmaxx, clipmaxy;

/* devclip1.c only*/
void 	GdSetClipRects(PSD psd, int count, DPCLIPRECT *table);

/* devclip2.c only*/
void	GdSetClipRegion(PSD psd, DPCLIPREGION *reg);
void	GdPrintClipRects(PDPBLITPARMS gc);

/* devrgn.c - multi-rectangle region entry points*/
DPBOOL GdPtInRegion(DPCLIPREGION *rgn, DPCOORD x, DPCOORD y);
int    GdRectInRegion(DPCLIPREGION *rgn, const DPRECT *rect);
DPBOOL GdEqualRegion(DPCLIPREGION *r1, DPCLIPREGION *r2);
DPBOOL GdEmptyRegion(DPCLIPREGION *rgn);
DPCLIPREGION *GdAllocRegion(void);
DPCLIPREGION *GdAllocRectRegion(DPCOORD left, DPCOORD top, DPCOORD right, DPCOORD bottom);
DPCLIPREGION *GdAllocRectRegionIndirect(DPRECT *prc);
void GdSetRectRegion(DPCLIPREGION *rgn, DPCOORD left, DPCOORD top, DPCOORD right, DPCOORD bottom);
void GdSetRectRegionIndirect(DPCLIPREGION *rgn, DPRECT *prc);
void GdDestroyRegion(DPCLIPREGION *rgn);
void GdOffsetRegion(DPCLIPREGION *rgn, DPCOORD x, DPCOORD y);
int  GdGetRegionBox(DPCLIPREGION *rgn, DPRECT *prc);
void GdUnionRectWithRegion(const DPRECT *rect, DPCLIPREGION *rgn);
void GdSubtractRectFromRegion(const DPRECT *rect, DPCLIPREGION *rgn);
void GdCopyRegion(DPCLIPREGION *d, DPCLIPREGION *s);
void GdIntersectRegion(DPCLIPREGION *d, DPCLIPREGION *s1, DPCLIPREGION *s2);
void GdUnionRegion(DPCLIPREGION *d, DPCLIPREGION *s1, DPCLIPREGION *s2);
void GdSubtractRegion(DPCLIPREGION *d, DPCLIPREGION *s1, DPCLIPREGION *s2);
void GdXorRegion(DPCLIPREGION *d, DPCLIPREGION *s1, DPCLIPREGION *s2);
DPCLIPREGION *GdAllocBitmapRegion(DPIMAGEBITS *bitmap, DPCOORD width, DPCOORD height);

/* devrgn2.c*/
DPCLIPREGION *GdAllocPolygonRegion(DPPOINT *points, int count, int mode);
DPCLIPREGION *GdAllocPolyPolygonRegion(DPPOINT *points, int *count, int nbpolygons, int mode);

/* devmouse.c*/
int	GdOpenMouse(void);
void	GdCloseMouse(void);
void	GdGetButtonInfo(int *buttons);
void	GdRestrictMouse(DPCOORD newminx, DPCOORD newminy, DPCOORD newmaxx, DPCOORD newmaxy);
void	GdSetAccelMouse(int newthresh, int newscale);
void	GdMoveMouse(DPCOORD newx, DPCOORD newy);
int		GdReadMouse(DPCOORD *px, DPCOORD *py, int *pb);
void	GdMoveCursor(DPCOORD x, DPCOORD y);
DPBOOL	GdGetCursorPos(DPCOORD *px, DPCOORD *py);
void	GdSetCursor(PDPCURSOR pcursor);
int 	GdShowCursor(PSD psd);
int 	GdHideCursor(PSD psd);
void	GdCheckCursor(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2);
void 	GdFixCursor(PSD psd);
void    GdSetTransform(MWTRANSFORM *);

extern MOUSEDEVICE mousedev;

/* devkbd.c*/
int  	GdOpenKeyboard(void);
void 	GdCloseKeyboard(void);
void 	GdGetModifierInfo(MWKEYMOD *modifiers, MWKEYMOD *curmodifiers);
int  	GdReadKeyboard(MWKEY *buf, MWKEYMOD *modifiers, MWSCANCODE *scancode);
extern KBDDEVICE kbddev;

#ifdef MW_FEATURE_TWO_KEYBOARDS
int  	GdOpenKeyboard2(void);
extern KBDDEVICE kbddev2;
#endif

/* devimage.c */
#if MW_FEATURE_IMAGES
PSD		GdLoadImageFromFile(char *path, int flags);
PSD		GdLoadImageFromBuffer(void *buffer, int size, int flags);
void	GdDrawImageFromFile(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width,
	DPCOORD height, char *path, int flags);
void	GdDrawImageFromBuffer(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width,
	DPCOORD height, void *buffer, int size, int flags);
void	GdDrawImagePartToFit(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height,
	DPCOORD sx, DPCOORD sy, DPCOORD swidth, DPCOORD sheight, PSD pmd);
DPBOOL	GdGetImageInfo(PSD pmd, PMWIMAGEINFO pii);
void	GdStretchImage(PMWIMAGEHDR src, DPCLIPRECT *srcrect, PMWIMAGEHDR dst, DPCLIPRECT *dstrect);

/* Buffered input functions to replace stdio functions*/
typedef struct {  /* structure for reading images from buffer   */
	unsigned char *start;	/* The pointer to the beginning of the buffer */
	unsigned long offset;	/* The current offset within the buffer       */
	unsigned long size;	/* The total size of the buffer               */
} buffer_t;

void	GdImageBufferInit(buffer_t *buffer, void *startdata, int size);
void	GdImageBufferSeekTo(buffer_t *buffer, unsigned long offset);
int		GdImageBufferRead(buffer_t *buffer, void *dest, unsigned long size);
int		GdImageBufferGetChar(buffer_t *buffer);
char *	GdImageBufferGetString(buffer_t *buffer, char *dest, unsigned int size);
int		GdImageBufferEOF(buffer_t *buffer);

/* image conversion*/
PSD		GdConvertImageRGBA(PSD pmd);		/* convert palettized image to RGBA*/

											/* individual decoders*/
#ifdef HAVE_BMP_SUPPORT
PSD	GdDecodeBMP(buffer_t *src, DPBOOL readfilehdr);
#endif
#ifdef HAVE_JPEG_SUPPORT
PSD	GdDecodeJPEG(buffer_t *src, DPBOOL fast_grayscale);
#endif
#ifdef HAVE_PNG_SUPPORT
PSD	GdDecodePNG(buffer_t *src);
#endif
#ifdef HAVE_GIF_SUPPORT
PSD	GdDecodeGIF(buffer_t *src);
#endif
#ifdef HAVE_PNM_SUPPORT
PSD	GdDecodePNM(buffer_t *src);
#endif
#ifdef HAVE_XPM_SUPPORT
PSD	GdDecodeXPM(buffer_t *src);
#endif
#ifdef HAVE_TIFF_SUPPORT
PSD	GdDecodeTIFF(char *path);
#endif
#endif /* MW_FEATURE_IMAGES */

/* devlist.c*/
/* field offset*/
#define MWITEM_OFFSET(type, field)    ((long)&(((type *)0)->field))

void * 	GdItemAlloc(unsigned int size);
void	GdListAdd(PMWLISTHEAD pHead, PMWLIST pItem);
void	GdListInsert(PMWLISTHEAD pHead, PMWLIST pItem);
void	GdListRemove(PMWLISTHEAD pHead, PMWLIST pItem);
#define GdItemNew(type)	((type *)GdItemAlloc(sizeof(type)))
#define GdItemFree(ptr)	free((void *)ptr)
/* return base item address from list ptr*/
#define GdItemAddr(p,type,list)	((type *)((long)p - MWITEM_OFFSET(type,list)))

/* devstipple.c */
void	GdSetDash(uint32_t *mask, int *count);
void	GdSetStippleBitmap(DPIMAGEBITS *stipple, DPCOORD width, DPCOORD height);
void	GdSetTSOffset(int xoff, int yoff);
int		GdSetFillMode(int mode);
void	GdSetTilePixmap(PSD src, DPCOORD width, DPCOORD height);
void	ts_drawpoint(PSD psd, DPCOORD x, DPCOORD y);
void	ts_drawrow(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y);
void	ts_fillrect(PSD psd, DPCOORD x, DPCOORD y, DPCOORD w, DPCOORD h);
void	set_ts_origin(int x, int y);

/*
#if MW_FEATURE_TIMERS
#include <sys/time.h>

typedef void(*MWTIMERCB)(void *);

#define  MWTIMER_ONESHOT         0
#define  MWTIMER_PERIODIC        1

typedef struct mw_timer MWTIMER;
struct mw_timer {
struct timeval	timeout;
MWTIMERCB	callback;
void		*arg;
MWTIMER		*next;
MWTIMER		*prev;
int         type;     // MWTIMER_ONESHOT or MWTIMER_PERIODIC
MWTIMEOUT   period;
};

MWTIMER		*GdAddTimer(MWTIMEOUT timeout, MWTIMERCB callback, void *arg);
MWTIMER         *GdAddPeriodicTimer(MWTIMEOUT timeout, MWTIMERCB callback, void *arg);
void		GdDestroyTimer(MWTIMER *timer);
MWTIMER		*GdFindTimer(void *arg);
DPBOOL		GdGetNextTimeout(struct timeval *tv, MWTIMEOUT timeout);
DPBOOL		GdTimeout(void);

#endif // MW_FEATURE_TIMERS
*/
/* error.c*/
int	GdError(const char *format, ...);
int	GdErrorNull(const char *format, ...);  /* doesn't print msgs */
