#include "clex.h"
#include "parse.h"

int main()
{
	lexer_init("tests/a.c");
	parse("output.s");
}
