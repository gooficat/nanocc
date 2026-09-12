#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct arb_vec
{
  size_t len;
  size_t elem_size;
  size_t cap;  
};

void *vec_create(size_t size)
{
  struct arb_vec *vec;
  vec = malloc(sizeof *vec + size);
  vec->elem_size = size;
  vec->len = 0;
  vec->cap = 1;
  return vec + 1;
}

void *vec_push(void *vec, void const *val)
{
  struct arb_vec *arb_vec;
  arb_vec = (struct arb_vec *)vec - 1;  
  ++arb_vec->len;
  if (arb_vec->len >= arb_vec->cap)
  {
    arb_vec = realloc(arb_vec, ((sizeof *arb_vec) + (arb_vec->elem_size * (arb_vec->cap *= 2))));
  }
  memcpy(((char *)arb_vec) + ((arb_vec->len - 1) * arb_vec->elem_size), val, arb_vec->elem_size);
  return arb_vec + 1;
}

void *vec_pop(void *vec)
{
  --(((struct arb_vec *)vec) - 1)->len;
  return vec;
}

void *vec_free(void *vec)
{
  free(vec);
  return NULL;
}

size_t vec_len(void *vec)
{
  return (((struct arb_vec *)vec) - 1)->len;
}
