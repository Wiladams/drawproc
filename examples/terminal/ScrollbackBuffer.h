#pragma once


#include "Console.h"

class ScrollbackBuffer {
	Console & con;

	unsigned int sb_count;		// number of lines in sb
	struct line *sb_first;		// first line; was moved first
	struct line *sb_last;		//last line; was moved last
	unsigned int sb_max;		// max-limit of lines in sb
	struct line *sb_pos;		// current position in sb or NULL
	uint64_t sb_last_id;		// last id given to sb-line

	void setMax(size_t num);

public:
	ScrollbackBuffer(Console &con, size_t max);

	// getters
	size_t getCount() { return sb_count; }
	struct line * getFirst() { return sb_first; }
	struct line * getLast() { return sb_last; }
	uint64_t getLastId() { return sb_last_id; }

	size_t getMax() { return sb_max; }
	struct line * getPosition() { return sb_pos; }

	// setters
	void setFirst(struct line * aline) { sb_first = aline; }
	void setLast(struct line * aline) { sb_last = aline; }
	void setPosition(struct line * pos) { sb_pos = pos; }

	// Operations
	void decrementCount() { sb_count = sb_count - 1; }
	void incrementCount() { sb_count = sb_count + 1; }
	uint64_t incrementLastId() { sb_last_id = sb_last_id + 1; return sb_last_id; }

	void reset();
	void clear();
	void down(size_t num);
	void up(size_t num);
	void pageDown(size_t num);
	void pageUp(size_t num);
};