#pragma once

enum {
	TOK_EOF,
	TOK_EXTRN,
	TOK_AUTO,
	TOK_SEMI,
	TOK_PAREN_L,
	TOK_PAREN_R,
};

struct tok {
	int type;
	char const *str;
};
extern struct tok TOK;
