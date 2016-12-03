
#include <stdio.h>
#include <vector>

#include "drawproc.h"
#include "sortroutine.h"


int MAX_VALUE = 480;
int nElems = 1024;
int *Elems = nullptr;

bool showLines = false;

std::vector<sorter *> arrayOfSorts;

int sorterIndex = 0;

void initElems()
{
	arrayOfSorts.push_back(new BubbleSort(Elems, nElems));
	arrayOfSorts.push_back(new SelectionSort(Elems, nElems));

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
	arrayOfSorts[sorterIndex]->reset(Elems, nElems);
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
		if (sorterIndex == arrayOfSorts.size() - 1)
			sorterIndex = 0;
		else
			sorterIndex = sorterIndex + 1;
		
		break;
	case KC_LEFT:
		if (sorterIndex == 0)
			sorterIndex = arrayOfSorts.size()-1;
		else
			sorterIndex = sorterIndex - 1;

		break;
	}

	initElems();
}

void drawForeground()
{
	char debugstr[256];
	stroke(pBlack);
	setFont(verdana18_bold);

	sprintf_s(debugstr, "%s", arrayOfSorts[sorterIndex]->name);
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
	arrayOfSorts[sorterIndex]->step();

	// Draw the array of values scaled to fit the window
	for (int idx = 0; idx < nElems; idx++) {
		double mappedX = (int)MAP(idx, 0, nElems - 1, 4, width - 4-1);
		double mappedY = (int)MAP(arrayOfSorts[sorterIndex]->fElements[idx], 0, MAX_VALUE, height-4-1, 4);

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
