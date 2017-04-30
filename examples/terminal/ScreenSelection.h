#pragma once
#include "Console.h"

class ScreenSelection
{
	Console &con;

	/* selection */
	bool sel_active;
	struct selection_pos sel_start;
	struct selection_pos sel_end;

public:
	ScreenSelection(Console &con);

	void setSelection(struct selection_pos &sel, size_t x, size_t y);

	void reset();
	void start(size_t x, size_t y);
	void target(size_t posx, size_t posy);
	int copySelection(char **out);

};