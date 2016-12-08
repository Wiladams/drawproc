
#include <stdio.h>

#include "drawproc.h"
#include "BubbleSort.h"
#include "SelectionSort.h"
#include "SortRoutineGraphic.h"
#include "Aview.h"

static const int appWidth = 1024;
static const int appHeight = 768;

AView gMainView(0, 0, appWidth, appHeight);


void keyTyped()
{
	gMainView.KeyTyped();
}

void keyPressed()
{
	gMainView.KeyPressed();
}

void keyReleased()
{
	gMainView.KeyReleased();
}

void mouseClicked()
{
}

void mouseDragged()
{
}

void mouseMoved()
{
	gMainView.MouseMoved();
}

void mousePressed()
{
	gMainView.MousePressed();
}

void mouseReleased()
{
	gMainView.MouseReleased();
}

void draw()
{
	gMainView.draw();
}

void setup()
{
	size(appWidth,appHeight);

	gMainView.AddChild(new SortRoutineGraphic(new BubbleSort, 0, 0, 320, 240));
	gMainView.AddChild(new SortRoutineGraphic(new SelectionSort, 324, 0, 320, 240));
}
