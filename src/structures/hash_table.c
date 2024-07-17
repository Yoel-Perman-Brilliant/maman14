/**
 * Includes functions that allow for interacting with hash-tables.
 */
 
#include "../../headers/structures/hash_table.h"
#include "stdlib.h"
#include "stdio.h"
#include "../../headers/exit_codes.h"

#define HASH_MULTIPLIER 31

/**
 * Generates a hash value for a given string, that represents the index of the list that a value that has the given
 * string as its name should be entered into.
 * Does so by multiplying each character in the string by a power of 31 (with the power increasing by 1 for each
 * character), and then taking the modulo 101 of the value so it can exists as a slot in the table.
 * 
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
 * 
 * @return a pointer to the new table.
 */
HashTable *create_table() {
    HashTable *table = malloc(sizeof(HashTable));
    int i;
    if (table == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating table");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        table->lists[i] = create_list();
    }
    return table;
}

/**
 * Checks if a table contains an item represented by a given name.
 * Does so by checking if the list at the index of the name's hash-value contains an item that has the given name.
 * 
 * @param table a pointer to the table to be checked
 * @param name  the name to be checked
 * @return 1 if the table contains an item with the given name, 0 otherwise
 */
int table_contains(HashTable *table, char *name) {
    return list_contains(table->lists[hash(name)], name);
}

/**
 * Looks for a name in a hash-table and retrieves the content associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its content.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * 
 * @param table a pointer to the hash-table that the content should be retrieved from
 * @param name  the name to look for
 * @return a pointer to the content associated with the given name
 */
Content *table_get(HashTable *table, char *name) {
    return list_get(table->lists[hash(name)], name);
}

/**
 * Looks for a name in a hash-table and retrieves the macro content associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its macro content.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * 
 * @param table a pointer to the hash-table that the macro should be retrieved from
 * @param name  the name to look for
 * @return a pointer to the macro content associated with the given name
 */
MacroContent *table_get_macro(HashTable *table, char *name) {
    return &table_get(table, name)->macro;
}

/**
 * Looks for a name in a hash-table and retrieves the symbol content associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its symbol content.
 * Should only be used after verifying that the name exists in the table using table_contains.
 * 
 * @param table a pointer to the hash-table that the symbol should be retrieved from
 * @param name  the name to look for
 * @return a pointer to the symbol content associated with the given name
 */
SymbolContent *table_get_symbol(HashTable *table, char *name) {
    return &table_get(table, name)->symbol;
}

/**
 * Adds an item with a given name to a hash-table.
 * Does so by adding it to the list at the index of the name's hash-value.
 * 
 * @param table         a pointer to the table that the item should be added to
 * @param name          the name of the item
 * @param macro_content the content of the item to be added
 */
void table_add(HashTable *table, char *name, Content content) {
    list_add(table->lists[hash(name)], name, content);
}

/**
 * Adds a macro with a given name to a hash-table.
 * Does so by adding it to the list at the index of the name's hash-value.
 * 
 * @param table         a pointer to the table that the macro should be added to
 * @param name          the name of the macro
 * @param macro_content the content of the macro to be added
 */
void table_add_macro(HashTable *table, char *name, MacroContent macro_content) {
    list_add_macro(table->lists[hash(name)], name, macro_content);

}

/**
 * Adds an symbol with a given name to a hash-table.
 * Does so by adding it to the list at the index of the name's hash-value.
 * 
 * @param table          a pointer to the table that the symbol should be added to
 * @param name           the name of the symbol
 * @param symbol_content the content of the symbol to be added
 */
void table_add_symbol(HashTable *table, char *name, SymbolContent symbol_content) {
    list_add_symbol(table->lists[hash(name)], name, symbol_content);

}

/**
 * Frees a hash-table and all of its contents from the memory.
 * Does so by freeing the list in every slot and their contents, and then freeing the pointer to the table.
 * 
 * @param table the table to be freed
 */
void free_table(HashTable *table) {
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        free_list(table->lists[i]);
    }
    free(table);
}

/**
 * Adds a given integer to the value of every symbol in a hash-table that meets a given condition.
 * Does so by applying the change to the list in every slot.  
 * Assumes that that the content of every item in the table is a symbol.
 * 
 * @param table     a pointer to the hash table whose items' values should be changed
 * @param to_add    the integer to add to the values
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void table_add_to_all_that_apply(HashTable *table, int to_add, int (*condition)(SymbolContent symbol)) {
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        list_add_to_all_that_apply(table->lists[i], to_add, condition);
    }
}

void table_print_symbols(HashTable *table) {
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        list_print_symbols(table->lists[i]);
    }
}
