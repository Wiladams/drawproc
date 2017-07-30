#pragma once

#include <SDKDDKVer.h>

//#define VC_EXTRALEAN 1
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <tchar.h>
#include <windows.h>


#include "dproc_config.h"



#ifdef __cplusplus
extern "C" {
#endif

HMODULE getClientModule();


// These can be called by client code
void * GetPixelBuffer(const int width, const int height);
void * SetWindowSize(const int width, const int height);

LRESULT quit();
DPROC_API double seconds();
void resettime();

void setDrawInLoop(bool);
void forceDraw();

// Callback functions
typedef void(*LoopHandler)();
typedef void(*SetupHandler)();
typedef void(*CallToHandler)(); 


// These can be called by the client, but not
// strictly necessary
SetupHandler setSetupRoutine(SetupHandler handler);
CallToHandler setLoopRoutine(LoopHandler handler);




typedef LRESULT(CALLBACK* WinProcHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK* KeyboardHandler)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT(CALLBACK*MouseHandler)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// These should be called by the client 
void setKeyboardHandler(KeyboardHandler handler);
void setMouseHandler(MouseHandler handler);
#ifdef __cplusplus
}
#endif
