/**
 * Includes functions that allow for interacting with hash-tables.
 */
 
#include "../../headers/structures/hash_table.h"
#include "stdlib.h"
#include "stdio.h"

#define HASH_MULTIPLIER 31

/**
 * Generates a hash value for a given string, that represents the index of the list that a value that has the given
 * string as its name should be entered into.
 * Does so by multiplying each character in the string by a power of 31 (with the power increasing by 1 for each
 * character), and then taking the modulo 101 of the value so it can exists as a slot in the table.
 * @param s the name to be hashed
 * @return the hash value for the given string
 */
unsigned hash(char *string) {
    unsigned hash_val;
    for (hash_val = 0; *string != '\0'; string++) {
        hash_val = *string + HASH_MULTIPLIER * hash_val;
    }
    return hash_val % HASH_TABLE_SIZE;
}

/**
 * Creates a new, empty hash-table.
 * Does so by allocating the necessary memory in the heap, and creating a new list for every slot.
 * @return a pointer to the new table.
 */
HashTable *create_table() {
    HashTable *table = malloc(sizeof(HashTable));
    int i;
    if (table == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating table");
        return NULL;
    }
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        table->lists[i] = create_list();
    }
    return table;
}

/**
 * Checks if a table contains a value represented by a given name.
 * Does so by checking if the list at the index of the name's hash-value contains a value the has the given name.
 * @param table a pointer to the table to be checked
 * @param name the name to be checked
 * @return 1 if the table contains a value with the given name, 0 otherwise
 */
int table_contains(HashTable *table, char *name) {
    return list_contains(table->lists[hash(name)], name);
}

/**
 * Looks for a name in a hash-table and retrieves the value associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its value.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
Value table_get(HashTable *table, char *name) {
    return list_get(table->lists[hash(name)], name);
}

/**
 * Looks for a name in a hash-table and retrieves the integer value associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its integer value.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
int table_get_int(HashTable *table, char *name) {
    return table_get(table, name).ival;
}

/**
 * Looks for a name in a hash-table and retrieves the string value associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its string value.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * @param table a pointer to the hash-table that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
char *table_get_string(HashTable *table, char *name) {
    return table_get(table, name).sval;
}

/**
 * Adds a value with a given name to a hash-table.
 * Does so by adding it to the list at the index of the name's hash-value.
 * @param table a pointer to the table that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void table_add(HashTable *table, char *name, Value value) {
    list_add(table->lists[hash(name)], name, value);
}

/**
 * Adds an integer value with a given name to a hash-table.
 * Does so by adding it to the list at the index of the name's hash-value.
 * @param table a pointer to the table that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void table_add_int(HashTable *table, char *name, int value) {
    list_add_int(table->lists[hash(name)], name, value);
}

/**
 * Adds a string value with a given name to a hash-table.
 * Does so by adding it to the list at the index of the name's hash-value.
 * @param table a pointer to the table that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void table_add_string(HashTable *table, char *name, char *value) {
    list_add_string(table->lists[hash(name)], name, value);
}

/**
 * Frees a hash-table and all of its contents from the memory.
 * Does so by freeing the list in every slot and their contents, and then freeing the pointer to the table.
 * @param table the table to be freed
 */
void free_table(HashTable *table) {
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        free_list(table->lists[i]);
    }
    free(table);
}
