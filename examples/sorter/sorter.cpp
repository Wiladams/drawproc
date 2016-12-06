
#include <stdio.h>
#include <vector>

#include "drawproc.h"
#include "sortroutine.h"
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "SortRoutineGraphic.h"

std::vector<SortRoutineGraphic *> arrayOfSorts;
SortRoutineGraphic *srg = nullptr;
SortRoutineGraphic *srg2 = nullptr;

BubbleSort * bsorter = new BubbleSort;

void keyTyped()
{
	switch (keyChar)
	{
		case 'l':
			srg->toggleLines();
			srg2->toggleLines();
			break;

		case ' ':
			srg->reset();
			srg2->reset();
			break;
	}
}

void keyPressed()
{
	switch (keyCode)
	{
	case KC_UP:
		srg->increaseElements();
		srg2->increaseElements();
		break;
	
	case KC_DOWN:
		srg->decreaseElements();
		srg2->decreaseElements();
		break;
	/*
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
	*/
	}
}

void draw()
{
	background(pLightGray);
	
	srg->draw();
	srg->step();

	srg2->draw();
	srg2->step();
}

void setup()
{
	size(1024, 768);

	srg = new SortRoutineGraphic(new BubbleSort, 0, 0, 320, 240);
	srg2 = new SortRoutineGraphic(new SelectionSort, 324, 0, 320, 240);

}
