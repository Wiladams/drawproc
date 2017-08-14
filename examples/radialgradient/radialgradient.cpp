// radialgradient.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

int dim;

void setup() {
	createCanvas(640, 360);
	dim = width / 2;
	colorMode(COLOR_MODE_HSB, 360, 100, 100);
}


void drawGradient(float x, float y) 
{
	int radius = dim / 2;
	float h = ceil(random(0, 360));

	ellipseMode(RADIUS);
	//colorMode(COLOR_MODE_HSB, 360, 100, 100);

	for (int r = radius; r > 0; --r) {
		fillValues(h, 90, 90);
		ellipse(x, y, r, r);
		h = int(h + 1) % 360;
	}
}

void draw() {
	background(0);
	noStroke();

	for (int x = 0; x <= width; x += dim) {
		drawGradient(x, height / 2);
	}
}
