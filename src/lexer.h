#pragma once

#include <stdio.h>

#define LINE_BUF_MAX 4096

typedef enum {
#define TOK(id, str) TOK_##id,
#include "toks.h"
#undef TOK
	TOK_IDENTIFIER,
	TOK_CONSTANT,
	TOK_EOF,
} TokenType;

typedef struct {
	TokenType type;
	size_t index;
} Token;

typedef struct {
	Token token;
	char line_buf[LINE_BUF_MAX];
	size_t line_num;
	size_t col_num;
	FILE *file_ptr;
	char const *file_path;
} Lexer;

struct Context;

Lexer Lexer_new(char const *file_path, struct Context *ctx);
Token Lexer_peek(Lexer const *lexer);
Token Lexer_consume(Lexer *lexer, struct Context *ctx);
Token Lexer_next(Lexer *lexer, struct Context *ctx);
void Lexer_close(Lexer *lexer);

extern char const *const TOKS[];
