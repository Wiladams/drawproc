// nanowindow.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "dpdevice.h"
#include "fblin32.h"
#include "genfont.h"

size_t gWidth = 640;
size_t gHeight = 480;

void testHLineBlend()
{
	for (DPCOORD y = 0; y < height; y++) {
		raster_rgba_hline_blend(gpb, 0, y, width, pRed);

	}
}

void testRect()
{
	DPPIXELVAL color1 = cornsilk;
	DPPIXELVAL color2 = pGreen;
	size_t boxwidth = 320;
	size_t boxheight = 240;

	rectMode(CORNER);
	noStroke();
	fill(color1);
	rect(0, 0, boxwidth, boxheight);
	//rect((c*tilewidth) + boxwidth, (r*tileheight) + boxheight-1, boxwidth, boxheight);

	fill(color2);
	rect(320, 0, boxwidth, boxheight);
	//rect(c*tilewidth, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2);

}


void draw()
{
	testHLineBlend();
	testRect();

	//scrdev.DrawHorzLine(&scrdev, 10, 200, 10, RGB2PIXEL8888(125, 255, 127));
	//scrdev.DrawVertLine(&scrdev, 300, 10, 200, RGB2PIXEL8888(64, 127, 255));
	//scrdev.FillRect(&scrdev, 10, 20, 200, 200, RGB2PIXEL8888(125, 255, 255));


	//gen_fonts[0].fontprocs->DrawText((PDPFONT)gen_fonts[0].cfont,
	//		&scrdev, 20, 20,
	//		L"Hello, World!", 1, 0);
}


void setup()
{
	createCanvas(640, 480);
	background(pRed);
	
	GdOpenMouse();
	GdSetMode(DPFILL_SOLID);
//	noLoop();
}
