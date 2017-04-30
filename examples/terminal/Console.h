#pragma once

#include "drawproc.h"

#include "libtsm_int.h"
#include "SymbolTable.h"

#include <stdio.h>

class ScrollbackBuffer;
class ScreenSelection;

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
	struct tsm_screen_attr defaultattr; // = { -1,-1, 255, 255, 255, 0,0,0,0,0,0,0,0 };
	ScrollbackBuffer * scrollBuffer;

	// internal routines
	int _init(const size_t width, const size_t height);
	void moveCursor(unsigned int x, unsigned int y);
	void eraseRegion(unsigned int x_from,
		unsigned int y_from,
		unsigned int x_to,
		unsigned int y_to,
		bool protect);
	void scrollScreenUp(size_t num);
	void scrollScreenDown(size_t num);

public:
	// Construction
	Console(const size_t width, const size_t height);
	~Console();

	void incrementAge();

	void setFlags(unsigned int flags);
	void resetFlags(unsigned int flags);
	unsigned int getFlags() const;


	void setOptions(unsigned int opts)
	{
		if (!opts)
			return;

		screen.opts |= opts;
	}

	void resetOptions(unsigned int opts)
	{
		if (!opts)
			return;

		screen.opts &= ~opts;
	}

	unsigned int getOptions()
	{
		return screen.opts;
	}

	void setDefaultAttribute(const struct tsm_screen_attr & attr);

	void reset();
	int resize(size_t x, size_t y);
	int createNewLine(struct line **out, size_t width);

	// Various screen attributes
	size_t getCursorX() const { return screen.cursor_x; }
	size_t getCursorY() const { return screen.cursor_y; }
	size_t getWidth() const { return screen.size_x; }
	size_t getHeight() const { return screen.size_y; }
	struct line ** getLines() { return screen.lines; }


	// writing text
	void writeSymbolAt(size_t x, size_t y, tsm_symbol_t ch, unsigned int len, const struct tsm_screen_attr *attr);
	void writeSymbol(tsm_symbol_t ch, const struct tsm_screen_attr *attr);
	void write(const char *str);
	void writeLine(const char *str);

	// Erasing parts of screen
	void eraseScreen(bool protect);
	void eraseCursor();
	void eraseChars( size_t num);
	void eraseCursorToEnd( bool protect);
	void eraseHomeToCursor( bool protect);
	void eraseCurrentLine( bool protect);
	void eraseScreenToCursor( bool protect);
	void eraseCursorToScreen( bool protect);
	
	// Deleting and inserting chars and lines
	void deleteChars(size_t num);
	void insertChars(size_t num);
	void deleteLines(size_t num);
	void insertLines(size_t num);


	void moveTo(unsigned int x, unsigned int y);
	void newline();
	void moveDown(size_t num, bool scroll);
	void moveUp(size_t num, bool scroll);
	void moveLeft(size_t num);
	void moveRight(size_t num);
	void moveLineEnd();
	void moveLineHome();


	// Scrolling Control
	void setMaxScrollback(size_t num);
	void scrollBufferUp(size_t num);
	void scrollBufferDown(size_t num);

	void scrollUp(size_t num);
	void scrollDown(size_t num);

	// Margins
	int setMargins(size_t top, size_t bottom);


	// Tabbing
	void setTabstop();
	void resetTabstop();
	void resetAllTabstops();
	void tabLeft(size_t num);
	void tabRight(size_t num);



	// Drawing current state of screen
	tsm_age_t drawScreen(void *data);

	void draw();
};
