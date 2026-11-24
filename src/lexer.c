#include "lexer.h"
#include "error.h"
#include <ctype.h>
#include <stdio.h>

static void Lexer_get_line(Lexer *lexer) {
	if (!fgets(lexer->line_buf, LINE_BUF_MAX, lexer->file_ptr)) {
		Error_terminate((Error){
			.file_path = lexer->file_path,
			.type = ERROR_SYNTAX,
			.line = lexer->line_num,
			.col = 0,
			.msg = "Failed to fetch new line. Maybe your file doesn't "
				   "end in an empty new line?",
		});
	}
	lexer->col_num = 0;
	lexer->line_num += 1;
}

Lexer Lexer_new(char const *file_path) {
	Lexer lexer;
	lexer.file_path = file_path;
	lexer.file_ptr = fopen(file_path, "r");
	if (!lexer.file_path) {
		Error_terminate((Error){
			.type = ERROR_PARAMS,
			.file_path = file_path,
			.msg = "Failed to open file",
		});
	}
	lexer.line_num = 0;
	Lexer_get_line(&lexer);
	return lexer;
}

Token Lexer_peek(Lexer const *lexer) {
	return lexer->token;
}

Token Lexer_consume(Lexer *lexer) {
	Token token = lexer->token;
	Lexer_next(lexer);
	return token;
}

void Lexer_next(Lexer *lexer) {
	bool token_found = false;
	char const *src = lexer->line_buf + lexer->col_num;
	Token token;
	while (!token_found) {
		if (*src == '\0') {
			token.type = TOK_EOF;
		} else if (*src == '\n') {
			Lexer_get_line(lexer);
			src = lexer->line_buf + lexer->col_num;
		} else if (*src == '"') {

		} else if (*src == '\'') {

		} else if (isspace(*src)) {
			src += 1;
		} else if (isdigit(lexer->line_buf[lexer->col_num])) {
			token.type = TOK_CONSTANT;
		} else {
			for (size_t i = 0; TOKS[i])
		}
	}
	lexer->token = token;
	lexer->col_num = src - lexer->line_buf;
}
