#pragma once

#include "AView.h"
#include "sortroutine.h"

class SortRoutineGraphic : public AView
{
	static const int MAX_VALUE = 480;
	int nElems = 1024;
	int *fElems = nullptr;

	bool fShowLines = false;
	bool fAnimate = false;
	sorter *fSorter;
	DPCLIPRECT fOrigin;

public:
	SortRoutineGraphic(sorter * routine, const int x, const int y, const int gWidth, const int gHeight)
		:AView(x, y, gWidth, gHeight),
		fSorter(routine),
		fOrigin(x, y, gWidth, gHeight)
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
	virtual void drawBackground();
	virtual void drawForeground();

	virtual bool OnKeyPressed();
	virtual bool OnKeyTyped();

	virtual bool OnMouseEntered();
	virtual bool OnMouseExited();
};
