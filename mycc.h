#ifndef MYCC_H
#define MYCC_H

#include <stdbool.h>
#include <stddef.h>

#include "vec.h"

enum c_type_type
{
  C_TYPE_INT,
  C_TYPE_FLOAT,
  C_TYPE_VOID,
  C_TYPE_POINTER,
  C_TYPE_STRUCT_OR_UNION,
  C_TYPE_ARRAY,
};

struct c_type_int
{
  bool is_affirmed : 1;
  bool is_long : 1;
  bool is_huge : 1;
  bool is_short : 1;
  bool is_signed : 1;
  bool is_unsigned : 1;
};

struct c_type_float
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

union c_type_val
{
  struct c_type_int int;
  struct c_type_float float;
  struct c_type_pointer pointer;
  struct c_type_struct_or_union struct_or_union;
  struct c_type_array c_type_array;
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
  struct c_type type;
};


#endif
