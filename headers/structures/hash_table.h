/**
 * Includes a hash-table data structure, which consists of linked-lists, whose nodes can hold either a string or
 * an integer value, each associated with a certain name.
 * In addition, includes prototype for functions that allow for interacting with hash-tables.
 */
#ifndef MAMAN14_HASH_TABLE_H
#define MAMAN14_HASH_TABLE_H

#include "linked_list.h"
#include "stdarg.h"

#define HASH_TABLE_SIZE 101

/**
 * A hash-table data structure, represented by an array of linked-lists whose nodes can hold either a string or an
 * integer value, which each value having a corresponding name.
 */
typedef struct {
    LinkedList *lists[HASH_TABLE_SIZE];
} HashTable;

 /**
  * Creates a new, empty hash-table.
  * @return a pointer to the new table.
  */
HashTable *create_table();

/**
 * Checks if a hash-table contains an item represented by a given name.
 * @param table a pointer to the hash-table to be checked
 * @param name the name to be checked
 * @return 1 if the hash-table contains an item with the given name, 0 otherwise
 */
int table_contains(HashTable *table, char *name);

/**
 * Looks for a name in a hash-table and retrieves the macro content associated with that name.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the macro should be retrieved from
 * @param name the name to look for
 * @return the macro content associated with the given name
 */
MacroContent table_get_macro(HashTable *table, char *name);

/**
 * Looks for a name in a hash-table and retrieves the symbol content associated with that name.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the symbol should be retrieved from
 * @param name the name to look for
 * @return the symbol content associated with the given name
 */
SymbolContent table_get_symbol(HashTable *table, char *name);

/**
 * Adds a macro with a given name to a hash-table.
 * @param table a pointer to the table that the macro should be added to
 * @param name the name of the macro
 * @param macro_content the content of the macro to be added
 */
void table_add_macro(HashTable *table, char *name, MacroContent macro_content);

/**
 * Adds an symbol with a given name to a hash-table.
 * @param table a pointer to the table that the symbol should be added to
 * @param name the name of the symbol
 * @param symbol_content the content of the symbol to be added
 */
void table_add_symbol(HashTable *table, char *name, SymbolContent symbol_content);

/**
 * Frees a hash-table and all of its contents from the memory.
 * @param table the table to be freed
 */
void free_table(HashTable *table);

void table_add_to_all_that_apply(HashTable *table, int to_add, int (*condition)(SymbolContent symbol));

void table_print_symbols(HashTable *table);

#endif
