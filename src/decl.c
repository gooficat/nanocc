#include "tok.h"
#include <stdio.h>
#include <stdlib.h>

static int SCOPE = 0;

enum {
	DECL_EXTRN,
	DECL_AUTO,
	DECL_STATIC,
};

struct decl {
	int type;
	int scope;
	char const *name;
	struct decl *next;
};

struct decl DECL;

dout(struct decl *d) {
	struct decl *b;
	b = malloc(sizeof *b);
	*b = DECL;
	DECL.scope = SCOPE;
	DECL = *d;
	DECL.next = d;
}

decl() {
	next();
	struct decl n_decl;

	if (TOK.type == TOK_EOF) {
		return 0;
	}

	if (TOK.type == TOK_EXTRN) {
		next();
		n_decl.type = DECL_EXTRN;
	} else if (TOK.type == TOK_AUTO) {
		next();
		n_decl.type = DECL_AUTO;
	} else {
		n_decl.type = DECL_STATIC;
	}

	if (TOK.type == TOK_PAREN_L) {
		func();
		return 1;
	}

	for (;;) {
		n_decl.name = TOK.str;
		next();

		dout(&n_decl);

		if (TOK.type == TOK_SEMI) {
			break;
		}
	}

	next(); /* ; */

	return 2;
}

func() {
	fprintf(stderr, "Function decl (unimplemented)\n");
}
