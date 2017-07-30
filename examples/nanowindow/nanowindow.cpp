// nanowindow.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

PSD screen;

void loop()
{
	screen->FillRect(screen, 10, 10, 100, 100, RED);

}

void setup()
{
	createCanvas(320, 240);
	background(pWhite);

	screen = GdOpenScreen();
}
