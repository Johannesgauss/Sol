#include "ast.h"
#include <stdlib.h>

// Hash table

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;
typedef struct Symbol
{
	symbol_t kind;
	Type *type;
	char *name;
	int which;
} Symbol;

Symbol symbol__init(symbol_t kind, Type *type, char *name, int which)
{
	Symbol self;
	
	self.kind = kind; 
	self.type = type;
	self.name = name;
	self.which = which;

	return self;
}

struct Scope {
	Symbol *symbol;
	struct Scope *next;
} Scope;

#define HASH_TABLE_SIZE 500
typedef struct Hash_table {
	struct Scope *scope_array[HASH_TABLE_SIZE];
	size_t size;
} Hash_table;

size_t hash_function(const char *str, size_t table_size)
{
	size_t hash = 5381; // hard coded
	int c;
	while ((c=*str++))
		hash = ((hash<<5) + hash) + c;
	return hash % table_size;
}

struct Hash_table_stack {
	Hash_table *hash_table_array;
	size_t hash_table_size;
}; 
// needs to be init at some point, remember that
struct Hash_table_stack hash_table_stack;

void symbol__push(Hash_table hash_table, Symbol symbol)
{
	struct Scope *scope = scope__create();// needs to be created;

	size_t index = hash_function(symbol.name, hash_table.size);
	//if (index > size - 1) { fprintf(stderr, "Your compiler is crap!"); exit(-1); }
	struct Scope *current_scope = hash_table.scope_array[index];
	while (current_scope->next != NULL) current_scope = current_scope->next;
	current_scope->next = scope;


}

void scope_enter()
{
	Hash_table *hash_table = 
}
