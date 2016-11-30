#include "drawproc.h"
#include "sortroutine.h"
#include <stdio.h>

int MAX_VALUE = 480;
int nElems = 1024;
int *Elems = nullptr;

bool showLines = false;

BubbleSort *sort1 = new BubbleSort(Elems, nElems);
SelectionSort *sort2 = new SelectionSort(Elems, nElems);

sorter *sortee = sort2;

void initElems()
{
	if (Elems != nullptr)
	{
		free(Elems);
		Elems = nullptr;
	}

	Elems = (int *)malloc(sizeof(int)*nElems);

	for (int idx = 0; idx < nElems; idx++)
	{
		Elems[idx] = random(MAX_VALUE);
	}
	sortee->reset(Elems, nElems);
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

void keyPressed()
{
	switch (keyCode)
	{
	case KC_UP:
		if (nElems < 16384)
			nElems = nElems * 2;
		break;
	
	case KC_DOWN:
		nElems = nElems / 2;
		if (nElems < 128)
			nElems = 128;
		break;

	case KC_RIGHT:
		if (sortee == sort2)
			sortee = sort1;
		else
			sortee = sort2;
		
		break;
	case KC_LEFT:
		if (sortee == sort1)
			sortee = sort2;
		else
			sortee = sort1;

		break;
	}

	initElems();
}

void drawForeground()
{
	char debugstr[256];
	stroke(pBlack);
	setFont(verdana18_bold);

	sprintf_s(debugstr, "%s", sortee->name);
	text(debugstr, 20, 20);

	sprintf_s(debugstr, "Items: %d", nElems);
	text(debugstr, 20, 40);
}

void draw()
{
	//background(pLightGray);
	background(pDarkGray);

	//fill(0);
	fill(255);

	stroke(pWhite);
	strokeWeight(0.5);

	// do a step in the sorter
	sortee->step();

	// Draw the array of values scaled to fit the window
	for (int idx = 0; idx < nElems; idx++) {
		double mappedX = (int)MAP(idx, 0, nElems - 1, 4, width - 4-1);
		double mappedY = (int)MAP(sortee->fElements[idx], 0, MAX_VALUE, height-4-1, 4);

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


