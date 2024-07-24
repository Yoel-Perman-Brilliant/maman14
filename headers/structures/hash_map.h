/**
 * Includes a hash-map data structure, which maps a string (name) to its content - either a macro or a symbol.
 * It is based on a hash-table which consists of linked-lists, whose nodes can hold either a macro content or a symbol content,
 * each associated with a certain name.
 * In addition, includes prototype for functions that allow for interacting with hash-maps.
 */
#ifndef MAMAN14_HASH_MAP_H
#define MAMAN14_HASH_MAP_H

#include "linked_list.h"
#include "stdarg.h"

/**
 * The number of slots in the hash-table which the map is based on.
 */
#define MAP_HASH_TABLE_SIZE 101

/**
 * A hash-map data structure, which maps a string (name) to its content - either a macro or a symbol.
 * It is based on a hash-table which consists of linked-lists, whose nodes can hold either a macro content or a symbol
 * content, each associated with a certain name.
 */
typedef struct {
    LinkedList *lists[MAP_HASH_TABLE_SIZE];
} HashMap;

 /**
  * Creates a new, empty hash-map.
  * @return a pointer to the new map.
  */
HashMap *create_map(ContentType content_type);

/**
 * Checks if a hash-map contains an item represented by a given name.
 * 
 * @param map   a pointer to the hash-map to be checked
 * @param name  the name to be checked
 * @return      1 if the hash-map contains an item with the given name, 0 otherwise
 */
int map_contains(HashMap *map, char *name);

/**
 * Looks for a name in a hash-map and retrieves the macro content associated with that name.
 * Should only be used after verifying that the name exists in the map using map_contains.
 * 
 * @param map   a pointer to the hash-map that the macro should be retrieved from
 * @param name  the name to look for
 * @return      a pointer to the macro content associated with the given name
 */
MacroContent *map_get_macro(HashMap *map, char *name);

/**
 * Looks for a name in a hash-map and retrieves the symbol content associated with that name.
 * Should only be used after verifying that the name exists in the map using map_contains.
 * 
 * @param map   a pointer to the hash-map that the symbol should be retrieved from
 * @param name  the name to look for
 * @return      a pointer to the symbol content associated with the given name
 */
SymbolContent *map_get_symbol(HashMap *map, char *name);

/**
 * Adds a macro with a given name to a hash-map.
 * 
 * @param map           a pointer to the map that the macro should be added to
 * @param name          the name of the macro
 * @param macro_content the content of the macro to be added
 */
void map_add_macro(HashMap *map, char *name, MacroContent macro_content);

/**
 * Adds an symbol with a given name to a hash-map.
 * 
 * @param map            a pointer to the map that the symbol should be added to
 * @param name           the name of the symbol
 * @param symbol_content the content of the symbol to be added
 */
void map_add_symbol(HashMap *map, char *name, SymbolContent symbol_content);

/**
 * Frees a hash-map and all of its contents from the memory.
 * 
 * @param map the map to be freed
 */
void free_map(HashMap *map);

/**
 * Adds a given integer to the value of every symbol in a hash-map that meets a given condition.
 * Assumes that that the content of every item in the map is a symbol.
 * 
 * @param map       a pointer to the hash map whose items' values should be changed
 * @param to_add    the integer to add to the values
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void map_add_to_all_that_apply(HashMap *map, int to_add, int (*condition)(SymbolContent symbol));

void map_add_matching_to_list(HashMap *map, LinkedList *list, int (*condition)(SymbolContent symbol));

void map_print_symbols(HashMap *map);

#endif
