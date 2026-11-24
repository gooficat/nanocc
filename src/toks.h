#ifndef TOK
#define TOK(id, str)
#define TOK_LSP_FIX
#endif

TOK(AUTO, "auto")
TOK(PAREN_L, "(")
TOK(PAREN_R, ")")
TOK(SEMI, ";")

#ifdef TOK_LSP_FIX
#undef TOK
#undef TOK_LSP_FIX
#endif
