#ifndef SOL_HASH_TABLE_H
#define SOL_HASH_TABLE_H

#include "string.h"
#include <stddef.h>

struct Hash_table_entry {
	str key;
	void *value;
	struct Hash_table_entry *prev;
};

typedef struct Hash_table {
	struct Hash_table_entry **hash_table_entry_array;
	size_t size;
} Hash_table;

size_t hash_function(str s, size_t table_size);
Hash_table hash_table__init(size_t size);
struct Hash_table_entry *hash_table__insert(Hash_table hash_table, str key, void *value);
void *hash_table__lookup(Hash_table hash_table, str key);

#endif // SOL_HASH_TABLE_H
