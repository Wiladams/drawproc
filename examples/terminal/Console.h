#pragma once

#include "drawproc.h"
//#include "libtsm.h"
#include "libtsm_int.h"

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
	struct tsm_screen screen;
	struct tsm_screen_attr defaultattr = { -1,-1, 255, 255, 255, 0,0,0,0,0,0,0,0 };

	// internal routines
	int _init();
	void moveCursor(unsigned int x, unsigned int y);
	void eraseRegion(unsigned int x_from,
		unsigned int y_from,
		unsigned int x_to,
		unsigned int y_to,
		bool protect);

public:
	// Construction
	Console(const size_t width, const size_t height);
	~Console();

	void reset();

	// Various screen attributes
	size_t getCursorX() const { return screen.cursor_x; }
	size_t getCursorY() const { return screen.cursor_y; }
	size_t getWidth() const { return screen.size_x; }
	size_t getHeight() const { return screen.size_y; }



	// writing text
	void write(const char *str);
	void writeLine(const char *str);


	
	void moveDown(size_t num, bool scroll);
	void moveTo(unsigned int x, unsigned int y);
	void newline();

	// Scrolling Control
	void setMaxScrollback(size_t num);
	void scrollBufferUp(size_t num);
	void scrollBufferDown(size_t num);

	void scrollUp(size_t num);
	void scrollDown(size_t num);

	// Tabbing
	void setTabstop();
	void resetTabstop();
	void resetAllTabstops();
	void tabLeft(size_t num);
	void tabRight(size_t num);

	// Drawing current state of screen
	void draw();
};
