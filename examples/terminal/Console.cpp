#include "drawproc.h"
#include "libtsm.h"
#include "Console.h"
#include "ScrollbackBuffer.h"
#include "ScreenSelection.h"

#include <stdio.h>


void cell::init(struct tsm_screen *con)
{
	ch = 0;
	width = 1;
	age = con->age_cnt;
	memcpy(&attr, &con->def_attr, sizeof(attr));
}

cell::cell()
{
	ch = 0;
	width = 1;
	age = 0;
}

cell::cell(struct tsm_screen *con)
{
	init(con);
}

line::line(struct tsm_screen *con, size_t width)
{
	next = nullptr;
	prev = nullptr;
	size = width;
	this->age = con->age_cnt;

	cells = (cell *)malloc(sizeof(struct cell) * width);

	initCells(*con);
}


line::~line()
{
	free(cells);
}

void line::initCells(struct tsm_screen &con)
{
	for (int i = 0; i < size; i++) {
		cells[i].init(&con);
	}
}

int line::resize(struct tsm_screen *con, size_t width)
{
	struct cell *tmp;

	if (!width)
		return -EINVAL;

	if (size < width) {
		tmp = (cell *)realloc(cells, width * sizeof(struct cell));
		if (!tmp)
			return -ENOMEM;

		cells = tmp;

		while (size < width) {
			cells[size].init(con);
			++size;
		}
	}

	return 0;
}



static inline unsigned int to_abs_x(struct tsm_screen &con, unsigned int x)
{
	return x;
}

static inline unsigned int to_abs_y(struct tsm_screen &con, unsigned int y)
{
	if (!(con.flags & TSM_SCREEN_REL_ORIGIN))
		return y;

	return con.margin_top + y;
}


static int draw_cb(struct tsm_screen *con,
		uint32_t id,
		const uint32_t *ch,
		size_t len,
		unsigned int width,
		unsigned int posx,
		unsigned int posy,
		const struct tsm_screen_attr *attr,
		tsm_age_t age,
		void *data) {

		char str[255];
		sprintf_s(str, "%c", *ch);
		text(str, posx * 8, posy * 10);

	return 0;
}

int Console::_init(const size_t width, const size_t height)
{
	int ret;
	unsigned int i;

	memset(&screen, 0, sizeof(screen));
	screen.ref = 1;
	//screen.llog = log;
	//screen.llog_data = log_data;
	screen.age_cnt = 1;
	screen.age = screen.age_cnt;
	screen.def_attr.fr = 255;
	screen.def_attr.fg = 255;
	screen.def_attr.fb = 255;


	ret = resize(width, height);
	if (ret)
		goto err_free;

	return 0;

err_free:
	for (i = 0; i < screen.line_num; ++i) {
		//line_free(screen.main_lines[i]);
		//line_free(screen.alt_lines[i]);

		delete screen.main_lines[i];
		delete screen.alt_lines[i];

	}
	free(screen.main_lines);
	free(screen.alt_lines);
	free(screen.tab_ruler);

	return ret;
}

Console::Console(const size_t width, const size_t height) 
	:selection(*this),
	scrollBuffer(*this, height)
{
		int err = _init(width, height);
}

Console::~Console()
{
	unsigned int i;

	//llog_debug(con, "destroying screen");

	for (int i = 0; i < screen.line_num; ++i) 
	{
		//line_free(screen.main_lines[i]);
		//line_free(screen.alt_lines[i]);

		delete screen.main_lines[i];
		delete screen.alt_lines[i];
	}
	free(screen.main_lines);
	free(screen.alt_lines);
	free(screen.tab_ruler);
}

void Console::incrementAge()
{
	if (!++screen.age_cnt) {
		screen.age_reset = 1;
		++screen.age_cnt;
	}
}


void Console::setDefaultAttribute(const struct tsm_screen_attr & attr)
{
	screen.def_attr = attr;
}

static struct cell *get_cursor_cell(tsm_screen &con)
{
	unsigned int cur_x, cur_y;

	cur_x = con.cursor_x;
	if (cur_x >= con.size_x)
		cur_x = con.size_x - 1;

	cur_y = con.cursor_y;
	if (cur_y >= con.size_y)
		cur_y = con.size_y - 1;

	return &con.lines[cur_y]->cells[cur_x];
}

void Console::setFlags(unsigned int flags)
{
	unsigned int old;
	struct cell *c;

	if (!flags)
		return;

	incrementAge();

	old = screen.flags;
	screen.flags |= flags;

	if (!(old & TSM_SCREEN_ALTERNATE) && (flags & TSM_SCREEN_ALTERNATE)) {
		screen.age = screen.age_cnt;
		screen.lines = screen.alt_lines;
	}

	if (!(old & TSM_SCREEN_HIDE_CURSOR) &&
		(flags & TSM_SCREEN_HIDE_CURSOR)) {
		c = get_cursor_cell(screen);
		c->age = screen.age_cnt;
	}

	if (!(old & TSM_SCREEN_INVERSE) && (flags & TSM_SCREEN_INVERSE))
		screen.age = screen.age_cnt;
}

void Console::resetFlags(unsigned int flags)
{
	unsigned int old;
	struct cell *c;

	if (!flags)
		return;

	incrementAge();

	old = screen.flags;
	screen.flags &= ~flags;

	if ((old & TSM_SCREEN_ALTERNATE) && (flags & TSM_SCREEN_ALTERNATE)) {
		screen.age = screen.age_cnt;
		screen.lines = screen.main_lines;
	}

	if ((old & TSM_SCREEN_HIDE_CURSOR) &&
		(flags & TSM_SCREEN_HIDE_CURSOR)) {
		c = get_cursor_cell(screen);
		c->age = screen.age_cnt;
	}

	if ((old & TSM_SCREEN_INVERSE) && (flags & TSM_SCREEN_INVERSE))
		screen.age = screen.age_cnt;
}


unsigned int Console::getFlags() const
{
	return screen.flags;
}

int Console::createNewLine(struct line **out, size_t width)
{
	struct line *aline;
	unsigned int i;

	if (!width)
		return -EINVAL;

	aline = new struct line(&screen, width);

	if (!line)
		return -ENOMEM;

	*out = aline;

	return 0;
}

int Console::resize(size_t x, size_t y)
{
	struct tsm_screen *con = &screen;
	struct line **cache;
	unsigned int i, j, width, diff, start;
	int ret;
	bool *tab_ruler;

	if (!x || !y)
		return -EINVAL;

	if (screen.size_x == x && screen.size_y == y)
		return 0;

	/* First make sure the line buffer is big enough for our new screen.
	* That is, allocate all new lines and make sure each line has enough
	* cells to hold the new screen or the current screen. If we fail, we
	* can safely return -ENOMEM and the buffer is still valid. We must
	* allocate the new lines to at least the same size as the current
	* lines. Otherwise, if this function fails in later turns, we will have
	* invalid lines in the buffer. */
	if (y > screen.line_num) {
		// resize main buffer
		cache = (struct line **)realloc(screen.main_lines, sizeof(struct line*) * y);
		if (!cache)
			return -ENOMEM;

		if (screen.lines == screen.main_lines)
			screen.lines = cache;
		screen.main_lines = cache;

		/* resize alt buffer */
		cache = (struct line **)realloc(screen.alt_lines, sizeof(struct line*) * y);
		if (!cache)
			return -ENOMEM;

		if (screen.lines == screen.alt_lines)
			screen.lines = cache;
		screen.alt_lines = cache;

		/* allocate new lines */
		if (x > screen.size_x)
			width = x;
		else
			width = screen.size_x;

		while (screen.line_num < y) {
			ret = createNewLine(&screen.main_lines[screen.line_num], width);
			if (ret)
				return ret;

			ret = createNewLine(&screen.alt_lines[screen.line_num], width);
			if (ret) {
				delete screen.main_lines[screen.line_num];
				return ret;
			}

			++screen.line_num;
		}
	}

	/* Resize all lines in the buffer if we increase screen width. This
	* will guarantee that all lines are big enough so we can resize the
	* buffer without reallocating them later. */
	if (x > screen.size_x) {
		tab_ruler = (bool *)realloc(screen.tab_ruler, sizeof(bool) * x);
		if (!tab_ruler)
			return -ENOMEM;
		screen.tab_ruler = tab_ruler;

		for (i = 0; i < screen.line_num; ++i) {
			ret = screen.main_lines[i]->resize(con, x);
			if (ret)
				return ret;

			ret = screen.alt_lines[i]->resize(con, x);
			if (ret)
				return ret;
		}
	}

	incrementAge();

	/* clear expansion/padding area */
	start = x;
	if (x > screen.size_x)
		start = screen.size_x;
	for (j = 0; j < screen.line_num; ++j) {
		/* main-lines may go into SB, so clear all cells */
		i = 0;
		if (j < screen.size_y)
			i = start;

		for (; i < screen.main_lines[j]->size; ++i) {
			screen.main_lines[j]->cells[i].init(con);
		}

		/* alt-lines never go into SB, only clear visible cells */
		i = 0;
		if (j < screen.size_y)
			i = screen.size_x;

		for (; i < x; ++i) {
			screen.alt_lines[j]->cells[i].init(con);
		}
	}

	/* xterm destroys margins on resize, so do we */
	screen.margin_top = 0;
	screen.margin_bottom = screen.size_y - 1;

	/* reset tabs */
	for (i = 0; i < x; ++i) {
		if (i % 8 == 0)
			screen.tab_ruler[i] = true;
		else
			screen.tab_ruler[i] = false;
	}

	/* We need to adjust x-size first as screen_scroll_up() and friends may
	* have to reallocate lines. The y-size is adjusted after them to avoid
	* missing lines when shrinking y-size.
	* We need to carefully look for the functions that we call here as they
	* have stronger invariants as when called normally. */

	screen.size_x = x;
	if (screen.cursor_x >= screen.size_x)
		moveCursor(screen.size_x - 1, screen.cursor_y);

	/* scroll buffer if screen height shrinks */
	if (y < screen.size_y) {
		diff = screen.size_y - y;
		scrollScreenUp(diff);
		if (screen.cursor_y > diff)
			moveCursor(screen.cursor_x, screen.cursor_y - diff);
		else
			moveCursor(screen.cursor_x, 0);
	}

	screen.size_y = y;
	screen.margin_bottom = screen.size_y - 1;
	if (screen.cursor_y >= screen.size_y)
		moveCursor(screen.cursor_x, screen.size_y - 1);

	return 0;
}

void Console::newline()
{
	incrementAge();

	moveDown(1, true);
	moveLineHome();
}

void Console::writeSymbolAt(size_t x, size_t y, tsm_symbol_t ch, unsigned int len, const struct tsm_screen_attr *attr)
{
	struct line *line;
	unsigned int i;

	if (!len)
		return;

	if (x >= screen.size_x || y >= screen.size_y) {
		//llog_warning(con, "writing beyond buffer boundary");
		return;
	}

	line = screen.lines[y];

	if ((screen.flags & TSM_SCREEN_INSERT_MODE) &&
		(int)x < ((int)screen.size_x - len)) {
		line->age = screen.age_cnt;
		memmove(&line->cells[x + len], &line->cells[x],
			sizeof(struct cell) * (screen.size_x - len - x));
	}

	line->cells[x].age = screen.age_cnt;
	line->cells[x].ch = ch;
	line->cells[x].width = len;
	memcpy(&line->cells[x].attr, attr, sizeof(*attr));

	for (i = 1; i < len && i + x < screen.size_x; ++i) {
		line->cells[x + i].age = screen.age_cnt;
		line->cells[x + i].width = 0;
	}
}

/*
	writeSymbol(ch, attr)

	write the given symbol at the current screen cursor location
*/
void Console::writeSymbol(tsm_symbol_t ch, const struct tsm_screen_attr *attr)
{
	unsigned int last, len;

	len = screen.sym_table.getWidth(ch);
	if (!len)
		return;

	incrementAge();

	if (screen.cursor_y <= screen.margin_bottom ||
		screen.cursor_y >= screen.size_y)
		last = screen.margin_bottom;
	else
		last = screen.size_y - 1;

	if (screen.cursor_x >= screen.size_x) {
		if (screen.flags & TSM_SCREEN_AUTO_WRAP)
			moveCursor(0, screen.cursor_y + 1);
		else
			moveCursor(screen.size_x - 1, screen.cursor_y);
	}

	if (screen.cursor_y > last) {
		moveCursor(screen.cursor_x, last);
		scrollUp(1);
	}

	writeSymbolAt(screen.cursor_x, screen.cursor_y, ch, len, attr);
	moveCursor(screen.cursor_x + len, screen.cursor_y);
}

void Console::write(const char *str) 
{
	int idx = 0;
	while (str[idx] != 0) 
	{
		writeSymbol(str[idx], &defaultattr);

		idx = idx + 1;
	}
}

void Console::writeLine(const char *str)
{
	write(str);
	newline();
}



void Console::moveCursor(unsigned int x, unsigned int y)
{
	struct cell *c;

	// if cursor is hidden, just move it
	if (screen.flags & TSM_SCREEN_HIDE_CURSOR) {
		screen.cursor_x = x;
		screen.cursor_y = y;
		return;
	}

	/* If cursor is visible, we have to mark the current and the new cell
	* as changed by resetting their age. We skip it if the cursor-position
	* didn't actually change. */

	if (screen.cursor_x == x && screen.cursor_y == y)
		return;

	c = get_cursor_cell(screen);
	c->age = screen.age_cnt;

	screen.cursor_x = x;
	screen.cursor_y = y;

	c = get_cursor_cell(screen);
	c->age = screen.age_cnt;
}

void Console::moveTo(unsigned int x, unsigned int y)
{
	unsigned int last;


	incrementAge();

	if (screen.flags & TSM_SCREEN_REL_ORIGIN)
		last = screen.margin_bottom;
	else
		last = screen.size_y - 1;

	x = to_abs_x(screen, x);
	if (x >= getWidth())
		x = screen.size_x - 1;

	y = to_abs_y(screen, y);
	if (y > last)
		y = last;

	moveCursor(x, y);
}

void Console::moveUp(size_t num, bool scroll)
{
	unsigned int diff, size;

	if (!num)
		return;

	incrementAge();

	if (screen.cursor_y >= screen.margin_top)
		size = screen.margin_top;
	else
		size = 0;

	diff = screen.cursor_y - size;
	if (num > diff) {
		num -= diff;
		if (scroll)
			scrollDown(num);
		moveCursor(screen.cursor_x, size);
	}
	else {
		moveCursor(screen.cursor_x, screen.cursor_y - num);
	}
}

void Console::moveDown(size_t num, bool scroll)
{
	unsigned int diff, size;

	incrementAge();

	if (screen.cursor_y <= screen.margin_bottom)
		size = screen.margin_bottom + 1;
	else
		size = screen.size_y;

	diff = size - screen.cursor_y - 1;
	if (num > diff) {
		num -= diff;
		if (scroll)
			scrollUp(num);
		moveCursor(screen.cursor_x, size - 1);
	}
	else {
		moveCursor(screen.cursor_x, screen.cursor_y + num);
	}
}



void Console::moveLeft(size_t num)
{
	unsigned int x;

	if (!num)
		return;

	incrementAge();

	if (num > screen.size_x)
		num = screen.size_x;

	x = screen.cursor_x;
	if (x >= screen.size_x)
		x = screen.size_x - 1;

	if (num > x)
		moveCursor(0, screen.cursor_y);
	else
		moveCursor(x - num, screen.cursor_y);
}


void Console::moveRight(size_t num)
{
	if (!num)
		return;

	incrementAge();

	if (num > screen.size_x)
		num = screen.size_x;

	if (num + screen.cursor_x >= screen.size_x)
		moveCursor(screen.size_x - 1, screen.cursor_y);
	else
		moveCursor(screen.cursor_x + num, screen.cursor_y);
}


void Console::moveLineEnd()
{
	incrementAge();

	moveCursor(screen.size_x - 1, screen.cursor_y);
}


void Console::moveLineHome()
{
	incrementAge();

	moveCursor(0, screen.cursor_y);
}


//
// Scroll Buffer routines
//

void Console::setMaxScrollback(size_t num)
{
	scrollBuffer.setMax(num);
}

void Console::scrollBufferUp(size_t num)
{
	scrollBuffer.up(num);
}

void Console::scrollBufferDown(size_t num)
{
	scrollBuffer.down(num);
}

// 
// Screen Scrolling, without scroll buffer
//
/* This links the given line into the scrollback-buffer */
void Console::link_to_scrollback(struct line *aline)
{
	struct line *tmp;

	alignAge();

	if (scrollBuffer.getMax() == 0) {
		if (selection.isActive()) {
			if (selection.getStart().line == aline) {
				selection.getStart().line = nullptr;
				selection.getStart().y = SELECTION_TOP;
			}
			if (selection.getEnd().line == aline) {
				selection.getEnd().line = nullptr;
				selection.getEnd().y = SELECTION_TOP;
			}
		}
		delete aline;

		return;
	}

	/* Remove a line from the scrollback buffer if it reaches its maximum.
	* We must take care to correctly keep the current position as the new
	* line is linked in after we remove the top-most line here.
	* sb_max == 0 is tested earlier so we can assume sb_max > 0 here. In
	* other words, buf->sb_first is a valid line if sb_count >= sb_max. */
	if (scrollBuffer.getCount() >= scrollBuffer.getMax()) {
		tmp = scrollBuffer.getFirst();
		scrollBuffer.setFirst(tmp->next);
		if (tmp->next)
			tmp->next->prev = nullptr;
		else
			scrollBuffer.setLast(nullptr);

		scrollBuffer.decrementCount();


		/* (position == tmp && !next) means we have sb_max=1 so set
		* position to the new line. Otherwise, set to new first line.
		* If position!=tmp and we have a fixed-position then nothing
		* needs to be done because we can stay at the same line. If we
		* have no fixed-position, we need to set the position to the
		* next inserted line, which can be "line", too. */
		if (scrollBuffer.getPosition()) {
			if (scrollBuffer.getPosition() == tmp ||
				!(screen.flags & TSM_SCREEN_FIXED_POS)) {
				if (scrollBuffer.getPosition()->next)
					scrollBuffer.setPosition(scrollBuffer.getPosition()->next);
				else
					scrollBuffer.setPosition(aline);
			}
		}

		if (selection.isActive()) {
			if (selection.getStart().line == tmp) {
				selection.getStart().line = nullptr;
				selection.getStart().y = SELECTION_TOP;
			}
			if (selection.getEnd().line == tmp) {
				selection.getEnd().line = nullptr;
				selection.getEnd().y = SELECTION_TOP;
			}
		}
		delete tmp;
	}

	aline->sb_id = scrollBuffer.incrementLastId();
	aline->next = nullptr;
	aline->prev = scrollBuffer.getLast();
	if (scrollBuffer.getLast())
		scrollBuffer.getLast()->next = aline;
	else
		scrollBuffer.setFirst(aline);
	scrollBuffer.setLast(aline);

	scrollBuffer.incrementCount();
}

void Console::scrollScreenUp(size_t num)
{
	size_t i, j, max, pos;
	int ret;

	if (!num)
		return;

	// TODO: more sophisticated ageing 
	screen.age = screen.age_cnt;

	max = screen.margin_bottom + 1 - screen.margin_top;
	if (num > max)
		num = max;

	// We cache lines on the stack to speed up the scrolling. However, if
	// num is too big we might get overflows here so use recursion if num
	// exceeds a hard-coded limit.
	// 128 seems to be a sane limit that should never be reached but should
	// also be small enough so we do not get stack overflows.
	if (num > 128) {
		scrollUp(128);
		return scrollUp(num - 128);
	}
	struct line **cache = (struct line **)malloc(num * sizeof(struct line *));

	for (i = 0; i < num; ++i) {
		pos = screen.margin_top + i;
		if (!(screen.flags & TSM_SCREEN_ALTERNATE))
			ret = createNewLine(&cache[i], screen.size_x);
		else
			ret = -EAGAIN;

		if (!ret) {
			link_to_scrollback(screen.lines[pos]);
		} else {
			cache[i] = screen.lines[pos];
			for (j = 0; j < screen.size_x; ++j) {
				cache[i]->cells[j].init(&screen);
			}
		}
	}

	if (num < max) {
		memmove(&screen.lines[screen.margin_top],
			&screen.lines[screen.margin_top + num],
			(max - num) * sizeof(struct line*));
	}

	memcpy(&screen.lines[screen.margin_top + (max - num)],
		cache, num * sizeof(struct line*));

	if (selection.isActive()) {
		if (!selection.getStart().line && selection.getStart().y >= 0) {
			selection.getStart().y -= num;
			if (selection.getStart().y < 0) {
				selection.getStart().line = screen.sb_last;
				while (selection.getStart().line && ++selection.getStart().y < 0)
					selection.getStart().line = selection.getStart().line->prev;
				selection.getStart().y = SELECTION_TOP;
			}
		}
		if (!selection.getEnd().line && selection.getEnd().y >= 0) {
			selection.getEnd().y -= num;
			if (selection.getEnd().y < 0) {
				selection.getEnd().line = screen.sb_last;
				while (selection.getEnd().line && ++selection.getEnd().y < 0)
					selection.getEnd().line = selection.getEnd().line->prev;
				selection.getEnd().y = SELECTION_TOP;
			}
		}
	}

	free(cache);
}


void Console::scrollScreenDown(size_t num)
{
unsigned int i, j, max;

	if (!num)
		return;

	// TODO: more sophisticated ageing
	screen.age = screen.age_cnt;

	max = screen.margin_bottom + 1 - screen.margin_top;
	if (num > max)
		num = max;

	// see scrollScreenUp() for an explanation
	if (num > 128) {	
		scrollScreenDown(128);
		return scrollScreenDown(num - 128);
	}

	struct line **cache = (struct line **)malloc(num * sizeof(struct line *));

	for (i = 0; i < num; ++i) {
		cache[i] = screen.lines[screen.margin_bottom - i];
		for (j = 0; j < screen.size_x; ++j) {
			cache[i]->cells[j].init(&screen);
		}
	}

	if (num < max) {
		memmove(&screen.lines[screen.margin_top + num],
			&screen.lines[screen.margin_top],
			(max - num) * sizeof(struct line*));
	}

	memcpy(&screen.lines[screen.margin_top], cache, num * sizeof(struct line*));

	if (selection.isActive()) {
		if (!selection.getStart().line && selection.getStart().y >= 0)
			selection.incrementYStart(num);
		
		if (!selection.getEnd().line && selection.getEnd().y >= 0)
			selection.incrementYEnd(num);
	}

	free(cache);
}

void Console::scrollUp(size_t num)
{
	if (!num)
		return;

	incrementAge();

	scrollScreenUp(num);
}

void Console::scrollDown(size_t num)
{
	if (!num)
		return;

	incrementAge();

	scrollScreenDown(num);
}


int Console::setMargins(size_t top, size_t bottom)
{
	unsigned int upper, lower;

	if (!top)
		top = 1;

	if (bottom <= top) {
		upper = 0;
		lower = screen.size_y - 1;
	}
	else if (bottom > screen.size_y) {
		upper = 0;
		lower = screen.size_y - 1;
	}
	else {
		upper = top - 1;
		lower = bottom - 1;
	}

	screen.margin_top = upper;
	screen.margin_bottom = lower;

	return 0;
}

void Console::setTabstop()
{
	if (screen.cursor_x >= screen.size_x)
		return;

	screen.tab_ruler[screen.cursor_x] = true;
}

void Console::resetTabstop()
{
	if (screen.cursor_x >= screen.size_x)
		return;

	screen.tab_ruler[screen.cursor_x] = false;
}

void Console::resetAllTabstops()
{
	unsigned int i;


	for (i = 0; i < screen.size_x; ++i)
		screen.tab_ruler[i] = false;
}


void Console::tabRight(size_t num)
{
	unsigned int i, j, x;

	if (!num)
		return;

	incrementAge();

	x = screen.cursor_x;
	for (i = 0; i < num; ++i) {
		for (j = x + 1; j < screen.size_x; ++j) {
			if (screen.tab_ruler[j])
				break;
		}

		x = j;
		if (x + 1 >= screen.size_x)
			break;
	}

	/* tabs never cause pending new-lines */
	if (x >= screen.size_x)
		x = screen.size_x - 1;

	moveCursor(x, screen.cursor_y);
}


void Console::tabLeft(size_t num)
{
	unsigned int i, x;
	int j;

	if (!num)
		return;

	incrementAge();

	x = screen.cursor_x;
	for (i = 0; i < num; ++i) {
		for (j = x - 1; j > 0; --j) {
			if (screen.tab_ruler[j])
				break;
		}

		if (j <= 0) {
			x = 0;
			break;
		}
		x = j;
	}

	moveCursor(x, screen.cursor_y);
}

void Console::eraseRegion(unsigned int x_from,
	unsigned int y_from,
	unsigned int x_to,
	unsigned int y_to,
	bool protect)
{
	unsigned int to;
	struct line *line;

	/* TODO: more sophisticated ageing */
	screen.age = screen.age_cnt;

	if (y_to >= screen.size_y)
		y_to = screen.size_y - 1;
	if (x_to >= screen.size_x)
		x_to = screen.size_x - 1;

	for (; y_from <= y_to; ++y_from) {
		line = screen.lines[y_from];
		if (!line) {
			x_from = 0;
			continue;
		}

		if (y_from == y_to)
			to = x_to;
		else
			to = screen.size_x - 1;
		for (; x_from <= to; ++x_from) {
			if (protect && line->cells[x_from].attr.protect)
				continue;

			//screen_cell_init(&screen, &line->cells[x_from]);
			line->cells[x_from].init(&screen);
		}
		x_from = 0;
	}
}

// Erasing
void Console::eraseScreen(bool protect)
{
	incrementAge();

	eraseRegion(0, 0, screen.size_x - 1, screen.size_y - 1, protect);
}


void Console::eraseCursor()
{
	unsigned int x;

	incrementAge();

	if (screen.cursor_x >= screen.size_x)
		x = screen.size_x - 1;
	else
		x = screen.cursor_x;

	eraseRegion(x, screen.cursor_y, x, screen.cursor_y, false);
}


void Console::eraseChars(size_t num)
{
	unsigned int x;

	if (!num)
		return;

	incrementAge();

	if (screen.cursor_x >= screen.size_x)
		x = screen.size_x - 1;
	else
		x = screen.cursor_x;

	eraseRegion(x, screen.cursor_y, x + num - 1, screen.cursor_y, false);
}


void Console::eraseCursorToEnd(bool protect)
{
	unsigned int x;

	incrementAge();

	if (screen.cursor_x >= screen.size_x)
		x = screen.size_x - 1;
	else
		x = screen.cursor_x;

	eraseRegion(x, screen.cursor_y, screen.size_x - 1, screen.cursor_y, protect);
}


void Console::eraseHomeToCursor(bool protect)
{
	incrementAge();

	eraseRegion(0, screen.cursor_y, screen.cursor_x,
		screen.cursor_y, protect);
}


void Console::eraseCurrentLine(bool protect)
{
	incrementAge();

	eraseRegion(0, screen.cursor_y, screen.size_x - 1,
		screen.cursor_y, protect);
}


void Console::eraseScreenToCursor(bool protect)
{
	incrementAge();

	eraseRegion(0, 0, screen.cursor_x, screen.cursor_y, protect);
}


void Console::eraseCursorToScreen(bool protect)
{
	unsigned int x;

	incrementAge();

	if (screen.cursor_x >= screen.size_x)
		x = screen.size_x - 1;
	else
		x = screen.cursor_x;

	eraseRegion(x, screen.cursor_y, screen.size_x - 1,
		screen.size_y - 1, protect);
}



void Console::insertLines(size_t num)
{
	unsigned int i, j, max;

	if (!num)
		return;

	if (screen.cursor_y < screen.margin_top ||
		screen.cursor_y > screen.margin_bottom)
		return;

	incrementAge();
	// TODO: more sophisticated ageing
	screen.age = screen.age_cnt;

	max = screen.margin_bottom - screen.cursor_y + 1;
	if (num > max)
		num = max;

	struct line **cache = (struct line **)malloc(num * sizeof(struct line *));

	for (i = 0; i < num; ++i) {
		cache[i] = screen.lines[screen.margin_bottom - i];
		for (j = 0; j < screen.size_x; ++j) {
			cache[i]->cells[j].init(&screen);
		}
	}

	if (num < max) {
		memmove(&screen.lines[screen.cursor_y + num],
			&screen.lines[screen.cursor_y],
			(max - num) * sizeof(struct line*));

		memcpy(&screen.lines[screen.cursor_y],
			cache, num * sizeof(struct line*));
	}

	screen.cursor_x = 0;
	free(cache);
}


void Console::deleteLines(size_t num)
{
	unsigned int i, j, max;

	if (!num)
		return;

	if (screen.cursor_y < screen.margin_top ||
		screen.cursor_y > screen.margin_bottom)
		return;

	incrementAge();
	// TODO: more sophisticated ageing
	screen.age = screen.age_cnt;

	max = screen.margin_bottom - screen.cursor_y + 1;
	if (num > max)
		num = max;

	struct line **cache = (struct line **)malloc(num * sizeof(struct line *));

	for (i = 0; i < num; ++i) {
		cache[i] = screen.lines[screen.cursor_y + i];
		for (j = 0; j < screen.size_x; ++j) {
			//screen_cell_init(con, &cache[i]->cells[j]);
			cache[i]->cells[j].init(&screen);
		}
	}

	if (num < max) {
		memmove(&screen.lines[screen.cursor_y],
			&screen.lines[screen.cursor_y + num],
			(max - num) * sizeof(struct line*));

		memcpy(&screen.lines[screen.cursor_y + (max - num)],
			cache, num * sizeof(struct line*));
	}

	screen.cursor_x = 0;
	free(cache);
}

void Console::insertChars(size_t num)
{
	struct cell *cells;
	unsigned int max, mv, i;

	if (!num || !screen.size_y || !screen.size_x)
		return;

	incrementAge();
	// TODO: more sophisticated ageing */
	screen.age = screen.age_cnt;

	if (screen.cursor_x >= screen.size_x)
		screen.cursor_x = screen.size_x - 1;
	if (screen.cursor_y >= screen.size_y)
		screen.cursor_y = screen.size_y - 1;

	max = screen.size_x - screen.cursor_x;
	if (num > max)
		num = max;
	mv = max - num;

	cells = screen.lines[screen.cursor_y]->cells;
	if (mv)
		memmove(&cells[screen.cursor_x + num],
			&cells[screen.cursor_x],
			mv * sizeof(*cells));

	for (i = 0; i < num; ++i) {
		cells[screen.cursor_x + i].init(&screen);
	}
}


void Console::deleteChars(size_t num)
{
	struct cell *cells;
	unsigned int max, mv, i;

	if (!num || !screen.size_y || !screen.size_x)
		return;

	incrementAge();
	/* TODO: more sophisticated ageing */
	screen.age = screen.age_cnt;

	if (screen.cursor_x >= screen.size_x)
		screen.cursor_x = screen.size_x - 1;
	if (screen.cursor_y >= screen.size_y)
		screen.cursor_y = screen.size_y - 1;

	max = screen.size_x - screen.cursor_x;
	if (num > max)
		num = max;
	mv = max - num;

	cells = screen.lines[screen.cursor_y]->cells;
	if (mv)
		memmove(&cells[screen.cursor_x],
			&cells[screen.cursor_x + num],
			mv * sizeof(*cells));

	for (i = 0; i < num; ++i) {
		cells[screen.cursor_x + mv + i].init(&screen);
	}
}


void Console::reset()
{
	unsigned int i;

	incrementAge();
	screen.age = screen.age_cnt;

	screen.flags = 0;
	screen.margin_top = 0;
	screen.margin_bottom = screen.size_y - 1;
	screen.lines = screen.main_lines;

	// set tabs every 8 spaces by default
	for (i = 0; i < screen.size_x; ++i) {
		if (i % 8 == 0)
			screen.tab_ruler[i] = true;
		else
			screen.tab_ruler[i] = false;
	}
}



/*
	drawScreen()

	Take all the data that is in the console buffer and display
	it on the screen.
*/
tsm_age_t Console::drawScreen(void *data)
{
	unsigned int cur_x, cur_y;
	unsigned int i, j, k;
	struct line *iter, *line = NULL;
	struct cell *cell, empty;
	struct tsm_screen_attr attr;
	int ret = 0, warned = 0;
	const uint32_t *ch;
	size_t len;
	bool in_sel = false, sel_start = false, sel_end = false;
	bool was_sel = false;
	tsm_age_t age;



	//screen_cell_init(&screen, &empty);
	empty.init(&screen);

	cur_x = screen.cursor_x;
	if (screen.cursor_x >= screen.size_x)
		cur_x = screen.size_x - 1;
	cur_y = screen.cursor_y;
	if (screen.cursor_y >= screen.size_y)
		cur_y = screen.size_y - 1;

	/* push each character into rendering pipeline */

	iter = screen.sb_pos;
	k = 0;

	if (selection.isActive()) {
		if (!selection.getStart().line && selection.getStart().y == SELECTION_TOP)
			in_sel = !in_sel;
		if (!selection.getEnd().line && selection.getEnd().y == SELECTION_TOP)
			in_sel = !in_sel;

		if (selection.getStart().line &&
			(!iter || selection.getStart().line->sb_id < iter->sb_id))
			in_sel = !in_sel;
		if (selection.getEnd().line &&
			(!iter || selection.getEnd().line->sb_id < iter->sb_id))
			in_sel = !in_sel;
	}

	for (i = 0; i < screen.size_y; ++i) {
		if (iter) {
			line = iter;
			iter = iter->next;
		}
		else {
			line = screen.lines[k];
			k++;
		}

		if (selection.isActive()) {
			if (selection.getStart().line == line ||
				(!selection.getStart().line &&
					selection.getStart().y == k - 1))
				sel_start = true;
			else
				sel_start = false;
			if (selection.getEnd().line == line ||
				(!selection.getEnd().line &&
					selection.getEnd().y == k - 1))
				sel_end = true;
			else
				sel_end = false;

			was_sel = false;
		}

		for (j = 0; j < screen.size_x; ++j) {
			if (j < line->size)
				cell = &line->cells[j];
			else
				cell = &empty;

			memcpy(&attr, &cell->attr, sizeof(attr));

			if (selection.isActive()) {
				if (sel_start &&
					j == selection.getStart().x) {
					was_sel = in_sel;
					in_sel = !in_sel;
				}
				if (sel_end &&
					j == selection.getEnd().x) {
					was_sel = in_sel;
					in_sel = !in_sel;
				}
			}

			if (k == cur_y + 1 && j == cur_x &&
				!(screen.flags & TSM_SCREEN_HIDE_CURSOR))
				attr.inverse = !attr.inverse;

			// TODO: do some more sophisticated inverse here. When
			// INVERSE mode is set, we should instead just select
			// inverse colors instead of switching background and
			// foreground
			if (screen.flags & TSM_SCREEN_INVERSE)
				attr.inverse = !attr.inverse;

			if (in_sel || was_sel) {
				was_sel = false;
				attr.inverse = !attr.inverse;
			}

			if (screen.age_reset) {
				age = 0;
			}
			else {
				age = cell->age;
				if (line->age > age)
					age = line->age;
				if (screen.age > age)
					age = screen.age;
			}

			ch = screen.sym_table.get(&cell->ch, &len);
			if (cell->ch == ' ' || cell->ch == 0)
				len = 0;

			ret = draw_cb(&screen, cell->ch, ch, len, cell->width,
				j, i, &attr, age, data);
			if (ret && warned++ < 3) {
				//llog_debug(con,"cannot draw glyph at %ux%u via text-renderer",j, i);
				//if (warned == 3)
					//llog_debug(con,"suppressing further warnings during this rendering round");
			}
		}
	}

	if (screen.age_reset) {
		screen.age_reset = 0;
		return 0;
	}
	else {
		return screen.age_cnt;
	}
}


void Console::draw() 
{
	background(0);

	setFont(mcs6x10_mono);
	drawScreen(nullptr);
}
