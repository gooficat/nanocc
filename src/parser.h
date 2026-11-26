#pragma once

#include "ast.h"
#include <stddef.h>
typedef struct {
	char const *name;
	bool is_defined;
} C_Label;

typedef struct {
	C_Storage storage;
	C_Type *type;
	char const *name;
} C_Var;

typedef struct {
	C_Label *val;
	size_t len;
} C_LabelList;

typedef struct {
	C_Var *val;
	size_t len;
} C_VarList;

typedef struct C_VarFrame {
	C_VarList vars;
	struct C_VarFrame *previous;
} C_VarFrame;

typedef struct {
	C_LabelList labels;
	C_VarFrame vars;
} Parser;
