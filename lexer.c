#include <ctype.h>

typedef enum token_type {} token_type;
typedef struct Token {
	char *begin, *end;
} Token;

typedef struct token_node {
	Token token;
	Token *next;
} Token_node;
Token token_header = nullptr;

token *tokenizer(char *file, uint64_t file_length)
{
	uint64_t count = 0;
	while (count < file_length) {
		
	}
	
	while ()
}

