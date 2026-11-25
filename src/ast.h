#pragma once

#include "lexer.h"
#include <stddef.h>

typedef enum {
	C_STORAGE_EXTERN,
	C_STORAGE_STATIC,
	C_STORAGE_LOCAL,
	C_STORAGE_AUTO,
	C_STORAGE_TYPEDEF,
} C_Storage;

typedef enum {
	C_TYPE_VOID,
	C_TYPE_INT,
	C_TYPE_FLOAT,
	C_TYPE_STRUCT,
	C_TYPE_ARRAY,
	C_TYPE_POINTER,
	C_TYPE_FUNCTION,
} C_TypeType;

typedef struct C_Type {
	C_TypeType type;
	struct C_Type *under;
	size_t count; /* either the number of  */
} C_Type;

typedef struct {
	struct ASTStmt *val;
	size_t len;
} ASTStmtList;

typedef struct {
	struct ASTExpr *initializer;
	ASTStmtList body;
} ASTDeclVal;

typedef struct {
	char const *name;
	C_Type type;
} ASTDecl;

typedef struct {
	ASTDecl *vals;
	size_t len;
} ASTDeclList;

typedef enum {
	AST_EXPR_IDENT,
	AST_EXPR_UNARY,
	AST_EXPR_BINARY,
	AST_EXPR_TERNARY,
	AST_EXPR_CALL,
	AST_EXPR_LITERAL,
	AST_EXPR_CAST,
} ASTExprType;

typedef struct {
	TokenType op;
	struct ASTExpr *expr;
} ASTExprUnary;

typedef struct {
	TokenType op;
	struct ASTExpr *a;
	struct ASTExpr *b;
} ASTExprBinary;

typedef struct {
	struct ASTExpr *val;
	size_t len;
} ASTExprList;

typedef struct {
	struct ASTExpr *of;
	ASTExprList with;
} ASTCall;

typedef union {
	char const *ident;
	ASTExprUnary unary;
	ASTExprBinary binary;
	ASTCall call;
} ASTExprVal;

typedef struct ASTExpr {
	ASTExprType type;
	ASTExprVal val;
} ASTExpr;

typedef enum {
	AST_STMT_DECL,
	AST_STMT_ORDER,
	AST_STMT_EXPR,
	AST_STMT_COND,
} ASTStmtType;

typedef union {
	ASTDecl decl;
	ASTExpr expr;
} ASTStmtVal;

typedef struct ASTStmt {
	char const *label;
	ASTStmtType type;
	ASTStmtVal val;
} ASTStmt;

typedef struct {
	ASTDeclList decls;
} AST;
