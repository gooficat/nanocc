
#include <stdio.h>
extern union {
	char str[8];
	long igr;
	double flt;
} TVALS;

main() {
	int tok;

rpt:
	tok = next();
	switch (tok) {
	case 0xEF:
		puts("eof");
		break;
	case 0xFD:
		printf("float %lf\n", TVALS.flt);
		goto rpt;
	case 0xFE:
		printf("intgr %ld\n", TVALS.igr);
		goto rpt;
	case 0xFF:
		printf("ident `%s`\n", TVALS.str);
		goto rpt;
	default:
		if (tok < 33)
			printf("token %s\n", TVALS.str);
		else
			printf("token `%c`\n", tok);
		goto rpt;
	}
	printf("Bye\n");
	fflush(stdout);
}
