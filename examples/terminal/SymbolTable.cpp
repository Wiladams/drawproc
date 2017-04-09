#include "SymbolTable.h"
#include "Unicode.h"
#include <string.h>

const tsm_symbol_t tsm_symbol_default = 0;

static size_t hash_ucs4(const void *key, void *priv)
{
	size_t i, val = 5381;
	const uint32_t *ucs4 = (uint32_t *)key;

	for (i = 0; ucs4[i] <= TSM_UCS4_MAX; ++i)
		val = val * 33 + ucs4[i];

	return val;
}

static bool cmp_ucs4(const void *a, const void *b)
{
	size_t i;
	const uint32_t *v1 = (uint32_t *)a;
	const uint32_t *v2 = (uint32_t *)b;

	//v1 = a;
	//v2 = b;

	for (i = 0; ; ++i) {
		if (v1[i] > TSM_UCS4_MAX && v2[i] > TSM_UCS4_MAX)
			return true;
		if (v1[i] != v2[i])
			return false;
	}
}

static void free_ucs4(void *elem, void *priv)
{
	uint32_t *v = (uint32_t *)elem;

	/* key is prefix with actual value so pass correct pointer */
	free(--v);
}

SymbolTable::SymbolTable()
{
	struct tsm_symbol_table *tbl;
	int ret;
	static const uint32_t *val = NULL; /* we need a valid lvalue */


	//memset(tbl, 0, sizeof(*tbl));
	ref = 1;
	next_id = TSM_UCS4_MAX + 2;
	shl_htable_init(&symbols, cmp_ucs4, hash_ucs4, NULL);

	ret = shl_array_new(&index, sizeof(uint32_t*), 4);
	//if (ret)
	//	goto err_free;

	/* first entry is not used so add dummy */
	shl_array_push(index, &val);
}

SymbolTable::~SymbolTable()
{
	shl_htable_clear(&symbols, free_ucs4, NULL);
	shl_array_free(index);
}


tsm_symbol_t SymbolTable::make(uint32_t ucs4)
{
	if (ucs4 > TSM_UCS4_MAX)
		return 0;
	else
		return ucs4;
}

/*
* This decomposes a symbol into a ucs4 string and a size value. If \sym is a
* valid UCS4 character, this returns a pointer to \sym and writes 1 into \size.
* Therefore, the returned value may get destroyed if your \sym argument gets
* destroyed.
* If \sym is a composed ucs4 string, then the returned value points into the
* hash table of the symbol table and lives as long as the symbol table does.
*
* This always returns a valid value. If an error happens, the default character
* is returned. If \size is NULL, then the size value is omitted.
*/
const uint32_t * SymbolTable::get(tsm_symbol_t *sym, size_t *size)
{
	uint32_t *ucs4, idx;

	if (*sym <= TSM_UCS4_MAX) {
		if (size)
			*size = 1;
		return sym;
	}

	idx = *sym - (TSM_UCS4_MAX + 1);
	if (idx >= shl_array_get_length(index))
		ucs4 = NULL;
	else
		ucs4 = *SHL_ARRAY_AT(index, uint32_t*, idx);

	if (!ucs4) {
		if (size)
			*size = 1;
		return &tsm_symbol_default;
	}

	if (size) {
		*size = 0;
		while (ucs4[*size] <= TSM_UCS4_MAX)
			++*size;
	}

	return ucs4;
}



tsm_symbol_t SymbolTable::append(tsm_symbol_t sym, uint32_t ucs4)
{
	uint32_t buf[TSM_UCS4_MAXLEN + 1], nsym, *nval;
	const uint32_t *ptr;
	size_t s;
	bool res;
	int ret;

	if (ucs4 > TSM_UCS4_MAX)
		return sym;

	ptr = get(&sym, &s);
	if (s >= TSM_UCS4_MAXLEN)
		return sym;

	memcpy(buf, ptr, s * sizeof(uint32_t));
	buf[s++] = ucs4;
	buf[s++] = TSM_UCS4_MAX + 1;

	res = shl_htable_lookup(&symbols, buf, hash_ucs4(buf, NULL),
		(void**)&nval);
	if (res) {
		/* key is prefixed with actual value */
		return *--nval;
	}

	/* We save the key in nval and prefix it with the new ID. Note that
	* the prefix is hidden, we actually store "++nval" in the htable. */
	nval = (uint32_t *)malloc(sizeof(uint32_t) * (s + 1));
	if (!nval)
		return sym;

	++nval;
	memcpy(nval, buf, s * sizeof(uint32_t));

	nsym = next_id + 1;
	/* Out of IDs; we actually have 2 Billion IDs so this seems
	* very unlikely but lets be safe here */
	if (nsym <= next_id++)
		goto err_id;

	/* store ID hidden before the key */
	*(nval - 1) = nsym;

	ret = shl_htable_insert(&symbols, nval, hash_ucs4(nval, NULL));
	if (ret)
		goto err_id;

	ret = shl_array_push(index, &nval);
	if (ret)
		goto err_symbol;

	return nsym;

err_symbol:
	shl_htable_remove(&symbols, nval, hash_ucs4(nval, NULL), NULL);
err_id:
	--next_id;
	free(nval);
	
	return sym;
}

unsigned int SymbolTable::getWidth(tsm_symbol_t sym)
{
	const uint32_t *ch;
	size_t len;

	ch = get(&sym, &len);
	if (len == 0)
		return 0;

	return tsm_ucs4_get_width(*ch);
}
