#include <stdio.h>
extern union {
	char str[8];
	long igr;
	double flt;
} TVALS;

main() {
	fprintf(stdout, ".code64\n"
					".text\n");
	while (!decl())
		;
}
