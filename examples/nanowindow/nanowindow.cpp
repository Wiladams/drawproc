// nanowindow.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"

PSD screen;

void loop()
{

}

void setup()
{
	//createCanvas(320, 240);
	//background(pWhite);

	// can create a screen, but need
	// to have an event loop to actually run it.

	// better to get the drawproc window to be a PSD
	// and build up from there.
	screen = GdOpenScreen();



}
