#include "semantic_analysis_util.h"
#include <stdlib.h>
#define HASH_TABLE_SIZE 257 //509

struct Hash_table_entry {
	Symbol symbol;
	struct Hash_table_entry *prev;
};

typedef struct Hash_table {
	struct Hash_table_entry **hash_table_entry_array;
	size_t size;
} Hash_table;
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

size_t hash_function(str s, size_t table_size)
{
	size_t hash = 5381; // hard coded
	for (size_t i = 0; i < s.size; i++) {
		int c = s.data[i];
		hash = ((hash<<5) + hash) + c;
	}
	return hash % table_size;
}
// Hash table
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

struct Hash_table_entry *symbol__push(Hash_table hash_table, Symbol symbol)
{
	struct Hash_table_entry *hash_table_entry = malloc(sizeof(*hash_table_entry));
	hash_table_entry->symbol = symbol;

	size_t index = hash_function(symbol.name, hash_table.size);
	//if (index > size - 1) { fprintf(stderr, "Your compiler is crap!"); exit(-1); }
	struct Hash_table_entry *current_hash_table_entry = hash_table.hash_table_entry_array[index];
	//if (current_hash_table_entry != NULL)
		hash_table_entry->prev = current_hash_table_entry;
	hash_table.hash_table_entry_array[index] = hash_table_entry;

	return current_hash_table_entry;
}

Symbol *symbol__create(symbol_t kind, Type *type, str name, int which, Ast ast)
{
	Symbol self = symbol__init(kind, type, name, which, ast);
	struct Hash_table_entry *htentry_ptr = symbol__push(hash_table_stack.top->hash_table, self);

	return htentry_ptr->symbol;
}


Hash_table hash_table__init(size_t size)
{
	struct Hash_table self;// = malloc(sizeof(*self));

	self.hash_table_entry_array = calloc(size, sizeof(self.hash_table_entry_array)); 
	self.size = size;

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
	symbol__push(hash_table_stack.top->hash_table, symbol);
}

Symbol *scope_lookup(str name)
{
	struct Hash_table_stack_entry *current = hash_table_stack.top;
	while (current) {
		size_t index = hash_function(name, current->hash_table.size);
		struct Hash_table_entry *current_hash_table_entry = current->hash_table.hash_table_entry_array[index];
		while (current_hash_table_entry != NULL &&
		       (current_hash_table_entry->symbol.name.size != name.size ||
		        memcmp(current_hash_table_entry->symbol.name.data, name.data, name.size) != 0)) {
			current_hash_table_entry = current_hash_table_entry->prev;
		}
		if (current_hash_table_entry != NULL)
			return &current_hash_table_entry->symbol;
		current = current->prev;
	}

	return NULL;
}

Symbol *scope_lookup_current(str name)
{
	if (!hash_table_stack.top) return NULL;
	struct Hash_table_stack_entry *current = hash_table_stack.top;

	size_t index = hash_function(name, current->hash_table.size);
	struct Hash_table_entry *current_hash_table_entry = current->hash_table.hash_table_entry_array[index];
	while (current_hash_table_entry != NULL &&
	       (current_hash_table_entry->symbol.name.size != name.size ||
	        memcmp(current_hash_table_entry->symbol.name.data, name.data, name.size) != 0)) {
		current_hash_table_entry = current_hash_table_entry->prev;
	}
	if (current_hash_table_entry != NULL)
		return &current_hash_table_entry->symbol;

	return NULL;
}
