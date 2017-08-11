#include "drawproc.h"
#include "checkerboard.h"

int appWidth = 640;
int appHeight = 480;

DPPIXELVAL color1 = RGBA(60, 10, 60, 255);
DPPIXELVAL color2 = RGBA(190, 190, 190, 255);

checkerboard cboard(0,0,appWidth,appHeight, 8, 8, color1, color2);


void draw()
{
	// create checkerboard background
	cboard.Draw();

	// Draw some blended rectangles atop the whole
	for (int iter = 1; iter <= 9; iter++) {
		float alpha = MAP(iter, 1, 9, 0, 255);

		noStroke();
		fillValues(0, 255, 255, (int)alpha);
		int offset = iter * 40;
		rect(offset, offset, 100, 100);
	}
}

void setup()
{
	createCanvas(640, 480);
	noLoop();
}
