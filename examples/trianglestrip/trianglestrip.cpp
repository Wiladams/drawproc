#include "drawproc.h"

int cx=0;
int cy=0;
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


void drawFan()
{
	stroke(pWhite);

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

	stroke(pBlack);
	fill(pWhite);
	beginShape(GR_TRIANGLE_STRIP);
	for (int i = 0; i <= numPoints; i++) {
		float px = cx + cos(radians(angle)) * outsideRadius;
		float py = cy + sin(radians(angle)) * outsideRadius;
		angle += angleStep;
		vertex(px, py);
		px = cx + cos(radians(angle)) * insideRadius;
		py = cy + sin(radians(angle)) * insideRadius;
		vertex(px, py);
		angle += angleStep;
	}
	endShape();
}

void draw() {
	background(RGBA(204,204,204,255));

	if (currentMode == 1) {
		drawStrip();
	}
	else if (currentMode == 2) {
		drawFan();
	}
}


void setup() {
	createCanvas(640, 360);

	cx = width / 2;
	cy = height / 2;
}
