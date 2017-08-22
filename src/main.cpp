

#include "animwin32.h"
#include "dproc_clock.h"
#include "dpdevice.h"
#include "dp_win32.h"
#include "fb.h"
#include "genmem.h"
#include "genfont.h"
#include "global_state.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#ifdef _MSC_VER
// This is here so that we can simply use a 'main()' 
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#define MAX_LOADSTRING 100

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	myWindowProc(HWND, UINT, WPARAM, LPARAM);

// Global Windows Variables:

HMODULE clientModule;
extern HWND winRootWindow;



// offscreen bitmap
bool continueRunning = true;
bool gDrawDuringLoop = true;



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
	InvalidateRect(winRootWindow, 0, TRUE);
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

bool GsInitialize(const size_t lwidth, const size_t lheight)
{
	PSD		psd;

	// OPEN KEYBOARD

	// OPEN SCREEN
	psd = GdOpenScreen();
	if (nullptr == psd)
		return false;

	//GdSetPortraitMode(psd, portraitmode);

	// OPEN MOUSE
	GdOpenMouse();

	//stdfont = GdCreateFont(psd, DPFONT_SYSTEM_VAR, 0, 0, NULL);

	// Initialize root Window

	return true;
}

void * SetWindowSize(const size_t lwidth, const size_t lheight)
{
	width = lwidth;
	height = lheight;

	GsInitialize(lwidth, lheight);

	return scrdev.addr;
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
	eventLoop(winRootWindow);

	return 0;
}