#include "vec.h"
#include "mycc.h"
#include <stdlib.h>
#include <string.h>
#include "clex.h"

vec(char const *) labels;

struct scope {
  vec(struct c_var) vars;
  struct scope *previous;
};

struct scope scope;

void enter_scope(void)
{
  struct scope *previous;
  previous = malloc(sizeof *previous);
  if (previous == NULL)
  {
    exit(EXIT_FAILURE);
  }
  memcpy(previous, &scope, sizeof *previous);
  *previous = scope;
  scope.previous = previous;
  scope.vars = vec_create(struct c_var);
}

void parse(void)
{
  labels = vec_create(char const *);
  scope.vars = vec_create(struct c_var);
  scope.previous = NULL;

  while (lexer.token.type != TOK_EOF)
  {
    if (lexer.token.type < TOK_IDENTIFIER)
    {
      printf("%s\n", TOKENS[lexer.token.type]);
    }
    else
    {
      printf("token of %i\n", lexer.token.type);
    }
    lexer_next();
  }
}
