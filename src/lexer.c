#include "lexer.h"
#include "const.h"
#include "context.h"
#include "error.h"
#include "pool.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Lexer_get_line(Lexer *lexer, Context *ctx) {
	if (!fgets(lexer->line_buf, LINE_BUF_MAX, lexer->file_ptr)) {
		lexer->line_buf[0] = '\0';
	} else {
		size_t line_len = strlen(lexer->line_buf);
		if (line_len > 0 && lexer->line_buf[line_len - 1] != '\n') {
			Context_error_terminate(
				ctx, ERROR_SYNTAX,
				"Next line in the file is too long, or the file does "
				"not end in a new line.");
		}
	}
	lexer->col_num = 0;
	lexer->line_num += 1;
}

Lexer Lexer_new(char const *file_path, Context *ctx) {
	Lexer lexer;
	lexer.file_path = file_path;
	lexer.file_ptr = fopen(file_path, "r");
	if (!lexer.file_path) {
		Context_error_terminate(ctx, ERROR_PARAMS, "Failed to open file");
	}
	lexer.line_num = 0;
	Lexer_get_line(&lexer, ctx);
	Lexer_next(&lexer, ctx);
	return lexer;
}

Token Lexer_peek(Lexer const *lexer) {
	return lexer->token;
}

Token Lexer_consume(Lexer *lexer, Context *ctx) {
	Token token = lexer->token;
	Lexer_next(lexer, ctx);
	return token;
}

bool first_number_has_decimal_point(char const *src) {
	do {
		src += 1;
	} while (isalnum(*src));
	if (*src == '.') {
		return true;
	} else {
		return false;
	}
}

Token Lexer_next(Lexer *lexer, Context *ctx) {
	bool token_found = false;
	Token token;

	while (!token_found) {
		char const *src = lexer->col_num + lexer->line_buf;
		if (*src == '\0') {
			token.type = TOK_EOF;
			token_found = true;
		} else if (*src == '\n') {
			Lexer_get_line(lexer, ctx);
			src = lexer->line_buf + lexer->col_num;
		} else if (*src == '"') {
			C_Const constant;
			size_t str_length = 0;
			constant.type = C_CONST_STRING;
			src += 1;
			while (src[str_length - 1] == '\\' || src[str_length] != '"') {
				str_length += 1;
			}

			constant.val.string = malloc(str_length + 1);
			memcpy(constant.val.string, src, str_length);
			constant.val.string[str_length] = '\0';

			token.index = Pool_add_or_get_const(&ctx->pool, constant);
			token_found = true;
			src += str_length;
		} else if (*src == '\'') {
			C_Const constant;
			constant.type = C_CONST_INTEGER;
			src += 1;
			if (*src == '\\') {
				Context_error_terminate(
					ctx, ERROR_INTERNAL,
					"Escape sequences are not implemented.");
			} else {
				constant.val.integer = *src;
				src += 1;
			}
			src += 1;
			token_found = true;
		} else if (isspace(*src)) {
			src += 1;
		} else if (isdigit(lexer->line_buf[lexer->col_num])) {
			C_Const constant;
			token.type = TOK_CONSTANT;
			if (!first_number_has_decimal_point(src)) {
				constant.type = C_CONST_INTEGER;
				constant.val.integer = strtoll(src, (char **)&src, 0);
				Pool_add_or_get_const(&ctx->pool, constant);
			} else {
				Context_error_terminate(
					ctx, ERROR_INTERNAL,
					"Unimplemented: Floating point numbers.");
			}
		} else {
			for (size_t i = 0; TOKS[i] != NULL; i += 1) {
				size_t tok_len = strlen(TOKS[i]);
				if (!strncmp(TOKS[i], src, tok_len) &&
					(!(*src == '_' || isalpha(*src)) ||
					 !(src[tok_len] == '_' || isalpha(src[tok_len])))) {
					token.type = i;
					token_found = true;
					src += tok_len;
					break;
				}
			}
			if (!token_found) {
				size_t ident_len = 0;
				char *ident;
				token.type = TOK_IDENTIFIER;
				while (src[ident_len] == '_' || isalnum(src[ident_len])) {
					ident_len += 1;
				}
				ident = malloc(ident_len + 1);
				memcpy(ident, src, ident_len);
				ident[ident_len] = '\0';

				token.index = Pool_add_or_get_ident(&ctx->pool, ident);
				token_found = true;
				src += ident_len;
			}
		}
		lexer->col_num = src - lexer->line_buf;
	}
	lexer->token = token;

	return token;
}

void Lexer_close(Lexer *lexer) {
	fclose(lexer->file_ptr);
	memset(lexer, 0, sizeof *lexer);
}
