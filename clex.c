#include "clex.h"
#include <stddef.h>

struct lexer lexer;

static void lexer_get_buffer(void)
{
  ++lexer.file.line_num;
  lexer.seeker = lexer.buffer;
  if (fgets(lexer.buffer, LINE_BUFFER_LEN, lexer.file.fptr) == NULL)
  {
    /* error */
  }
}

void lexer_init(char const *file_path)
{
  lexer.file.fptr = fopen(file_path, "r");
  if (lexer.file.fptr == NULL)
  {
    /* error */
  }
  lexer.file.line_num = 0;
  
  lexer_get_buffer();
}

void lexer_next(void)
{
  
}

void lexer_close(void)
{
  	
}
