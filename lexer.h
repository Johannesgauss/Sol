#include <ctype.h>
#include <stddef.h>
#include <string.h>
typedef enum token_type {
        TOKEN__INTERNAL_USAGE__SYMBOLS_BEGIN

	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_EQUAL,
	TOKEN_EQUALEQUAL,
	TOKEN_DIFFERENT,
	TOKEN_SEMICOLON,
	TOKEN_COLON,
	TOKEN_ASTERISK,
	TOKEN_AMPERSAND,
	TOKEN_LT,
	TOKEN_GT,
	TOKEN_GE,
	TOKEN_LE,
	TOKEN_NEG,

        TOKEN__INTERNAL_USAGE__SYMBOLS_END

	TOKEN_OPENPARENTHESIS,
	TOKEN_CLOSEDPARENTHESIS,
	TOKEN_OPENBRACE,
	TOKEN_CLOSEDBRACE,
	TOKEN_DEFINETAG,
	TOKEN_NON_PROTECTED_WORD,
	TOKEN_DIGIT,
	TOKEN_EOF,

	// Protected words
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_WHILE,
	TOKEN_FOR,
	TOKEN_RETURN,
	TOKEN_CONTINUE,
	TOKEN_BREAK,
	TOKEN_GOTO,

	// Types
	TOKEN_CHAR,
	TOKEN_TINY,
	TOKEN_INT,
	TOKEN_LONG,
	TOKEN_TUX,

	TOKEN_ERROR,
        TOKEN_END
} token_type;
typedef struct Token {
	token_type type;
	char *begin, *end;
} Token;

static inline Token get_token(char **src_ptr)
{
	char *src = *src_ptr;
	while (1) { // AI suggestion, I must change it
		while (*src == ' ' || *src == '\t' || *src == '\n') src++;
		if (src[0] == '/' && src[1] == '/') { 
			size_t i = 0; 
			while (*src != '\n' && *src != '\0') 
				src++; 
			if (*src != '\0') src++;
		} else break;
	}
	Token self; self.begin = src; self.end = src;
	if (*src == '\0') {
		self.type = TOKEN_EOF;
		goto END;
	}

	switch(*src) {
		case '=': {
			char *next = src+1; 
			if (*next == '=')  {
				self.type = TOKEN_EQUALEQUAL;
				src++;
			} else {
				self.type = TOKEN_EQUAL;
			}
			src++; goto END;
		}
		case '!': {
			char *next = src+1; 
			if (*next == '=')  {
				self.type = TOKEN_DIFFERENT;
			} else {
				self.type = TOKEN_NEG;
			}
			src++; goto END;
		}
		case '-': {
			self.type = TOKEN_MINUS;
			src++; goto END;
		}
		case '+': {
			self.type = TOKEN_PLUS;
			src++; goto END;
		}
		case '#': {
			self.type = TOKEN_DEFINETAG;
			src++; goto END;
		}
		case ';': {
			self.type = TOKEN_SEMICOLON;
			src++; goto END;
		}
		case '&': {
			self.type = TOKEN_AMPERSAND;
			src++; goto END;
		}
		case '*': {
			self.type = TOKEN_ASTERISK;
			src++; goto END;
		}
		case '(': {
			self.type = TOKEN_OPENPARENTHESIS;
			src++; goto END;
		}
		case ')': {
			self.type = TOKEN_CLOSEDPARENTHESIS;
			src++; goto END;
		}
		case '{': {
			self.type = TOKEN_OPENBRACE;
			src++; goto END;
		}
		case '}': {
			self.type = TOKEN_CLOSEDBRACE;
			src++; goto END;
		}
		case '<': {
			char *next = src+1; 
			if (*next == '=')  {
				self.type = TOKEN_LE;
				src++;
			} else {
				self.type = TOKEN_LT;
			}
			src++; goto END;
		}
		case '>': {
			char *next = src+1; 
			if (*next == '=')  {
				self.type = TOKEN_GE;
				src++;
			} else {
				self.type = TOKEN_GT;
			}
			src++; goto END;
		}
	}
	if (isdigit(*src)) {
		self.type = TOKEN_DIGIT;
		while (isdigit(*src)) {
			src++;
		}
		goto END;
	}
	if (isalpha(*src) || *src == '_') {
		size_t len = 0; while (isalnum(*src) || *src == '_') {
			src++; len++;
		}
		if (len == 2 && !memcmp(self.begin, "if", len)) {
			self.type = TOKEN_IF;
		} else if (len == 4 && !memcmp(self.begin, "else", len)) {
			self.type = TOKEN_ELSE;
		} else if (len == 5 && !memcmp(self.begin, "while", len)) {
			self.type = TOKEN_WHILE;
		} else if (len == 3 && !memcmp(self.begin, "for", len)) {
			self.type = TOKEN_FOR;
		} else if (len == 6 && !memcmp(self.begin, "return", len)) {
			self.type = TOKEN_RETURN;
		} else if (len == 8 && !memcmp(self.begin, "continue", len)) {
			self.type = TOKEN_CONTINUE;
		} else if (len == 5 && !memcmp(self.begin, "break", len)) {
			self.type = TOKEN_BREAK;
		} else if (len == 4 && !memcmp(self.begin, "goto", len)) {
			self.type = TOKEN_GOTO;
		// types
		} else if (len == 4 && !memcmp(self.begin, "char", len)) {
			self.type = TOKEN_CHAR;
		} else if (len == 4 && !memcmp(self.begin, "tiny", len)) {
			self.type = TOKEN_TINY;
		} else if (len == 3 && !memcmp(self.begin, "int", len)) {
			self.type = TOKEN_INT;
		} else if (len == 4 && !memcmp(self.begin, "long", len)) {
			self.type = TOKEN_LONG;
		} else if (len == 3 && !memcmp(self.begin, "tux", len)) {
			self.type = TOKEN_TUX;
		} else {
			self.type = TOKEN_NON_PROTECTED_WORD;
		}
		goto END;
	} else {
		self.type = TOKEN_ERROR;
	}
END:
	self.end = src;
	*src_ptr = src;
	return self;
}
