#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union {
	char str[8];
	long igr;
	double flt;
} TVALS;

/* may not exceed 33 identifying keywords lest it encroach upon other symbols*/
next() {
	static char line[80] = {'\n'};
	static char *skr = line;
	static char const *const KWORDS[] = {"auto", "return", "goto", NULL};
	int i;
rpt:
	/* number */
	if (isdigit(*skr)) {
		/* alphanumeric, because of `0x` and potentially `0b` */
		for (i = 0; isalnum(skr[++i]);)
			;
		if (skr[i] == '.')
			return TVALS.flt = strtod(skr, &skr), 0xFD; /* flt */
		return TVALS.igr = strtol(skr, &skr, 0), 0xFE;	/* igr */
	}
	/* identifier may start with _ or an alpha character (fixed, was isalnum,
	 * which would be wrong) */
	if (*skr == '_' || isalpha(*skr)) {
		/* identifiers cap out at 7, they fit in a buffer of 8 chars */
		for (i = 0; i < 7 && (skr[i] == '_' || isalnum(skr[i])); ++i)
			TVALS.str[i] = skr[i];
		TVALS.str[i] = 0;
		skr += i;
		for (i = 0; KWORDS[i]; ++i)
			if (!strcmp(TVALS.str, KWORDS[i]))
				return i;
		return 0xFF; /* identifier */
	} else
		switch (*skr) {
		case '\0':
		eof:
			return 0xEF; /*eof */
		case '\n':
			if (!fgets(line, 80, stdin))
				goto eof;
			skr = line;
			break;
		case '\t':
		case '\b':
		case '\r':
		case ' ':
			++skr;
			break;
		default:
			return *skr++;
		}
	goto rpt;
}
