#include "semantic_analysis_util.h"
#include "hash_table.h"
#include <stdlib.h>

#define HASH_TABLE_SIZE 257 //509

struct Hash_table_stack_entry {
	Hash_table hash_table;
	struct Hash_table_stack_entry *prev;
}; 

struct Hash_table_stack {
	struct Hash_table_stack_entry *top;
	size_t size;
};
// needs to be init at some point, remember that
struct Hash_table_stack hash_table_stack;

Symbol symbol__init(symbol_t kind, Type *type, str name, int which, Ast ast)
{
	Symbol self;
	
	self.kind = kind; 
	self.type = type;
	self.name = name;
	self.which = which;
	self.ast = ast;

	return self;
}

Symbol *symbol__create(symbol_t kind, Type *type, str name, int which, Ast ast)
{
	Symbol *self = malloc(sizeof(*self));
	*self = symbol__init(kind, type, name, which, ast);
	if (hash_table_stack.top) {
		hash_table__insert(hash_table_stack.top->hash_table, name, self);
	}

	return self;
}

void hash_table_stack__push(Hash_table hash_table)
{
	struct Hash_table_stack_entry *new_hash_table = malloc(sizeof(*new_hash_table));
	new_hash_table->hash_table = hash_table;
	new_hash_table->prev = hash_table_stack.top;
	hash_table_stack.top = new_hash_table;

	hash_table_stack.size++;	
}

void hash_table_stack__pop()
{
	struct Hash_table_stack_entry *top = hash_table_stack.top;
	hash_table_stack.top = top->prev;
	hash_table_stack.size--;
	free(top);
}

void scope_enter()
{
	Hash_table hash_table = hash_table__init(HASH_TABLE_SIZE);
	hash_table_stack__push(hash_table);
}

void scope_exit()
{
	hash_table_stack__pop();
}

size_t scope_level()
{
	return hash_table_stack.size;
}

void scope_bind(Symbol symbol)
{
	Symbol *self = malloc(sizeof(*self));
	*self = symbol;
	if (hash_table_stack.top) {
		hash_table__insert(hash_table_stack.top->hash_table, symbol.name, self);
	}
}

Symbol *scope_lookup(str name)
{
	struct Hash_table_stack_entry *current = hash_table_stack.top;
	while (current) {
		Symbol *symbol = hash_table__lookup(current->hash_table, name);
		if (symbol != NULL)
			return symbol;
		current = current->prev;
	}

	return NULL;
}

Symbol *scope_lookup_current(str name)
{
	if (!hash_table_stack.top) return NULL;
	return hash_table__lookup(hash_table_stack.top->hash_table, name);
}
