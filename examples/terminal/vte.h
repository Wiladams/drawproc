
#include "libtsm.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @defgroup vte State Machine
* Virtual terminal emulation with state machine
*
* A TSM VTE object provides the terminal state machine. It takes input from the
* application (which usually comes from a TTY/PTY from a client), parses it,
* modifies the attach screen or returns data which has to be written back to
* the client.
*
* Furthermore, VTE objects accept keyboard or mouse input from the application
* which is interpreted compliant to DEV-VTs.
*
* @{
*/

/* virtual terminal emulator */

struct tsm_vte;

/* keep in sync with shl_xkb_mods */
enum tsm_vte_modifier {
	TSM_SHIFT_MASK = (1 << 0),
	TSM_LOCK_MASK = (1 << 1),
	TSM_CONTROL_MASK = (1 << 2),
	TSM_ALT_MASK = (1 << 3),
	TSM_LOGO_MASK = (1 << 4),
};

/* keep in sync with TSM_INPUT_INVALID */
#define TSM_VTE_INVALID 0xffffffff

typedef void(*tsm_vte_write_cb) (struct tsm_vte *vte,
	const char *u8,
	size_t len,
	void *data);

int tsm_vte_new(struct tsm_vte **out, struct tsm_screen *con,
	tsm_vte_write_cb write_cb, void *data,
	tsm_log_t log, void *log_data);
void tsm_vte_ref(struct tsm_vte *vte);
void tsm_vte_unref(struct tsm_vte *vte);

int tsm_vte_set_palette(struct tsm_vte *vte, const char *palette);
void tsm_vte_get_def_attr(struct tsm_vte *vte, struct tsm_screen_attr *out);

void tsm_vte_reset(struct tsm_vte *vte);
void tsm_vte_hard_reset(struct tsm_vte *vte);
void tsm_vte_input(struct tsm_vte *vte, const char *u8, size_t len);
bool tsm_vte_handle_keyboard(struct tsm_vte *vte, uint32_t keysym,
	uint32_t ascii, unsigned int mods,
	uint32_t unicode);

/** @} */

#ifdef __cplusplus
}
#endif
