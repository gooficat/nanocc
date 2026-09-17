#include "vec.h"
#include "mycc.h"
#include <stdbool.h>
#include <stdint.h>
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
  FILE *file;
};

static struct parse_unit unit;

static size_t size_of_type(struct c_type *type)
{
  switch (type->type)
  {
    case C_TYPE_UNKNOWN:
      printf("Cannot find type of unknown\n");
      exit(EXIT_FAILURE);
    case C_TYPE_INT:
      return 4; /* TODO */
    case C_TYPE_FLOAT:
      return 4;
    case C_TYPE_VOID:
      printf("Cannot find type of unknown\n");
      exit(EXIT_FAILURE);
    case C_TYPE_POINTER:
      return 8;
    case C_TYPE_STRUCT_OR_UNION: {
        size_t size;
        size_t i;
        size = 0;
        for (i = 0; i < vec_len(type->val.struct_or_union.members); ++i)
        {
          size_t type_size;
          type_size = size_of_type(&type->val.struct_or_union.members[i]);
          size += type_size + (8 - (type_size % 8));
        }
        return size;
      };
    case C_TYPE_ARRAY:
      return size_of_type(type->val.array.type) * type->val.array.length;
    case C_TYPE_FUNCTION:
      return 8;
  }
}

static void parse_var(struct c_var *var)
{
  memset(var, 0, sizeof *var);

  while (token_is_type(lexer.token.type) == true)
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
          printf("Type error: type is not assignable as integer.\n");
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
          printf("Type error: Char can only be char!\n");
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
          printf("Type error: float cannot have these specifiers!\n");
          exit(EXIT_FAILURE);
        }
        break;
      case TOK_VOID:
        if (var->c_type.type == C_TYPE_UNKNOWN)
        {
          var->c_type.type = C_TYPE_VOID;
        }
        else
        {
          printf("Type error: void cannot have such properties!\n");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        printf("Token %i not allowed here\n", lexer.token.type);
        exit(EXIT_FAILURE);
    }
    lexer_next();
    if (lexer.token.type == TOK_ASTER)
    {
      struct c_type *under;
      under = malloc(sizeof *under);
      if (!under)
      {
        printf("Alloc failed\n");
        exit(EXIT_FAILURE);
      }
      memcpy(under, &var->c_type, sizeof *under);
      var->c_type.type = C_TYPE_POINTER;
      var->c_type.val.pointer.underlying = under;
      lexer_next();
    }
  }
  if (lexer.token.type == TOK_IDENTIFIER)
  {
    printf("named %s (%zu)\n", lexer.pool.identifiers[lexer.token.index], lexer.token.index);
    var->name = lexer.pool.identifiers[lexer.token.index];
    lexer_next();
  }
  else
  {
    printf("No name given, %i\n", lexer.token.type);
  }
  if (lexer.token.type == TOK_PAREN_L)
  {
    var->is_function = true;
    {
      struct c_type return_type;
      return_type = var->c_type;
      var->c_type.type = C_TYPE_FUNCTION;
      var->c_type.val.function.return_type = malloc(sizeof *var->c_type.val.function.return_type);
/*      if ()*/
      memcpy(var->c_type.val.function.return_type, &return_type, sizeof *var->c_type.val.function.return_type);
    }
    lexer_next();
    var->c_type.val.function.params = vec_create(struct c_var);
    while (lexer.token.type != TOK_PAREN_R)
    {
      struct c_var param;
      parse_var(&param);
      vec_push(var->c_type.val.function.params, &param);
    }
    lexer_next();
  }
}

static void enter_scope(void)
{
  struct scope *previous;
  previous = malloc(sizeof *previous);
  if (previous == NULL)
  {
    printf("Alloc failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(previous, &unit.scope, sizeof *previous);
  *previous = unit.scope;
  unit.scope.previous = previous;
  unit.scope.vars = vec_create(struct c_var);
}

static void gen_frame(void)
{
  size_t len;
  if (vec_len(unit.scope.vars) != 1 || unit.scope.vars[0].c_type.type != C_TYPE_VOID)
  {
    size_t i;
    len = 0;
    for (i = 0; i < vec_len(unit.scope.vars); ++i)
    {
      size_t type_len;
      type_len += size_of_type(&unit.scope.vars[i].c_type);
      len += type_len + (8 - (type_len % 8));
    }
    fprintf(unit.file, "\tsub %zu, %%rsp\n", len);
  }
  fprintf(unit.file, "\tpush %%rbp\n"
                     "\tmov %%rbp, %%rsp\n");
}

static void exit_scope(void)
{
  struct scope *previous;
  previous = unit.scope.previous;
  if (previous == NULL)
  {
    printf("Exiting into nothing!\n");
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
    printf("Type incompatibility\n");
    exit(EXIT_FAILURE);
  }
}

static void c_type_free(struct c_type *type)
{
  size_t i;
  i = 0;
  switch (type->type)
  {
    case C_TYPE_FUNCTION:
      while (i < vec_len(type->val.function.params))
      {
        c_type_free(&type->val.function.params[i].c_type);
      }
      vec_free(type->val.function.params);
      break;
    case C_TYPE_POINTER:
      c_type_free(type->val.pointer.underlying);
      free(type->val.pointer.underlying);
      break;
    case C_TYPE_STRUCT_OR_UNION:
      while (i < vec_len(type->val.struct_or_union.members))
      {
        c_type_free(&type->val.struct_or_union.members[i]);
      }
      vec_free(type->val.struct_or_union.members);
      break;
    default:
      break;
  }
}

static void c_var_free(struct c_var *var)
{
  c_type_free(&var->c_type);
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
    if (!c_types_same(&var->c_type, &existing->c_type))
    {
      printf("Existing not same!\n");
      exit(EXIT_FAILURE);
    }
    else
    {
      if (unit.scope.previous != NULL)
      {
        if (var->type != C_VAR_EXTERN || existing->type != C_VAR_EXTERN)
        {
          printf("Either must be extern");
          exit(EXIT_FAILURE);
        }
      }
      else
      {
        if (var->type != C_VAR_EXTERN)
        {
          if (existing->type != C_VAR_EXTERN)
          {
            printf("Both must be extern");
            exit(EXIT_FAILURE);
          }
          else
          {
            c_var_free(existing);
            *existing = *var;
          }
        }
      }
    }
  }
  else
  {
    vec_push(unit.scope.vars, var);
  }
}

static void output_constant(struct c_const const *const constant)
{
  switch (constant->type)
  {
    case C_CONST_INTEGER:
      fprintf(unit.file, "\tmov $%jd, %%rax\n", constant->val.integer.integer);
      break;
    default:
      printf("Unimplemented constant category");
      exit(EXIT_FAILURE);
  }
}

static void parse_expr(void)
{
  switch (lexer.token.type)
  {
    case TOK_CONSTANT:
      output_constant(&lexer.pool.constants[lexer.token.index]);
      lexer_next();
      break;
    default:
      printf("expr not compatible with this");
      exit(EXIT_FAILURE);
  }
}

static void parse_order(void)
{
  switch (lexer.token.type)
  {
    case TOK_RETURN:
      lexer_next();
      parse_expr();
      fprintf(unit.file, "\tpop %%rbp\n"
                         "\tret\n");
      break;
    case TOK_GOTO:
      lexer_next();
      fprintf(unit.file, "\tgoto %s\n", lexer.pool.identifiers[lexer.token.index]);
      break;
    default:
      exit(EXIT_FAILURE);
  }
}

static void parse_decl(void);

static void parse_stmt(void)
{
  if (lexer.token.type >= TOK_RETURN && lexer.token.type <= TOK_GOTO)
  {
    parse_order();
  }
  else if (lexer.token.type == TOK_BRACE_L)
  {
    enter_scope();
    lexer_next();
    while (lexer.token.type != TOK_BRACE_R)
    {
      parse_stmt();
    }
    lexer_next();
    exit_scope();
  }
  else
  {
    printf("Unimplemented\n");
    exit(EXIT_FAILURE);
  }
  lexer_next();
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
    fprintf(unit.file, "%s:\n", var.name);
    gen_frame();
    while (lexer.token.type != TOK_BRACE_R)
    {
      parse_stmt();
    }
    fprintf(unit.file, "\tpop %%rbp\n"
           "\tret\n");
    exit_scope();
  }
  else if (lexer.token.type == TOK_COMMA)
  {
    printf("Unimplemented\n");
    exit(EXIT_FAILURE);
  }
  else if (lexer.token.type != TOK_SEMI)
  {
    printf("%i is not a semicolon!\n", lexer.token.type);
    exit(EXIT_FAILURE); 
  }
  lexer_next();
}

void parse(char const *input_path)
{
  unit.file = fopen(input_path, "w");
  if (unit.file == NULL)
  {
    printf("File invalid or failed to open\n");
    exit(EXIT_FAILURE);
  }
  unit.labels = vec_create(char const *);
  unit.scope.vars = vec_create(struct c_var);
  unit.scope.previous = NULL;

  while (lexer.token.type != TOK_EOF)
  {
    parse_decl();
  }
  fclose(unit.file);
}
