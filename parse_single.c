#include "vec.h"
#include "mycc.h"
#include <stdbool.h>
#include <stdio.h>
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

static struct c_var *find_var(char const *name)
{
  return NULL;
}

static bool c_types_same(struct c_type *a, struct c_type *b)
{
  if (a->type != b->type)
  {
    return false;
  }
  switch (a->type)
  {
  case C_TYPE_INT:
  case C_TYPE_FLOAT:
  case C_TYPE_VOID:
    return true;
  case C_TYPE_ARRAY:
    return c_types_same(a->val.array.type, b->val.array.type) == true && a->val.array.length == b->val.array.length;
  case C_TYPE_FUNCTION:
    if (c_types_same(a->val.function.return_type, b->val.function.return_type) || vec_len(a->val.function.params) != vec_len(b->val.function.params))
    {
      return false;
    }
    else
    {
      size_t i;
      for (i = 0; i < vec_len(a->val.function.params); ++i)
      {
        if (c_types_same(&a->val.function.params[i].c_type, &b->val.function.params[i].c_type))
        {
          return false;
        }
      }
      return true;
    }
  case C_TYPE_POINTER:
    return c_types_same(a->val.pointer.underlying, b->val.pointer.underlying) == true;
  default:
    exit(EXIT_FAILURE);
  }
}

static void add_var(struct c_var *var)
{
  struct c_var *existing;
  size_t i;
  existing = NULL;
  for (i = 0; i < vec_len(unit.scope.vars); ++i)
  {
    if (!strcmp(var->name, unit.scope.vars[i].name))
    {
      existing = &unit.scope.vars[i];
      break;
    }
  }
  if (existing != NULL) {
    if (unit.scope.previous != NULL)
    {
      if (var->type != C_VAR_EXTERN || existing->type != C_VAR_EXTERN || c_types_same(&var->c_type, &existing->c_type) == false)
      {
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      
    }
  }
  else
  {
    vec_push(unit.scope.vars, var);
  }
}

static void parse_decl(void)
{
  struct c_var var;
  parse_var(&var);

  if (lexer.token.type == TOK_BRACE_L)
  {
    size_t i;
    if (var.is_function == false || unit.scope.previous != NULL)
    {
      exit(EXIT_FAILURE);
    }
    lexer_next();
    enter_scope();
    for (i = 0; i < vec_len(var.c_type.val.function.params); ++i) {
      add_var(&var.c_type.val.function.params[i]);
    }
    while (lexer.token.type != TOK_BRACE_R)
    {
      parse_decl();
    }
    exit_scope();
  }
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
