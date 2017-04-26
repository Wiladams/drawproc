#pragma once


#include "Console.h"


class ScrollbackBuffer {
	Console & con;

	/* scroll-back buffer */
	unsigned int sb_count;		/* number of lines in sb */
	struct line *sb_first;		/* first line; was moved first */
	struct line *sb_last;		/* last line; was moved last*/
	unsigned int sb_max;		/* max-limit of lines in sb */
	struct line *sb_pos;		/* current position in sb or NULL */
	uint64_t sb_last_id;		/* last id given to sb-line */

	void setMax(size_t num);

public:
	ScrollbackBuffer(Console &con, size_t max);


	void reset();
	void clear();
	void down(size_t num);
	void up(size_t num);
	void pageDown(size_t num);
	void pageUp(size_t num);
};