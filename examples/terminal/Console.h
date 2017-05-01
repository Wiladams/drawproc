#pragma once

#include "drawproc.h"

#include "libtsm_int.h"
#include "SymbolTable.h"

#include <stdio.h>


#include "ScrollbackBuffer.h"
#include "ScreenSelection.h"

struct tsm_screen;

struct cell {
	tsm_symbol_t ch;		/* stored character */
	unsigned int width;		/* character width */
	struct tsm_screen_attr attr;	/* cell attributes */
	tsm_age_t age;			/* age of the single cell */



public:
	void init(struct tsm_screen *con);
	cell();
	cell(struct tsm_screen *con);

};

struct line {
	struct line *next;		/* next line (NULL if not sb) */
	struct line *prev;		/* prev line (NULL if not sb) */

	unsigned int size;		/* real width */
	struct cell *cells;		/* actuall cells */
	uint64_t sb_id;			/* sb ID */
	tsm_age_t age;			/* age of the whole line */

public:
	line(struct tsm_screen *con, size_t width);
	~line();
	void initCells(struct tsm_screen &con);
	int resize(struct tsm_screen *con, size_t width);
};

struct tsm_screen {
	size_t ref;
	llog_submit_t llog;
	void *llog_data;
	unsigned int opts;
	unsigned int flags;
	struct SymbolTable sym_table;

	/* default attributes for new cells */
	struct tsm_screen_attr def_attr;

	/* ageing */
	tsm_age_t age_cnt;		/* current age counter */
	unsigned int age_reset : 1;	/* age-overflow flag */

								/* current buffer */
	unsigned int size_x;		/* width of screen */
	unsigned int size_y;		/* height of screen */
	unsigned int margin_top;	/* top-margin index */
	unsigned int margin_bottom;	/* bottom-margin index */
	unsigned int line_num;		/* real number of allocated lines */
	struct line **lines;		/* active lines; copy of main/alt */
	struct line **main_lines;	/* real main lines */
	struct line **alt_lines;	/* real alternative lines */
	tsm_age_t age;			/* whole screen age */

							/* scroll-back buffer */

	unsigned int sb_count;		// number of lines in sb 
	struct line *sb_first;		// first line; was moved first 
	struct line *sb_last;		// last line; was moved last
	unsigned int sb_max;		// max-limit of lines in sb 
	struct line *sb_pos;		// current position in sb or NULL 
	uint64_t sb_last_id;		// last id given to sb-line 


								// cursor: positions are always in-bound, but cursor_x might be
								// bigger than size_x if new-line is pending
	unsigned int cursor_x;		// current cursor x-pos
	unsigned int cursor_y;		// current cursor y-pos

								/* tab ruler */
	bool *tab_ruler;		/* tab-flag for all cells of one row */

							/* selection */
							//bool sel_active;
							//struct selection_pos sel_start;
							//struct selection_pos sel_end;
};

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
	ScrollbackBuffer scrollBuffer;
	ScreenSelection selection;

	// internal routines
	int _init(const size_t width, const size_t height);
	void moveCursor(unsigned int x, unsigned int y);
	void eraseRegion(unsigned int x_from,
		unsigned int y_from,
		unsigned int x_to,
		unsigned int y_to,
		bool protect);
	
	void link_to_scrollback(struct line *aline);
	void scrollScreenUp(size_t num);
	void scrollScreenDown(size_t num);

public:
	// Construction
	Console(const size_t width, const size_t height);
	~Console();

	void incrementAge();
	void alignAge() {	screen.age = screen.age_cnt;}

	struct line ** getLines() { return screen.lines; }
	ScrollbackBuffer *getScrollbackBuffer() { return &scrollBuffer; }
	ScreenSelection & getSelection() { return selection; }

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
