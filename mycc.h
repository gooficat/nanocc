#ifndef MYCC_H
#define MYCC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vec.h"

enum c_type_type
{
  C_TYPE_UNKNOWN,
  C_TYPE_INT,
  C_TYPE_FLOAT,
  C_TYPE_VOID,
  C_TYPE_POINTER,
  C_TYPE_STRUCT_OR_UNION,
  C_TYPE_ARRAY,
  C_TYPE_FUNCTION,
};

struct c_type_integer
{
  bool is_affirmed : 1;
  bool is_long : 1;
  bool is_huge : 1;
  bool is_short : 1;
  bool is_signed : 1;
  bool is_unsigned : 1;
};

struct c_type_floating
{
  bool is_double : 1;
  bool is_long : 1;
};

struct c_type_pointer
{
  struct c_type *underlying;
};

struct c_type_struct_or_union
{
  vec(struct c_type) members;
  bool is_union;
};

struct c_type_array
{
  struct c_type *type;
  size_t length;
};

struct c_type_function
{
  struct c_type *return_type;
  vec(struct c_var) params;
};

union c_type_val
{
  struct c_type_integer integer;
  struct c_type_floating floating;
  struct c_type_pointer pointer;
  struct c_type_struct_or_union struct_or_union;
  struct c_type_array array;
  struct c_type_function function;
};

struct c_type
{
  enum c_type_type type;
  union c_type_val val;
};

enum c_var_type
{
  C_VAR_EXTERN,
  C_VAR_GLOBAL,
  C_VAR_AUTO,
  C_VAR_STATIC
};

struct c_var
{
  enum c_var_type type;
  char const *name;
  struct c_type c_type;
  bool is_function;
};

enum c_const_type
{
  C_CONST_INTEGER,
  C_CONST_FLOATING,
  C_CONST_STRING,
  C_CONST_COMPOUND,
  C_CONST_ARRAY
};

struct c_const_integer
{
  intmax_t integer;
};

struct c_const_floating
{
  long double value;
};

struct c_const_string
{
  char* value;
};

struct c_const_compound
{
  vec(struct c_const) values;
};

union c_const_val
{
  struct c_const_integer integer;
  struct c_const_floating floating;
  struct c_const_string string;
  struct c_const_compound compound;
};

struct c_const
{
  enum c_const_type type;
  union c_const_val val;
};

#endif
