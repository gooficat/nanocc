#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

#include <stdbool.h>

bool token_is_type(int token);
bool token_is_operator(int token);
bool token_is_prefix(int token);
bool token_is_postfix(int token);

#endif
