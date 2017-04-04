#pragma once
#include "drawproc.h"
#include "libtsm.h"
#include <stdio.h>

/*
	Console

	A class encapsulating the concept and rendering of a generic console.
	This class has the simple writing functions necessary to put symbols
	on the screen in a structured manner.

	Construction of a more complex virtual terminal requires parsing input
	and interpreting the commands, turning them into console instructions.

	This console object assumes it is drawing into the general drawproc window,
	so it makes use of the global drawproc drawing calls, such as 'text()'.
*/

class Console {
	size_t Width;
	size_t Height;
	struct tsm_screen *screen = nullptr;
	struct tsm_screen_attr defaultattr = { -1,-1, 255, 255, 255, 0,0,0,0,0,0,0,0 };


public:
	// Construction
	Console(const size_t width, const size_t height);

	void write(const char *str);
	
	// Cursor control
	void getCursor(unsigned int &x, unsigned int &y);
	
	void moveDown(size_t num, bool scroll);
	void moveTo(unsigned int x, unsigned int y);
	void newline();

	// Screen Control
	void setMaxScrollback(size_t num);
	void scrollBufferUp(size_t num);
	void scrollBufferDown(size_t num);

	void scrollUp(size_t num);
	void scrollDown(size_t num);

	// Drawing current state of screen
	void draw();
};
