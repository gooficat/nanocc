#include "pool.h"
#include "const.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

size_t Pool_add_or_get_ident(Pool *pool, char *ident) {
	size_t i;
	for (i = 0; i < pool->idents.len; ++i) {
		if (!strcmp(ident, pool->idents.val[i])) {
			free(ident);
			return i;
		}
	}
	pool->idents.len += 1;
	pool->idents.val =
		realloc(pool->idents.val, pool->idents.len * sizeof *pool->idents.val);
	pool->idents.val[i] = ident;
	return i;
}

size_t Pool_add_or_get_const(Pool *pool, C_Const cons) {
	size_t i;
	for (i = 0; i < pool->consts.len; ++i) {
		C_Const *i_cons = &pool->consts.val[i];
		if (cons.type == i_cons->type) {
			switch (cons.type) {
			case C_CONST_INTEGER:
				if (cons.val.integer == i_cons->val.integer) {
					return i;
				}
				break;
			case C_CONST_STRING:
				if (!strcmp(cons.val.string, i_cons->val.string)) {
					return i;
				}
				break;
			}
		}
	}
	pool->consts.len += 1;
	pool->consts.val =
		realloc(pool->consts.val, pool->consts.len * sizeof *pool->consts.val);
	pool->consts.val[i] = cons;
	return i;
}

Pool Pool_new() {
	return (Pool){};
}
