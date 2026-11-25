#pragma once

#include "lexer.h"
#include "parser.h"
#include "pool.h"
#include "writer.h"
#include <stdnoreturn.h>

typedef struct Context {
	Lexer lexer;
	Pool pool;
	Parser parser;
	Writer writer;
} Context;

typedef enum {
	ERROR_QUIRKY,
	ERROR_USAGE,
	ERROR_PARAMS,
	ERROR_INTERNAL,
	ERROR_SYNTAX,
} ErrorType;

void noreturn Context_error_terminate(Context *ctx, ErrorType type,
									  char const *msg);
