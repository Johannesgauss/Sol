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

bool parser_stat()
{
        return true; 
}

bool parser_if_S()
{
        Token token = scan_token();
        if (token.type == IF) {
                if(expect_token(OPENPARENTHESIS) && parser_stat() && expect_token(CLOSEDPARENTHESIS) && parser_expr()) {
                }
        }
}

