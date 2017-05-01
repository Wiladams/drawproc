#pragma once


class Console;

#define SELECTION_TOP -1
struct selection_pos {
	struct line *line;
	unsigned int x;
	int y;

public:
	selection_pos() { line = nullptr; x = 0; y = 0; }
	selection_pos(struct line * aline, size_t x, int y)
		: line(aline), x(x), y(y)
	{}
};

class ScreenSelection
{
	Console &con;

	/* selection */
	bool sel_active;
	struct selection_pos sel_start;
	struct selection_pos sel_end;

public:
	ScreenSelection(Console &con);

	bool isActive() { return sel_active; }
	struct selection_pos & getStart() { return sel_start; }
	struct selection_pos & getEnd() { return sel_start; }

	void setSelection(struct selection_pos &sel, size_t x, size_t y);
	void incrementYStart(int dy) { sel_start.y += dy; }
	void incrementYEnd(int dy) { sel_end.y += dy; }

	void reset();
	void start(size_t x, size_t y);
	void target(size_t posx, size_t posy);
	int copySelection(char **out);

};