#ifndef POOL_H_
#define POOL_H_

#include "constructs.h"
#include <stddef.h>

struct pool
{
	struct
	{
		char const **vals;
		size_t len;
	} identifiers;
	struct
	{
		struct constant *vals;
		size_t len;
	} constants;
};

extern struct pool POOL;

void pool_init(void);
void pool_add_identifier(char const *);
void pool_add_constant(struct constant *);
/* TODO tweak to add an error state */
size_t pool_find_identifier(char const *);
size_t pool_find_constant(struct constant *);
void pool_close(void);

#endif
