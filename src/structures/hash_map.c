/**
 * Includes functions that allow for interacting with hash-maps.
 */

#include "../../headers/structures/hash_map.h"
#include "stdlib.h"
#include "stdio.h"
#include "../../headers/exit_codes.h"

#define HASH_MULTIPLIER 31

/**
 * Generates a hash value for a given string, that represents the index of the list that a value which has the given
 * string as its name should be entered into.
 * Does so by multiplying each character in the string by a power of 31 (with the power increasing by 1 for each
 * character), and then taking the modulo 101 of the value so it can exists as a slot in the map.
 * 
 * @param  s the name to be hashed
 * @return the hash value for the given string
 */
unsigned map_hash(char *string) {
    unsigned hash_val;
    for (hash_val = 0; *string != '\0'; string++) {
        hash_val = *string + HASH_MULTIPLIER * hash_val;
    }
    return hash_val % MAP_HASH_TABLE_SIZE;
}

/**
 * Creates a new, empty hash-map.
 * Does so by allocating the necessary memory in the heap, and creating a new list for every slot.
 * 
 * @return a pointer to the new map.
 */
HashMap *create_map(ContentType content_type) {
    HashMap *map = malloc(sizeof(HashMap));
    int i;
    if (map == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating map");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    for (i = 0; i < MAP_HASH_TABLE_SIZE; i++) {
        map->lists[i] = create_list(content_type);
    }
    return map;
}

/**
 * Checks if a map contains an item represented by a given name.
 * Does so by checking if the list at the index of the name's hash-value contains an item that has the given name.
 * 
 * @param map   a pointer to the map to be checked
 * @param name  the name to be checked
 * @return      1 if the map contains an item with the given name, 0 otherwise
 */
int map_contains(HashMap *map, char *name) {
    return list_contains(map->lists[map_hash(name)], name);
}

/**
 * Looks for a name in a hash-map and retrieves the content associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its content.
 * Should only be used after verifying that the name exists in the map using map_contains.
 * 
 * @param map   a pointer to the hash-map that the content should be retrieved from
 * @param name  the name to look for
 * @return      a pointer to the content associated with the given name
 */
Content *map_get(HashMap *map, char *name) {
    return list_get(map->lists[map_hash(name)], name);
}

/**
 * Looks for a name in a hash-map and retrieves the macro content associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its macro content.
 * Should only be used after verifying that the name exists in the map using map_contains.
 * 
 * @param map   a pointer to the hash-map that the macro should be retrieved from
 * @param name  the name to look for
 * @return      a pointer to the macro content associated with the given name
 */
MacroContent *map_get_macro(HashMap *map, char *name) {
    return &map_get(map, name)->macro;
}

/**
 * Looks for a name in a hash-map and retrieves the symbol content associated with that name.
 * Does so by looking for the name in the list at the index of the name's hash-value and retrieving its symbol content.
 * Should only be used after verifying that the name exists in the map using map_contains.
 * 
 * @param map   a pointer to the hash-map that the symbol should be retrieved from
 * @param name  the name to look for
 * @return      a pointer to the symbol content associated with the given name
 */
SymbolContent *map_get_symbol(HashMap *map, char *name) {
    return &map_get(map, name)->symbol;
}

/**
 * Adds an item with a given name to a hash-map.
 * Does so by adding it to the list at the index of the name's hash-value.
 * 
 * @param map           a pointer to the map that the item should be added to
 * @param name          the name of the item
 * @param macro_content the content of the item to be added
 */
void map_add(HashMap *map, char *name, Content content) {
    list_add(map->lists[map_hash(name)], name, content);
}

/**
 * Adds a macro with a given name to a hash-map.
 * Does so by adding it to the list at the index of the name's hash-value.
 * 
 * @param map           a pointer to the map that the macro should be added to
 * @param name          the name of the macro
 * @param macro_content the content of the macro to be added
 */
void map_add_macro(HashMap *map, char *name, MacroContent macro_content) {
    list_add_macro(map->lists[map_hash(name)], name, macro_content);
}

/**
 * Adds an symbol with a given name to a hash-map.
 * Does so by adding it to the list at the index of the name's hash-value.
 * 
 * @param map           a pointer to the map that the symbol should be added to
 * @param name           the name of the symbol
 * @param symbol_content the content of the symbol to be added
 */
void map_add_symbol(HashMap *map, char *name, SymbolContent symbol_content) {
    list_add_symbol(map->lists[map_hash(name)], name, symbol_content);
}

/**
 * Frees a hash-map and all of its contents from the memory.
 * Does so by freeing the list in every slot and their contents, and then freeing the pointer to the map.
 * 
 * @param map the map to be freed
 */
void free_map(HashMap *map) {
    int i;
    for (i = 0; i < MAP_HASH_TABLE_SIZE; i++) {
        deep_free_list(map->lists[i]);
    }
    free(map);
}

/**
 * Adds a given integer to the value of every symbol in a hash-map that meets a given condition.
 * Does so by applying the change to the list in every slot.  
 * Assumes that that the content of every item in the map is a symbol.
 * 
 * @param map       a pointer to the hash map whose items' values should be changed
 * @param to_add    the integer to add to the values
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void map_add_to_all_that_apply(HashMap *map, int to_add, int (*condition)(SymbolContent symbol)) {
    int i;
    for (i = 0; i < MAP_HASH_TABLE_SIZE; i++) {
        list_add_to_all_that_apply(map->lists[i], to_add, condition);
    }
}

void map_print_symbols(HashMap *map) {
    int i;
    for (i = 0; i < MAP_HASH_TABLE_SIZE; i++) {
        list_print_symbols(map->lists[i]);
    }
}

void map_add_matching_to_list(HashMap *map, LinkedList *list, int (*condition)(SymbolContent symbol)) {
    int i;
    for (i = 0; i < MAP_HASH_TABLE_SIZE; i++) {
        list_add_matching_to_list(map->lists[i], list, condition);
    }
}

