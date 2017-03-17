// sliders.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

static const int maxSlides = 10;
int currentSlide;

void setup()
{
	createCanvas(640, 480);
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