// nanowindow.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "dpdevice.h"
#include "fblin32.h"
#include "genfont.h"

size_t gWidth = 640;
size_t gHeight = 480;
DPCLIPREGION * cregion = nullptr;
PDPFONT cfont = nullptr;

void testPoints();
void testHLineBlend();
void testLines();
void testRect();
void testEllipse();
void testPolygon();
void testText();


typedef void(*testcase)();

testcase cases[] = {
	testPoints,
	testHLineBlend,
	testLines,
	testRect,
	testEllipse,
	testPolygon,
	testText
};
size_t nCases = sizeof(cases)/ sizeof(testcase);
size_t currentCase = 0;


void mouseReleased()
{
	currentCase = currentCase + 1;
	if (currentCase >= nCases)
		currentCase = 0;
}

void testHLineBlend()
{
	for (DPCOORD y = 0; y < height/2; y++) {
		raster_rgba_hline_blend(gpb, 0, y, width, pRed);

	}
}

void testRect()
{
	DPPIXELVAL color1 = cornsilk;
	DPPIXELVAL color2 = pGreen;
	size_t boxwidth = 320;
	size_t boxheight = 240;

	//GdSetMode(DPROP_SRC_OVER);
	GdSetMode(DPROP_COPY);
	GdSetMode(DPROP_SRC);
	rectMode(CORNER);
	noStroke();
	fill(RGBA(255, 0, 0, 127));

	rect(20, 10, boxwidth, boxheight);
	//rect((c*tilewidth) + boxwidth, (r*tileheight) + boxheight-1, boxwidth, boxheight);

	fill(RGBA(0,255, 0, 127));
	rect(320-20, 10, boxwidth, boxheight);
	//rect(c*tilewidth, (r*tileheight) + tileheight / 2, tilewidth / 2, tileheight / 2);

}

void testPolygon()
{
	DPPOINT pts[] = {
		10,10,
		100,23,
		120,43,
		100,63,
		10,200
	};

	GdFillPoly(&scrdev,5, pts);
}

void testPoints()
{
	background(pBlack);
	stroke(pWhite);
	for (size_t cnt = 1; cnt <= 1000; cnt++)
	{
		DPCOORD x = random(width - 1);
		DPCOORD y = random(height - 1);
		point(x, y);
	}

}

void testLines()
{
	background(pBlack);
	scrdev.DrawHorzLine(&scrdev, 10, 200, 10, RGB2PIXEL8888(125, 255, 127));
	scrdev.DrawVertLine(&scrdev, 300, 10, 200, RGB2PIXEL8888(64, 127, 255));
	scrdev.FillRect(&scrdev, 10, 20, 200, 200, RGB2PIXEL8888(125, 255, 255));
}

void testText()
{
	DPCOORD twidth = 0;
	DPCOORD theight = 0;
	DPCOORD pbase;

	cfont = GdCreateFont(&scrdev, DPFONT_SYSTEM_VAR, 0, 0, NULL);

	//GdGetTextSize((PDPFONT)(gen_fonts[0].cfont), "Hello, World", 0, &twidth, &theight, &pbase,0);

	//gen_fonts[0].fontprocs->DrawText((PDPFONT)(gen_fonts[0].cfont),
	//		&scrdev, 20, 20,
	//		L"Hello, World!", 1, 0);
	backgroundValues(0, 0, 0, 0);
	stroke(pRed);
	fill(pRed);

	GdSetFontRotation(cfont, 15*10);
	GdSetFontSize(cfont, 64, 32);
	GdText(&scrdev, cfont, 100, 100, "Hello, World!", -1, 0);

	GdDestroyFont(cfont);
}

void testEllipse()
{
	DPCOORD rx = 20;
	DPCOORD ry = 10;

	GdSetMode(DPROP_SRC_OVER);

	ellipseMode(RADIUS);

	for (size_t cnt = 1; cnt <= 1000; cnt++) {
		DPCOORD cx = (DPCOORD)random(10, width-10);
		DPCOORD cy = (DPCOORD)random(10, height-10);
		int r = random(255);
		int g = random(255);
		int b = random(255);
		int a = random(64, 200);


		noStroke();
		stroke(RGBA(r, g, b, a));
		fill(RGBA(r, g, b, a));
		ellipse(cx, cy, rx, ry);
	}

	// red
	stroke(RGBA(255, 0, 0, 140));
	ellipse(270, 140, 100, 100);

	// green
	stroke(RGBA(0, 255, 0, 140));
	ellipse(380, 140, 100, 100);

	// blue
	stroke(RGBA(0, 0, 255, 140));
	ellipse(320, 240, 100, 100);

}

void draw()
{
	background(RGBA(204,204,204,255));
	cases[currentCase]();
}


void setup()
{
	createCanvas(640, 480);
}
