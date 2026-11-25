

#include "context.h"
#include "lexer.h"
#include "pool.h"
#include <stdio.h>

int main() {
	Context ctx;

	ctx.pool = Pool_new();
	ctx.lexer = Lexer_new("tests/1.c", &ctx);

	while (ctx.lexer.token.type != TOK_EOF) {
		switch (ctx.lexer.token.type) {
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
			printf("IDENTIFIER `%s`",
				   ctx.pool.idents.val[ctx.lexer.token.index]);
			break;
		case TOK_CONSTANT:
			printf("CONSTANT");
			break;
		case TOK_EOF:
			printf("EOF");
			break;
		}
		putchar('\n');
		Lexer_next(&ctx.lexer, &ctx);
	}
}
