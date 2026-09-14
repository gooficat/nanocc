#ifndef TOK
#define TOK(id, str)
#define TOKISNOTDEFINEDHERE
#endif

TOK(INT, "int")
TOK(CHAR, "char")
TOK(FLOAT, "float")
TOK(VOID, "void")

TOK(PAREN_L, "(")
TOK(PAREN_R, ")")

TOK(BRACE_L, "{")
TOK(BRACE_R, "}")

TOK(BRACK_L, "[")
TOK(BRACK_R, "]")

TOK(SEMI, ";")
TOK(COLON, ":")
TOK(COMMA, ",")

TOK(PLUS, "+")
TOK(MINUS, "-")

TOK(ASTER, "*")
TOK(AMPER, "&")

#ifdef TOKISNOTDEFINEDHERE
#undef TOK
#undef TOKISNOTDEFINEDHERE
#endif
