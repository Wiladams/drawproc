
#include <stdio.h>

#include "drawproc.h"
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "SortRoutineGraphic.h"
#include "AUIApplication.h"

static const int appWidth = 640;
static const int appHeight = 480;


AUIApplication * app = nullptr;


void setup()
{
	app = AUIApplication::CreateApplication(0, 0, appWidth, appHeight);

	app->AddChild(new SortRoutineGraphic(new BubbleSort, 0, 0, 320, 240));
	app->AddChild(new SortRoutineGraphic(new SelectionSort, 324, 0, 320, 240));
}
