#ifndef SEMANTIC_ANALYSIS_UTIL_H
#define SEMANTIC_ANALYSIS_UTIL_H

#include "ast.h"
#include "string.h"
#include <stddef.h>

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

typedef union Ast {
	Param_list *param_list;
	Stmt *stmt;
	Expr *expr;
	Decl *decl;
} Ast;

typedef struct Symbol {
	symbol_t kind;
	Type *type;
	str name;
	int which;

	Ast ast;
} Symbol;

Symbol symbol__init(symbol_t kind, Type *type, str name, int which, Ast ast);
Symbol *symbol__create(symbol_t kind, Type *type, str name, int which, Ast ast);

void scope_enter();
void scope_exit();
size_t scope_level();
void scope_bind(Symbol symbol);

Symbol *scope_lookup(str name);
Symbol *scope_lookup_current(str name);

#endif // SEMANTIC_ANALYSIS_UTIL_H
