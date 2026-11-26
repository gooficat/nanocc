#include "ast.h"
#include "context.h"
#include "lexer.h"
#include <stddef.h>
#include <stdlib.h>

C_Storage C_Storage_generate(Context *ctx) {
	switch (Lexer_peek(&ctx->lexer).type) {
	case TOK_AUTO:
		return C_STORAGE_AUTO;
	default:
		Context_error_terminate(ctx, ERROR_SYNTAX,
								"Cannot declare with this symbol currently");
	}
}

C_Type C_Type_generate(Context *ctx) {
	C_Type type = {};
}

size_t ASTDeclList_push(ASTDeclList *list, ASTDecl decl) {
	size_t index = list->len;
	list->len += 1;
	list->vals = realloc(list->vals, list->len * sizeof *list->vals);
	list->vals[index] = decl;
	return index;
}

static ASTDecl ASTDecl_generate(Context *ctx) {
	ASTDecl decl;
	decl.storage = C_Storage_generate(ctx);
	decl.type = C_Type_generate(ctx);
	return decl;
}

AST AST_generate(Context *ctx) {
	AST ast = {};

	while (ctx->lexer.token.type != TOK_EOF) {
		ASTDeclList_push(&ast.decls, ASTDecl_generate(ctx));
	}

	return ast;
}
