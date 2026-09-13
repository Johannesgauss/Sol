#include "parser_util.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#define GOOD_IDEA 51

Expr *parser_expr();
Decl *parser_decl(Token token);
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
                is_void = true;
                break;
        default:
                perror("Error! Invalid type in declaration");
                return NULL;
        }

        Type *type; Type *subtype = NULL; Param_list *param_list = NULL;

        Token name_token = scan_token();
        if (name_token.type != TOKEN_NON_PROTECTED_WORD)
                perror("Error! Expected identifier name");

        char *name = name_token.begin;

        if (!expect_token(TOKEN_EQUAL) || is_void) {
                if (!expect_token(TOKEN_OPENPARENTHESIS))
                        perror("Error! Expected '=' or '(' in declaration");
                else {
                        subtype = type__create(TYPE_FUNCTION);
                        param_list = parser_param_list();
                }
        }

        Type *type = type__create(kind, subtype, param_list);

        Expr *value = parser_expr();

        if (!expect_token(TOKEN_SEMICOLON))
                perror("Error! Expected ';' at end of declaration");

        return decl__create_value(name, type, value, NULL);
}

Param_list *parser_param_list()
{
        Param_list *param_array = malloc(GOOD_IDEA*sizeof(*param_array)); 

        Token *current_token = peek_token();

        // ex: int *var[NUM], where * and [NUM] are opt
        for (int i = 0; i < GOOD_IDEA && current_token.type != TOKEN_CLOSEDPARENTHESIS; i++) {

                Type *type; Type *subtype = NULL;
                char *name;

                first_token = scan_token(); // ex: int
                switch (first_token.type) {
                case TOKEN_INT:
                        param_array[i].type = TYPE_INTEGER;
                }
                Token *second_token = peek_token(); // ex: * or var
                if (second_token.type == TOKEN_ASTERISK) {
                        subtype = type__create() // TYPE_POINTER
                        consume_token(); // if *, consume it
                }
                Token *third_token = scan_token();
                name = second_token.begin;

                Token *fourth_token = peek_token(); // ex: [
                if (fourth_token.type == TOKEN_OPENBRACKET) {
                        consume_token();
                        Token fourth_one_token = scan_token(); // ex: NUM, ]
                        if (fourth_one_token.type == TOKEN_DIGIT)
                                // yet to be implemented
                        else {
                                if (fourth_one_token.type != TOKEN_CLOSEDBRACKET) // not ]
                                        perror;
                                // yet to be implemented
                        }
                } else  {
                        putback_token();// [] processing done
                }
                Token *fifth_token= scan_token();
                if (third_token.type != TOKEN_COMMA)
                        perror
        }

        return param_array;
}

Expr *parser_expr()
{
        
        Token first_token = scan_token();
        if (first_token.type == TOKEN_DIGIT) {

        } else if (first_token.type == TOKEN_NON_PROTECTED_WORD) {
                expr->left->name = first_token.begin
        } else {
                perror("Wrong A"); // tmp
        }

        Token second_token = scan_token();
        switch(second_token.type) {
        case TOKEN_PLUS:
                expr->kind = EXPR_ADD;
        default:
                perror("Non-existent operation");
        }


        Token first_token = scan_token();
        if (first_token.type == TOKEN_DIGIT) {

        } else if (first_token.type == TOKEN_NON_PROTECTED_WORD) {
                expr->right->name = first_token.begin
        } else {
                perror("Wrong B"); // tmp
        }

        return expr;
}



Stmt *parser_if_else(Token token)
{
        if (!expect_token(TOKEN_OPENPARENTHESIS))
                perror("Error! if must be followed by an open parenthesis '('");

        Expr *expr = parser_expr();
        if (expr == NULL)
                perror("Error inside if expression.");

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                perror("Error! if must be followed by a closed parenthesis ')'");

        Stmt *body = parser_body(scan_token());
        if (body == NULL)
                perror("Error with body");

        Stmt *else_body = NULL;
        Token next_token = scan_token();
        if (next_token.type == TOKEN_ELSE)
                else_body = parser_body(scan_token());
        else
                putback_token();

        return stmt__create_if_else(expr, body, else_body, NULL);
}

Stmt *parser_for(Token token)
{
        if (!expect_token(TOKEN_OPENPARENTHESIS))
                perror("Error! for must be followed by an open parenthesis '('");

        Expr *init_expr = parser_expr();
        if (init_expr == NULL)
                perror("Error inside for initialization expression");

        Expr *expr = parser_expr();
        if (expr == NULL)
                perror("Error inside for loop expression");

        Expr *next_expr = parser_expr();
        if (next_expr == NULL)
                perror("Error inside for condition expression");

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                perror("Error! for must be followed by a closed parenthesis ')'");

        Stmt *body = parser_body(scan_token());
        if (body == NULL)
                perror("Error with body");

        return stmt__create_for(init_expr, expr, next_expr, body, NULL);
}

Stmt *parser_while(Token token)
{
        if (!expect_token(TOKEN_OPENPARENTHESIS))
                perror("Error! while must be followed by an open parenthesis '('");

        Expr *expr = parser_expr();
        if (expr == NULL)
                perror("Error with while expression");

        if (!expect_token(TOKEN_CLOSEDPARENTHESIS))
                perror("Error! while must be followed by a closed parenthesis ')'");

        Stmt *body = parser_body(scan_token());
        if (body == NULL)
                perror("Error with body");

        return stmt__create_while(expr, body, NULL);
}

Stmt *parser_return(Token token)
{
        Expr *expr = NULL;
        if (!expect_token(TOKEN_SEMICOLON)) {
                expr = parser_expr();
                if (!expect_token(TOKEN_SEMICOLON))
                        perror("Error! Expected ';' after return expression");
        }

        return stmt__create_return(expr, NULL);
}

Stmt *parser_break(Token token)
{
        if (!expect_token(TOKEN_SEMICOLON))
                perror("Error! Expected ';' after break");

        return stmt__create_break(NULL);
}

Stmt *parser_continue(Token token)
{
        if (!expect_token(TOKEN_SEMICOLON))
                perror("Error! Expected ';' after continue");

        return stmt__create_continue(NULL);
}

Stmt *parser_goto(Token token)
{
        Token label_token = scan_token();
        if (label_token.type != TOKEN_NON_PROTECTED_WORD)
                perror("Error! Expected label name after goto");

        char *label = label_token.begin;

        if (!expect_token(TOKEN_SEMICOLON))
                perror("Error! Expected ';' after goto label");

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
        case TOKEN_TUX: {
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

/*
 * [AI Note - Why head/tail was considered vs. dummy node]:
 * In this implementation, `Stmt *body = malloc(sizeof(*body))` is allocated as
 * a sentinel/dummy head node. The first actual statement is stored in `body->next`.
 *
 * Why an alternative with `head` and `tail` was previously suggested:
 * 1. Without head/tail, `body` is an uninitialized Stmt node (its `kind` and union
 *    contain undefined garbage values). Any code walking the AST list needs to know
 *    to skip the initial dummy node and start at `body->next`.
 * 2. With `head = NULL` and `tail = NULL`, no dummy node is allocated:
 *    - The first statement becomes `head = stmt; tail = stmt;`.
 *    - Subsequent statements append via `tail->next = stmt; tail = stmt;`.
 *    - The resulting list can be passed directly to `stmt__create_block(head, NULL)`
 *      so that the block's body points directly to the real first statement.
 *
 * Keeping this dummy-head approach is totally fine as long as downstream consumers
 * (destructors, code generators, printers) expect statements starting at `body->next`.
 */
Stmt *parser_body(Token token)
{
        Stmt *body = malloc(sizeof(*body));
        Stmt *current = body;
        bool loop = false;
        if (token.type == TOKEN_OPENBRACE) {
                loop = true;
                token = scan_token();
        }
        do {
                current->next = parser_statement(token);
                if (current->next)
                        current = current->next;
                token = scan_token();
        } while (loop && token.type != TOKEN_CLOSEDBRACE);

        return body;
}
