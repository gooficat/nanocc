#ifndef CLEX_H
#define CLEX_H

#include <stdio.h>
#include <stddef.h>

#define LINE_BUFFER_LEN 4096

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

struct lexer
{
  struct file_state file;
  char buffer[LINE_BUFFER_LEN];
  char *seeker;
};

extern struct lexer lexer;

void lexer_open(char const *file_path);
void lexer_next(void);
void lexer_close(void);

#endif
