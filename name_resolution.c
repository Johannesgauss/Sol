#include "name_resolution.h"
#include "semantic_analysis_util.h"
#include <stdbool.h>
#include <stdio.h>

void param_list_resolve(Param_list *param_list, int which)
{
	if (!param_list) return;

	type_kind kind = param_list->type->kind;
	param_list->symbol = symbol__create(SYMBOL_PARAM, param_list->type, param_list->name, which, (Ast){.param_list = param_list}); //automatically "binds"
	param_list_resolve(param_list->next, which + 1);
	//if (kind == TYPE_INTEGER) { }
}

void stmt_resolve(Stmt *stmt)
{
	if (!stmt) return;

	switch (stmt->kind) {
	case STMT_DECL:
		decl_resolve(stmt->decl);
		break;
	case STMT_BLOCK:
		scope_enter();
		stmt_resolve(stmt->body);
		scope_exit();
		break;
	case STMT_IF_ELSE:
		expr_resolve(stmt->if_else_block.expr);
		stmt_resolve(stmt->if_else_block.body);
		stmt_resolve(stmt->if_else_block.else_body);
		break;
	case STMT_EXPR:
		expr_resolve(stmt->expr);
		break;
	case STMT_LABEL:
	case STMT_GOTO:
	case STMT_BREAK:
	case STMT_CONTINUE:
		break;
	case STMT_RETURN:
	case STMT_PRINT:
		expr_resolve(stmt->expr);
		break;
	case STMT_FOR:
		stmt_resolve(stmt->for_block.init_stmt);
		expr_resolve(stmt->for_block.expr);
		expr_resolve(stmt->for_block.next_expr);
		stmt_resolve(stmt->for_block.body);
		break;
	case STMT_WHILE:
		expr_resolve(stmt->while_block.expr);
		stmt_resolve(stmt->while_block.body);
		break;
	default:
		fprintf(stderr, "ERROR!");
	}

	stmt_resolve(stmt->next);
}


void expr_resolve(Expr *expr)
{
	if(!expr) return;

	if (expr->kind==EXPR_NAME) {
		expr->symbol = scope_lookup(expr->name);
		if (!expr->symbol) {
			fprintf(stderr, "ERROR");
		}
	} else {
		expr_resolve(expr->left);
		expr_resolve(expr->right);

		if (expr->kind == EXPR_CALL && expr->left) {
			expr->symbol = expr->left->symbol;
		}
	}
}
static int local_which = 0;

void decl_resolve(Decl *decl)
{
	if (!decl) return;

	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : (decl->is_extern ? SYMBOL_EXTERN : SYMBOL_GLOBAL);
	int which = 0;
	if (kind == SYMBOL_LOCAL) {
		which = ++local_which;
	}
	decl->symbol = symbol__create(kind, decl->type, decl->name, which, (Ast) {.decl = decl});

	if (decl->type->kind == TYPE_FUNCTION || decl->type->kind == TYPE_VOID) {
		if (!decl->code) goto END;
		local_which = 0;
		scope_enter();
		param_list_resolve(decl->type->params, 0);
		stmt_resolve(decl->code);
		scope_exit();
	} else {
		expr_resolve(decl->value);
	}
END:
	decl_resolve(decl->next);
}

void resolve(Decl *root)
{
	scope_enter();
	decl_resolve(root);
	scope_exit();
}
