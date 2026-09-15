#include "lexer.h"
#include <stdbool.h>

Token parser_init();
Token peek_token();
Token scan_token();
void consume_all_tokens();
void consume_token();
bool expect_token(token_type token_type);


void putback_token();
void putback_all_tokens();
