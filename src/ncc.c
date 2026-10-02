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
static char *seeker = line_buffer;
/*the linked list of vars lives here. It's a pointer so that the first var acts
 * the same as the lower ones.*/
static struct var *vars = NULL;

/*This function trims out the input until it reaches non whitespace.*/
int trim(void) {
  while (isspace(*seeker))
    /*upon a line feed (which, by standard, precedes the carriage return) we
     * must read a new line from the input*/
    if (*seeker == '\n') {
      if (!fgets(line_buffer, LINE_BUFFER_LEN, stdin))
        line_buffer[0] = '\0';
      seeker = line_buffer;
    } else
      ++seeker;
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

int stmt(void) { return 0; }

int func(void) {
  /* This function is given the responsibility of clearing the first
   * parentheses*/
  ++seeker;
  nccassert(trim());
  while (*seeker != ')') {
    /*TODO : implement function arguments*/
    nccassert(trim());
  }
  ++seeker;
  nccassert(trim());
  if (*seeker == '{') {
    ++seeker;
    nccassert(trim());
    while (*seeker != '}') {
      nccassert(stmt());
      nccassert(trim());
    }
    ++seeker;
  }
  return 0;
}

int expr(void) { return 0; }

int decl(void) {
  /*everything is a "word".*/
  int i;
  nccassert(trim());
  i = 0;
  while (*(seeker + i) == '_' || isalnum(*(seeker + i)))
    ++i;
  varpush();
  vars->name = malloc(i + 1);
  nccassert(!vars->name);
  memcpy(vars->name, seeker, i);
  vars->name[i] = '\0';

  seeker += i;

  vars->scope = scope;

  nccassert(trim());

  if (*seeker == '(') {
    nccassert(func());
    return 0;
  } else {
    if (*seeker == '=') {
      ++seeker;
      nccassert(trim());
      /* Since we are using a stack hungry policy for both expression results
       * and variables, not cleaning up the stack is the same as writing a
       * declaration.*/
      nccassert(expr());
    }

    if (*seeker == ';') {
      ++seeker;
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
    printf("var `%s`,\t", var->name);
    var = var->next;
  }
  printf("\n");
  return 0;
}

int main(void) {
  /*trim it at the beginning to kickstart the input*/
  nccassert(trim());
  while (*seeker) {
    /*in C, everything at the top level is a declaration*/
    nccassert(decl());
    nccassert(trim());
  }
  return 0;
}
