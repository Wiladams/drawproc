#include "drawproc.h"

void setup() {
	size(640, 360);
	noStroke();
	rectMode(CENTER);
}

void draw() {
	backgroundValues(51);
	fillValues(255, 0,0, 118);
	rect(mouseX, height / 2, mouseY / 2 + 10, mouseY / 2 + 10);
	fillValues(0,0,255, 118);
	int inverseX = width - mouseX;
	int inverseY = height - mouseY;
	rect(inverseX, height / 2, (inverseY / 2) + 10, (inverseY / 2) + 10);
}
