#ifndef CONSTRUCTS_H_
#define CONSTRUCTS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct constant {
  enum {
    C_CONST_INTEGER,
    C_CONST_FLOATING,
    C_CONST_STRING,
    C_CONST_COMPOUND
  } type;
  union {
    intmax_t integer;
    long double floating;
    char const *string;
    struct {
      struct constant *vals;
      size_t len;
    } compound;
  } val;
};

enum var_storage {
  VAR_STOR_EXTERN,
  VAR_STOR_STATIC,
  VAR_STOR_GLOBAL,
  VAR_STOR_AUTO,
  VAR_STOR_TYPEDEF
};

enum type_type {
  TYPE_UNKNOWN,
  TYPE_VOID,
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRUC,
  TYPE_PTR,
  TYPE_FUNC
};

struct type_list {
  struct type *vals;
  size_t len;
};

struct var_list {
  struct var *vals;
  size_t len;
};

enum int_type { INT_CHAR, INT_SHORT, INT_INT, INT_LONG, INT_LONG_LONG };

union type_vals {
  struct type *under;
  struct {
    struct type_list members;
    bool is_unified;
  } comp;
  struct {
    bool is_signed : 1;
    enum int_type type;
  } int_;
  struct {
    struct type *under;
    size_t len;
  } array;
  struct {
    struct type *return_type;
    struct var_list params;
  } func;
};

struct type {
  enum type_type type;
  union type_vals vals;
};

struct var {
  enum var_storage storage;
  struct type type;
  char const *name;
};

#endif
