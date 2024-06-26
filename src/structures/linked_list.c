/**
 * Includes prototype functions that allow for interacting with linked-lists.
 */

#include "../../headers/structures/linked_list.h"
#include "stdlib.h"
#include "../../headers/util/string_ops.h"
#include "../../headers/exit_codes.h"
#include "stdio.h"

#define VALUE_NOT_FOUND_EXIT_CODE 1

/**
 * Creates a new, empty linked-list.
 * Does so by allocating the required memory on the heap, then setting the list's head to a null pointer.
 * @return a pointer to the new list.
 */
LinkedList *create_list() {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating list");
        return NULL;
    }
    list->head = NULL;
    return list;
}

/**
 * Checks if a linked-list contains an item represented by a given name.
 * Does so by going over every node on the list and checking if its name is is equal to the given name.
 * @param list a pointer to the linked-list to be checked
 * @param name the name to be checked
 * @return 1 if the list contains an item with the given name, 0 otherwise
 */
int list_contains(LinkedList *list, char *name) {
    Node *node = list->head;
    while (node != NULL) {
        if (equal(node->name, name)) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

/**
 * Looks for a name in a linked-list and retrieves the content associated with that name.
 * Does so by going over every item on the list, and if its name is equal to the given name, retrieving its content.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the content should be retrieved from
 * @param name the name to look for
 * @return the content associated with the given name
 */
Content list_get(LinkedList *list, char *name) {
    Node *node = list->head;
    /* an effectively empty content that should only be returned when a code error occurs */
    Content content;
    content.macro = NULL;
    while (node != NULL) {
        if (equal(node->name, name)) {
            return node->content;
        }
        node = node->next;
    }
    /* this part of the code should not be reached, if it is then a bug exists in the code */
    fprintf(stderr, "Code Error: Name not found in data structure!\n");
    return content;
}


/**
 * Looks for a name in a linked-list and retrieves the macro content associated with that name.
 * Does so by getting the content associated with the name, and retrieving its macro content.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the macro should be retrieved from
 * @param name the name to look for
 * @return the macro content associated with the given name
 */
MacroContent list_get_macro(LinkedList *list, char *name) {
    return list_get(list, name).macro;
}

/**
 * Looks for a name in a linked-list and retrieves the symbol content associated with that name.
 * Does so by getting the content associated with the name, and retrieving its symbol content.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the symbol should be retrieved from
 * @param name the name to look for
 * @return the symbol content associated with the given name
 */
SymbolContent list_get_symbol(LinkedList *list, char *name) {
    return list_get(list, name).symbol;
}

/**
 * Adds a content with a given name to a linked-list.
 * Does so by creating a new node, setting its content to be the given content, setting it to point to the current 
 * head of the list as its next node, and finally setting it to be the new head of the list.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param content the content to be added
 */
void list_add(LinkedList *list, char *name, Content content) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating node");
        return;
    }
    node->name = name;
    node->content = content;
    node->next = list->head;
    list->head = node;
}

/**
 * Adds a macro with a given name to a linked-list.
 * Does so by creating a new Content struct with its macro field being the given macro content, and adding it to the
 * list using list_add.
 * @param list a pointer to the list that the macro should be added to
 * @param name the name associated with the macro
 * @param macro_content the content of the macro to be added
 */
void list_add_macro(LinkedList *list, char *name, MacroContent macro_content) {
    Content content;
    content.macro = macro_content;
    list_add(list, name, content);
}

/**
 * Adds a symbol with a given name to a linked-list.
 * Does so by creating a new Content struct with its symbol field being the given symbol content, and adding it to the
 * list using list_add.
 * @param list a pointer to the list that the symbol should be added to
 * @param name the name associated with the symbol
 * @param symbol_content the content of the symbol to be added
 */
void list_add_symbol(LinkedList *list, char *name, SymbolContent symbol_content) {
    Content content;
    content.symbol = symbol_content;
    list_add(list, name, content);
}

/**
 * Frees a linked-list and all of its contents from the memory.
 * Does so by going over every node in the list, getting the next node and freeing the current one, then doing the same
 * with the next node.
 * Finally frees the list pointer.
 * @param list a pointer to the list that should be freed
 */
void free_list(LinkedList *list) {
    Node *node = list->head;
    Node *next;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    free(list);
}
