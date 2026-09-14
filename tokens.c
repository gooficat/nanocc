#include <stddef.h>

char const * const TOKENS[] = {
#define TOK(id, str) str,
#include "tokens.h"
#undef TOK
  NULL
};
