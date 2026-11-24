#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void noreturn Error_terminate(Error error) {
	switch (error.type) {
	case ERROR_QUIRKY:
		fprintf(stderr, "Special error:\n`%s`\n", error.msg);
		break;
	case ERROR_USAGE:
		fprintf(stderr, "Usage error:\n`%s`\n", error.msg);
		break;
	case ERROR_PARAMS:
		fprintf(stderr, "Parameter error:\n`%s`\n", error.msg);
		break;
	case ERROR_INTERNAL:
		fprintf(stderr, "Internal error:\n`%s`\n", error.msg);
		break;
	case ERROR_SYNTAX:
		fprintf(stderr, "Syntax error:\n`%s`\n", error.msg);
		break;
	}
	exit(EXIT_FAILURE);
}
