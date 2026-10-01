#include "vec.h"
#include <stdlib.h>

void *vec_init(size_t e)
{
  gvec_s *v;
  v = malloc(sizeof *v + e);
  v->el = e;
  v->len = 0;
  return v;
}

void *vec_push(void *v, void *e)
{
  void *n;
  n = realloc(v, );
}
