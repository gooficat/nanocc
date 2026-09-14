#include <stdbool.h>
#include "clex.h"


bool token_is_type(int token)
{
  return (token >= TOK_INT && token <= TOK_VOID) || token == TOK_ASTER;
}
bool token_is_operator(int token)
{
  return (token >= TOK_PLUS && token <= TOK_AMPER);
}
bool token_is_prefix(int token)
{
  return token == TOK_PLUS || token == TOK_MINUS;
}
bool token_is_postfix(int token)
{
  return false;
}
