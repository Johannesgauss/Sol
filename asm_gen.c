#include "asm_gen_util.h"
#include "ast.h"
#include <stdbool.h>

void stmt_codegen(Stmt *stmt);
void expr_codegen(Expr *expr);
void decl_codegen(Decl *decl);

void expr_codegen(Expr *expr)
{
	if (!expr) return;

	switch (expr->kind) {
	case EXPR_INTEGER_LITERAL:
		expr->reg = scratch_alloc();
		emit("\tmovq $%d, %s", expr->int_value, scratch_name(expr->reg));
		break;
	case EXPR_BOOLEAN_LITERAL:
		expr->reg = scratch_alloc();
		emit("\tmovq $%d, %s", expr->bool_value, scratch_name(expr->reg));
		break;
	case EXPR_CHAR_LITERAL:
		expr->reg = scratch_alloc();
		emit("\tmovq $%d, %s", expr->char_value, scratch_name(expr->reg));
		break;
	case EXPR_STRING_LITERAL:;
		int str_label = label_create();
		emit(".data");
		emit("%s:", label_name(str_label));
		emit("\t.string \"%.*s\"", (int)expr->name.size, expr->str_literal);
		emit(".text");
		expr->reg = scratch_alloc();
		emit("\tleaq %s(%%rip), %s", label_name(str_label), scratch_name(expr->reg));
		break;
	case EXPR_NAME:
		expr->reg = scratch_alloc();
		emit("\tmovq %s, %s", symbol_codegen(expr->symbol), scratch_name(expr->reg));
		break;

	case EXPR_ADD:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\taddq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		expr->reg = expr->right->reg;
		scratch_free(expr->left->reg);
		break;

	case EXPR_SUB:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tsubq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		expr->reg = expr->right->reg;
		scratch_free(expr->left->reg);
		break;

	case EXPR_MUL:
		/* mov a, r1
		 * mov b, r2
		 * imul r1, r2
		 *
		 * */
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\timulq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		//emit("\tmovq %%rax, %s", scratch_name(expr->right->reg));
		expr->reg = expr->right->reg;
		scratch_free(expr->left->reg);
		break;
	case EXPR_ASSIGN:
		expr_codegen(expr->right);
		emit("\tmovq %s, %s", scratch_name(expr->right->reg), symbol_codegen(expr->left->symbol));
		expr->reg = expr->right->reg;
		break;
	case EXPR_EQ:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tcmpq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		emit("\tsete %al");
		emit("\tmovzbq %al, %s", scratch_name(expr->right->reg));
		scratch_free(expr->left->reg);
		//scratch_free(expr->right->reg); scratch_free(expr->reg);
		expr->reg = expr->right->reg;
		break;
	case EXPR_GE:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tcmpq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		emit("\tsetge %al");
		emit("\tmovzbq %al, %s", scratch_name(expr->right->reg));
		scratch_free(expr->left->reg);
		expr->reg = expr->right->reg;
		break;
	case EXPR_NEQ:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tcmpq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		emit("\tsetne %al");
		emit("\tmovzbq %al, %s", scratch_name(expr->right->reg));
		scratch_free(expr->left->reg);
		expr->reg = expr->right->reg;
		break;
	case EXPR_LT:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tcmpq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		emit("\tsetl %al");
		emit("\tmovzbq %al, %s", scratch_name(expr->right->reg));
		scratch_free(expr->left->reg);
		expr->reg = expr->right->reg;
		break;
	case EXPR_LE:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tcmpq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		emit("\tsetle %al");
		emit("\tmovzbq %al, %s", scratch_name(expr->right->reg));
		scratch_free(expr->left->reg);
		expr->reg = expr->right->reg;
		break;
	case EXPR_GT:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tcmpq %s, %s", scratch_name(expr->left->reg), scratch_name(expr->right->reg));
		emit("\tsetg %al");
		emit("\tmovzbq %al, %s", scratch_name(expr->right->reg));
		scratch_free(expr->left->reg);
		expr->reg = expr->right->reg;
		break;
	case EXPR_DIV:
	case EXPR_MOD:
		expr_codegen(expr->left);
		expr_codegen(expr->right);
		emit("\tmovq %s, %%rax", scratch_name(expr->left->reg));
		emit("\tcqto");
		emit("\tidivq %s", scratch_name(expr->right->reg));
		if (expr->kind == EXPR_DIV) {
			emit("\tmovq %%rax, %s", scratch_name(expr->right->reg));
		} else {
			emit("\tmovq %%rdx, %s", scratch_name(expr->right->reg));
		}
		expr->reg = expr->right->reg;
		scratch_free(expr->left->reg);
		break;
	case EXPR_NEG:
		expr_codegen(expr->left);
		emit("\tnegq %s", scratch_name(expr->left->reg));
		expr->reg = expr->left->reg;
		break;
	case EXPR_NOT:
		expr_codegen(expr->left);
		emit("\txorq $1, %s", scratch_name(expr->left->reg));
		expr->reg = expr->left->reg;
		break;

	case EXPR_CALL:;
		const char arg_regs[6][4+1] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

		Expr *current = expr->right;
		for (int i = 0; current && i < 6; i++) {
			Expr *arg;
			if (current->kind == EXPR_COMMA) {
				arg = current->left;
				current = current->right;
			} else {
				arg = current;
				current = NULL;
			}
			expr_codegen(arg);
			emit("\tmovq %s, %s", scratch_name(arg->reg), arg_regs[i]);
			scratch_free(arg->reg);
		}
		emit("\tcall %.*s", (int)expr->symbol->name.size, expr->symbol->name.data);

		Type *ret_type = expr->symbol->type->subtype ? expr->symbol->type->subtype : expr->symbol->type;
		if (ret_type->kind != TYPE_VOID) {
			expr->reg = scratch_alloc();
			emit("\tmovq %%rax, %s", scratch_name(expr->reg));
		}
		break;
	default:;
	}
}

static char fn_name[32]; // remove it later!

void decl_codegen(Decl *decl)
{
	if (!decl) return;
	if (decl->is_extern || (decl->symbol && decl->symbol->kind == SYMBOL_EXTERN)) {
		decl_codegen(decl->next);
		return;
	}
	switch (decl->type->kind) {
	case TYPE_TINY:
	case TYPE_CHARACTER:
	case TYPE_BOOLEAN:
	case TYPE_INTEGER:
	case TYPE_LONG:
		if (decl->symbol->kind == SYMBOL_GLOBAL) {
			if (decl->value) {
				emit(".data");
				emit("\t.global %s", decl->name.data);
				emit("%s:", decl->name.data);
				emit("\t.quad %d", decl->value ? decl->value->int_value : 0);
			} else {
				emit(".bss");
				emit("\t.global %s", decl->name.data);
				emit("%s:", decl->name.data);
				emit("\t.skip 8");
			}
			emit(".text");                             
		} else if (decl->value) {
			expr_codegen(decl->value);
			emit("\tmovq %s, %s", scratch_name(decl->value->reg), symbol_codegen(decl->symbol));
			scratch_free(decl->value->reg);
		}
		break;
	case TYPE_TUX:
		// TODO
		break;
	case TYPE_VOID:	
	case TYPE_FUNCTION:	
		if (!decl->code) break;
		snprintf(fn_name, sizeof(fn_name), "%.*s", (int)decl->name.size, decl->name.data);

		emit("\t.global %s", fn_name);

		emit("%s:", fn_name);

		emit("\tpushq %rbp");
		emit("\tmovq %rsp, %rbp");

		// args (inneficient, I know! But, I'll modify this later! haha)
		// Doing these kind of comments is so cool!
		emit("\tpushq %rdi");
		emit("\tpushq %rsi");
		emit("\tpushq %rdx");
		emit("\tpushq %rcx");
		emit("\tpushq %r8");
		emit("\tpushq %r9");

		emit("\tsubq $24, %rsp"); // System V AMD64 ABI 16-multiple rule, just, why?! Why have they got to obligate us to do it?!

		// callee save
		emit("\tpushq %rbx");
		emit("\tpushq %r12");
		emit("\tpushq %r13");
		emit("\tpushq %r14");
		emit("\tpushq %r15");

		stmt_codegen(decl->code);

		emit(".%s_epilogue:", fn_name);

		// calle save pops
		emit("\tpopq %r15");
		emit("\tpopq %r14");
		emit("\tpopq %r13");
		emit("\tpopq %r12");
		emit("\tpopq %rbx");

		emit("\taddq $24, %rsp");

		// arg pops
		emit("\tpopq %r9");
		emit("\tpopq %r8");
		emit("\tpopq %rcx");
		emit("\tpopq %rdx");
		emit("\tpopq %rsi");
		emit("\tpopq %rdi");

		emit("\tmovq %rbp, %rsp");
		emit("\tpopq %rbp");
		emit("\tret");
		break;
	default:;
	}
	decl_codegen(decl->next);
}

void stmt_codegen(Stmt *stmt)
{
	if (!stmt) return;
	switch (stmt->kind) {
	case STMT_EXPR:
		expr_codegen(stmt->expr);
		scratch_free(stmt->expr->reg);
		break;
	case STMT_DECL:
		decl_codegen(stmt->decl);
		break;
	case STMT_RETURN:
		expr_codegen(stmt->expr);
		emit("\tmovq %s, %%rax", scratch_name(stmt->expr->reg));
		emit("\tjmp .%s_epilogue", fn_name);
		scratch_free(stmt->expr->reg);
		break;	
	case STMT_IF_ELSE:;
		int if_else_done_label = label_create();
		expr_codegen(stmt->if_else_block.expr);
		emit("\tcmp $0, %s", scratch_name(stmt->if_else_block.expr->reg));
		scratch_free(stmt->if_else_block.expr->reg);

		if (stmt->if_else_block.else_body) {
			int else_label = label_create();
			emit("\tje %s", label_name(else_label));
			stmt_codegen(stmt->if_else_block.body);
			emit("\tjmp %s", label_name(if_else_done_label));
			emit("%s:", label_name(else_label));
			stmt_codegen(stmt->if_else_block.else_body);
		} else {
			emit("\tje %s", label_name(if_else_done_label));
			stmt_codegen(stmt->if_else_block.body);
		}
		emit("%s:", label_name(if_else_done_label));
		break;
	case STMT_FOR:;
		/*
		 * init_expr
		 * for_loop_label:
		 * 	body
		 * 	bool_expr_cmp
		 * 	je done
		 * 	next_expr;
		 * 	jmp for_loop_label;
		 * done:
		*/
		int for_init_label = label_create();
		int for_done_label = label_create();
		stmt_codegen(stmt->for_block.init_stmt);
		emit("%s:", label_name(for_init_label));
		expr_codegen(stmt->for_block.expr);
		emit("\tcmp $0, %s", scratch_name(stmt->for_block.expr->reg));
		scratch_free(stmt->for_block.expr->reg);
		emit("\tje %s", label_name(for_done_label));
		stmt_codegen(stmt->for_block.body);
		expr_codegen(stmt->for_block.next_expr);
		scratch_free(stmt->for_block.next_expr->reg);
		emit("\tjmp %s", label_name(for_init_label));
		emit("\t%s:", label_name(for_done_label));
		break;
	case STMT_WHILE:;
		/*init_expr
		 * body
		 * while_done:
		 */
		int while_init_label = label_create();
		int while_done_label = label_create();
		emit("%s:", label_name(while_init_label));
		expr_codegen(stmt->while_block.expr);
		emit("\tcmp $0, %s", scratch_name(stmt->while_block.expr->reg));
		scratch_free(stmt->while_block.expr->reg);
		emit("\tje %s", label_name(while_done_label));
		stmt_codegen(stmt->while_block.body);
		emit("\tjmp %s", label_name(while_init_label));
		emit("%s:", label_name(while_done_label));
		break;
	case STMT_BLOCK:
		stmt_codegen(stmt->body);
		break;
	default:;
	}

	stmt_codegen(stmt->next);
}
