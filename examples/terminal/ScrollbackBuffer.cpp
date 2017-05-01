#include "ScrollbackBuffer.h"
#include "Console.h"


/* set maximum scrollback buffer size */
void ScrollbackBuffer::setMax(size_t max)
{

	con.incrementAge();
	con.alignAge();
	
	// Making it smaller
	// We only allow this if resizing is allowed
	struct line *line;
	while (sb_count > max) {
		line = sb_first;
		sb_first = line->next;
		if (line->next)
			line->next->prev = nullptr;
		else
			sb_last = nullptr;
		
		sb_count--;

		// We treat fixed/unfixed position the same here because we
		// remove lines from the TOP of the scrollback buffer.
		// this should be done by a selection object attached to the 
		// console
		if (sb_pos == line)
			sb_pos = sb_first;

		if (con.getSelection().isActive()) {
			if (con.getSelection().getStart().line == line) {
				con.getSelection().getStart().line = nullptr;
				con.getSelection().getStart().y = SELECTION_TOP;
			}
			if (con.getSelection().getEnd().line == line) {
				con.getSelection().getEnd().line = nullptr;
				con.getSelection().getEnd().y = SELECTION_TOP;
			}
		}
		delete line;
	}

	sb_max = max;
}


ScrollbackBuffer::ScrollbackBuffer(Console &con, size_t max)
	:con(con)
{
	setMax(max);
}


/* clear scrollback buffer */
void ScrollbackBuffer::clear()
{
	struct line *iter, *tmp;

	con.incrementAge();
	// TODO: more sophisticated ageing
	//con->age = con->age_cnt;

	for (iter = sb_first; iter; ) {
		tmp = iter;
		iter = iter->next;

		delete tmp;
	}

	sb_first = nullptr;
	sb_last = nullptr;
	sb_count = 0;
	sb_pos = nullptr;

	// adjust selection if necessary
	/* WAA - BUGBUG, refactor console to deal with changing buffer
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
	*/
}


void ScrollbackBuffer::up(size_t num)
{
	if (!num)
		return;

	con.incrementAge();
	/* TODO: more sophisticated ageing */
	//con->age = con->age_cnt;

	while (num--) {
		if (sb_pos) {
			if (!sb_pos->prev)
				return;

			sb_pos = sb_pos->prev;
		}
		else if (!sb_last) {
			return;
		}
		else {
			sb_pos = sb_last;
		}
	}
}


void ScrollbackBuffer::down(size_t num)
{
	if (!num)
		return;

	con.incrementAge();
	// TODO: more sophisticated ageing
	//con->age = con->age_cnt;

	while (num--) {
		if (sb_pos)
			sb_pos = sb_pos->next;
		else
			return;
	}
}


void ScrollbackBuffer::pageUp(size_t num)
{
	if (!num)
		return;

	con.incrementAge();

	up(num * con.getHeight());
}


void ScrollbackBuffer::pageDown(size_t num)
{
	if (!num)
		return;

	con.incrementAge();
	down(num * con.getHeight());
}


void ScrollbackBuffer::reset()
{
	con.incrementAge();
	// TODO: more sophisticated ageing
	//con->age = con->age_cnt;

	sb_pos = nullptr;
}
