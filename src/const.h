#pragma once

#include <stdint.h>
typedef enum {
	C_CONST_INTEGER,
	C_CONST_STRING,
} C_ConstType;

typedef union {
	intmax_t integer;
	long double decimal;
	char *string;
} C_ConstVal;

typedef struct {
	C_ConstType type;
	C_ConstVal val;
} C_Const;
