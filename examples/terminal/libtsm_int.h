/*
 * TSM - Main internal header
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

#ifndef TSM_LIBTSM_INT_H
#define TSM_LIBTSM_INT_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include "libtsm.h"
#include "shl_llog.h"
#include "SymbolTable.h"


#define SHL_EXPORT

/* symbols */

struct tsm_symbol_table;

extern const tsm_symbol_t tsm_symbol_default;


struct tsm_screen_attr {
	int8_t fccode;			/* foreground color code or <0 for rgb */
	int8_t bccode;			/* background color code or <0 for rgb */
	uint8_t fr;			/* foreground red */
	uint8_t fg;			/* foreground green */
	uint8_t fb;			/* foreground blue */
	uint8_t br;			/* background red */
	uint8_t bg;			/* background green */
	uint8_t bb;			/* background blue */
	unsigned int bold : 1;		/* bold character */
	unsigned int underline : 1;	/* underlined character */
	unsigned int inverse : 1;	/* inverse colors */
	unsigned int protect : 1;	/* cannot be erased */
	unsigned int blink : 1;		/* blinking character */

	tsm_screen_attr() {
		fccode = -1;
		bccode = -1;

		fr = 255;
		fg = 255;

		br = 0;
		bg = 0;
		bb = 0;
		fb = 255;

		bold = 0;
		underline = 0;
		inverse = 0;
		protect = 0;
		blink = 0;
	}
};

/* TSM screen */

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


void link_to_scrollback(struct tsm_screen *con, struct line *line);

#define SELECTION_TOP -1
struct selection_pos {
	struct line *line;
	unsigned int x;
	int y;

public:
	selection_pos() { line = nullptr; x = 0; y = 0; }
	selection_pos(struct line * aline, size_t x, int y) 
		: line(aline),x(x),y(y)
	{}
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
	unsigned int sb_count;		/* number of lines in sb */
	struct line *sb_first;		/* first line; was moved first */
	struct line *sb_last;		/* last line; was moved last*/
	unsigned int sb_max;		/* max-limit of lines in sb */
	struct line *sb_pos;		/* current position in sb or NULL */
	uint64_t sb_last_id;		/* last id given to sb-line */

	/* cursor: positions are always in-bound, but cursor_x might be
	 * bigger than size_x if new-line is pending */
	unsigned int cursor_x;		/* current cursor x-pos */
	unsigned int cursor_y;		/* current cursor y-pos */

	/* tab ruler */
	bool *tab_ruler;		/* tab-flag for all cells of one row */

	/* selection */
	bool sel_active;
	struct selection_pos sel_start;
	struct selection_pos sel_end;
};


static inline void screen_inc_age(struct tsm_screen *con)
{
	if (!++con->age_cnt) {
		con->age_reset = 1;
		++con->age_cnt;
	}
}

/* available character sets */

typedef tsm_symbol_t tsm_vte_charset[96];

extern tsm_vte_charset tsm_vte_unicode_lower;
extern tsm_vte_charset tsm_vte_unicode_upper;
extern tsm_vte_charset tsm_vte_dec_supplemental_graphics;
extern tsm_vte_charset tsm_vte_dec_special_graphics;

#endif /* TSM_LIBTSM_INT_H */
