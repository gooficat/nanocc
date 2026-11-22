#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern union {
	char str[8];
	long igr;
	double flt;
} TVALS;

static int SCOPE = 0;
static unsigned long long STKD = 0;
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

struct labl {
	char name[8];
	int disam;
};

int DISAM = 0;

struct decl DECLS[512] = {0};
struct labl LABLS[512] = {0};
struct decl *DECL = DECLS;
struct labl *LABL = LABLS;

fdecl(int ft) {
	struct decl *d;
	d = DECL++;
	switch (ft) {
	case 0: /* extrn */
		d->type = 0;
		break;
	case 1: /* auto */
		d->type = 1;
		break;
	case 0xEF: /* eof */
		return -1;
	default:
		goto ident;
	}
	next();
ident:
	strcpy(d->name, TVALS.str);
	switch (next()) {
	case '(':
		fprintf(stdout, ".globl %s\n\t\t%s:\n", d->name, d->name);
		return func();
	case '=':
		if (SCOPE)
			fprintf(stdout, "\t\t.globl %s\n\t\t%s:\n", d->name, d->name),
				expr();
		else
			cval(), fprintf(stdout, "\tpush %%eax\n"), ++STKD;
	default:
		return 0;
	}
}

decl() {
	return fdecl(next());
}

cval() {
	return 0;
}

fexpr(int tf) {
	switch (tf) {
	case 0xFD:
		exit(EXIT_FAILURE);
	case 0xFE:
		fprintf(stdout, "\tmov $%ld, %%eax\n", TVALS.igr);
		break;
	case 0xFF: {
		printf("Ident\n");
		exit(EXIT_FAILURE);
	}
	}
	return 0;
}

expr() {
	return fexpr(next());
}

upon(int t, char *n) {
	struct decl *d;
	long long p;

	for (d = DECL; strcmp(n, d->name); --d)
		if (d->type == 1)
			++p;
	switch (t) {
	case '(': {
		int i;
		i = args();
		if (d->type == 1)
			fprintf(stdout, "\tcall %lld(%%esp)\n", p * 4);
		else
			fprintf(stdout, "\tcall %s\n", n);
		fprintf(stdout, "\tadd $%d, %%esp\n", i);
	} break;
	case '=':
		expr();
		break;
	default:
		return 1;
	}
	return 0;
}

func() {
	scenter();
	while (next() != ')')
		strcpy(DECL->name, TVALS.str), DECL->type = 1, DECL->scope = SCOPE,
									   ++DECL, next();
	next(); /* { */
	while (!stmt())
		;
	return 0;
}

rargs(int *i) {
	++*i;
	expr();
	if (next() != ')') {
		rargs(i);
	}
	fputs("\tpush %eax\n", stdout);
	return 0;
}
args() {
	int i;
	return i = 0, rargs(&i), i;
}

stmt() {
	int t;
	struct labl *l;
	switch (t = next()) {
	case 2:
		expr(), scexit(), LABL = LABLS, fputs("\tret\n", stdout);
		break;
	case 3: {
		l = LABLS;
		next();
		while (strcmp(l->name, TVALS.str))
			--l;
		fprintf(stdout, "\tgoto %s%d\n", l->name, l->disam);
		break;
	}
	case 0xFF: {
		char n[8];
		strcpy(n, TVALS.str);
		if ((t = next()) == ':')
			fprintf(stdout, "\t\t%s%d:\n", l->name, l->disam),
				LABL->disam = DISAM++, strcpy(LABL++->name, n);
		else
			upon(t, n);
		return 0;
	}
	case 0:
	case 1:
		fdecl(t);
		break;
	case ';':
		return stmt();
	case 0xEF:
	case '}':
		return 1;
	default:
		fexpr(t);
		break;
	}
	return 0;
}

scenter() {
	++SCOPE;
	fputs("\tpush %ebp\n"
		  "\tmov %esp, %ebp\n",
		  stdout);
	return 0;
}

scexit() {
	struct decl *d;
	d = DECLS;
	--SCOPE;
	while (d < DECLS) {
		if (d->scope > SCOPE)
			d->name[0] = 0;
		++d;
	}

	fputs("\tmov %ebp, %esp\n"
		  "\tpop %ebp\n",
		  stdout);
	return 0;
}
