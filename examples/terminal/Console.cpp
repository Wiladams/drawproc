#include "drawproc.h"
#include "libtsm.h"
#include "Console.h"

#include <stdio.h>

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


// Console Line routines
static void line_free(struct line *line)
{
	free(line->cells);
	free(line);
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

int Console::_init()
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

	ret = tsm_symbol_table_new(&screen.sym_table);
	if (ret)
		goto err_free;

	ret = tsm_screen_resize(&screen, Width, Height);
	if (ret)
		goto err_free;

	return 0;

err_free:
	for (i = 0; i < screen.line_num; ++i) {
		line_free(screen.main_lines[i]);
		line_free(screen.alt_lines[i]);
	}
	free(screen.main_lines);
	free(screen.alt_lines);
	free(screen.tab_ruler);
	tsm_symbol_table_unref(screen.sym_table);

	return ret;
}

Console::Console(const size_t width, const size_t height) 
{
		Width = width;
		Height = height;
		int err = _init();
}

Console::~Console()
{
	unsigned int i;

	//llog_debug(con, "destroying screen");

	for (int i = 0; i < screen.line_num; ++i) 
	{
		line_free(screen.main_lines[i]);
		line_free(screen.alt_lines[i]);
	}
	free(screen.main_lines);
	free(screen.alt_lines);
	free(screen.tab_ruler);
	tsm_symbol_table_unref(screen.sym_table);

}

void Console::write(const char *str) 
{
	int idx = 0;
	while (str[idx] != 0) 
	{
		tsm_screen_write(&screen, str[idx], &defaultattr);

		idx = idx + 1;
	}
}

void Console::writeLine(const char *str)
{
	write(str);
	newline();
}

void Console::moveDown(size_t num, bool scroll)
{
	tsm_screen_move_down(&screen, num, scroll);
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


	screen_inc_age(&screen);

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

void Console::newline()
{
	screen_inc_age(&screen);

	tsm_screen_move_down(&screen, 1, true);
	tsm_screen_move_line_home(&screen);
}

void Console::setMaxScrollback(size_t num)
{
	tsm_screen_set_max_sb(&screen, num);
}

void Console::scrollBufferUp(size_t num)
{
	tsm_screen_sb_up(&screen, num);
}

void Console::scrollBufferDown(size_t num)
{
	tsm_screen_sb_down(&screen, num);
}

void Console::scrollUp(size_t num)
{
	tsm_screen_scroll_up(&screen, num);
}

void Console::scrollDown(size_t num)
{
	tsm_screen_scroll_down(&screen, num);
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

	screen_inc_age(&screen);

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

	screen_inc_age(&screen);

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

			screen_cell_init(&screen, &line->cells[x_from]);
		}
		x_from = 0;
	}
}

void Console::reset()
{
	unsigned int i;

	screen_inc_age(&screen);
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

void Console::draw() 
{
	background(0);

	setFont(mcs6x10_mono);
	tsm_age_t age = tsm_screen_draw(&screen, draw_cb, nullptr);
}
