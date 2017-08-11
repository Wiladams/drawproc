#include "drawproc.h"

int x;
int y;
float outsideRadius = 150;
float insideRadius = 100;
int drawModes = 2;
int currentMode = 1;

void mousePressed()
{
	currentMode = currentMode + 1;
	if (currentMode > drawModes) {
		currentMode = 1;
	}
}

void setup() {
	size(640, 360);
	backgroundValues(204);
	x = width / 2;
	y = height / 2;
}

void drawFan()
{
	int cx = width / 2;
	int cy = height / 2;

	beginShape(GR_TRIANGLE_FAN);
	vertex(cx, cy);
	vertex(cx, cy-100);
	vertex(cx + 100, cy);
	vertex(cx, cy + 100);
	vertex(cx - 100, cy);
	vertex(cx, cy - 100);
	endShape();
}

void drawStrip()
{
	int numPoints = int(map(mouseX, 0, width, 6, 60));
	float angle = 0;
	float angleStep = 180.0 / numPoints;

	beginShape(GR_TRIANGLE_STRIP);
	for (int i = 0; i <= numPoints; i++) {
		float px = x + cos(radians(angle)) * outsideRadius;
		float py = y + sin(radians(angle)) * outsideRadius;
		angle += angleStep;
		vertex(px, py);
		px = x + cos(radians(angle)) * insideRadius;
		py = y + sin(radians(angle)) * insideRadius;
		vertex(px, py);
		angle += angleStep;
	}
	endShape();
}

void draw() {
	backgroundValues(204);

	if (currentMode == 1) {
		drawStrip();
	}
	else if (currentMode == 2) {
		drawFan();
	}
}
