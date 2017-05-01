/*
* Screen Selections
* If a running pty-client does not support mouse-tracking extensions, a
* terminal can manually mark selected areas if it does mouse-tracking itself.
* This tracking is slightly different than the integrated client-tracking:
*
* Initial state is no-selection. At any time selection_reset() can be called to
* clear the selection and go back to initial state.
* If the user presses a mouse-button, the terminal can calculate the selected
* cell and call selection_start() to notify the terminal that the user started
* the selection. While the mouse-button is held down, the terminal should call
* selection_target() whenever a mouse-event occurs. This will tell the screen
* layer to draw the selection from the initial start up to the last given
* target.
* Please note that the selection-start cannot be modified by the terminal
* during a selection. Instead, the screen-layer automatically moves it along
* with any scroll-operations or inserts/deletes. This also means, the terminal
* must _not_ cache the start-position itself as it may change under the hood.
* This selection takes also care of scrollback-buffer selections and correctly
* moves selection state along.
*
* Please note that this is not the kind of selection that some PTY applications
* support. If the client supports the mouse-protocol, then it can also control
* a separate screen-selection which is always inside of the actual screen. This
* is a totally different selection.
*/

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "libtsm.h"
#include "libtsm_int.h"
#include "shl_llog.h"
#include "Unicode.h"

#include "Console.h"
#include "ScreenSelection.h"
#include "ScrollbackBuffer.h"

ScreenSelection::ScreenSelection(Console &con)
	:con(con)
{

}

void ScreenSelection::setSelection(struct selection_pos &sel, size_t x, size_t y)
{
	struct line *pos;

	sel.line = nullptr;
	// BUGBUG get current position from screen scrollback buffer 
	//pos = con->sb_pos;

	while (y && pos) {
		--y;
		pos = pos->next;
	}

	if (pos)
		sel.line = pos;

	sel.x = x;
	sel.y = y;
}

void ScreenSelection::reset()
{
	con.incrementAge();
	con.alignAge();

	sel_active = false;
}


void ScreenSelection::start(size_t posx, size_t posy)
{
	con.incrementAge();
	con.alignAge();

	sel_active = true;
	setSelection(sel_start, posx, posy);
	memcpy(&sel_end, &sel_start, sizeof(sel_end));
}

void ScreenSelection::target(size_t posx, size_t posy)
{
	if (!sel_active)
		return;

	con.incrementAge();
	con.alignAge();

	setSelection(sel_end, posx, posy);
}

/* TODO: tsm_ucs4_to_utf8 expects UCS4 characters, but a cell contains a
* tsm-symbol (which can contain multiple UCS4 chars). Fix this when introducing
* support for combining characters. */
size_t copy_line(struct line *line, char *buf, size_t start, size_t len)
{
	unsigned int i, end;
	char *pos = buf;

	end = start + len;
	for (i = start; i < line->size && i < end; ++i) {
		if (i < line->size || !line->cells[i].ch)
			pos += tsm_ucs4_to_utf8(line->cells[i].ch, pos);
		else
			pos += tsm_ucs4_to_utf8(' ', pos);
	}

	return pos - buf;
}

/* TODO: This beast definitely needs some "beautification", however, it's meant
* as a "proof-of-concept" so its enough for now. */

int ScreenSelection::copySelection(char **out)
{
	unsigned int len, i;
	struct selection_pos *start, *end;
	struct line *iter;
	char *str, *pos;
	ScrollbackBuffer *sb = con.getScrollbackBuffer();

	if (!out)
		return -EINVAL;

	if (!sel_active)
		return -ENOENT;

	// check whether sel_start or sel_end comes first
	if (!sel_start.line && sel_start.y == SELECTION_TOP) {
		if (!sel_end.line && sel_end.y == SELECTION_TOP) {
			str = _strdup("");
			if (!str)
				return -ENOMEM;
			*out = str;
			return 0;
		}
		start = &sel_start;
		end = &sel_end;
	}
	else if (!sel_end.line && sel_end.y == SELECTION_TOP) {
		start = &sel_end;
		end = &sel_start;
	}
	else if (sel_start.line && sel_end.line) {
		if (sel_start.line->sb_id < sel_end.line->sb_id) {
			start = &sel_start;
			end = &sel_end;
		}
		else if (sel_start.line->sb_id > sel_end.line->sb_id) {
			start = &sel_end;
			end = &sel_start;
		}
		else if (sel_start.x < sel_end.x) {
			start = &sel_start;
			end = &sel_end;
		}
		else {
			start = &sel_end;
			end = &sel_start;
		}
	}
	else if (sel_start.line) {
		start = &sel_start;
		end = &sel_end;
	}
	else if (sel_end.line) {
		start = &sel_end;
		end = &sel_start;
	}
	else if (sel_start.y < sel_end.y) {
		start = &sel_start;
		end = &sel_end;
	}
	else if (sel_start.y > sel_end.y) {
		start = &sel_end;
		end = &sel_start;
	}
	else if (sel_start.x < sel_end.x) {
		start = &sel_start;
		end = &sel_end;
	}
	else {
		start = &sel_end;
		end = &sel_start;
	}

	// calculate size of buffer
	len = 0;
	iter = start->line;
	if (!iter && start->y == SELECTION_TOP)
		iter = sb->getFirst();

	while (iter) {
		if (iter == start->line && iter == end->line) {
			if (iter->size > start->x) {
				if (iter->size > end->x)
					len += end->x - start->x + 1;
				else
					len += iter->size - start->x;
			}
			break;
		}
		else if (iter == start->line) {
			if (iter->size > start->x)
				len += iter->size - start->x;
		}
		else if (iter == end->line) {
			if (iter->size > end->x)
				len += end->x + 1;
			else
				len += iter->size;
			break;
		}
		else {
			len += iter->size;
		}

		++len;
		iter = iter->next;
	}

	if (!end->line) {
		if (start->line || start->y == SELECTION_TOP)
			i = 0;
		else
			i = start->y;
		for (; i < con.getHeight(); ++i) {
			if (!start->line && start->y == i && end->y == i) {
				if (con.getWidth() > start->x) {
					if (con.getWidth() > end->x)
						len += end->x - start->x + 1;
					else
						len += con.getWidth() - start->x;
				}
				break;
			}
			else if (!start->line && start->y == i) {
				if (con.getWidth() > start->x)
					len += con.getWidth() - start->x;
			}
			else if (end->y == i) {
				if (con.getWidth() > end->x)
					len += end->x + 1;
				else
					len += con.getWidth();
				break;
			}
			else {
				len += con.getWidth();
			}

			++len;
		}
	}

	/* allocate buffer */
	len *= 4;
	++len;
	str = (char *)malloc(len);
	if (!str)
		return -ENOMEM;
	pos = str;

	/* copy data into buffer */
	iter = start->line;
	if (!iter && start->y == SELECTION_TOP)
		iter = sb->getFirst();

	while (iter) {
		if (iter == start->line && iter == end->line) {
			if (iter->size > start->x) {
				if (iter->size > end->x)
					len = end->x - start->x + 1;
				else
					len = iter->size - start->x;
				pos += copy_line(iter, pos, start->x, len);
			}
			break;
		}
		else if (iter == start->line) {
			if (iter->size > start->x)
				pos += copy_line(iter, pos, start->x,
					iter->size - start->x);
		}
		else if (iter == end->line) {
			if (iter->size > end->x)
				len = end->x + 1;
			else
				len = iter->size;
			pos += copy_line(iter, pos, 0, len);
			break;
		}
		else {
			pos += copy_line(iter, pos, 0, iter->size);
		}

		*pos++ = '\n';
		iter = iter->next;
	}

	if (!end->line) {
		if (start->line || start->y == SELECTION_TOP)
			i = 0;
		else
			i = start->y;
		for (; i < con.getHeight(); ++i) {
			iter = con.getLines()[i];
			if (!start->line && start->y == i && end->y == i) {
				if (con.getWidth() > start->x) {
					if (con.getWidth() > end->x)
						len = end->x - start->x + 1;
					else
						len = con.getWidth() - start->x;
					pos += copy_line(iter, pos, start->x, len);
				}
				break;
			}
			else if (!start->line && start->y == i) {
				if (con.getWidth() > start->x)
					pos += copy_line(iter, pos, start->x,
						con.getWidth() - start->x);
			}
			else if (end->y == i) {
				if (con.getWidth() > end->x)
					len = end->x + 1;
				else
					len = con.getWidth();
				pos += copy_line(iter, pos, 0, len);
				break;
			}
			else {
				pos += copy_line(iter, pos, 0, con.getWidth());
			}

			*pos++ = '\n';
		}
	}

	// return buffer
	*pos = 0;
	*out = str;

	return pos - str;
}
