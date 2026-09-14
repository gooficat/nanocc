#ifndef CLEX_H
#define CLEX_H

#include <stdio.h>
#include <stddef.h>
#include "mycc.h"
#include "vec.h"

#define LINE_BUFFER_LEN 4096

extern char const * const TOKENS[];

struct file_state 
{
  FILE *fptr;
  size_t line_num;
};

enum token_type
{

#define TOK(id, str) TOK_##id,
#include "tokens.h"
#undef TOK

  TOK_IDENTIFIER,
  TOK_CONSTANT,
  TOK_EOF
};

struct token
{
  enum token_type type;
  size_t index;
};

struct pool
{
  vec(struct c_const) constants;
  vec(char const *) identifiers;
};

struct lexer
{
  struct token token;
  struct pool pool;
  struct file_state file;
  char buffer[LINE_BUFFER_LEN];
  char *seeker;
};

extern struct lexer lexer;

void lexer_init(char const *file_path);
void lexer_next(void);
void lexer_close(void);

#endif
