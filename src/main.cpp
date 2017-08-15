

#include "animwin32.h"
#include "dproc_clock.h"
#include "dpdevice.h"
#include "dp_win32.h"
#include "fb.h"
#include "genmem.h"
#include "genfont.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#ifdef _MSC_VER
// This is here so that we can simply use a 'main()' 
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#define MAX_LOADSTRING 100

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Windows Variables:
//HINSTANCE hInst;								// current instance
HMODULE clientModule;

char szTitle[] = "Window";					// The title bar text
#define CLASS_NAME "animwin"			// the main window class name
HWND ghWnd;

SCREENDEVICE scrdev;
/*
SCREENDEVICE scrdev = {
	0, 0, 0, 0, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, 0, 0, 0,
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
*/

// offscreen bitmap
HBITMAP gbmHandle;
HDC ghMemDC; 
void *gPixelData = nullptr;
static int gbmWidth = 0;
static int gbmHeight = 0;
bool continueRunning = true;
bool gDrawDuringLoop = true;



// Keyboard event handlers
static KeyboardHandler gkbdHandler = nullptr;

// Mouse event handlers
static MouseHandler gmouseHandler = nullptr;

// Setup and Loop handlers
static SetupHandler gSetupRoutine = nullptr;
static LoopHandler gLoopRoutine = nullptr;

// Application clock
struct dproc_clock *gClock = nullptr;


// Routines called by client
SetupHandler setSetupRoutine(SetupHandler handler)
{
	SetupHandler oldRoutine = gSetupRoutine;
	gSetupRoutine = handler;
	return oldRoutine;
}

LoopHandler setLoopRoutine(LoopHandler handler)
{
	LoopHandler oldRoutine = gLoopRoutine;
	gLoopRoutine = handler;
	return oldRoutine;
}

void setKeyboardHandler(KeyboardHandler handler)
{
	gkbdHandler = handler;
}

void setMouseHandler(MouseHandler handler)
{
	gmouseHandler = handler;
}

LRESULT quit()
{
	PostQuitMessage(0);
	continueRunning = false;

	return 0;
}

void forceDraw()
{
	// Assume the 'draw()' did something which requires the 
	// screen to be redrawn, so, invalidate the entire client area
	InvalidateRect(ghWnd, 0, TRUE);
}


// time keeping
void resettime()
{
	dproc_clock_init(gClock);
}

double seconds()
{
	return dproc_clock_seconds(gClock);
}

HMODULE getClientModule()
{
	return clientModule;
}

// Internal to animwin32
bool InitializeInstance(const char *moduleName)
{

	// Get pointers to client setup and loop routines
	clientModule = LoadLibrary(moduleName);

	printf("modH: 0x%p\n", clientModule);

	SetupHandler procAddr = (SetupHandler)GetProcAddress(clientModule, "setup");
	printf("proc Address: 0x%p\n", procAddr);

	if (procAddr != NULL) {
		setSetupRoutine(procAddr);
	}

	LoopHandler loopAddr = (LoopHandler)GetProcAddress(clientModule, "draw");
	printf("loop Addr: 0x%p\n", loopAddr);

	if (loopAddr != NULL) {
		setLoopRoutine(loopAddr);
	}

	if ((procAddr == nullptr) && (loopAddr == nullptr))
	{
		return false;
	}

	gClock = dproc_clock_new();

	return true;
}

//
// Internal Win32 specificconstruction
//
HDC CreateOffscreenDC(HWND hWnd, const size_t width, const size_t height, void **data)
{
	gbmWidth = width;
	gbmHeight = height;

	BITMAPINFO info;
	memset(&info, 0, sizeof(BITMAPINFO));

	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -(int)height;	// top-down DIB Section
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;


	gbmHandle = ::CreateDIBSection(
		NULL,
		&info,
		DIB_RGB_COLORS,
		data,
		NULL,
		0);

	// Create offscreen memory DC for DIB Section rendering
	HDC hdc = ::CreateCompatibleDC(NULL);
	::SelectObject(hdc, gbmHandle);

	return hdc;
}

void * GetPixelBuffer(const int width, const int height)
{
	void *data;


	ghMemDC = CreateOffscreenDC(ghWnd, width, height, &data);

	return data;
}



void CreateWindowHandle(int lwidth, int lheight)
{
	UINT style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	HMODULE hInst = ::GetModuleHandleA(NULL);

	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = style;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);;				//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;	// (HBRUSH)GetStockObject(WHITE_BRUSH);		//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hIconSm = NULL;			//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));

	ATOM winclassatom = ::RegisterClassExA(&wcex);



	DWORD dwExStyle = 0;
	DWORD winstyle = WS_OVERLAPPEDWINDOW;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;
	HWND hWndParent = NULL;
	HMENU hMenu = NULL;

	if (!winclassatom) {
		return ;
	}

	RECT clientRECT = { 0, 0, lwidth, lheight };
	BOOL err = AdjustWindowRect(&clientRECT, WS_CAPTION, 0);

	// The following sequence of messages will come in through the callback
	// function before this function returns:
	// WM_GETMINMAXINFO
	// WM_NCCREATE
	// WM_NCCALCSIZE
	// WM_CREATE

	ghWnd = ::CreateWindowExA(
		0,
		CLASS_NAME,
		szTitle,
		winstyle,
		x, y, clientRECT.right - clientRECT.left, clientRECT.bottom - clientRECT.top,
		hWndParent,
		hMenu,
		hInst,
		NULL);
}

void dpOpenScreen()
{
	PSUBDRIVER subdriver = nullptr;

	// Create offscreen bitmap
	gPixelData = GetPixelBuffer(width, height);	// open the window

												// Build up the screendevice
	scrdev.xvirtres = width;
	scrdev.yvirtres = height;

	scrdev.xres = scrdev.xvirtres;
	scrdev.yres = scrdev.yvirtres;
	scrdev.planes = 1;
	scrdev.pixtype = DPPIXEL_FORMAT;
#if (DPPIXEL_FORMAT == DPPF_TRUECOLOR8888) || (DPPIXEL_FORMAT == DPPF_TRUECOLORABGR)
	scrdev.bpp = 32;
#elif (DPPIXEL_FORMAT == DPPF_TRUECOLOR888)
	scrdev.bpp = 24;
#elif (DPPIXEL_FORMAT == DPPF_TRUECOLOR565) || (DPPIXEL_FORMAT == DPPF_TRUECOLOR555)
	scrdev.bpp = 16;
#else
#error "No support bpp < 16"
#endif 
	/* set standard data format from bpp and pixtype*/
	//scrdev.data_format = set_data_format(&scrdev);

	/* Calculate size and pitch*/
	GdCalcMemGCAlloc(&scrdev, scrdev.xres, scrdev.yres, scrdev.planes, scrdev.bpp,
		&scrdev.size, &scrdev.pitch);

	//if ((scrdev.addr = (uint8_t *)malloc(scrdev.size)) == NULL)
	//	break;
	scrdev.addr = (uint8_t *)gPixelData;

	scrdev.ncolors = scrdev.bpp >= 24 ? (1 << 24) : (1 << scrdev.bpp);
	scrdev.flags = PSF_SCREEN | PSF_ADDRMALLOC;
	scrdev.portrait = DPPORTRAIT_NONE;

	// select an fb subdriver matching our planes and bpp for backing store
	subdriver = select_fb_subdriver(&scrdev);
	if (!subdriver)
		return ;

	// set subdriver into screen driver
	set_subdriver(&scrdev, subdriver);

	// from devopen
	GdSetMode(DPROP_COPY);
	GdSetFillMode(DPFILL_SOLID);  /* Set the fill mode to solid */

	GdSetForegroundColor(&scrdev, DPRGB(255, 255, 255));	/* WHITE*/
	GdSetBackgroundColor(&scrdev, DPRGB(0, 0, 0));		/* BLACK*/
	GdSetUseBackground(true);

	/* select first builtin font (usually MWFONT_SYSTEM_VAR)*/
	//GdSetFont(GdCreateFont(&scrdev, nullptr, 0, 0, nullptr));

	GdSetDash(0, 0);  /* No dashing to start */
	GdSetStippleBitmap(0, 0, 0);  /* No stipple to start */

	GdSetClipRegion(&scrdev, GdAllocRectRegion(0, 0, width, height));
}

void * SetWindowSize(const int width, const int height)
{

	CreateWindowHandle(width, height);

	dpOpenScreen();


	// Display the window on the screen
	ShowWindow(ghWnd, SW_SHOW);
	UpdateWindow(ghWnd);

	return gPixelData;
}



LRESULT OnPaint(HDC hdc, PAINTSTRUCT &ps)
{
	LRESULT ret = 0;

	// bitblt bmhandle to client area
	// we should actually look at the paint struct
	// and only blit the part that needs to be drawn
	//if ((NULL != ghMemDC) && (nullptr != gPixelData)) {
	//	::BitBlt(hdc, 0, 0, gbmWidth, gbmHeight, ghMemDC, 0, 0, SRCCOPY);
	//}

	if ((NULL != ghMemDC) && (nullptr != gPixelData)) {
		::BitBlt(hdc, 
			ps.rcPaint.left, ps.rcPaint.top, 
			ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
			ghMemDC, 
			ps.rcPaint.left, ps.rcPaint.top, 
			SRCCOPY);
	}

	return ret;
}





//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

uint32_t bitops32_extract_range(const uint32_t value, const int offset, const int length)
{
	uint32_t res = value >> offset;
	res = res & ~(1 << length);
	return res;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		case WM_KEYDOWN:
		case WM_KEYUP:{
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

			OnPaint(hdc, ps);

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

void setDrawInLoop(bool doDraw)
{
	gDrawDuringLoop = doDraw;
}



void eventLoop(HWND hWnd)
{
	MSG msg;

	// call draw at least once before we start looping
	if (gLoopRoutine != nullptr) {
		gLoopRoutine();
	}

	forceDraw();

	while (continueRunning)
	{
		// Drain messages out of queue
		while (::PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (gDrawDuringLoop) {
			// This should adhere to a framerate 
			// if one is specified
			if (gLoopRoutine != nullptr) {
				gLoopRoutine();
			}

			// Assume the 'draw()' did something which requires the 
			// screen to be redrawn, so, invalidate the entire client area
			forceDraw();
		}
	}
}


int main(int argc, char **argv)
{
	char *moduleName = nullptr;

	if (argc < 2) {
		moduleName = "nanowindow.dll";
		//return 1;
	}
	else {
		moduleName = argv[1];
	}

	if (!InitializeInstance(moduleName))
	{
		printf("Must specify setup() or draw()\n");
		return 1;
	}

	// Call setup to get the window size
	// any any global variables set
	if (gSetupRoutine != nullptr) {
		gSetupRoutine();
	}

	// Start running the event loop
	eventLoop(ghWnd);

	return 0;
}