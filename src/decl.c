#include <stdio.h>
#include <string.h>

extern union {
	char str[8];
	long igr;
	double flt;
} TVALS;

static int SCOPE = 0;

enum {
	DECL_EXTRN,
	DECL_AUTO,
	DECL_STATIC,
};

struct decl {
	int type;
	int scope;
	char name[8];
};

extern struct decl DECLS[512];

dout(struct decl *d) {
	if (SCOPE > 0) {
		fprintf(stdout, "\t\t%s:\n", d->name);
	} else {
		fprintf(stdout, "push %%rax\n");
	}
	return 0;
}

decl() {
	struct decl *d;
	d = DECLS;
	while (*d->name)
		++d;
	if (next() != 0xFF) {
		return -1;
	}
	strcpy(d->name, TVALS.str);
	next();
	return 0;
}

func() {
	fprintf(stderr, "Function decl (unimplemented)\n");
	return -1;
	/*	while (decl())
		;*/
}

struct decl DECLS[512] = {0};
