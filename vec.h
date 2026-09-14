#ifndef VEC_H
#define VEC_H

#include <stddef.h>

void *vec_create(size_t size);
void *vec_push(void *vec, void const *val);
void *vec_pop(void *vec);
void *vec_free(void *vec);
size_t vec_len(void *vec);

#define vec(type) type *
#define vec_create(type) vec_create(sizeof(type))
#define vec_push(vec, value) ((vec) = vec_push((vec), (value)))
#define vec_pop(vec) ((vec) = vec_pop(vec))
#define vec_free(vec) ((vec) = vec_free(vec))

#endif
