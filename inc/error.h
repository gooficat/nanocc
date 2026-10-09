#ifndef ERROR_H_
#define ERROR_H_

/* This error handler module is the exit point for every runtime error state.
 * This allows for easy debugging, as we can set a breakpoint here to trace back
 * errors.*/

enum error
{
	ERROR_INTERNAL,
	ERROR_PARSER,
	ERROR_LEXER,
	ERROR_PRINTER,
	ERROR_OTHER,
	ERROR_ARGUMENTS
};

void error(enum error);

#endif
