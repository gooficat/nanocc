#ifndef PRINTER_H_
#define PRINTER_H_
/* The printer is the module that outputs assembly. Each function it
 * exposes is called at the appropriate time by the parser. This allows us to
 * separate architecture-specific code from parsing cleanly and maintainably.*/

#include "constructs.h"

void printer_init(char const *);
void printer_comment(char const *);
void printer_declare(struct var *);

#endif
