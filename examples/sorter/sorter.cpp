#include "drawproc.h"
#include "sortroutine.h"
#include <stdio.h>

int MAX_VALUE = 480;
static const int nElems = 1024;
int Elems[nElems];

bool showLines = false;

BubbleSort sortee(Elems, nElems);

void initElems()
{
	for (int idx = 0; idx < nElems; idx++)
	{
		Elems[idx] = random(MAX_VALUE);
	}
	sortee.reset();
}

void keyTyped()
{
	switch (keyChar)
	{
		case 'l':
			showLines = !showLines;
			break;

		case ' ':
			initElems();
			break;
	}
}

void drawForeground()
{
	char debugstr[256];
	sprintf_s(debugstr, "showLines: %d", (int)showLines);

	text(debugstr, 20, 20);
}

void draw()
{
	background(pWhite);

	fill(0);

	stroke(pBlack);
	strokeWeight(0.5);

	// do a step in the sorter
	sortee.step();

	// Draw the array of values scaled to fit the window
	for (int idx = 0; idx < nElems; idx++) {
		double mappedX = (int)MAP(idx, 0, nElems - 1, 4, width - 4-1);
		double mappedY = (int)MAP(sortee.fElements[idx], 0, MAX_VALUE, height-4-1, 4);

		point(mappedX, mappedY);

		if (showLines) {
			line(4, height - 1, mappedX, mappedY);
		}
	}

	drawForeground();
}

void setup()
{
	initElems();

	size(640, 480);
}


