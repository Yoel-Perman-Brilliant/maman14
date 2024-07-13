/**
 * Includes a linked-list data structure, which consists of nodes that can hold either a string or
 * an integer value, each associated with a certain name.
 * In addition, includes prototype for functions that allow for interacting with linked-lists.
 */
#ifndef MAMAN14_LINKED_LIST_H
#define MAMAN14_LINKED_LIST_H

#include "../fields.h"
#include "stdarg.h"

typedef union {
    MacroContent macro;
    SymbolContent symbol;
} Content;

/**
 * Represents an item in a linked-list. Includes its content, a name associated with the content and a pointer to 
 * the next item on the list.
 */
typedef struct Node {
    char *name;
    Content content;
    struct Node *next;
} Node;

/**
 * Represents a linked-list data structure. Includes a pointer to the first item on the list.
 */
typedef struct LinkedList {
    Node *head;
} LinkedList;

/**
 * Creates a new, empty linked-list.
 * @return a pointer to the new list.
 */
LinkedList *create_list();

/**
 * Checks if a linked-list contains an item represented by a given name.
 * @param list a pointer to the linked-list to be checked
 * @param name the name to be checked
 * @return 1 if the list contains an item with the given name, 0 otherwise
 */
int list_contains(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the content associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the content should be retrieved from
 * @param name the name to look for
 * @return the content associated with the given name
 */
Content list_get(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the macro content associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the macro should be retrieved from
 * @param name the name to look for
 * @return the macro content associated with the given name
 */
MacroContent list_get_macro(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the symbol content associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the symbol should be retrieved from
 * @param name the name to look for
 * @return the symbol content associated with the given name
 */
SymbolContent list_get_symbol(LinkedList *list, char *name);

/**
 * Adds a content with a given name to a linked-list.
 * @param list a pointer to the list that the content should be added to
 * @param name the name associated with the content
 * @param content the content to be added
 */
void list_add(LinkedList *list, char *name, Content content);

/**
 * Adds a macro with a given name to a linked-list.
 * @param list a pointer to the list that the macro should be added to
 * @param name the name of the macro
 * @param macro_content the content of the macro to be added
 */
void list_add_macro(LinkedList *list, char *name, MacroContent macro_content);

/**
 * Adds a symbol with a given name to a linked-list.
 * @param list a pointer to the list that the symbol should be added to
 * @param name the name of the symbol
 * @param symbol_content the content of the symbol to be added
 */
void list_add_symbol(LinkedList *list, char *name, SymbolContent symbol_content);

/**
 * Frees a linked-list and all of its contents from the memory.
 * @param list a pointer to the list that should be freed
 */
void free_list(LinkedList *list);

void list_add_to_all_that_apply(LinkedList *list, int to_add, int (*condition)(SymbolContent symbol));

void list_print_symbols(LinkedList *list);

#endif
