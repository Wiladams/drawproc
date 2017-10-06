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




void draw()
{
	background(pLightGray);


	for (int i = 1; i <= 1000; i++)
	{
		int x1 = random(width - 1);
		int y1 = random(height - 1);
		int lwidth = random(4, 64);
		int lheight = random(4, 64);
		int r = random(255);
		int g = random(255);
		int b = random(255);

		if (outlineOnly)
		{
			stroke(RGBA(r, g, b, 255));
			noFill();
			GdSetMode(DPROP_COPY);
		}
		else
		{
			fill(RGBA(r, g, b, 127));
			noStroke();
			GdSetMode(DPROP_SRC_OVER);
		}

		rect(x1, y1, lwidth, lheight);
	}
}

void setup()
{
	createCanvas(640, 480);
	background(pLightGray);
}
