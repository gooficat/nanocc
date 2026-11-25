

#include "lexer.h"
#include "pool.h"
#include <stdio.h>

int main() {
	Pool pool = Pool_new();
	Lexer lexer = Lexer_new("tests/1.c", &pool);
	bool is_eof = false;
	while (!is_eof) {
		switch (lexer.token.type) {
		case TOK_AUTO:
			printf("AUTO");
			break;
		case TOK_PAREN_L:
			printf("PAREN_L");
			break;
		case TOK_PAREN_R:
			printf("PAREN_R");
			break;
		case TOK_SEMI:
			printf("SEMI");
			break;
		case TOK_IDENTIFIER:
			printf("IDENTIFIER `%s`", pool.idents.val[lexer.token.index]);
			break;
		case TOK_CONSTANT:
			printf("CONSTANT");
			break;
		case TOK_EOF:
			printf("EOF");
			is_eof = true;
			break;
		}
		putchar('\n');
		Lexer_next(&lexer, &pool);
	}
}
