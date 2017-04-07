#pragma once
#include <inttypes.h>

#define TSM_UCS4_MAX (0x7fffffffUL)
#define TSM_UCS4_INVALID (TSM_UCS4_MAX + 1)
#define TSM_UCS4_REPLACEMENT (0xfffdUL)
#define TSM_UCS4_MAXLEN 10

enum tsm_utf8_mach_state {
	TSM_UTF8_START,
	TSM_UTF8_ACCEPT,
	TSM_UTF8_REJECT,
	TSM_UTF8_EXPECT1,
	TSM_UTF8_EXPECT2,
	TSM_UTF8_EXPECT3,
};

struct UTF8Machine {
	int state;
	uint32_t ch;

	UTF8Machine();

	uint32_t get();
	void reset();
	int feed(char ci);

};
