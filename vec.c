#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct arb_vec
{
  size_t len;
  size_t elem_size;
  size_t cap;  
  size_t id;
};

void *vec_create(size_t size)
{
  struct arb_vec *vec;
  static size_t id = 0;
  vec = malloc(sizeof *vec + size);
  if (vec == NULL)
  {
    printf("Alloc failed\n");
    exit(EXIT_FAILURE);
  }
  vec->elem_size = size;
  vec->len = 0;
  vec->cap = 1;
  vec->id = id++;
  return vec + 1;
}

void *vec_push(void *vec, void const *val)
{
  struct arb_vec *arb_vec;
  size_t old_len;
  arb_vec = (struct arb_vec *)vec - 1;  
  old_len = arb_vec->len++;
  while (arb_vec->len >= arb_vec->cap)
  {
    arb_vec->cap *= 2;
    arb_vec = realloc(arb_vec, ((sizeof *arb_vec) + (arb_vec->elem_size * arb_vec->cap)));
    if (arb_vec == NULL)
    {
      printf("Alloc failed\n");
      exit(EXIT_FAILURE);
    }
  }
  memcpy(&((char *)arb_vec)[sizeof *arb_vec + old_len * arb_vec->elem_size], val, arb_vec->elem_size);
  return arb_vec + 1;
}

void *vec_pop(void *vec)
{
  --((((struct arb_vec *)vec) - 1)->len);
  return vec;
}

void *vec_free(void *vec)
{
  free(((struct arb_vec *)vec) - 1);
  return NULL;
}

size_t vec_len(void *vec)
{
  return (((struct arb_vec *)vec) - 1)->len;
}
