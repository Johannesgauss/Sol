#include "parser_util.h"
#include "ast.h"
#include <stdbool.h>

typedef enum binding_power
{
        POWER_NULL,
        POWER_ASSIGN,
        POWER_ADD,
        POWER_MUL,
        POWER_PARENTHESIS
} binding_power;
#define parser_expr() parser_expr_internal(POWER_NULL)

Expr *parser_expr_internal(binding_power bp);
Decl *parser_decl(Token token);
Decl *parser_function(char *name, type_kind kind);
Param_list *parser_param_list();
Stmt *parser_if_else(Token token);
Stmt *parser_for(Token token);
Stmt *parser_while(Token token);
Stmt *parser_return(Token token);
Stmt *parser_break(Token token);
Stmt *parser_continue(Token token);
Stmt *parser_goto(Token token);
Stmt *parser_statement(Token token);
Stmt *parser_body(Token token);
