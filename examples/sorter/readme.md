This example is primarily about showing a graphic visualization 
of different sort routines.  At the same time, it demonstrates a
method for creating UI applications using some structure.

The AUIApplication class is introduced.  This class implements
the various keyboard, mouse, and draw() routines.  A single instance
of this class must be constructed within the setup() function, as
demonstrated in the sorter.cpp file.

The AView class further implements the mouse and keyboard activities
as well as the drawing routine.  The intention is that you would
create sub-classes of the AView class, and these would be where
you would do your own drawing and reactions to keyboard and mouse.

Your own views are added to the application using the app->AddChild() function.

Here is the sorter.cpp file, which is typical of how you would setup
such an application.

```
#include <stdio.h>

#include "drawproc.h"
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "SortRoutineGraphic.h"
#include "AUIApplication.h"

static const int appWidth = 1024;
static const int appHeight = 768;


AUIApplication * app = nullptr;


void setup()
{
	app = AUIApplication::CreateApplication(0, 0, appWidth, appHeight);

	app->AddChild(new SortRoutineGraphic(new BubbleSort, 0, 0, 320, 240));
	app->AddChild(new SortRoutineGraphic(new SelectionSort, 324, 0, 320, 240));
}
```

Typical UI elements such as buttons, sliders, scrolling views and the like can 
further be implemented atop the AView class.

The AUIApplication class can NOT be a part of the drawproc library itself because
it implements all the methods for keyboard and mouse, which would prevent other
forms of application from being implemented.
