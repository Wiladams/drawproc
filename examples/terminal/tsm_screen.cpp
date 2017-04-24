/*
 * libtsm - Screen Management
 *
 * Copyright (c) 2011-2013 David Herrmann <dh.herrmann@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Screen Management
 * This provides the abstracted screen management. It does not do any
 * terminal-emulation, instead it provides a resizable table of cells. You can
 * insert, remove and modify the cells freely.
 * A screen has always a fixed, but changeable, width and height. This defines
 * the number of columns and rows. The screen doesn't care for pixels, glyphs or
 * framebuffers. The screen only contains information about each cell.
 *
 * Screens are the logical model behind a real screen of a terminal emulator.
 * Users usually allocate a screen for each terminal-emulator they run. All they
 * have to do is render the screen onto their widget on each change and forward
 * any widget-events to the screen.
 *
 * The screen object already includes scrollback-buffers, selection support and
 * more. This simplifies terminal emulators a lot, but also prevents them from
 * accessing the real screen data. However, terminal emulators should have no
 * reason to access the data directly. The screen API should provide everything
 * they need.
 *
 * AGEING:
 * Each cell, line and screen has an "age" field. This field describes when it
 * was changed the last time. After drawing a screen, the current screen age is
 * returned. This allows users to skip drawing specific cells, if their
 * framebuffer was already drawn with a newer age than a given cell.
 * However, the screen-age might overflow. This is properly detected and causes
 * drawing functions to return "0" as age. Users must reset all their
 * framebuffer ages then. Otherwise, further drawing operations might
 * incorrectly skip cells.
 * Furthermore, if a cell has age "0", it means it _has_ to be drawn. No ageing
 * information is available.
 */

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "libtsm.h"
#include "libtsm_int.h"
#include "shl_llog.h"

#define LLOG_SUBSYSTEM "tsm_screen"

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


/* This links the given line into the scrollback-buffer */
void link_to_scrollback(struct tsm_screen *con, struct line *line)
{
	struct line *tmp;

	/* TODO: more sophisticated ageing */
	con->age = con->age_cnt;

	if (con->sb_max == 0) {
		if (con->sel_active) {
			if (con->sel_start.line == line) {
				con->sel_start.line = NULL;
				con->sel_start.y = SELECTION_TOP;
			}
			if (con->sel_end.line == line) {
				con->sel_end.line = NULL;
				con->sel_end.y = SELECTION_TOP;
			}
		}
		delete line;

		return;
	}

	/* Remove a line from the scrollback buffer if it reaches its maximum.
	 * We must take care to correctly keep the current position as the new
	 * line is linked in after we remove the top-most line here.
	 * sb_max == 0 is tested earlier so we can assume sb_max > 0 here. In
	 * other words, buf->sb_first is a valid line if sb_count >= sb_max. */
	if (con->sb_count >= con->sb_max) {
		tmp = con->sb_first;
		con->sb_first = tmp->next;
		if (tmp->next)
			tmp->next->prev = NULL;
		else
			con->sb_last = NULL;
		--con->sb_count;

		/* (position == tmp && !next) means we have sb_max=1 so set
		 * position to the new line. Otherwise, set to new first line.
		 * If position!=tmp and we have a fixed-position then nothing
		 * needs to be done because we can stay at the same line. If we
		 * have no fixed-position, we need to set the position to the
		 * next inserted line, which can be "line", too. */
		if (con->sb_pos) {
			if (con->sb_pos == tmp ||
			    !(con->flags & TSM_SCREEN_FIXED_POS)) {
				if (con->sb_pos->next)
					con->sb_pos = con->sb_pos->next;
				else
					con->sb_pos = line;
			}
		}

		if (con->sel_active) {
			if (con->sel_start.line == tmp) {
				con->sel_start.line = NULL;
				con->sel_start.y = SELECTION_TOP;
			}
			if (con->sel_end.line == tmp) {
				con->sel_end.line = NULL;
				con->sel_end.y = SELECTION_TOP;
			}
		}
		delete tmp;
	}

	line->sb_id = ++con->sb_last_id;
	line->next = NULL;
	line->prev = con->sb_last;
	if (con->sb_last)
		con->sb_last->next = line;
	else
		con->sb_first = line;
	con->sb_last = line;
	++con->sb_count;
}


/* set maximum scrollback buffer size */
SHL_EXPORT
void tsm_screen_set_max_sb(struct tsm_screen *con, unsigned int max)
{
	struct line *line;

	if (!con)
		return;

	screen_inc_age(con);
	/* TODO: more sophisticated ageing */
	con->age = con->age_cnt;

	while (con->sb_count > max) {
		line = con->sb_first;
		con->sb_first = line->next;
		if (line->next)
			line->next->prev = NULL;
		else
			con->sb_last = NULL;
		con->sb_count--;

		/* We treat fixed/unfixed position the same here because we
		 * remove lines from the TOP of the scrollback buffer. */
		if (con->sb_pos == line)
			con->sb_pos = con->sb_first;

		if (con->sel_active) {
			if (con->sel_start.line == line) {
				con->sel_start.line = NULL;
				con->sel_start.y = SELECTION_TOP;
			}
			if (con->sel_end.line == line) {
				con->sel_end.line = NULL;
				con->sel_end.y = SELECTION_TOP;
			}
		}
		delete line;
	}

	con->sb_max = max;
}

/* clear scrollback buffer */
SHL_EXPORT
void tsm_screen_clear_sb(struct tsm_screen *con)
{
	struct line *iter, *tmp;

	if (!con)
		return;

	screen_inc_age(con);
	/* TODO: more sophisticated ageing */
	con->age = con->age_cnt;

	for (iter = con->sb_first; iter; ) {
		tmp = iter;
		iter = iter->next;

		delete tmp;
	}

	con->sb_first = NULL;
	con->sb_last = NULL;
	con->sb_count = 0;
	con->sb_pos = NULL;

	if (con->sel_active) {
		if (con->sel_start.line) {
			con->sel_start.line = NULL;
			con->sel_start.y = SELECTION_TOP;
		}
		if (con->sel_end.line) {
			con->sel_end.line = NULL;
			con->sel_end.y = SELECTION_TOP;
		}
	}
}

SHL_EXPORT
void tsm_screen_sb_up(struct tsm_screen *con, unsigned int num)
{
	if (!con || !num)
		return;

	screen_inc_age(con);
	/* TODO: more sophisticated ageing */
	con->age = con->age_cnt;

	while (num--) {
		if (con->sb_pos) {
			if (!con->sb_pos->prev)
				return;

			con->sb_pos = con->sb_pos->prev;
		} else if (!con->sb_last) {
			return;
		} else {
			con->sb_pos = con->sb_last;
		}
	}
}

SHL_EXPORT
void tsm_screen_sb_down(struct tsm_screen *con, unsigned int num)
{
	if (!con || !num)
		return;

	screen_inc_age(con);
	/* TODO: more sophisticated ageing */
	con->age = con->age_cnt;

	while (num--) {
		if (con->sb_pos)
			con->sb_pos = con->sb_pos->next;
		else
			return;
	}
}

SHL_EXPORT
void tsm_screen_sb_page_up(struct tsm_screen *con, unsigned int num)
{
	if (!con || !num)
		return;

	screen_inc_age(con);
	tsm_screen_sb_up(con, num * con->size_y);
}

SHL_EXPORT
void tsm_screen_sb_page_down(struct tsm_screen *con, unsigned int num)
{
	if (!con || !num)
		return;

	screen_inc_age(con);
	tsm_screen_sb_down(con, num * con->size_y);
}

SHL_EXPORT
void tsm_screen_sb_reset(struct tsm_screen *con)
{
	if (!con)
		return;

	screen_inc_age(con);
	/* TODO: more sophisticated ageing */
	con->age = con->age_cnt;

	con->sb_pos = NULL;
}

SHL_EXPORT
void tsm_screen_set_def_attr(struct tsm_screen *con,
				 const struct tsm_screen_attr *attr)
{
	if (!con || !attr)
		return;

	memcpy(&con->def_attr, attr, sizeof(*attr));
}
