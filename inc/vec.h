#ifndef VEC_H
#define VEC_H

#include <stddef.h>

typedef struct
{
  size_t len;
  size_t el;
} gvec_s;

void *vec_init(size_t);
void *vec_push(void *, void *);

#define vec_to_gvec(v) ((gvec_s *)(v) - 1)
#define vec_len(v) vec_to_gvec(v)->len
#define vec_free(v) free(vec_to_gvec(v))

#endif
