// lines.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

static const int maxSides = 4;
int nSides = 1;

void setup()
{
	createCanvas(480, 480);
	background(pWhite);
}

void keyReleased()
{
	if (keyCode == KC_SPACE) {
		nSides++;
		if (nSides > maxSides) {
			nSides = 1;
		}
	}
}

void drawMouseTrackLines()
{
	int gap = 4;
	int cntSides = nSides;

	background(pWhite);

	switch (cntSides) {
	case 4:
		// Right Side
		stroke(pBlue);
		for (int cnt = 1; cnt <= 120; cnt++) {
			line(width - 1, (cnt - 1)*gap, mouseX, mouseY);
		}
		cntSides--;

	case 3:
		// Left Side
		stroke(pBlue);
		for (int cnt = 1; cnt <= 120; cnt++) {
			line(0, (cnt - 1)*gap, mouseX, mouseY);
		}
		cntSides--;

	case 2:
		// Bottom
		stroke(pRed);
		for (int cnt = 1; cnt <= 120; cnt++) {
			line((cnt - 1) * gap, height, mouseX, mouseY);
		}
		cntSides--;

	case 1:
		// Top down
		stroke(pRed);
		for (int cnt = 1; cnt <= 120; cnt++) {
			line((cnt - 1) * gap, 0, mouseX, mouseY);
		}
	}
}


void drawMoire()
{
	int gap = 4;

	stroke(pRed);

	for (int i = 1; i <= 120; i++) {
		line(0, i*gap, width - gap*i, 0);
	}
}

void draw()
{
	drawMouseTrackLines();
}

