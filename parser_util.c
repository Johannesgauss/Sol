#include "parser_util.h"

typedef struct parser {
        Token head;
        Token backup;
} Parser;
Parser parser;
/*
        if() {

        }

        if expr commands
        if expr commands else commands
*/


//typedef struct token { Token token; char *ptr; } Token;

Token scan_token()
{
        parser.backup = parser.head;
        Token token; token = get_token(&parser.head.begin);

        return token;
} 

void putback_token()
{
        parser.head = parser.backup;
}

bool expect_token(token_type token_type)
{
        scan_token();
        if (parser.head.type == token_type) {
                return true;
        } else {
                putback_token();
                return false;
        }
}



