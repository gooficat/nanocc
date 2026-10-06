#ifndef LEXER_H_
#define LEXER_H_

/* The lexer reads from the file and generates tokens. It sends symbols and
 * constants to the pool to conserve memory both in the compilation process and
 * in the outputted assembly. */

#include "pool.h"
#include <stdio.h>

#define LEXER_BUFFER_LEN 4096

struct lexer {
  FILE *file;
  char buffer[LEXER_BUFFER_LEN];
  char *src;
  size_t line_num;
  char const *const file_path;
};

extern struct lexer LEXER;

void lexer_init(char const *);
void lexer_next(void);
void lexer_close(void);

#endif
