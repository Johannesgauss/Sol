#include "parser_util.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
Expr *parser_expr();
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

/*
        if (EXPRESSION) TEXT

        IF grammar:
        S-> if(E)T

        text = set of sentences. Ex:
        {
                ...
        }
        
*/

// What will see ';': decls, not stmts (except return, maybe)

// A @ B, where @ = {* + - / % == != >= <= > < =}
Decl *parser_decl(Token token)
{
        type_kind kind;
        bool is_void = false;
        switch (token.type) {
        case TOKEN_CHAR:
                kind = TYPE_CHARACTER;
                break;
        case TOKEN_TINY:
                kind = TYPE_TINY;
                break;
        case TOKEN_INT:
                kind = TYPE_INTEGER;
                break;
        case TOKEN_LONG:
                kind = TYPE_LONG;
                break;
        case TOKEN_TUX:
                kind = TYPE_TUX;
                break;
        case TOKEN_VOID:
                kind = TYPE_VOID;
                is_void = true;
                break;
        default:
                fprintf(stderr, "Error! Invalid type in declaration");
                return NULL;
        }

        Type *type = NULL; Type *subtype = NULL; Param_list *param_list = NULL;

        Token name_token = scan_token();
        if (name_token.type != TOKEN_NON_PROTECTED_WORD)
                fprintf(stderr, "Error! Expected identifier name");

        char *name = name_token.begin;

        if (expect_token(TOKEN_OPENPARENTHESIS))
                return parser_function(name, kind);

        if (is_void)
                fprintf(stderr, "Error! Variable cannot be of type void");

        type = type__create(kind, subtype, param_list);

        Expr *value = NULL;
        if (expect_token(TOKEN_EQUAL))
                value = parser_expr();

        if (!expect_token(TOKEN_SEMICOLON))
                fprintf(stderr, "Error! Expected ';' at end of declaration");

        return decl__create_value(name, type, value, NULL);
}

Decl *parser_function(char *name, type_kind kind)
{
        Param_list *param_list = parser_param_list();
        Type *subtype = type__create(kind, NULL, NULL);
        Type *type = type__create(TYPE_FUNCTION, subtype, param_list);

        Token next = peek_token();
        if (next.type == TOKEN_OPENBRACE) {
                consume_token();
                Stmt *code = parser_body(next);
                return decl__create_code(name, type, code, NULL);
        } else {
                putback_token();
                if (!expect_token(TOKEN_SEMICOLON))
                        fprintf(stderr, "Error! Expected ';' after function declaration");
                return decl__create_code(name, type, NULL, NULL);
        }
}

Param_list *parser_param_list()
{
        Token current_token = peek_token();
        if (current_token.type == TOKEN_CLOSEDPARENTHESIS) {
                consume_token();
                return NULL;
        }
        putback_token();

        Param_list *param = malloc(sizeof(*param));
        Param_list *head_param = param;

        // ex: int *var[NUM], where * and [NUM] are opt
        while (current_token.type != TOKEN_CLOSEDPARENTHESIS) {

                Type *type = NULL; Type *subtype = NULL;
                char *name;

                Token type_token = scan_token(); // ex: int
                switch (type_token.type) {
                case TOKEN_INT:
                        type = type__create(TYPE_INTEGER, NULL, NULL);
                        break;
                case TOKEN_CHAR:
                        type = type__create(TYPE_CHARACTER, NULL, NULL);
                        break;
                case TOKEN_TINY:
                        type = type__create(TYPE_TINY, NULL, NULL);
                        break;
                case TOKEN_LONG:
                        type = type__create(TYPE_LONG, NULL, NULL);
                        break;
                case TOKEN_TUX:
                        type = type__create(TYPE_TUX, NULL, NULL);
                        break;
                case TOKEN_VOID:
                        type = type__create(TYPE_VOID, NULL, NULL);
                        break;
                default:
                        break;
                }

                if (type_token.type == TOKEN_VOID) {
                        Token peek = peek_token();
                        if (peek.type == TOKEN_CLOSEDPARENTHESIS) {
                                consume_token();
                                free(head_param);
                                return NULL;
                        }
                        putback_token();
                }

                current_token = scan_token();

                // I do need more knowledge about semantic analysis before doing that, so I'll just ignore pointer for now
                /*while (current_token.type == TOKEN_ASTERISK);
                        param->type = type__create(TYPE_POINTER, NULL, NULL);
                        param->type = param->type->subtype;
                        current_token = scan_token();
                }*/

                param->type = type;

                param->name = current_token.begin;
                param->next = NULL;

                /*
                current_token = peek_token();
                if (fourth_token.type == TOKEN_OPENBRACKET) {
                        consume_token();
                        current_token = peek_token(); // ex: NUM, ]
                        if (current_token.type == TOKEN_DIGIT) {
                                // dthain's structs do not support it, so I won't do the same so soon
                        } else {
                                if (current_token.type != TOKEN_CLOSEDBRACKET) // not ]
                                        fprintf(stderr, "Error! Expected ']'");
                                // yet to be implemented
                        }

                        param->type->
                        type = type__create(TYPE_ARRAY, type, NULL);
                        param->type = type;
                } else {
                        putback_token();
                }

                param->next = NULL;

                Token fifth_token = peek_token();
                */
                current_token = peek_token();
                if (current_token.type == TOKEN_COMMA) {
                        consume_token();
                } else {
                        putback_token();
                        break;
                }

                param->next = malloc(sizeof(*param->next));
                param = param->next;
        }

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                fprintf(stderr, "Error! Expected ')' after parameter list");

        return head_param;
}

Expr *parser_expr()
{
}



Stmt *parser_if_else(Token token)
{
        if (!expect_token(TOKEN_OPENPARENTHESIS))
                fprintf(stderr, "Error! if must be followed by an open parenthesis '('");

        Expr *expr = parser_expr();
        if (expr == NULL)
                fprintf(stderr, "Error inside if expression.");

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                fprintf(stderr, "Error! if must be followed by a closed parenthesis ')'");

        Stmt *body = parser_body(scan_token());
        if (body == NULL)
                fprintf(stderr, "Error with body");

        Stmt *else_body = NULL;
        Token next_token = peek_token();
        if (next_token.type == TOKEN_ELSE) {
                consume_token();
                else_body = parser_body(scan_token());
        } else {
                putback_token();
        }

        return stmt__create_if_else(expr, body, else_body, NULL);
}

Stmt *parser_for(Token token)
{
        if (!expect_token(TOKEN_OPENPARENTHESIS))
                fprintf(stderr, "Error! for must be followed by an open parenthesis '('");

        Expr *init_expr = parser_expr();
        if (init_expr == NULL)
                fprintf(stderr, "Error inside for initialization expression");

        Expr *expr = parser_expr();
        if (expr == NULL)
                fprintf(stderr, "Error inside for loop expression");

        Expr *next_expr = parser_expr();
        if (next_expr == NULL)
                fprintf(stderr, "Error inside for condition expression");

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                fprintf(stderr, "Error! for must be followed by a closed parenthesis ')'");

        Stmt *body = parser_body(scan_token());
        if (body == NULL)
                fprintf(stderr, "Error with body");

        return stmt__create_for(init_expr, expr, next_expr, body, NULL);
}

Stmt *parser_while(Token token)
{
        if (!expect_token(TOKEN_OPENPARENTHESIS))
                fprintf(stderr, "Error! while must be followed by an open parenthesis '('");

        Expr *expr = parser_expr();
        if (expr == NULL)
                fprintf(stderr, "Error with while expression");

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                fprintf(stderr, "Error! while must be followed by a closed parenthesis ')'");

        Stmt *body = parser_body(scan_token());
        if (body == NULL)
                fprintf(stderr, "Error with body");

        return stmt__create_while(expr, body, NULL);
}

Stmt *parser_return(Token token)
{
        Expr *expr = NULL;
        if (!expect_token(TOKEN_SEMICOLON)) {
                expr = parser_expr();
                if (!expect_token(TOKEN_SEMICOLON))
                        fprintf(stderr, "Error! Expected ';' after return expression");
        }

        return stmt__create_return(expr, NULL);
}

Stmt *parser_break(Token token)
{
        if (!expect_token(TOKEN_SEMICOLON))
                fprintf(stderr, "Error! Expected ';' after break");

        return stmt__create_break(NULL);
}

Stmt *parser_continue(Token token)
{
        if (!expect_token(TOKEN_SEMICOLON))
                fprintf(stderr, "Error! Expected ';' after continue");

        return stmt__create_continue(NULL);
}

Stmt *parser_goto(Token token)
{
        Token label_token = scan_token();
        if (label_token.type != TOKEN_NON_PROTECTED_WORD)
                fprintf(stderr, "Error! Expected label name after goto");

        char *label = label_token.begin;

        if (!expect_token(TOKEN_SEMICOLON))
                fprintf(stderr, "Error! Expected ';' after goto label");

        return stmt__create_goto(label, NULL);
}

Stmt *parser_statement(Token token)
{
        switch (token.type) {
        case TOKEN_IF:
                return parser_if_else(token);
        case TOKEN_FOR:
                return parser_for(token);
        case TOKEN_WHILE:
                return parser_while(token);
        case TOKEN_RETURN:
                return parser_return(token);
        case TOKEN_BREAK:
                return parser_break(token);
        case TOKEN_CONTINUE:
                return parser_continue(token);
        case TOKEN_GOTO:
                return parser_goto(token);
        case TOKEN_CHAR:
        case TOKEN_TINY:
        case TOKEN_INT:
        case TOKEN_LONG:
        case TOKEN_TUX:
        case TOKEN_VOID: {
                Decl *decl = parser_decl(token);
                return stmt__create_decl(decl, NULL);
        }
        case TOKEN_OPENBRACE:
                return parser_body(token);
        default:
                putback_token();
                return NULL;
        }
}

Stmt *parser_body(Token token)
{
        Stmt *body = malloc(sizeof(*body));
        Stmt *current = body;
        bool loop = false;
        if (token.type == TOKEN_OPENBRACE) {
                loop = true;
                token = scan_token();
                if (token.type != TOKEN_CLOSEDBRACE)
                        current = parser_statement(token);
                else
                        return NULL;
        }
        do {
                current->next = parser_statement(token);
                if (current->next)
                        current = current->next;
                token = scan_token();
        } while (loop && token.type != TOKEN_CLOSEDBRACE);

        return body;
}
