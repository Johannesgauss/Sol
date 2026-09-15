#include "parser_util.h"
#include <stdlib.h>

struct backup_linked_list {
        Token value;
        struct backup_linked_list *previous;
};

typedef struct parser {
        Token head;
        struct backup_linked_list backup;
} Parser;
Parser parser;
/*
        if() {

        }

        if expr commands
        if expr commands else commands
*/


//typedef struct token { Token token; char *ptr; } Token;
Token peek_token()
{
        struct backup_linked_list *new_backup_ll = malloc(sizeof(*new_backup_ll));
        new_backup_ll->previous = parser.backup;
        parser.backup = new_backup_ll;

        parser.backup.value = parser.head;
        parser.backup = *parser.backup.next;
        return get_token(&parser.head.begin);
}

Token scan_token()
{
        Token token; token = get_token(&parser.head.begin);

        return token;
} 

void consume_token()
{
        backup_linked_list *tail;
        while (parser.backup.next) {
                tail = parser.backup.next;
        }

        
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



