#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Lines in C should not exceed this many characters. This is above the 509
 * character requirement of the ANSI C standard. 3 extra are kept for line feed,
 * carriage return, and null terminator.*/
#define LINE_BUFFER_LEN 512

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
 * just looks like it's empty*/
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
    if (*seeker == '\n')
      fgets(line_buffer, LINE_BUFFER_LEN, stdin);
    else
      ++seeker;
  return 0;
}

/*push a new var onto the linked list*/
int varpush(void) {
  struct var *n;
  n = malloc(sizeof *n);
  n->next = vars;
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

int decl(void) { /*everything is a "word".*/
  int i;
  i = 0;
  while (*++seeker == '_' || isalnum(*seeker))
    ;
  varpush();
  vars->name = malloc(i + 1);
  if (!vars->name) {
    return 1;
  }
  memcpy(vars->name, seeker, i);
  vars->name[i] = '\0';
  vars->scope = scope;
  return 0;
}

int main(void) {
  /*trim it at the beginning to kickstart the input*/
  if (trim()) {
    return 1;
  }
  while (*seeker) {
    /*in C, everything at the top level is a declaration*/
    if (decl()) {
      return 1;
    }
  }
  return 0;
}
