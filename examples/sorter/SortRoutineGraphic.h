#pragma once

#include "sortroutine.h"
#include "drawproc.h"

class SortRoutineGraphic
{
	static const int MAX_VALUE = 480;
	int nElems = 1024;
	int *fElems = nullptr;

	bool fShowLines = false;
	sorter *fSorter;
	pb_rect fFrame;

public:
	SortRoutineGraphic::SortRoutineGraphic(sorter * routine, const int x, const int y, const int gWidth, const int gHeight)
		:fSorter(routine),
		fFrame(x, y, gWidth, gHeight)
	{
		reset();
	}

	virtual void toggleLines() { fShowLines = !fShowLines; };
	virtual void decreaseElements() {
		nElems = nElems / 2;
		if (nElems < 128)
			nElems = 128;
		reset();
	}
	virtual void increaseElements() {
		if (nElems < 16384)
			nElems = nElems * 2;
		reset();
	}

	virtual void step() 
	{
		fSorter->step();
	}

	virtual void reset();
	virtual void draw();
	virtual void drawForeground();
};
