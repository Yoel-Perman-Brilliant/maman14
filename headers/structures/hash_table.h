/**
 * Includes a hash-table data structure, which consists of linked-lists, whose nodes can hold either a string or
 * an integer value.
 * In addition, includes prototype for functions that allow for interacting with hash-tables.
 */
#ifndef MAMAN14_HASH_TABLE_H
#define MAMAN14_HASH_TABLE_H

#include "linked_list.h"

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
 * Checks if a table contains a value represented by a given name.
 * @param table a pointer to the table to be checked
 * @param name the name to be checked
 * @return 1 if the table contains a value with the given name, 0 otherwise
 */
int table_contains(HashTable *table, char *name);

/**
 * Looks for a name in a hash-table and retrieves the integer value associated with that name.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
int table_get_int(HashTable *table, char *name);

/**
 * Looks for a name in a hash-table and retrieves the string value associated with that name.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
char *table_get_string(HashTable *table, char *name);

/**
 * Adds an integer value with a given name to a hash-table.
 * @param table a pointer to the table that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void table_add_int(HashTable *table, char *name, int value);

/**
 * Adds an string value with a given name to a hash-table.
 * @param table a pointer to the table that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void table_add_string(HashTable *table, char *name, char *value);

/**
 * Frees a hash-table and all of its contents from the memory.
 * @param table the table to be freed
 */
void free_table(HashTable *table);

#endif
