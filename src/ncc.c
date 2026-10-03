#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Lines in C should not exceed this many characters. This is above the 509
 * character requirement of the ANSI C standard. 3 extra are kept for line feed,
 * carriage return, and null terminator.*/
#define LINE_BUFFER_LEN 512

/*A special assertion that just returns an error code from the current function
 * if we have an error on the one being called.*/
#define nccassert(f)                                                           \
  if (f) {                                                                     \
    return 1;                                                                  \
  }

/*linked list variable structure.*/
struct var {
  char *name;
  /*the scope depth associated with this variable.*/
  int scope;
  struct var *next;
};

/*current scope depth*/
static int scope = 0;
/*the actual line buffer we read to. We initialize it with a pretend line that
 * just looks like it's empty. This serves to "jump-start" the line feeding.
 * Otherwise it will try to read junk while checking for whitespace.*/
static char line_buffer[LINE_BUFFER_LEN] = {'\n'};
/*the seeker is the current read location in the line*/
static char *src = line_buffer;
/*the linked list of vars lives here. It's a pointer so that the first var acts
 * the same as the lower ones.*/
static struct var *vars = NULL;

/*This function trims out the input until it reaches non whitespace.*/
int trim(void) {
  while (isspace(*src))
    /*upon a line feed (which, by standard, precedes the carriage return) we
     * must read a new line from the input*/
    if (*src == '\n') {
      if (!fgets(line_buffer, LINE_BUFFER_LEN, stdin))
        line_buffer[0] = '\0';
      src = line_buffer;
    } else
      ++src;
  return 0;
}

/*push a new var onto the linked list*/
int varpush(void) {
  struct var *n;
  n = malloc(sizeof *n);
  nccassert(!n);
  n->next = vars;
  vars = n;
  return 0;
}

/*pop the top var off the list, deleting its contents*/
int varpop(void) {
  struct var *n;
  n = vars->next;
  free(vars->name);
  free(vars->next);
  vars = n;
  return 0;
}

int stmt(void) {
  /* For now, we just use the auto keyword for all scoped
   * variables. */
  if (src[0] == 'a' && src[1] == 'u' && src[2] == 't' && src[3] == 'o' &&
      (!isalnum(src[4]))) {
    src += 4;
    nccassert(trim());
    /*The declaration parser is used here.*/
    nccassert(decl());
  } else if (src[0] == 'r' && src[1] == 'e' && src[2] == 't' && src[3] == 'u' &&
             src[4] == 'r' && src[5] == 'n' && !isalnum(src[6])) {
    src += 6;
    nccassert(trim());
    nccassert(expr());
  } else {
    for (;;) {
      nccassert(expr());
      nccassert(trim());
      if (*src != ',') {
        break;
      }
      ++src;
      nccassert(trim());
    }
  }
  nccassert(trim());
  if (*src == ';') {
    ++src;
  }
  return 0;
}

int func(void) {
  /* This function is given the responsibility of clearing the first
   * parentheses*/
  ++src;
  ++scope; /* Increase the scope depth. We are no longer in static space.*/
  nccassert(trim());
  while (*src != ')') {
    /*TODO : implement function arguments*/
    nccassert(trim());
  }
  ++src;
  nccassert(trim());
  if (*src == '{') {
    ++src;
    nccassert(trim());
    while (*src != '}') {
      nccassert(stmt());
      nccassert(trim());
    }
    ++src;
  }
  return 0;
}

int expr(void) {
  if (*src == '(') {
    ++src;
    nccassert(expr());
    if (*src != ')') {
      return 1;
    }
    ++src;
  } else if (isdigit(*src)) {
    int n, p;
    n = 0;
    p = 1;
    while (isdigit(*src)) {
      n += (*src - '0') * p;
      p *= 10;
      ++src;
    }
  }
  return 0;
}

int decl(void) {
  /*everything is a "word".*/
  int i;
  nccassert(trim());
  i = 0;
  while (isalnum(*(src + i)))
    ++i;
  varpush();
  vars->name = malloc(i + 1);
  nccassert(!vars->name);
  memcpy(vars->name, src, i);
  vars->name[i] = '\0';

  src += i;

  vars->scope = scope;

  nccassert(trim());

  if (*src == '(') {
    if (scope) {
      /* if we are in a scope that isnt the top level then this is an error*/
      return 1;
    }
    nccassert(func());
    return 0;
  } else {
    if (*src == '=') {
      ++src;
      nccassert(trim());
      /* Since we are using a stack hungry policy for both expression results
       * and variables, not cleaning up the stack is the same as writing a
       * declaration.*/
      nccassert(expr());
    }

    if (*src == ';') {
      ++src;
      return 0;
    } else {
      return 1;
    }
  }
}

int printvars(void) {
  struct var *var;
  var = vars;
  while (var) {
    printf("var `%s` scope %i,\t", var->name, var->scope);
    var = var->next;
  }
  printf("\n");
  return 0;
}

int main(void) {
  /*trim it at the beginning to kickstart the input*/
  nccassert(trim());
  while (*src) {
    /*in C, everything at the top level is a declaration*/
    nccassert(decl());
    nccassert(trim());
  }
  printvars();
  return 0;
}
