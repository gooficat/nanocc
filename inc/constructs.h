#ifndef CONSTRUCTS_H_
#define CONSTRUCTS_H_

#include <bits/stdint-uintn.h>
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

enum variable_storage {
  VAR_STOR_EXTERN,
  VAR_STOR_STATIC,
  VAR_STOR_GLOBAL,
  VAR_STOR_AUTO,
  VAR_STOR_TYPEDEF
};

struct type {
  uint8_t is_const : 1;
  uint8_t is_volatile : 1;
  uint8_t is_restrict : 1;
  uint8_t is_atomic : 1;
  uint8_t longness : 2;
};

struct variable {
  enum variable_storage storage;
  struct type type;
  char const *name;
};

#endif
