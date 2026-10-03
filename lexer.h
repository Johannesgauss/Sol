#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include "string.h"
typedef enum token_type {
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
	TOKEN_SLASH,
	TOKEN_PERCENT,

	TOKEN_OPENPARENTHESIS,
	TOKEN_CLOSEDPARENTHESIS,
	TOKEN_OPENBRACE,
	TOKEN_CLOSEDBRACE,
	TOKEN_OPENBRACKET,
	TOKEN_CLOSEDBRACKET,
	TOKEN_COMMA,
	TOKEN_DEFINETAG,
	TOKEN_NON_PROTECTED_WORD,
	TOKEN_DIGIT,
	TOKEN_STRING,
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
	TOKEN_EXTERN,

	// Types
	TOKEN_CHAR,
	TOKEN_TINY,
	TOKEN_INT,
	TOKEN_LONG,
	TOKEN_TUX,
	TOKEN_VOID,

	TOKEN_ERROR,
        TOKEN_END
} token_type;
typedef struct Token {
	token_type type;
	str data;
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
	char *begin = src;
	Token self;
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
		case ':': {
			self.type = TOKEN_COLON;
			src++; goto END;
		}
		case '/': {
			self.type = TOKEN_SLASH;
			src++; goto END;
		}
		case '%': {
			self.type = TOKEN_PERCENT;
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
		case '[': {
			self.type = TOKEN_OPENBRACKET;
			src++; goto END;
		}
		case ']': {
			self.type = TOKEN_CLOSEDBRACKET;
			src++; goto END;
		}
		case ',': {
			self.type = TOKEN_COMMA;
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
		case '"': {
			src++;
			while (*src != '"' && *src != '\0') {
				if (*src == '\\' && *(src + 1) != '\0') {
					src += 2;
				} else {
					src++;
				}
			}
			self.type = TOKEN_STRING;
			if (*src == '"') {
				src++;
			}
			goto END;
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
		if (len == 2 && !memcmp(begin, "if", len)) {
			self.type = TOKEN_IF;
		} else if (len == 4 && !memcmp(begin, "else", len)) {
			self.type = TOKEN_ELSE;
		} else if (len == 5 && !memcmp(begin, "while", len)) {
			self.type = TOKEN_WHILE;
		} else if (len == 3 && !memcmp(begin, "for", len)) {
			self.type = TOKEN_FOR;
		} else if (len == 6 && !memcmp(begin, "return", len)) {
			self.type = TOKEN_RETURN;
		} else if (len == 8 && !memcmp(begin, "continue", len)) {
			self.type = TOKEN_CONTINUE;
		} else if (len == 5 && !memcmp(begin, "break", len)) {
			self.type = TOKEN_BREAK;
		} else if (len == 4 && !memcmp(begin, "goto", len)) {
			self.type = TOKEN_GOTO;
		} else if (len == 6 && !memcmp(begin, "extern", len)) {
			self.type = TOKEN_EXTERN;
		// types
		} else if (len == 4 && !memcmp(begin, "char", len)) {
			self.type = TOKEN_CHAR;
		} else if (len == 4 && !memcmp(begin, "tiny", len)) {
			self.type = TOKEN_TINY;
		} else if (len == 3 && !memcmp(begin, "int", len)) {
			self.type = TOKEN_INT;
		} else if (len == 4 && !memcmp(begin, "long", len)) {
			self.type = TOKEN_LONG;
		} else if (len == 3 && !memcmp(begin, "tux", len)) {
			self.type = TOKEN_TUX;
		} else if (len == 4 && !memcmp(begin, "void", len)) {
			self.type = TOKEN_VOID;
		} else {
			self.type = TOKEN_NON_PROTECTED_WORD;
		}
		goto END;
	} else {
		self.type = TOKEN_ERROR;
	}
END:
	self.data = str__create(begin, (size_t)(src - begin));
	*src_ptr = src;
	return self;
}
