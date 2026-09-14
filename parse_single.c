#include "vec.h"
#include "mycc.h"
#include <stdlib.h>
#include <string.h>
#include "clex.h"
#include "token_types.h"

struct scope {
  vec(struct c_var) vars;
  struct scope *previous;
};

struct parse_unit {
  struct scope scope;
  vec(char const *) labels;
};

static struct parse_unit unit;

static void parse_var(struct c_var *var)
{
  memset(var, 0, sizeof *var);
  while (token_is_type(lexer.token.type))
  {
    switch (lexer.token.type)
    {
      case TOK_INT:
        if (var->c_type.type == C_TYPE_UNKNOWN)
        {
          var->c_type.type = C_TYPE_INT;
        }
        else if (var->c_type.type != C_TYPE_INT || var->c_type.val.integer.is_affirmed == true)
        {
          exit(EXIT_FAILURE);
        }
        break;
      case TOK_CHAR:
        if (var->c_type.type == C_TYPE_UNKNOWN)
        {
          var->c_type.type = C_TYPE_INT;
        }
        else if (var->c_type.type != C_TYPE_INT ||
                var->c_type.val.integer.is_affirmed ||
                var->c_type.val.integer.is_long ||
                var->c_type.val.integer.is_short)
        {
          exit(EXIT_FAILURE);
        }
        break;
      case TOK_FLOAT:
        if (var->c_type.type == C_TYPE_UNKNOWN)
        {
          var->c_type.type = C_TYPE_FLOAT;
        }
        else
        {
          exit(EXIT_FAILURE);
        }
      case TOK_VOID:
        if (var->c_type.type == C_TYPE_UNKNOWN)
        {
          var->c_type.type = C_TYPE_VOID;
        }
        else
        {
          exit(EXIT_FAILURE);
        }
      default:
        exit(EXIT_FAILURE);
    }
  }
}

static void enter_scope(void)
{
  struct scope *previous;
  previous = malloc(sizeof *previous);
  if (previous == NULL)
  {
    exit(EXIT_FAILURE);
  }
  memcpy(previous, &unit.scope, sizeof *previous);
  *previous = unit.scope;
  unit.scope.previous = previous;
  unit.scope.vars = vec_create(struct c_var);
}

static void exit_scope(void)
{
  struct scope *previous;
  previous = unit.scope.previous;
  if (previous == NULL)
  {
    exit(EXIT_FAILURE);
  }
  unit.scope = *previous;
  free(previous);
}

static void parse_decl(void)
{
  struct c_type type;
  parse_type(&type);
}

void parse(void)
{
  unit.labels = vec_create(char const *);
  unit.scope.vars = vec_create(struct c_var);
  unit.scope.previous = NULL;

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
