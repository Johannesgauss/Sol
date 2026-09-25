#include "parser_util.h"
#include <stdlib.h>

struct backup_linked_list {
	Token value;
	struct backup_linked_list *previous;
};

typedef struct parser {
	Token head;
	struct backup_linked_list *backup;
} Parser;
Parser parser;
/*
	if() {
	}
	if expr commands
	if expr commands else commands
*/


//typedef struct token { Token token; char *ptr; } Token;
Token parser_init()
{
	parser.head = scan_token();
	parser.backup = malloc(sizeof(*parser.backup));
	parser.backup->value = parser.head;
	parser.backup->previous = NULL;

	return parser.head;
}

Token peek_token()
{
	struct backup_linked_list *new_backup_ll = malloc(sizeof(*new_backup_ll));
	new_backup_ll->previous = parser.backup;
	parser.backup = new_backup_ll;
	parser.backup->value = parser.head;

	Token token = get_token(&parser.head.begin);
	parser.head = token;

	return token;
}

Token scan_token()
{
	Token token = peek_token();
	consume_token();

	return token;
} 

void consume_all_tokens()
{
	while (parser.backup->previous)
		consume_token();
}

void consume_token()
{
	struct backup_linked_list *tail;
	tail = parser.backup->previous;
	free(parser.backup);
	parser.backup = tail;
}

void putback_token()
{
	parser.head = parser.backup->value;
	struct backup_linked_list *tail = parser.backup->previous;
	free(parser.backup);
	parser.backup = tail;
}

void putback_all_tokens()
{
	while (parser.backup->previous)
		putback_token();
}

bool expect_token(token_type token_type)
{
	peek_token();
	if (parser.head.type == token_type) {
		consume_token();
		return true;
	} else {
		putback_token();
		return false;
	}
}
