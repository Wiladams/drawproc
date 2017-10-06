// randomlines.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

void setup()
{
	createCanvas(640, 480);
	background(pWhite);

}

void draw()
{
	background(pLightGray);

	for (int i = 1; i <= 100; i++)
	{
		int x1 = random(width - 1);
		int y1 = random(height - 1);
		int x2 = random(width - 1);
		int y2 = random(height - 1);

		int r = random(255);
		int g = random(255);
		int b = random(255);

		stroke(RGBA(r, g, b, 255));
		fill(RGBA(r, g, b, 255));

		strokeWeight(1.0f);
		line(x1, y1, x2, y2);

		strokeWeight(8.0);
		point(x1, y1);
		point(x2, y2);
	}
}