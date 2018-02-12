// randomtriangles.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

bool outlineOnly = true;
int maxsize = 64;

void keyTyped()
{
	switch (keyChar)
	{
	case ' ':
		outlineOnly = !outlineOnly;
		break;

	}
}

void draw()
{
	background(pLightGray);

	if (outlineOnly)
		noFill();

	for (int cnt = 1001; cnt; cnt--)
	{
		uint8_t r = random(255);
		uint8_t g = random(255);
		uint8_t b = random(255);

		//int x1 = random(maxsize, width - 1 - (maxsize*2));
		//int y1 = random(maxsize, height - 1 -(maxsize * 2));
		DPCOORD x1 = randomRange(maxsize, width - 1 - (maxsize * 1));
		DPCOORD y1 = randomRange(maxsize, height - 1 - (maxsize * 1));


		int minx = x1 - maxsize;
		int miny = y1 - maxsize;

		int xrange = (x1 + maxsize) - (x1 - maxsize);
		int yrange = (y1 + maxsize) - (y1 - maxsize);

		int x2 = (rand() % xrange) + minx;
		int y2 = (rand() % yrange) + miny;

		int x3 = (rand() % xrange) + minx;
		int y3 = (rand() % yrange) + miny;

		if (!outlineOnly)
		{
			fillValues(r, g, b, 255);
			noStroke();
		}
		else {
			noFill();
			strokeValues(r, g, b, 255);
		}

		triangle(x1, y1, x2, y2, x3, y3);
	}
}

void setup()
{
	createCanvas(1024, 768);
	background(pBlack);
}
