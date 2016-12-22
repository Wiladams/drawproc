// randomlines.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

bool outlineOnly = true;

void keyTyped()
{
	switch (keyChar)
	{
	case ' ':
		outlineOnly = !outlineOnly;
		break;

	}
}

void setup()
{
	createCanvas(640, 480);
	background(pWhite);
}

void draw()
{
	background(pWhite);

	if (outlineOnly) {
		noFill();
	} else
	{
		fill(pWhite);
	}

	for (int i = 1; i <= 1000; i++)
	{
		int x1 = random(width - 1-64);
		int y1 = random(height - 1-64);
		int lwidth = random(4, 64);
		int lheight = random(4,64);

		int r = random(255);
		int g = random(255);
		int b = random(255);

		if (!outlineOnly)
		{
			fill(r, g, b, 255);
			stroke(pBlack);
		}
		else
		{
			stroke(r, g, b, 255);
		}

		rect(x1, y1, lwidth, lheight);
	}
}