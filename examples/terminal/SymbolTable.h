#pragma once

#include <inttypes.h>

#include "libtsm.h"
#include "shl_htable.h"
#include "shl_array.h"

#include "Unicode.h"

//extern const tsm_symbol_t tsm_symbol_default;

struct SymbolTable {
	unsigned long ref;
	uint32_t next_id;
	struct shl_array *index;
	struct shl_htable symbols;

	SymbolTable();
	~SymbolTable();

	const uint32_t * get(tsm_symbol_t *sym, size_t *size);
	tsm_symbol_t make(uint32_t ucs4);
	tsm_symbol_t append(tsm_symbol_t sym, uint32_t ucs4);
	unsigned int getWidth(tsm_symbol_t sym);

};
