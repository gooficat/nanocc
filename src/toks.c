#include <stdio.h>
char const *const TOKS[] = {
#define TOK(id, str) str,
#include "toks.h"
#undef TOK
	NULL,
};
