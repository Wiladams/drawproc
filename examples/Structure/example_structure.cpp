#include "drawproc.h"

int currentMode = 1;
int maxMode = 3;

void mousePressed()
{
	currentMode++;
	if (currentMode > maxMode) {
		currentMode = 1;
	}
}

// Function
void drawTarget(float xloc, float yloc, int size, int num) {
	float grayvalues = 255 / num;
	float steps = size / num;
	for (int i = 0; i < num; i++) {
		fillValues(i*grayvalues);
		ellipse(xloc, yloc, size - i*steps, size - i*steps);
	}
}

void drawTargets() {
	backgroundValues(51);
	noStroke();
	drawTarget(width*0.25, height*0.4, 200, 4);
	drawTarget(width*0.5, height*0.5, 300, 10);
	drawTarget(width*0.75, height*0.3, 120, 6);
}

// Recursion
void drawCircle(int x, int radius, int level) {
	float tt = 126 * level / 4.0;
	fillValues(tt);
	ellipse(x, height / 2, radius * 2, radius * 2);
	if (level > 1) {
		level = level - 1;
		drawCircle(x - radius / 2, radius / 2, level);
		drawCircle(x + radius / 2, radius / 2, level);
	}
}



void drawRecursion() {
	noStroke();
	drawCircle(width / 2, 280, 6);
}

// Width and Height
void drawWidthHeight() {
	backgroundValues(127);
	noStroke();
	for (int i = 0; i < height; i += 20) {
		fillValues(129, 206, 15);
		rect(0, i, width, 10);
		fillValues(255);
		rect(i, 0, 10, height);
	}
}

void draw()
{
	switch (currentMode) {
	case 1:
		drawTargets();
		break;

	case 2:
		drawRecursion();
		break;

	case 3:
		drawWidthHeight();
		break;
	}
}

void setup() {
	size(640, 360);
	//noLoop();
}