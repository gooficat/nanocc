#include <stdio.h>
extern union {
	char str[8];
	long igr;
	double flt;
} TVALS;

main() {
	fprintf(stdout, ".code32\n"
					".text\n");
	while (!decl())
		;
}
