#include "Unicode.h"

/*
* UTF8 State Machine
* This state machine parses UTF8 and converts it into a stream of Unicode
* characters (UCS4 values). A state-machine is represented by a
* "struct tsm_utf8_mach" object. It has no global state and all functions are
* re-entrant if called with different state-machine objects.
*
* tsm_utf8_mach_new(): This creates a new state-machine and resets it to its
* initial state. Returns 0 on success.
*
* tsm_uft8_mach_free(): This destroys a state-machine and frees all internally
* allocated memory.
*
* tsm_utf8_mach_reset(): Reset a given state-machine to its initial state. This
* is the same state the machine is in after it got created.
*
* tsm_uft8_mach_feed(): Feed one byte of the UTF8 input stream into the
* state-machine. This function returns the new state of the state-machine after
* this character has been parsed. If it is TSM_UTF8_ACCEPT or TSM_UTF8_REJECT,
* then there is a pending UCS4 character that you should retrieve via
* tsm_utf8_mach_get(). If it is TSM_UTF8_ACCEPT, then a character was
* successfully parsed. If it is TSM_UTF8_REJECT, the input was invalid UTF8 and
* some error recovery was tried or a replacement character was choosen. All
* other states mean that the machine needs more input to parse the stream.
*
* tsm_utf8_mach_get(): Returns the last parsed character. It has no effect on
* the state machine so you can call it multiple times.
*
* Internally, we use TSM_UTF8_START whenever the state-machine is reset. This
* can be used to ignore the last read input or to simply reset the machine.
* TSM_UTF8_EXPECT* is used to remember how many bytes are still to be read to
* get a full UTF8 sequence.
* If an error occurs during reading, we go to state TSM_UTF8_REJECT and the
* user will read a replacement character. If further errors occur, we go to
* state TSM_UTF8_START to avoid printing multiple replacement characters for a
* single misinterpreted UTF8 sequence. However, under some circumstances it may
* happen that we stay in TSM_UTF8_REJECT and a next replacement character is
* returned.
* It is difficult to decide how to interpret wrong input but this machine seems
* to be quite good at deciding what to do. Generally, we prefer discarding or
* replacing input instead of trying to decipher ASCII values from the invalid
* data. This guarantees that we do not send wrong values to the terminal
* emulator. Some might argue that an ASCII fallback would be better. However,
* this means that we might send very weird escape-sequences to the VTE layer.
* Especially with C1 codes applications can really break many terminal features
* so we avoid any non-ASCII+non-UTF8 input to prevent this.
*/

UTF8Machine::UTF8Machine()
{
	state = 0;
	ch = 0;
}


int UTF8Machine::feed(char ci)
{
	uint32_t c = ci;

	switch (state) {
	case TSM_UTF8_START:
	case TSM_UTF8_ACCEPT:
	case TSM_UTF8_REJECT:
		if (c == 0xC0 || c == 0xC1) {
			/* overlong encoding for ASCII, reject */
			state = TSM_UTF8_REJECT;
		}
		else if ((c & 0x80) == 0) {
			/* single byte, accept */
			ch = c;
			state = TSM_UTF8_ACCEPT;
		}
		else if ((c & 0xC0) == 0x80) {
			/* parser out of sync, ignore byte */
			state = TSM_UTF8_START;
		}
		else if ((c & 0xE0) == 0xC0) {
			/* start of two byte sequence */
			ch = (c & 0x1F) << 6;
			state = TSM_UTF8_EXPECT1;
		}
		else if ((c & 0xF0) == 0xE0) {
			/* start of three byte sequence */
			ch = (c & 0x0F) << 12;
			state = TSM_UTF8_EXPECT2;
		}
		else if ((c & 0xF8) == 0xF0) {
			/* start of four byte sequence */
			ch = (c & 0x07) << 18;
			state = TSM_UTF8_EXPECT3;
		}
		else {
			/* overlong encoding, reject */
			state = TSM_UTF8_REJECT;
		}
		break;
	case TSM_UTF8_EXPECT3:
		ch |= (c & 0x3F) << 12;
		if ((c & 0xC0) == 0x80)
			state = TSM_UTF8_EXPECT2;
		else
			state = TSM_UTF8_REJECT;
		break;
	case TSM_UTF8_EXPECT2:
		ch |= (c & 0x3F) << 6;
		if ((c & 0xC0) == 0x80)
			state = TSM_UTF8_EXPECT1;
		else
			state = TSM_UTF8_REJECT;
		break;
	case TSM_UTF8_EXPECT1:
		ch |= c & 0x3F;
		if ((c & 0xC0) == 0x80)
			state = TSM_UTF8_ACCEPT;
		else
			state = TSM_UTF8_REJECT;
		break;
	default:
		state = TSM_UTF8_REJECT;
		break;
	}

	return state;
}

uint32_t UTF8Machine::get()
{
	if (state != TSM_UTF8_ACCEPT)
		return TSM_UCS4_REPLACEMENT;

	return ch;
}

void UTF8Machine::reset()
{
	state = TSM_UTF8_START;
}