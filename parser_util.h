#include "lexer.h"
#include <stdbool.h>
/*
        if() {

        }

        if expr commands
        if expr commands else commands
*/


//typedef struct token { Token token; char *ptr; } Token;


Token scan_token();
Token peek_token();
void consume_token();
void putback_token();

bool expect_token(token_type token_type);
void parser_init(char *source);

