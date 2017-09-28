/*
* Copyright (c) 2000, 2001, 2003, 2005, 2010 Greg Haerr <greg@censoft.com>
*
* Microsoft Windows screen driver for Microwindows
*	Tested in NONETWORK mode only
*
* by Wilson Loi
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <wingdi.h>
#include <assert.h>

#include "animwin32.h"

#include "dpdevice.h"
#include "fb.h"
#include "genmem.h"
#include "genfont.h"

#define APP_NAME "Microwindows"
char szTitle[] = "drawproc";					// The title bar text
#define CLASS_NAME "animwin"			// the main window class name


/* SCREEN_WIDTH, SCREEN_HEIGHT and DPPIXEL_FORMAT define window size*/
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH	800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT	600
#endif

#ifndef DPPIXEL_FORMAT
#define DPPIXEL_FORMAT	DPPF_TRUECOLOR8888
#endif

/* externally set override values from nanox/srvmain.c*/
DPCOORD	nxres;			/* requested server x res*/
DPCOORD	nyres;			/* requested server y res*/

						/* specific driver entry points*/
static PSD win32_open(PSD psd);
static void win32_close(PSD psd);
static void win32_getscreeninfo(PSD psd, PDPSCREENINFO psi);
static void win32_update(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height);



SCREENDEVICE scrdev = {
0, 0, 0, 1, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, 0, 0, 0,
gen_fonts,
win32_open,
win32_close,
NULL,				// SetPalette
win32_getscreeninfo,
gen_allocatememgc,
gen_mapmemgc,
gen_freememgc,
NULL,				// SetPortrait
win32_update,
NULL				// PreSelect
};



HWND winRootWindow = NULL;
static HDC dcBuffer = NULL;
static HBITMAP dcBitmap = NULL;
void * gPixelData;
static HBITMAP dcOldBitmap;
static HANDLE dummyEvent = NULL;


// Keyboard event handlers
static KeyboardHandler gkbdHandler = nullptr;

// Mouse event handlers
static MouseHandler gmouseHandler = nullptr;

void setKeyboardHandler(KeyboardHandler handler)
{
	gkbdHandler = handler;
}

void setMouseHandler(MouseHandler handler)
{
	gmouseHandler = handler;
}

/*
LRESULT
myWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	BOOL ret;
	HDC dc;
	PAINTSTRUCT ps;

	switch (Msg) {
	case WM_CREATE:
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYUP:
		break;
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		break;
	case WM_PAINT:
		dc = BeginPaint(hWnd, &ps);
		ret = BitBlt(dc, ps.rcPaint.left, ps.rcPaint.top,
			ps.rcPaint.right - ps.rcPaint.left,
			ps.rcPaint.bottom - ps.rcPaint.top,
			dcBuffer, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
		assert(ret != FALSE);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		SelectObject(dcBuffer, dcOldBitmap);
		DeleteDC(dcBuffer);
		DeleteObject(dcBitmap);
		winRootWindow = NULL;
		CloseHandle(dummyEvent);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}
*/
// bit field operations
// retrieve a range of bits as a single value
uint32_t bitops32_extract_range(const uint32_t value, const int offset, const int length)
{
	uint32_t res = value >> offset;
	res = res & ~(1 << length);
	return res;
}

LRESULT CALLBACK myWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	PAINTSTRUCT ps;
	HDC hdc;
	CREATESTRUCT *crStruct = (CREATESTRUCT *)lParam;

	switch (message)
	{
	case WM_CREATE:
		ret = DefWindowProc(hWnd, message, wParam, lParam);
		break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP: {
		uint32_t scanCode = bitops32_extract_range((uint32_t)lParam, 16, 8);
		uint32_t context = bitops32_extract_range((uint32_t)lParam, 29, 1);
		uint32_t previousState = bitops32_extract_range((uint32_t)lParam, 30, 1);
		uint32_t transitionState = bitops32_extract_range((uint32_t)lParam, 31, 1);

		//printf("scan code: %d context: %d  previous: %d  transition: %d\n", scanCode, context, previousState, transitionState);

		if (gkbdHandler != nullptr)
		{
			return gkbdHandler(hWnd, message, wParam, lParam);
		}
	}
				   break;

	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		if (gkbdHandler != nullptr)
		{
			return gkbdHandler(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		if (gmouseHandler != nullptr) {
			return gmouseHandler(hWnd, message, wParam, lParam);
		}
		break;

	case WM_MOUSEWHEEL:
		if (gmouseHandler != nullptr) {
			return gmouseHandler(hWnd, message, wParam, lParam);
		}
		break;

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		if (gmouseHandler != nullptr) {
			return gmouseHandler(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// bitblt bmhandle to client area
		// we should actually look at the paint struct
		// and only blit the part that needs to be drawn
		if ((NULL != dcBuffer) && (nullptr != gPixelData)) {
			ret = ::BitBlt(hdc,
				ps.rcPaint.left, ps.rcPaint.top,
				ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
				dcBuffer,
				ps.rcPaint.left, ps.rcPaint.top,
				SRCCOPY);
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		ret = quit();
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return ret;
}

ATOM RegisterWindowClass(size_t lwidth, size_t lheight)
{
	HMODULE hInst = ::GetModuleHandleA(NULL);

	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = myWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);;				//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;	// (HBRUSH)GetStockObject(WHITE_BRUSH);		//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = NULL;			//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));

	return ::RegisterClassExA(&wcex);
}

HWND CreateWindowHandle(size_t lwidth, size_t lheight)
{
	ATOM winclassatom = RegisterWindowClass(lwidth, lheight);

	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;
	HWND hWndParent = NULL;
	HMENU hMenu = NULL;

	if (!winclassatom) {
		return NULL;
	}

	RECT clientRECT = { 0, 0, lwidth, lheight };
	BOOL err = AdjustWindowRect(&clientRECT, WS_CAPTION, 0);

	// The following sequence of messages will come in through the callback
	// function before this function returns:
	// WM_GETMINMAXINFO
	// WM_NCCREATE
	// WM_NCCALCSIZE
	// WM_CREATE
	HMODULE hInst = ::GetModuleHandleA(NULL);
	winRootWindow = ::CreateWindowExA(
		0,
		CLASS_NAME,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		x, y, clientRECT.right - clientRECT.left, clientRECT.bottom - clientRECT.top,
		hWndParent,
		hMenu,
		hInst,
		NULL);
}

HDC CreateOffscreenDC(HWND hWnd, const size_t width, const size_t height, void **data)
{
	BITMAPINFO info;
	memset(&info, 0, sizeof(BITMAPINFO));

	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -(int)height;	// top-down DIB Section
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;


	dcBitmap = ::CreateDIBSection(
		NULL,
		&info,
		DIB_RGB_COLORS,
		data,
		NULL,
		0);

	// Create offscreen memory DC for DIB Section rendering
	dcBuffer = ::CreateCompatibleDC(NULL);
	::SelectObject(dcBuffer, dcBitmap);

	return dcBuffer;
}


static PSD
win32_open(PSD psd)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	PSUBDRIVER subdriver = nullptr;
	size_t depth = 32;


	// Build up the screendevice
	psd->xvirtres = width;
	psd->yvirtres = height;

	psd->xres = psd->xvirtres;
	psd->yres = psd->yvirtres;
	psd->planes = 1;
	psd->pixtype = DPPIXEL_FORMAT;
#if (DPPIXEL_FORMAT == DPPF_TRUECOLOR8888) || (DPPIXEL_FORMAT == DPPF_TRUECOLORABGR)
	psd->bpp = 32;
#elif (DPPIXEL_FORMAT == DPPF_TRUECOLOR888)
	psd->bpp = 24;
#elif (DPPIXEL_FORMAT == DPPF_TRUECOLOR565) || (DPPIXEL_FORMAT == DPPF_TRUECOLOR555)
	psd->bpp = 16;
#else
#error "No support bpp < 16"
#endif 
	// set standard data format from bpp and pixtype
	psd->data_format = set_data_format(psd);

	// Calculate size and pitch
	GdCalcMemGCAlloc(psd, psd->xres, psd->yres, psd->planes, psd->bpp,
		&psd->size, &psd->pitch);

	//if ((scrdev.addr = (uint8_t *)malloc(scrdev.size)) == NULL)
	//	break;

	psd->ncolors = psd->bpp >= 24 ? (1 << 24) : (1 << psd->bpp);
	psd->flags = PSF_SCREEN | PSF_ADDRMALLOC;
	psd->portrait = DPPORTRAIT_NONE;

	// select an fb subdriver matching our planes and bpp for backing store
	subdriver = select_fb_subdriver(psd);
	if (!subdriver)
		return NULL;

	// set subdriver into screen driver
	set_subdriver(psd, subdriver);

	winRootWindow = CreateWindowHandle(width, height);

	// Create offscreen bitmap
	dcBuffer = CreateOffscreenDC(winRootWindow, width, height, &gPixelData);

	psd->addr = (uint8_t *)gPixelData;

	if (winRootWindow) {
		HDC dc = GetDC(winRootWindow);
		RECT drect;

		GetClientRect(winRootWindow, &drect);

		//dcBitmap = CreateCompatibleBitmap(dc, drect.right - drect.left, drect.bottom - drect.top);
		//dcBuffer = CreateCompatibleDC(dc);
		//dcOldBitmap = (HBITMAP)SelectObject(dcBuffer, dcBitmap);
		ReleaseDC(winRootWindow, dc);
		//dummyEvent = CreateEvent(NULL, TRUE, FALSE, "");
		
		ShowWindow(winRootWindow, SW_SHOW);
		UpdateWindow(winRootWindow);
	}

	return psd;
}



/*
	Start by getting the bitsperpixel for the display
	use that to setup our drawing buffer.
*/
/*
static PSD
win32_open(PSD psd)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HDC rootDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	int depth = GetDeviceCaps(rootDC, BITSPIXEL);
	RECT drect;
	PSUBDRIVER subdriver;
	WNDCLASS wc;

	DeleteDC(rootDC);

	// Get a rectangle for the size of the desktop
	::GetWindowRect(GetDesktopWindow(), &drect);
	psd->xvirtres = drect.right - drect.left;
	psd->yvirtres = drect.bottom - drect.top;
	if (psd->xvirtres > SCREEN_WIDTH)
		psd->xvirtres = SCREEN_WIDTH;
	if (psd->yvirtres > SCREEN_HEIGHT)
		psd->yvirtres = SCREEN_HEIGHT;
	psd->xres = psd->xvirtres;
	psd->yres = psd->yvirtres;
	psd->planes = 1;
	psd->pixtype = DPPIXEL_FORMAT;
#if (DPPIXEL_FORMAT == DPPF_TRUECOLOR8888) || (DPPIXEL_FORMAT == DPPF_TRUECOLORABGR)
	psd->bpp = 32;
#elif (DPPIXEL_FORMAT == DPPF_TRUECOLOR888)
	psd->bpp = 24;
#elif (DPPIXEL_FORMAT == DPPF_TRUECOLOR565) || (DPPIXEL_FORMAT == DPPF_TRUECOLOR555)
	psd->bpp = 16;
#else
#error "No support bpp < 16"
#endif 
	// set standard data format from bpp and pixtype
	psd->data_format = set_data_format(psd);

	// Calculate size and pitch
	GdCalcMemGCAlloc(psd, psd->xres, psd->yres, psd->planes, psd->bpp,
		&psd->size, &psd->pitch);
	if ((psd->addr = (uint8_t *)malloc(psd->size)) == NULL)
		return NULL;
	psd->ncolors = psd->bpp >= 24 ? (1 << 24) : (1 << psd->bpp);
	psd->flags = PSF_SCREEN | PSF_ADDRMALLOC;
	psd->portrait = DPPORTRAIT_NONE;
	DPRINTF("win32 emulated bpp %d\n", psd->bpp);

	// select an fb subdriver matching our planes and bpp for backing store
	subdriver = select_fb_subdriver(psd);
	if (!subdriver)
		return NULL;

	// set subdriver into screen driver
	set_subdriver(psd, subdriver);

	RegisterWindowClass(width, height);
	winRootWindow = CreateWindowHandle(width, height);
	
	if (winRootWindow) {
		HDC dc = GetDC(winRootWindow);

		GetClientRect(winRootWindow, &drect);
		dcBitmap = CreateCompatibleBitmap(dc, drect.right - drect.left, drect.bottom - drect.top);
		dcBuffer = CreateCompatibleDC(dc);
		dcOldBitmap = (HBITMAP)SelectObject(dcBuffer, dcBitmap);
		ReleaseDC(winRootWindow, dc);
		dummyEvent = CreateEvent(NULL, TRUE, FALSE, "");
		ShowWindow(winRootWindow, SW_SHOW);
		UpdateWindow(winRootWindow);
	}

	return &scrdev;
}
*/

static void
win32_close(PSD psd)
{
	if (winRootWindow)
		SendMessage(winRootWindow, WM_DESTROY, 0, 0);
}


static void
win32_getscreeninfo(PSD psd, PDPSCREENINFO psi)
{
	gen_getscreeninfo(psd, psi);
	psi->fbdriver = FALSE;	/* not running fb driver, no direct map */
}


#if 0000
static void
win32_drawpixel(PSD psd, DPCOORD x, DPCOORD y, MWPIXELVAL pixel)
{
	COLORREF c = PIXELVALTOCOLORVAL(pixel);
	HDC dc = GetDC(winRootWindow);

	SetPixel(dc, x, y, c);
	ReleaseDC(winRootWindow, dc);
	SetPixel(dcBuffer, x, y, c);
}

static MWPIXELVAL
win32_readpixel(PSD psd, DPCOORD x, DPCOORD y)
{
	COLORREF ret = GetPixel(dcBuffer, x, y);

	return COLORVALTOPIXELVAL(ret);
}

static void
drawLine(HDC dc, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2, COLORREF c)
{
	POINT p;
	HPEN pen = CreatePen(PS_SOLID, 1, c);

	if (pen) {
		HGDIOBJ old = SelectObject(dc, pen);

		MoveToEx(dc, x1, y1, &p);
		LineTo(dc, x2, y2);
		SelectObject(dc, old);
		DeleteObject(pen);
	}
}

static void
win32_drawhline(PSD psd, DPCOORD x1, DPCOORD x2, DPCOORD y, MWPIXELVAL pixel)
{
	COLORREF c = PIXELVALTOCOLORVAL(pixel);
	HDC dc = GetDC(winRootWindow);

	drawLine(dc, x1, y, x2, y, c);
	ReleaseDC(winRootWindow, dc);
	drawLine(dcBuffer, x1, y, x2, y, c);
}

static void
win32_drawvline(PSD psd, DPCOORD x, DPCOORD y1, DPCOORD y2, MWPIXELVAL pixel)
{
	COLORREF c = PIXELVALTOCOLORVAL(pixel);
	HDC dc = GetDC(winRootWindow);

	drawLine(dc, x, y1, x, y2, c);
	ReleaseDC(winRootWindow, dc);
	drawLine(dcBuffer, x, y1, x, y2, c);
}

static void
win32_fillrect(PSD psd, DPCOORD x1, DPCOORD y1, DPCOORD x2, DPCOORD y2,
	MWPIXELVAL pixel)
{
	RECT rect;
	HDC dc;
	HBRUSH hbr = CreateSolidBrush(PIXELVALTOCOLORVAL(pixel));

	if (hbr) {
		rect.bottom = y2;
		rect.top = y1;
		rect.left = x1;
		rect.right = x2;
		dc = GetDC(winRootWindow);
		FillRect(dc, &rect, hbr);
		ReleaseDC(winRootWindow, dc);
		FillRect(dcBuffer, &rect, hbr);
		DeleteObject(hbr);
	}
}

static void
win32_blit(PSD dstpsd, DPCOORD destx, DPCOORD desty, DPCOORD w, DPCOORD h,
	PSD srcpsd, DPCOORD srcx, DPCOORD srcy, long op)
{
	unsigned char *addr = srcpsd->addr;
	HDC dc;

	if (op == MWROP_NOOP)
		return;

	if (!(dstpsd->flags & PSF_SCREEN)) {
		/* memory to memory blit, use offscreen blitter */
		dstpsd->Blit(dstpsd, destx, desty, w, h,
			srcpsd, srcx, srcy, op);
		return;
	}

	dc = GetDC(winRootWindow);
	if (srcpsd->flags & PSF_MEMORY) {
		unsigned int i;
		unsigned char *bits = NULL;
		BITMAPV4HEADER bmpInfo;

		memset(&bmpInfo, 0, sizeof(bmpInfo));
		bmpInfo.bV4Size = sizeof(bmpInfo);
		bmpInfo.bV4Width = srcpsd->xres;
		bmpInfo.bV4Height = -srcpsd->yres; /* top-down image */
		bmpInfo.bV4Planes = 1;
		bmpInfo.bV4BitCount = srcpsd->bpp;
		bmpInfo.bV4SizeImage = srcpsd->size;

		switch (srcpsd->pixtype) {
		case MWPF_TRUECOLOR565:
			bmpInfo.bV4RedMask = 0xf800u;
			bmpInfo.bV4GreenMask = 0x07e0u;
			bmpInfo.bV4BlueMask = 0x001fu;
			bmpInfo.bV4V4Compression = BI_BITFIELDS;
			break;
		case MWPF_TRUECOLOR555:
			bmpInfo.bV4RedMask = 0x7c00u;
			bmpInfo.bV4GreenMask = 0x03e0u;
			bmpInfo.bV4BlueMask = 0x001fu;
			bmpInfo.bV4V4Compression = BI_BITFIELDS;
			break;
		case MWPF_TRUECOLOR888:
			if ((bits = (unsigned char *)malloc(bmpInfo.bV4SizeImage)) != NULL) {
				for (i = 0; i<bmpInfo.bV4SizeImage; i += 3) {
					bits[i] = addr[i + 2];
					bits[i + 1] = addr[i + 1];
					bits[i + 2] = addr[i];
				}
				addr = bits;
			}
			bmpInfo.bV4V4Compression = BI_RGB;
			break;
		case MWPF_TRUECOLORABGR:
			bmpInfo.bV4AlphaMask = 0xff000000;
			bmpInfo.bV4RedMask = 0x0000ff;
			bmpInfo.bV4GreenMask = 0x00ff00;
			bmpInfo.bV4BlueMask = 0xff0000;
			bmpInfo.bV4V4Compression = BI_BITFIELDS;
			break;
		case MWPF_TRUECOLOR8888:
			bmpInfo.bV4AlphaMask = 0xff000000;
			bmpInfo.bV4RedMask = 0xff0000;
			bmpInfo.bV4GreenMask = 0x00ff00;
			bmpInfo.bV4BlueMask = 0x0000ff;
			bmpInfo.bV4V4Compression = BI_BITFIELDS;
			break;
		}

		bmpInfo.bV4XPelsPerMeter = 3078;
		bmpInfo.bV4YPelsPerMeter = 3078;
		bmpInfo.bV4ClrUsed = 0;
		bmpInfo.bV4ClrImportant = 0;
		bmpInfo.bV4CSType = LCS_sRGB;
		srcy = srcpsd->yres - h - srcy;
#if 1
		SetDIBitsToDevice(dc, destx, desty, w, h, srcx, srcy, 0, srcpsd->yres,
			addr, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS);
		SetDIBitsToDevice(dcBuffer, destx, desty, w, h, srcx, srcy, 0, srcpsd->yres,
			addr, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS);
#else
		StretchDIBits(dc, destx, desty, w, h, srcx, srcy, w, h,
			addr, &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		StretchDIBits(dcBuffer, destx, desty, w, h, srcx, srcy, w, h,
			addr, &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
#endif 
		free(bits);
	}
	else if (srcpsd->flags & PSF_SCREEN) {
		/* Use offscreen equivalent as the source */
		BitBlt(dc, destx, desty, w, h, dcBuffer, srcx, srcy, SRCCOPY);
		BitBlt(dcBuffer, destx, desty, w, h, dcBuffer, srcx, srcy, SRCCOPY);
	}
	ReleaseDC(winRootWindow, dc);
}
#endif

static void
update_from_savebits(PSD psd, int destx, int desty, int w, int h)
{
#if 0
	XImage *img;
	unsigned int x, y;
	char *data;

	/* allocate buffer */
	if (x11_depth >= 24)
		data = malloc(w * 4 * h);
	else if (x11_depth > 8)	/* 15, 16 */
		data = malloc(w * 2 * h);
	else			/* 1,2,4,8 */
		data = malloc((w * x11_depth + 7) / 8 * h);

	/* copy from offscreen to screen */
	img = XCreateImage(x11_dpy, x11_vis, x11_depth, ZPixmap, 0, data, w, h, 8, 0);

	/* Use optimized loops for most common framebuffer modes */

#if MWPIXEL_FORMAT == MWPF_TRUECOLOR332
	{
		unsigned char *addr = psd->addr + desty * psd->pitch + destx;
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				MWPIXELVAL c = addr[x];
				unsigned long pixel = PIXELVAL_to_pixel(c);
				XPutPixel(img, x, y, pixel);
			}
			addr += psd->pitch;
		}
	}
#elif (MWPIXEL_FORMAT == MWPF_TRUECOLOR565) || (MWPIXEL_FORMAT == MWPF_TRUECOLOR555)
	{
		unsigned char *addr = psd->addr + desty * psd->pitch + (destx << 1);
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				MWPIXELVAL c = ((ADDR16)addr)[x];
				unsigned long pixel = PIXELVAL_to_pixel(c);
				XPutPixel(img, x, y, pixel);
			}
			addr += psd->pitch;
		}
	}
#elif MWPIXEL_FORMAT == MWPF_TRUECOLOR888
	{
		unsigned char *addr = psd->addr + desty * psd->pitch + destx * 3;
		unsigned int extra = psd->pitch - w * 3;
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				MWPIXELVAL c = RGB2PIXEL888(addr[2], addr[1], addr[0]);
				unsigned long pixel = PIXELVAL_to_pixel(c);
				XPutPixel(img, x, y, pixel);
				addr += 3;
			}
			addr += extra;
		}
	}
#elif (MWPIXEL_FORMAT == MWPF_TRUECOLOR8888) || (MWPIXEL_FORMAT == MWPF_TRUECOLORABGR)
	{
		unsigned char *addr = psd->addr + desty * psd->pitch + (destx << 2);
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				MWPIXELVAL c = ((ADDR32)addr)[x];
				unsigned long pixel = PIXELVAL_to_pixel(c);
				XPutPixel(img, x, y, pixel);
			}
			addr += psd->pitch;
		}
	}
#else /* MWPF_PALETTE*/
	{
		unsigned char *addr = psd->addr + desty * psd->pitch + destx;
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				MWPIXELVAL c = addr[x];
				unsigned long pixel = PIXELVAL_to_pixel(c);
				XPutPixel(img, x, y, pixel);
			}
			addr += psd->pitch;
		}
	}
#endif

	XPutImage(x11_dpy, x11_win, x11_gc, img, 0, 0, destx, desty, w, h);
	XDestroyImage(img);
#endif
}

static void
win32_update(PSD psd, DPCOORD x, DPCOORD y, DPCOORD width, DPCOORD height)
{
	if (!width)
		width = psd->xres;
	if (!height)
		height = psd->yres;
	update_from_savebits(psd, x, y, width, height);
}
