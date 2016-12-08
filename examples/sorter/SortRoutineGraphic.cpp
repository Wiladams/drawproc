#include "SortRoutineGraphic.h"
#include "drawproc.h"

void SortRoutineGraphic::reset()
{
	if (fElems != nullptr)
	{
		free(fElems);
		fElems = nullptr;
	}

	fElems = (int *)malloc(sizeof(int)*nElems);

	for (int idx = 0; idx < nElems; idx++)
	{
		fElems[idx] = random(MAX_VALUE);
	}
	fSorter->sort(fElems, nElems);
}

void SortRoutineGraphic::drawForeground()
{
	char debugstr[256];
	stroke(pBlack);
	setFont(verdana18_bold);

	sprintf_s(debugstr, "%s", fSorter->name.c_str());
	text(debugstr, fFrame.x+20, fFrame.y+20);

	sprintf_s(debugstr, "Items: %d", nElems);
	text(debugstr, fFrame.x+20, fFrame.y+40);
}

void SortRoutineGraphic::draw()
{
	fill(pDarkGray);
	rect(fFrame.x, fFrame.y, fFrame.width, fFrame.height);

	fill(255);

		stroke(pWhite);
		strokeWeight(0.5);

		if (fAnimate) {
			// do a step in the sorter
			if (!fSorter->step()) {
				reset();
			}
		}

		// Draw the array of values scaled to fit the window
		for (int idx = 0; idx < nElems; idx++) {
			double mappedX = (int)MAP(idx, 0, nElems - 1, fFrame.x, fFrame.x + fFrame.width - 1);
			double mappedY = (int)MAP(fSorter->fElements[idx], 0, MAX_VALUE, fFrame.y + fFrame.height - 1, fFrame.y);

			point(mappedX, mappedY);

			if (fShowLines) {
				line(fFrame.x, fFrame.y + fFrame.height - 1, mappedX, mappedY);
			}
		
	}

	drawForeground();
}

bool SortRoutineGraphic::OnKeyPressed()
{
	switch (keyCode)
	{
	case KC_UP:
		increaseElements();
	break;

	case KC_DOWN:
		decreaseElements();
	break;
	}

	return true;
}

bool SortRoutineGraphic::OnKeyTyped()
{
	switch (keyChar)
	{
	case 'l':
		toggleLines();
		break;

	case ' ':
		reset();
		break;
	}

	return true;
}

bool SortRoutineGraphic::OnMouseEntered()
{
	fAnimate = true;
	return true;
}

bool SortRoutineGraphic::OnMouseExited()
{
	fAnimate = false;
	return true;
}