#pragma once

#include "lexer.h"
#include <stdnoreturn.h>
typedef enum {
	ERROR_QUIRKY,
	ERROR_USAGE,
	ERROR_PARAMS,
	ERROR_INTERNAL,
	ERROR_SYNTAX,
} ErrorType;

typedef struct {
	ErrorType type;
	char const *msg;
	char const *file_path;
	size_t line;
	size_t col;
} Error;

void noreturn Error_terminate(Error error);
