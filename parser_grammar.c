#include "parser_util.h"

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

Decl *parser_decl()
{
        
        return decl__create_value
}


Stmt *parser_if_else(Token token)
{
        Stmt *if_else_stmt = malloc(sizeof(*if_else_stmt));
        if (!(expect_token(TOKEN_OPENPARENTHESIS)))
                perror("Error! if must be followed by an open parenthesis '('");
        if(if_else_stmt->if_else_block.expr = parser_expr() == NULL)
                perror("Error inside if expression.");
                
        if (!(expect_token(TOKEN_CLOSEDPARENTHESIS)))
                perror("Error! if must be followed by a closed parenthesis ')'");
        if(if_else_stmt->if_else_block.body = parser_body() == NULL)
                perror("Error with body");
        Token next_token = scan_token();
        if (next_token.type == TOKEN_ELSE)
                if_else_stmt.if_else_block.else_body = parser_body();
        else
                putback_token();

        return if_else_stmt;
}

Stmt *parser_body()
{
        Token token = scan_token();
        Stmt *body = malloc(sizeof(*body));
        Stmt *current = body;
        bool loop = false;
        if (token.type == TOKEN_OPENBRACE)
                loop = true;
        do {
                switch(token.type) {
                case TOKEN_IF:
                        current->next = parser_if_else(token);
                case TOKEN_FOR:
                        current->next = parser_for(token);
                } 
                current = current->next;
        token = scan_token() 
        } while (loop && token.type != TOKEN_CLOSEDBRACE) 

        return body;
}

Stmt *parser_for(Token token)
{
        Stmt *for_stmt = malloc(sizeof(*for_stmt));

        if (!(expect_token(TOKEN_OPENPARENTHESIS)))
                perror("Error! if must be followed by an open parenthesis '('");

        if(if_else_stmt->for_block.init_expr = parser_expr() == NULL);
                perror("Error inside for initialization expression");

        if(if_else_stmt->for_block.expr = parser_expr() == NULL);
                perror("Error inside for loop expression");

        if(if_else_stmt->for_block.next_expr = parser_expr() == NULL);
                perror("Error inside for condition expression");

        if (!(expect_token(TOKEN_CLOSEDPARENTHESIS)))
                perror("Error! for must be followed by a closed parenthesis ')'");

        if(if_else_stmt->for_block.body = parser_body() == NULL)
                perror("Error with body");
        
        return for_stmt;
}


Stmt *parser_while(Token token)
{
        Stmt *for_stmt = malloc(sizeof(*for_stmt));

        if (!(expect_token(TOKEN_OPENPARENTHESIS)))
                perror("Error! if must be followed by an open parenthesis '('");

        if(if_else_stmt->while_block.expr = parser_expr() == NULL);
                perror("Error with while expression");

        if (!(expect_token(TOKEN_CLOSEDPARENTHESIS)))
                perror("Error! while must be followed by a closed parenthesis ')'");

        if(if_else_stmt->for_block.body = parser_body() == NULL)
                perror("Error with body");
        
        return for_stmt;
}
