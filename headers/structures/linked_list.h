/**
 * Includes a linked-list data structure, which consists of nodes that can hold either a macro content, a symbol content
 * or an integer, each associated with a certain name.
 * Items containing integers are treated as having no name, since the integer itself represents the item at every
 * use case in the project.
 * In addition, includes prototype for functions that allow for interacting with linked-lists.
 * All functions assumes that the given pointer to a list is not null.
 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../fields.h"
#include "../symbols.h"

/**
 * Represents a macro's content, which is effectively one string.
 */
typedef char *MacroContent;

/**
 * Represents the content of an item in a linked list - either a macro content, a symbol content or an integer.
 */
typedef union {
    MacroContent macro;
    SymbolContent symbol;
    int num;
} Content;

typedef enum {
    INTEGER, SYMBOL, MACRO
} ContentType;

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
    ContentType content_type;
} LinkedList;

/**
 * Creates a new, empty linked-list.
 * 
 * @param content_type the type that the list's items' content should be
 * @return a pointer to the new list, or NULL if an allocation failure has occurred
 */
LinkedList *create_list(ContentType content_type);

/**
 * Checks if a linked-list is empty.
 * 
 * @param list a pointer to the list that should be checked
 * @return 1 if the list is empty, 0 otherwise
 */
int list_empty(LinkedList *list);

/**
 * Checks if a linked-list contains an item represented by a given name.
 * 
 * @param list a pointer to the linked-list to be checked
 * @param name the name to be checked
 * @return 1 if the list contains an item with the given name, 0 otherwise
 */
int list_contains(LinkedList *list, char *name);

/**
 * Checks if a linked-list contains given integer (as content).
 * 
 * @param list a pointer to the linked-list to be checked
 * @param num the integer to be found
 * @return 1 if the list includes num, 0 otherwise
 */
int list_contains_int(LinkedList *list, int num);

/**
 * Looks for a name in a linked-list and retrieves the content associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * 
 * @param list a pointer to the linked-list that the content should be retrieved from
 * @param name the name to look for
 * @return a pointer to the content associated with the given name
 */
Content *list_get(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the macro content associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * 
 * @param list a pointer to the linked-list that the macro should be retrieved from
 * @param name the name to look for
 * @return a pointer to the macro content associated with the given name
 */
MacroContent *list_get_macro(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the symbol content associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * 
 * @param list a pointer to the linked-list that the symbol should be retrieved from
 * @param name the name to look for
 * @return a pointer to the symbol content associated with the given name
 */
SymbolContent *list_get_symbol(LinkedList *list, char *name);

/**
 * Adds a content with a given name to a linked-list.
 * 
 * @param list    a pointer to the list that the content should be added to
 * @param name    the name associated with the content
 * @param content the content to be added
 */
void list_add(LinkedList *list, char *name, Content content);

/**
 * Adds a macro with a given name to a linked-list.
 * 
 * @param list          a pointer to the list that the macro should be added to
 * @param name          the name of the macro
 * @param macro_content the content of the macro to be added
 */
void list_add_macro(LinkedList *list, char *name, MacroContent macro_content);

/**
 * Adds a symbol with a given name to a linked-list.
 * 
 * @param list           a pointer to the list that the symbol should be added to
 * @param name           the name of the symbol
 * @param symbol_content the content of the symbol to be added
 */
void list_add_symbol(LinkedList *list, char *name, SymbolContent symbol_content);

/**
 * Adds an integer to a linked-list.
 * 
 * @param list          a pointer to the list that the integer should be added to
 * @param num           the integer that should be added
 */
void list_add_int(LinkedList *list, int num);

/**
 * Frees a linked-list and all of its items' names and contents from the memory.
 * Assumes that all of the list's contents are of the same type.
 * 
 * @param list a pointer to the list that should be freed
 */
void deep_free_list(LinkedList *list);

/**
 * Frees a linked-list and its nodes from the memory, without freeing the nodes' names and contents.
 * 
 * @param list a pointer to the list that should be freed
 */
void shallow_free_list(LinkedList *list);

/**
 * Adds a given integer to the value of every symbol in a hash-linked-list that meets a given condition.
 * Assumes that that the content of every item in the list is a symbol.
 * 
 * @param list      a pointer to the list whose items' values should be changed
 * @param to_add    the integer to add to the values
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void list_add_to_all_that_apply(LinkedList *list, int to_add, int (*condition)(SymbolContent symbol));

/**
 * Adds every symbol in a given linked-list that meets a given condition to another given linked-list.
 * Importantly, copies of the symbols are added, rather then references to the same symbols, and copies of the pointers
 * representing the names are added.
 * Assumes every item on the first list is a symbol.
 * 
 * @param list1     the list whose symbols should be added to the other list
 * @param list2     the list that the symbols should be added to
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void list_add_matching_to_list(LinkedList *list1, LinkedList *list2, int (*condition)(SymbolContent symbol));

#endif
