#ifndef SOL_STRING_H
#define SOL_STRING_H

#include <string.h>
#include <stddef.h>

typedef struct str {
	char *data;
	size_t size;
} str;

// Backwards compatibility alias
typedef struct str string;

static inline str str__create(char *data, size_t size)
{
	str self;

	self.data = data;
	self.size = size;

	return self;
}

static inline str str__from_token(char *begin, char *end)
{
	return str__create(begin, (size_t)(end - begin));
}

#endif // SOL_STRING_H
