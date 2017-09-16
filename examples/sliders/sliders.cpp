// sliders.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "genmem.h"

static const int maxSlides = 10;
int currentSlide;
PSD pmp = nullptr;
DPCOORD gWidth = 640;
DPCOORD gHeight = 480;

void draw()
{
	GdBlit(&scrdev, 10, 10, gWidth/2, gHeight/2,
		pmp, 0, 0, 0);
}

void setup()
{
	int format = 32;

	createCanvas(640, 480);

	pmp = GdCreatePixmap(&scrdev, gWidth / 2, gHeight / 2, format, nullptr, 0);
	GdSetClipRegion(pmp, GdAllocRectRegion(0, 0, 320, 240));
	fill(RGBA(255, 255, 0, 255));	// yellow
	stroke(RGBA(255, 255, 0, 255));
	GdRect(pmp, 0, 0, gWidth/2, gHeight/2);
}

void keyReleased()
{
	switch (keyCode) {
	case KC_SPACE:
		currentSlide = currentSlide + 1;
		if (currentSlide >= maxSlides) {
			currentSlide = 0;
		}
		break;
	}
}