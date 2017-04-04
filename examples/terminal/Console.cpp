#include "drawproc.h"
#include "libtsm.h"
#include "Console.h"

#include <stdio.h>



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


Console::Console(const size_t width, const size_t height) 
{
		Width = width;
		Height = height;
		int err = tsm_screen_new(&screen, nullptr, nullptr);

		err = tsm_screen_resize(screen, width, height);
}

void Console::write(const char *str) 
{
	int idx = 0;
	while (str[idx] != 0) 
	{
		tsm_screen_write(screen, str[idx], &defaultattr);

		idx = idx + 1;
	}
}

void Console::moveDown(size_t num, bool scroll)
{
	tsm_screen_move_down(screen, num, scroll);
}

void Console::moveTo(unsigned int x, unsigned int y)
{
	tsm_screen_move_to(screen, x, y);
}

void Console::newline()
{
	tsm_screen_newline(screen);
}

void Console::setMaxScrollback(size_t num)
{
	tsm_screen_set_max_sb(screen, num);
}

void Console::scrollBufferUp(size_t num)
{
	tsm_screen_sb_up(screen, num);
}

void Console::scrollBufferDown(size_t num)
{
	tsm_screen_sb_down(screen, num);
}

void Console::scrollUp(size_t num)
{
	tsm_screen_scroll_up(screen, num);
}

void Console::scrollDown(size_t num)
{
	tsm_screen_scroll_down(screen, num);
}

void Console::draw() 
{
	background(0);

	setFont(mcs6x10_mono);
	tsm_age_t age = tsm_screen_draw(screen, draw_cb, nullptr);
}
