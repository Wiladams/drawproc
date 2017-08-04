// nanowindow.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "dpdevice.h"
#include "fblin32.h"


void draw()
{
	scrdev.DrawHorzLine(&scrdev, 10, 200, 10, RGB2PIXEL8888(125, 255, 127));
	scrdev.DrawVertLine(&scrdev, 300, 10, 200, RGB2PIXEL8888(64, 127, 255));
	scrdev.FillRect(&scrdev, 10, 20, 200, 200, RGB2PIXEL8888(125, 255, 255));

	//GdFillRect(&scrdev, 10, 20, 200, 200);
	//GdRect(&scrdev, 10, 20, 200, 200);
}


void setup()
{
	createCanvas(640, 480);
	background(pRed);
	
	GdOpenMouse();
	GdSetMode(DPFILL_SOLID);
//	noLoop();
}
