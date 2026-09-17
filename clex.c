#include "clex.h"
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "mycc.h"

struct lexer lexer;

static void lexer_get_buffer(void)
{
  ++lexer.file.line_num;
  lexer.seeker = lexer.buffer;
  if (fgets(lexer.buffer, LINE_BUFFER_LEN, lexer.file.fptr) == NULL)
  {
    lexer.buffer[0] = '\0';
  }
  else
  {
    if (lexer.buffer[strlen(lexer.buffer) - 1] != '\n')
    {
      printf("Line too long!\n");
      exit(EXIT_FAILURE);
    }
  }
}

void lexer_init(char const *file_path)
{
  lexer.file.fptr = fopen(file_path, "r");
  if (lexer.file.fptr == NULL)
  {
    printf("Error opening file\n");
    exit(EXIT_FAILURE);
  }
  lexer.file.line_num = 0;
  lexer.pool.constants = vec_create(struct c_const);
  lexer.pool.identifiers = vec_create(char const *);
  lexer_get_buffer();
  lexer_next();
}

static void lexer_add_constant(struct c_const const *constant)
{
  lexer.pool.constants = vec_push(lexer.pool.constants, constant);
}

static void lexer_add_identifier(char *const *identifier)
{
  lexer.pool.identifiers = vec_push(lexer.pool.identifiers, identifier);
}

void lexer_next(void)
{
  while (isspace(*lexer.seeker))
  {
    if (*lexer.seeker == '\n')
    {
      lexer_get_buffer();
    }
    else {
      ++lexer.seeker;
    }
  }
  if (*lexer.seeker == '\0')
  {
    lexer.token.type = TOK_EOF;
  }
  else if (*lexer.seeker == '"')
  {
    int i;
    char *str;
    ++lexer.seeker;
    i = 0;
    while (*(lexer.seeker + i) == '\\' || *(lexer.seeker + ++i) != '"');
    str = malloc(i + 1);
    if (str == NULL)
    {
      printf("Alloc failed\n");
      exit(EXIT_FAILURE);
    }
    memcpy(str, lexer.seeker, i);
    str[i] = '\0';
    lexer.seeker += i + 1;
  }
  else if (*lexer.seeker == '\'')
  {
    struct c_const constant;
    /* temporary */
    constant.type = C_CONST_INTEGER;
    ++lexer.seeker;
    if (*lexer.seeker == '\\')
    {
      ++lexer.seeker;
      switch (*lexer.seeker)
      {
        case '0':
          constant.val.integer.integer = '\0';
          break;
        case 'n':
          constant.val.integer.integer = '\n';
          break;
        default:
          printf("Bad escape sequence\n");
          exit(EXIT_FAILURE);
      }
    }
    else
    {
      constant.val.integer.integer = *lexer.seeker;
    }
    lexer.seeker += 2;
    lexer_add_constant(&constant);
  }
  else if (isdigit(*lexer.seeker))
  {
    struct c_const constant;
    char const *ahead;
    ahead = lexer.seeker;
    while (isdigit(*(ahead++)));
    if (*ahead == '.')
    {
      constant.type = C_CONST_FLOATING;
      constant.val.floating.value = strtold(lexer.seeker, &lexer.seeker);
    }
    else
    {
      constant.type = C_CONST_INTEGER;
      constant.val.integer.integer = strtoll(lexer.seeker, &lexer.seeker, 0);
    }
    lexer_add_constant(&constant);
  }
  else
  {
    int len;
    for (lexer.token.type = 0; TOKENS[lexer.token.type]; ++lexer.token.type)
    {
      len = strlen(TOKENS[lexer.token.type]);
      if (memcmp(lexer.seeker, TOKENS[lexer.token.type], len) == 0 &&
        ((lexer.seeker[len] == '_' || isalnum(lexer.seeker[len])) == 0 ||
        (lexer.seeker[0] == '_' || isalnum(lexer.seeker[0])) == 0))
      {
        goto token_found;
      }
    }
    for (len = 1; lexer.seeker[len] == '_' || isalnum(lexer.seeker[len]); ++len);
    for (lexer.token.index = 0; lexer.token.index < vec_len(lexer.pool.identifiers); ++lexer.token.index)
    {
      if (memcmp(lexer.seeker, lexer.pool.identifiers[lexer.token.index], len) == 0 &&
          lexer.pool.identifiers[lexer.token.index][len] == '\0')
      {
        goto token_found;
      }
    }
    {
      char *identifier;
      identifier = malloc(len + 1);
      memcpy(identifier, lexer.seeker, len);
      identifier[len] = '\0';
      lexer_add_identifier(&identifier);
      printf("Index of ident thingy is %zu %s %s\n", lexer.token.index, identifier, lexer.pool.identifiers[lexer.token.index]);
    }
token_found:
  lexer.seeker += len;
  }
}

void lexer_close(void)
{
  fclose(lexer.file.fptr);
  vec_free(lexer.pool.constants);
  vec_free(lexer.pool.identifiers);
}
