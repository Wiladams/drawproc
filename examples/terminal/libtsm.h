/*
 * TSM - Main Header
 *
 * Copyright (c) 2011-2013 David Herrmann <dh.herrmann@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef TSM_LIBTSM_H
#define TSM_LIBTSM_H

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @mainpage
 *
 * TSM is a Terminal-emulator State Machine. It implements all common DEC-VT100
 * to DEC-VT520 control codes and features. A state-machine is used to parse TTY
 * input and saved in a virtual screen. TSM does not provide any rendering,
 * glyph/font handling or anything more advanced. TSM is just a simple
 * state-machine for control-codes handling.
 * The main use-case for TSM are terminal-emulators. TSM has no dependencies
 * other than an ISO-C99 compiler and C-library. Any terminal emulator for any
 * window-environment or rendering-pipline can make use of TSM. However, TSM can
 * also be used for control-code validation, TTY-screen-capturing or other
 * advanced users of terminal escape-sequences.
 */

/**
 * @defgroup misc Miscellaneous Definitions
 * Miscellaneous definitions
 *
 * This section contains several miscellaneous definitions of small helpers and
 * constants. These are shared between other parts of the API and have common
 * semantics/syntax.
 *
 * @{
 */

/**
 * Logging Callback
 *
 * @data: user-provided data
 * @file: Source code file where the log message originated or NULL
 * @line: Line number in source code or 0
 * @func: C function name or NULL
 * @subs: Subsystem where the message came from or NULL
 * @sev: Kernel-style severity between 0=FATAL and 7=DEBUG
 * @format: printf-formatted message
 * @args: arguments for printf-style @format
 *
 * This is the type of a logging callback function. You can always pass NULL
 * instead of such a function to disable logging.
 */
typedef void (*tsm_log_t) (void *data,
			   const char *file,
			   int line,
			   const char *func,
			   const char *subs,
			   unsigned int sev,
			   const char *format,
			   va_list args);

/** @} */

/**
 * @defgroup symbols Unicode Helpers
 * Unicode helpers
 *
 * Unicode uses 32bit types to uniquely represent symbols. However, combining
 * characters allow modifications of such symbols but require additional space.
 * To avoid passing around allocated strings, TSM provides a symbol-table which
 * can store combining-characters with their base-symbol to create a new symbol.
 * This way, only the symbol-identifiers have to be passed around (which are
 * simple integers). No string allocation is needed by the API user.
 *
 * The symbol table is currently not exported. Once the API is fixed, we will
 * provide it to outside users.
 *
 * Additionally, this contains some general UTF8/UCS4 helpers.
 *
 * @{
 */


/* symbols */

typedef uint32_t tsm_symbol_t;

/** @} */

/**
 * @defgroup screen Terminal Screens
 * Virtual terminal-screen implementation
 *
 * A TSM screen respresents the real screen of a terminal/application. It does
 * not render anything, but only provides a table of cells. Each cell contains
 * the stored symbol, attributes and more. Applications iterate a screen to
 * render each cell on their framebuffer.
 *
 * Screens provide all features that are expected from terminals. They include
 * scroll-back buffers, alternate screens, cursor positions and selection
 * support. Thus, it needs event-input from applications to drive these
 * features. Most of them are optional, though.
 *
 * @{
 */

struct tsm_screen;
typedef uint_fast32_t tsm_age_t;

#define TSM_SCREEN_INSERT_MODE	0x01
#define TSM_SCREEN_AUTO_WRAP	0x02
#define TSM_SCREEN_REL_ORIGIN	0x04
#define TSM_SCREEN_INVERSE	0x08
#define TSM_SCREEN_HIDE_CURSOR	0x10
#define TSM_SCREEN_FIXED_POS	0x20
#define TSM_SCREEN_ALTERNATE	0x40

struct tsm_screen_attr;




void tsm_screen_set_max_sb(struct tsm_screen *con, unsigned int max);
void tsm_screen_clear_sb(struct tsm_screen *con);

void tsm_screen_sb_up(struct tsm_screen *con, unsigned int num);
void tsm_screen_sb_down(struct tsm_screen *con, unsigned int num);
void tsm_screen_sb_page_up(struct tsm_screen *con, unsigned int num);
void tsm_screen_sb_page_down(struct tsm_screen *con, unsigned int num);
void tsm_screen_sb_reset(struct tsm_screen *con);

void tsm_screen_set_def_attr(struct tsm_screen *con,
			     const struct tsm_screen_attr *attr);


void tsm_screen_insert_lines(struct tsm_screen *con, unsigned int num);
void tsm_screen_delete_lines(struct tsm_screen *con, unsigned int num);
void tsm_screen_insert_chars(struct tsm_screen *con, unsigned int num);
void tsm_screen_delete_chars(struct tsm_screen *con, unsigned int num);


void tsm_screen_selection_reset(struct tsm_screen *con);
void tsm_screen_selection_start(struct tsm_screen *con, unsigned int posx, unsigned int posy);
void tsm_screen_selection_target(struct tsm_screen *con, unsigned int posx, unsigned int posy);
int tsm_screen_selection_copy(struct tsm_screen *con, char **out);

#ifdef __cplusplus
}
#endif

#endif /* TSM_LIBTSM_H */
