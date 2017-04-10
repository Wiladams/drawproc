// terminal.cpp : Defines the exported functions for the DLL application.
//

#include "drawproc.h"
#include "libtsm.h"
#include "console.h"

#include <stdio.h>



Console con(80, 24);

void keyReleased()
{
	switch (keyCode)
	{
	case KC_UP:
		con.scrollBufferUp(1);
		break;
	case KC_DOWN:
		con.scrollBufferDown(1);
		break;

	default:
		break;
	}
	//con.write("keyReleased");
	//con.newline();
}

void draw() {

	// draw the screen
	con.draw();
}


void setup() {
	createCanvas(640, 240);

	con.setMaxScrollback(1024);

	con.writeLine("          1         2         3         4         5         6         7");
	con.write("01234567890123456789012345678901234567890123456789012345678901234567890123456789");
	con.moveTo(0, 2);
	con.write("Hello,");
	con.moveDown(1, true);
	con.writeLine("World!");

	for (int i = 1; i <= 20; i++)
	{
		con.writeLine("line");
	}
}
