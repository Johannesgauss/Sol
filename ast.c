#include "ast.h"
#include <stdlib.h>

Decl *decl__create_value(char *name, Type *type, Expr *value, Decl *next)
{
        Decl *self = malloc(sizeof(*self));
        self->name = name;
        self->type = type;
        self->value = value;
        self->next = next;

        return self;
}

Decl *decl__create_code(char *name, Type *type, Stmt *code, Decl *next)
{
        Decl *self = malloc(sizeof(*self));
        self->name = name;
        self->type = type;
        self->code = code;
        self->next = next;

        return self;
}

void decl__destroy(Decl *self)
{
        while (self) {
                Decl *next = self->next;
                if (self->type && self->type->kind == TYPE_FUNCTION) {
                        stmt__destroy(self->code);
                } else {
                        expr__destroy(self->value);
                }
                type__destroy(self->type);
                free(self);
                self = next;
        }
}

Stmt *stmt__create_decl(Decl *decl, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_DECL;
        self->decl = decl;
        self->next = next;

        return self;
}

Stmt *stmt__create_expr(Expr *expr, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_EXPR;
        self->expr = expr;
        self->next = next;

        return self;
}

Stmt *stmt__create_if_else(Expr *expr, Stmt *body, Stmt *else_body, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_IF_ELSE;
        self->if_else_block.expr = expr;
        self->if_else_block.body = body;
        self->if_else_block.else_body = else_body;
        self->next = next;

        return self;
}

Stmt *stmt__create_for(Expr *init_expr, Expr *expr, Expr *next_expr, Stmt *body, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_FOR;
        self->for_block.init_expr = init_expr;
        self->for_block.expr = expr;
        self->for_block.next_expr = next_expr;
        self->for_block.body = body;
        self->next = next;

        return self;
}

Stmt *stmt__create_while(Expr *expr, Stmt *body, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_WHILE;
        self->while_block.expr = expr;
        self->while_block.body = body;
        self->next = next;

        return self;
}

Stmt *stmt__create_print(Expr *expr, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_PRINT;
        self->expr = expr;
        self->next = next;

        return self;
}

Stmt *stmt__create_return(Expr *expr, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_RETURN;
        self->expr = expr;
        self->next = next;

        return self;
}

Stmt *stmt__create_block(Stmt *body, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_BLOCK;
        self->body = body;
        self->next = next;

        return self;
}

Stmt *stmt__create_break(Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_BREAK;
        self->next = next;

        return self;
}

Stmt *stmt__create_continue(Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_CONTINUE;
        self->next = next;

        return self;
}

Stmt *stmt__create_goto(const char *label, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_GOTO;
        self->label = label;
        self->next = next;

        return self;
}

Stmt *stmt__create_label(const char *label, Stmt *next)
{
        Stmt *self = malloc(sizeof(*self));
        self->kind = STMT_LABEL;
        self->label = label;
        self->next = next;

        return self;
}

void stmt__destroy(Stmt *self)
{
        while (self) {
                Stmt *next = self->next;
                switch (self->kind) {
                case STMT_DECL:
                        decl__destroy(self->decl);
                        break;
                case STMT_EXPR:
                case STMT_PRINT:
                case STMT_RETURN:
                        expr__destroy(self->expr);
                        break;
                case STMT_IF_ELSE:
                        expr__destroy(self->if_else_block.expr);
                        stmt__destroy(self->if_else_block.body);
                        stmt__destroy(self->if_else_block.else_body);
                        break;
                case STMT_FOR:
                        expr__destroy(self->for_block.init_expr);
                        expr__destroy(self->for_block.expr);
                        expr__destroy(self->for_block.next_expr);
                        stmt__destroy(self->for_block.body);
                        break;
                case STMT_WHILE:
                        expr__destroy(self->while_block.expr);
                        stmt__destroy(self->while_block.body);
                        break;
                case STMT_BLOCK:
                        stmt__destroy(self->body);
                        break;
                case STMT_BREAK:
                case STMT_CONTINUE:
                case STMT_GOTO:
                case STMT_LABEL:
                        break;
                }
                free(self);
                self = next;
        }
}

Expr *expr__create(expr_kind kind, Expr *left, Expr *right)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = kind;
        self->left = left;
        self->right = right;

        return self;
}

Expr *expr__create_unary(expr_kind kind, Expr *expr)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = kind;
        self->left = expr;
        self->right = NULL;

        return self;
}

Expr *expr__create_name(const char *name)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = EXPR_NAME;
        self->left = NULL;
        self->right = NULL;
        self->name = name;

        return self;
}

Expr *expr__create_integer_literal(int int_value)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = EXPR_INTEGER_LITERAL;
        self->left = NULL;
        self->right = NULL;
        self->int_value = int_value;

        return self;
}

Expr *expr__create_string_literal(const char *str_literal)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = EXPR_STRING_LITERAL;
        self->left = NULL;
        self->right = NULL;
        self->str_literal = str_literal;

        return self;
}

Expr *expr__create_boolean_literal(int bool_value)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = EXPR_BOOLEAN_LITERAL;
        self->left = NULL;
        self->right = NULL;
        self->bool_value = bool_value;

        return self;
}

Expr *expr__create_char_literal(char char_value)
{
        Expr *self = malloc(sizeof(*self));
        self->kind = EXPR_CHAR_LITERAL;
        self->left = NULL;
        self->right = NULL;
        self->char_value = char_value;

        return self;
}

Expr *expr__create_deref(Expr *expr)
{
        return expr__create_unary(EXPR_DEREF, expr);
}

Expr *expr__create_addr(Expr *expr)
{
        return expr__create_unary(EXPR_ADDR, expr);
}

void expr__destroy(Expr *self)
{
        if (!self) return;

        expr__destroy(self->left);
        expr__destroy(self->right);
        free(self);
}

Type *type__create(type_kind kind, Type *subtype, Param_list *params)
{
        Type *self = malloc(sizeof(*self));
        self->kind = kind;
        self->subtype = subtype;
        self->params = params;

        return self;
}

void type__destroy(Type *self)
{
        if (!self) return;

        type__destroy(self->subtype);
        param_list__destroy(self->params);
        free(self);
}

Param_list *param_list__create(char *name, Type *type, Param_list *next)
{
        Param_list *self = malloc(sizeof(*self));
        self->name = name;
        self->type = type;
        self->next = next;

        return self;
}

void param_list__destroy(Param_list *self)
{
        while (self) {
                Param_list *next = self->next;
                type__destroy(self->type);
                free(self);
                self = next;
        }
}
