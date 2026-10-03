#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

size_t hash_function(str s, size_t table_size)
{
	size_t hash = 5381; // hard coded
	for (size_t i = 0; i < s.size; i++) {
		int c = s.data[i];
		hash = ((hash<<5) + hash) + c;
	}
	return hash % table_size;
}

Hash_table hash_table__init(size_t size)
{
	struct Hash_table self;

	self.hash_table_entry_array = calloc(size, sizeof(self.hash_table_entry_array)); 
	self.size = size;

	return self;
}

struct Hash_table_entry *hash_table__insert(Hash_table hash_table, str key, void *value)
{
	struct Hash_table_entry *hash_table_entry = malloc(sizeof(*hash_table_entry));
	hash_table_entry->key = key;
	hash_table_entry->value = value;

	size_t index = hash_function(key, hash_table.size);
	struct Hash_table_entry *current_hash_table_entry = hash_table.hash_table_entry_array[index];
	hash_table_entry->prev = current_hash_table_entry;
	hash_table.hash_table_entry_array[index] = hash_table_entry;

	return hash_table_entry;
}

void *hash_table__lookup(Hash_table hash_table, str key)
{
	size_t index = hash_function(key, hash_table.size);
	struct Hash_table_entry *current_hash_table_entry = hash_table.hash_table_entry_array[index];
	while (current_hash_table_entry != NULL &&
	       (current_hash_table_entry->key.size != key.size ||
	        memcmp(current_hash_table_entry->key.data, key.data, key.size) != 0)) {
		current_hash_table_entry = current_hash_table_entry->prev;
	}
	if (current_hash_table_entry != NULL)
		return current_hash_table_entry->value;

	return NULL;
}
