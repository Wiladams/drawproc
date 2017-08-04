#pragma once

#include "dproc_config.h"

/*
	This file contains all the calls that are related to GDI-like drawing.
*/

/* entry points*/
// devopen.cpp
DPROC_API extern	 PSD	GdOpenScreen(void);
DPROC_API extern	 void	GdCloseScreen(PSD psd);
DPROC_API extern	 int	GdSetPortraitMode(PSD psd, int portraitmode);

/* devdraw.c*/

DPROC_API extern	 int		GdSetMode(int mode);
DPROC_API extern	 bool		GdSetUseBackground(bool flag);
DPROC_API extern	 DPPIXELVAL GdSetForegroundPixelVal(PSD psd, DPPIXELVAL fg);
DPROC_API extern	 DPPIXELVAL GdSetBackgroundPixelVal(PSD psd, DPPIXELVAL bg);
DPROC_API extern	 DPPIXELVAL GdSetForegroundColor(PSD psd, DPCOLORVAL fg);
DPROC_API extern	 DPPIXELVAL GdSetBackgroundColor(PSD psd, DPCOLORVAL bg);
DPROC_API extern	 void		GdResetPalette(void);
DPROC_API extern	 void		GdSetPalette(PSD psd, int first, int count, DPPALENTRY *palette);
DPROC_API extern	 int		GdGetPalette(PSD psd, int first, int count, DPPALENTRY *palette);
DPROC_API extern	 DPCOLORVAL GdGetColorRGB(PSD psd, DPPIXELVAL pixel);
DPROC_API extern	 DPPIXELVAL GdFindColor(PSD psd, DPCOLORVAL c);
DPROC_API extern	 DPPIXELVAL GdFindNearestColor(DPPALENTRY *pal, int size, DPCOLORVAL cr);
DPROC_API extern	 int		GdCaptureScreen(PSD psd, char *pathname);	/* debug only*/
DPROC_API extern	 void	GdPrintBitmap(PDPBLITPARMS gc, int SSZ);	/* debug only*/
DPROC_API extern	 void	GdGetScreenInfo(PSD psd, PDPSCREENINFO psi);
DPROC_API extern	 void	GdPoint(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern	 void	GdLine(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, bool bDrawLastPoint);
DPROC_API extern	 void	GdRect(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height);
DPROC_API extern	 void	GdFillRect(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height);
DPROC_API extern	 bool	GdColorInPalette(DPCOLORVAL cr, DPPALENTRY *palette, int palsize);
DPROC_API extern	 void	GdMakePaletteConversionTable(PSD psd, DPPALENTRY *palette, int palsize,
	DPPIXELVALHW *convtable, int fLoadType);
DPROC_API extern	 void	GdDrawImage(PSD psd, DPCOORD x, DPCOORD y, PDPIMAGEHDR pimage);
DPROC_API extern	 void	GdBitmap(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height, const DPIMAGEBITS *imagebits);
DPROC_API extern	 void	GdBitmapByPoint(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height,
	const DPIMAGEBITS *imagebits, int clipresult);
DPROC_API extern	 void	GdPoly(PSD psd, int count, DPPOINT *points);
DPROC_API extern	 void	GdFillPoly(PSD psd, int count, DPPOINT *points);
DPROC_API extern	 void	GdReadArea(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height, DPPIXELVALHW *pixels);
DPROC_API extern	 void	GdArea(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height, void *pixels, int pixtype);
DPROC_API extern	 void	GdTranslateArea(DPCOORD width, DPCOORD height, void *in, int inpixtype,
	DPCOORD inpitch, void *out, int outpixtype, int outpitch);
DPROC_API extern	 void	drawpoint(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern	 void	drawrow(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y);


/* devblit.c*/
DPROC_API extern	 DPBLITFUNC GdFindConvBlit(PSD psd, int data_format, int op);
DPROC_API extern	 void	GdConversionBlit(PSD psd, PDPBLITPARMS parms);
DPROC_API extern	 void	GdConvBlitInternal(PSD psd, PDPBLITPARMS gc, DPBLITFUNC convblit);
DPROC_API extern	 void	GdBlit(PSD dstpsd, DPCOORD dstx, DPCOORD dsty, DPCOORD width, DPCOORD height,
	PSD srcpsd, DPCOORD srcx, DPCOORD srcy, int rop);
DPROC_API extern	 void	GdStretchBlit(PSD dstpsd, DPCOORD dx1, DPCOORD dy1, DPCOORD dx2,
	DPCOORD dy2, PSD srcpsd, DPCOORD sx1, DPCOORD sy1, DPCOORD sx2, DPCOORD sy2, int rop);

/* devarc.c*/
/* requires float*/
DPROC_API extern	 void	GdArcAngle(PSD psd, DPCOORD x0, DPCOORD y0, DPCOORD rx, DPCOORD ry,
	DPCOORD angle1, DPCOORD angle2, int type);
/* integer only*/
DPROC_API extern	 void	GdArc(PSD psd, DPCOORD x0, DPCOORD y0, DPCOORD rx, DPCOORD ry,
	DPCOORD ax, DPCOORD ay, DPCOORD bx, DPCOORD by, int type);
DPROC_API extern	 void	GdEllipse(PSD psd, DPCOORD x, DPCOORD y, DPCOORD rx, DPCOORD ry,
	bool fill);

/* devfont.c*/
DPROC_API extern	 void	GdClearFontList(void);
DPROC_API extern	 int		GdAddFont(char *fndry, char *family, char *fontname, PDPLOGFONT lf, unsigned int flags);
DPROC_API extern	 PDPFONT GdCreateFont(PSD psd, const char *name, DPCOORD height, DPCOORD width,
	const PDPLOGFONT plogfont);
DPROC_API extern	 DPCOORD	GdSetFontSize(PDPFONT pfont, DPCOORD height, DPCOORD width);
DPROC_API extern	 void 	GdGetFontList(DPFONTLIST ***list, int *num);
DPROC_API extern	 void 	GdFreeFontList(DPFONTLIST ***list, int num);
DPROC_API extern	 int		GdSetFontRotation(PDPFONT pfont, int tenthdegrees);
DPROC_API extern	 int		GdSetFontAttr(PDPFONT pfont, int setflags, int clrflags);
DPROC_API extern	 void	GdDestroyFont(PDPFONT pfont);
DPROC_API extern	 bool	GdGetFontInfo(PDPFONT pfont, PDPFONTINFO pfontinfo);
DPROC_API extern	 int		GdConvertEncoding(const void *istr, DPTEXTFLAGS iflags, int cc, void *ostr, DPTEXTFLAGS oflags);
DPROC_API extern	 void	GdGetTextSize(PDPFONT pfont, const void *str, int cc, DPCOORD *pwidth,
	DPCOORD *pheight, DPCOORD *pbase, DPTEXTFLAGS flags);
DPROC_API extern	 int		GdGetTextSizeEx(PDPFONT pfont, const void *str, int cc, int nMaxExtent,
	int *lpnFit, int *alpDx, DPCOORD *pwidth,
	DPCOORD *pheight, DPCOORD *pbase, DPTEXTFLAGS flags);
DPROC_API extern	 void	GdText(PSD psd, PDPFONT pfont, DPCOORD x, DPCOORD y, const void *str, int count, DPTEXTFLAGS flags);
DPROC_API extern	 PDPFONT	GdCreateFontFromBuffer(PSD psd, const unsigned char *buffer,
	unsigned length, const char *format, DPCOORD height, DPCOORD width);
DPROC_API extern	 PDPFONT	GdDuplicateFont(PSD psd, PDPFONT psrcfont, DPCOORD height, DPCOORD width);


/* both devclip1.c and devclip2.c */
DPROC_API extern	 bool	GdClipPoint(PSD psd, DPCOORD x, DPCOORD y);
DPROC_API extern	 int		GdClipArea(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2);
extern DPCOORD clipminx, clipminy, clipmaxx, clipmaxy;

/* devclip1.c only*/
DPROC_API extern	 void 	GdSetClipRects(PSD psd, int count, DPCLIPRECT *table);

/* devclip2.c only*/
DPROC_API extern	 void	GdSetClipRegion(PSD psd, DPCLIPREGION *reg);
DPROC_API extern	 void	GdPrintClipRects(PDPBLITPARMS gc);

/* devrgn.c - multi-rectangle region entry points*/
DPROC_API extern	 bool GdPtInRegion(DPCLIPREGION *rgn, DPCOORD x, DPCOORD y);
DPROC_API extern	 int  GdRectInRegion(DPCLIPREGION *rgn, const DPRECT *rect);
DPROC_API extern	 bool GdEqualRegion(DPCLIPREGION *r1, DPCLIPREGION *r2);
bool GdEmptyRegion(DPCLIPREGION *rgn);
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
DPROC_API extern	 int	GdOpenMouse(void);
DPROC_API extern	 void	GdCloseMouse(void);
DPROC_API extern	 void	GdGetButtonInfo(int *buttons);
DPROC_API extern	 void	GdRestrictMouse(DPCOORD newminx, DPCOORD newminy, DPCOORD newmaxx, DPCOORD newmaxy);
DPROC_API extern	 void	GdSetAccelMouse(int newthresh, int newscale);
DPROC_API extern	 void	GdMoveMouse(DPCOORD newx, DPCOORD newy);
DPROC_API extern	 int		GdReadMouse(DPCOORD *px, DPCOORD *py, int *pb);
DPROC_API extern	 void	GdMoveCursor(DPCOORD x, DPCOORD y);
DPROC_API extern	 bool	GdGetCursorPos(DPCOORD *px, DPCOORD *py);
DPROC_API extern	 void	GdSetCursor(PDPCURSOR pcursor);
DPROC_API extern	 int 	GdShowCursor(PSD psd);
DPROC_API extern	 int 	GdHideCursor(PSD psd);
DPROC_API extern	 void	GdCheckCursor(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2);
DPROC_API extern	 void 	GdFixCursor(PSD psd);
DPROC_API extern	 void    GdSetTransform(DPTRANSFORM *);

//extern MOUSEDEVICE mousedev;

/* devkbd.c*/
DPROC_API extern	 int  	GdOpenKeyboard(void);
DPROC_API extern	 void 	GdCloseKeyboard(void);
DPROC_API extern	 void 	GdGetModifierInfo(MWKEYMOD *modifiers, MWKEYMOD *curmodifiers);
DPROC_API extern	 int  	GdReadKeyboard(MWKEY *buf, MWKEYMOD *modifiers, MWSCANCODE *scancode);

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
bool	GdGetImageInfo(PSD pmd, PDPIMAGEINFO pii);
void	GdStretchImage(PDPIMAGEHDR src, DPCLIPRECT *srcrect, PDPIMAGEHDR dst, DPCLIPRECT *dstrect);



// image conversion
PSD		GdConvertImageRGBA(PSD pmd);		/* convert palettized image to RGBA*/

											/* individual decoders*/
#ifdef HAVE_BMP_SUPPORT
PSD	GdDecodeBMP(buffer_t *src, bool readfilehdr);
#endif
#ifdef HAVE_JPEG_SUPPORT
PSD	GdDecodeJPEG(buffer_t *src, bool fast_grayscale);
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
bool		GdGetNextTimeout(struct timeval *tv, MWTIMEOUT timeout);
bool		GdTimeout(void);

#endif // MW_FEATURE_TIMERS
*/
/* error.c*/
DPROC_API extern	 int	GdError(const char *format, ...);
DPROC_API extern	 int	GdErrorNull(const char *format, ...);  /* doesn't print msgs */
