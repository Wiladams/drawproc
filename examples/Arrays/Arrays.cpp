#include "drawproc.h"

static const int wide = 640;
static const int high = 360;

int currentMode = 1;
int maxMode = 3;

// Array
float * coswave;

// Array 2D
float distances[wide][high];
float maxDistance;
int spacer;

// Array Object


struct Module {
	int xOffset;
	int yOffset;
	float x, y;
	int unit;
	int xDirection = 1;
	int yDirection = 1;
	float speed;

	// Contructor
	Module(){};
	Module(int xOffsetTemp, int yOffsetTemp, int xTemp, int yTemp, float speedTemp, int tempUnit) {
		xOffset = xOffsetTemp;
		yOffset = yOffsetTemp;
		x = xTemp;
		y = yTemp;
		speed = speedTemp;
		unit = tempUnit;
	}

	// Custom method for updating the variables
	void update() 
	{
		x = x + (speed * xDirection);
		if (x >= unit || x <= 0) {
			xDirection *= -1;
			x = x + (1 * xDirection);
			y = y + (1 * yDirection);
		}
		if (y >= unit || y <= 0) {
			yDirection *= -1;
			y = y + (1 * yDirection);
		}
	}

	// Custom method for drawing the object
	void display() 
	{
		fillValues(255);
		ellipse(xOffset + x, yOffset + y, 6, 6);
	}
};

int unit = 40;
int count;
Module *mods;


void mousePressed()
{
	currentMode++;
	if (currentMode > maxMode) 
	{
		currentMode = 1;
	}
}




void setup() {
	size(wide, high);
	coswave = new float[width];
	for (int i = 0; i < width; i++) {
		float amount = map(i, 0, width, 0, G_PI);
		coswave[i] = fabs(cos(amount));
	}

	// Array 2D
	maxDistance = dist(width / 2, height / 2, width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float distance = dist(width / 2, height / 2, x, y);
			distances[x][y] = distance / maxDistance * 255;
		}
	}
	spacer = 10;
	strokeWeight(6);

	// Array Objects
	int wideCount = width / unit;
	int highCount = height / unit;
	count = wideCount * highCount;
	mods = new Module[count];

	int index = 0;
	for (int y = 0; y < highCount; y++) {
		for (int x = 0; x < wideCount; x++) {
			mods[index++] = Module(x*unit, y*unit, unit / 2, unit / 2, random(0.05, 0.8), unit);
		}
	}

}

// Array
void drawArray() 
{
	background(255);

	int y1 = 0;
	int y2 = height / 3;
	for (int i = 0; i < width; i++) {
		strokeValues(coswave[i] * 255);
		line(i, y1, i, y2);
	}

	y1 = y2;
	y2 = y1 + y1;
	for (int i = 0; i < width; i++) {
		strokeValues (coswave[i] * 255 / 4);
		line(i, y1, i, y2);
	}

	y1 = y2;
	y2 = height;
	for (int i = 0; i < width; i++) {
		strokeValues(255 - coswave[i] * 255);
		line(i, y1, i, y2);
	}

}

// Array 2D
void drawArray2D()
{
	background(0);

	for (int y = 0; y < height; y += spacer) {
		for (int x = 0; x < width; x += spacer) {
			strokeValues(distances[x][y]);
			point(x + spacer / 2, y + spacer / 2);
		}
	}
}

// Array Objects
void drawModules() 
{
	backgroundValues(0);
	for (int idx = 0; idx < count; idx++) 
	{
		mods[idx].update();
		mods[idx].display();
	}
}



void draw()
{
	switch (currentMode)
	{
	case 1:
		drawArray();
		break;

	case 2:
		drawArray2D();
		break;

	case 3:
		drawModules();
		break;
	}
}