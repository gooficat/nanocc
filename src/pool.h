#pragma once

#include "const.h"
#include <stddef.h>

typedef struct {
	char const **val;
	size_t len;
} StrList;

typedef struct {
	C_Const *val;
	size_t len;
} C_ConstList;

typedef struct {
	StrList idents;
	C_ConstList consts;
} Pool;

Pool Pool_new();
size_t Pool_add_or_get_ident(Pool *pool, char *ident);
size_t Pool_add_or_get_const(Pool *pool, C_Const cons);
