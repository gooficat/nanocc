#include "context.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

void Context_error_cleanup(Context *ctx) {
	Lexer_close(&ctx->lexer);
}

void noreturn Context_error_terminate(Context *ctx, ErrorType type,
									  char const *msg) {
	switch (type) {
	case ERROR_QUIRKY:
		fprintf(stderr, "Special error:\n`%s`\n", msg);
		break;
	case ERROR_USAGE:
		fprintf(stderr, "Usage error:\n`%s`\n", msg);
		break;
	case ERROR_PARAMS:
		fprintf(stderr, "Parameter error:\n`%s`\n", msg);
		break;
	case ERROR_INTERNAL:
		fprintf(stderr, "Internal error:\n`%s`\n", msg);
		break;
	case ERROR_SYNTAX:
		fprintf(stderr, "Syntax error:\n`%s`\n", msg);
		break;
	}
	Context_error_cleanup(ctx);
	exit(EXIT_FAILURE);
}
