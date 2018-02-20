// nanowindow.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "dpdevice.h"
#include "fblin32.h"
#include "genfont.h"
#include "genmem.h"
#include "bezier.h"

size_t gWidth = 640;
size_t gHeight = 480;
DPCLIPREGION * cregion = nullptr;
PDPFONT cfont = nullptr;

void testPoints();
void testHLineBlend();
void testLines();
void testBezier();
void testPixmap();
void testRect();
void testEllipse();
void testPolygon();
void testText();


typedef void(*testcase)();

testcase cases[] = {
	testPoints,
//	testHLineBlend,
	testLines,
	testBezier,
	testRect,
	testEllipse,
	testPixmap,
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
	for (DPCOORD y = 0; y < (DPCOORD)(height/2); y++) {
		raster_rgba_hline_blend(gpb, 0, y, width, pRed);

	}
}

void testPixmap()
{
	// create a pixel map
	int format = DPIF_RGB888;
	//int format = 0;		// screen compatible
	PSD pmd = GdCreatePixmap(&scrdev, width, height, format, nullptr, 0);
	
	// do some drawing into it
	GdSetMode(DPROP_COPY);
	GdSetForegroundPixelVal(pmd,RGBA(127, 220, 220, 255));
	GdFillRect(pmd, 10, 10, 620, 460);

	// image it to the screen
	GdBlit(&scrdev, 0, 0, width, height, pmd, 0, 0, DPROP_COPY);

	// destroy it
	GdFreePixmap(pmd);
}

void testRect()
{
	DPPIXELVAL color1 = cornsilk;
	DPPIXELVAL color2 = pGreen;
	size_t boxwidth = 320;
	size_t boxheight = 240;

	background(RGBA(255, 255, 255, 255));

	GdSetMode(DPROP_SRC_OVER);
	//GdSetMode(DPROP_COPY);
	//GdSetMode(DPROP_SRC);
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

	background(RGBA(204, 204, 204, 255));

	GdFillPoly(&scrdev,5, pts);
}

void testPoints()
{
	background(pBlack);
	stroke(pWhite);
	for (size_t cnt = 1; cnt <= 1000; cnt++)
	{
		DPCOORD x = (DPCOORD)random(width - 1.0f);
		DPCOORD y = (DPCOORD)random(height - 1.0f);
		stroke(RGBA((int)random(255), (int)random(255), (int)random(255), 255));
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

void testBezier()
{
	// Do some control points
	background(pLightGray);

	stroke(pBlack);

	float p0x = 100; float p0y = 10;
	float p1x = 105; float p1y = 200;
	float p2x = 300; float p2y = 200;
	float p3x = 400; float p3y = 10;
	
	float lpx = bezier_cubic_point(0, p0x, p1x, p2x, p3x);
	float lpy = bezier_cubic_point(0, p0y, p1y, p2y, p3y);

	int i = 1;
	while (i <= 60) {
		float u = i / 60.0f;

		float px = bezier_cubic_point(u, p0x, p1x, p2x, p3x);
		float py = bezier_cubic_point(u, p0y, p1y, p2y, p3y);

		line(lpx, lpy, px, py);
		lpx = px;
		lpy = py;

		//point(px, py);
		i++;
	}
	
	// Ellipses at control points
	fill(pRed);
	ellipseMode(RADIUS);
	ellipse(p0x,p0y, 2, 2);
	ellipse(p3x, p3y, 2, 2);

	fill(pBlue);
	ellipse(p1x, p1y, 2, 2);
	ellipse(p2x, p2y, 2, 2);

	// Draw lines between control points for visualization
	stroke(pYellow);
	line(p0x, p0y, p1x, p1y);
	line(p1x, p1y, p2x, p2y);
	line(p2x, p2y, p3x, p3y);
}


void testText()
{
	DPCOORD twidth = 0;
	DPCOORD theight = 0;
	//DPCOORD pbase;

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
	float rx = 20;
	float ry = 10;

	GdSetMode(DPROP_SRC_OVER);

	ellipseMode(RADIUS);

	for (size_t cnt = 1; cnt <= 100; cnt++) {
		double cx = randomRange(10.0f, width-10.0f);
		double cy = randomRange(10.0f, height-10.0f);
		int r = (int)random(255);
		int g = (int)random(255);
		int b = (int)random(255);
		int a = (int)randomRange(64, 200);


		noStroke();
		stroke(RGBA(r, g, b, a));
		fill(RGBA(r, g, b, a));
		ellipse(cx, cy, rx, ry);
	}

	// red
	fill(RGBA(255, 0, 0, 140));
	ellipse(270, 140, 100, 100);

	// green
	fill(RGBA(0, 255, 0, 140));
	ellipse(380, 140, 100, 100);

	// blue
	fill(RGBA(0, 0, 255, 140));
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
