#ifndef TOK
#define TOK(id, str)
#define TOK_UNDEF_CHECKER
#endif

TOK(AUTO, "auto")
TOK(EXTERN, "extern")
TOK(STATIC, "static")
TOK(TYPEDEF, "typedef")
TOK(INT, "int")
TOK(CHAR, "char")
TOK(FLOAT, "float")
TOK(DOUBLE, "double")
TOK(LONG, "long")
TOK(SHORT, "short")
TOK(SIGNED, "signed")
TOK(UNSIGNED, "unsigned")

#ifdef TOK_UNDEF_CHECKER
#undef TOK
#undef TOK_UNDEF_CHECKER
#endif
