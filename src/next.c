#include "tok.h"
#include <ctype.h>
#include <stdio.h>
#define BUF_MAX 80

struct tok TOK;

char BUF[BUF_MAX] = {[0] = '\n'};
char *SKR = BUF;

next() {
	while (*SKR) {
		if (*SKR == '\n') {
			fgets(BUF, BUF_MAX, stdin);
			SKR = BUF;
		} else if (isspace(*SKR)) {
			++SKR;
		} else if (isdigit(*SKR)) {

		} else {
		}
	}
	TOK.type = TOK_EOF;
}
