#ifndef SOL_NAME_RESOLUTION_H
#define SOL_NAME_RESOLUTION_H

#include "ast.h"

void resolve(Decl *root);
void decl_resolve(Decl *decl);
void stmt_resolve(Stmt *stmt);
void expr_resolve(Expr *expr);
void param_list_resolve(Param_list *param_list, int which);

#endif // SOL_NAME_RESOLUTION_H
