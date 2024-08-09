/**
 * Includes functions that allow for interacting with linked-lists.
 */

#include "../../headers/structures/linked_list.h"
#include "stdlib.h"
#include "../../headers/util/string_ops.h"
#include "../../headers/exit_codes.h"
#include "stdio.h"
#include "../../headers/util/general_util.h"

#define VALUE_NOT_FOUND_EXIT_CODE 1

/**
 * Creates a new, empty linked-list.
 * Does so by allocating the required memory on the heap, then setting the list's head to a null pointer.
 * Also sets the content_type field to the given type.
 * 
 * @param content_type the type that the list's items' content should be
 * @return a pointer to the new list
 */
LinkedList *create_list(ContentType content_type) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating list");
        set_alloc_failure();
        return NULL;
    }
    list->head = NULL;
    list->content_type = content_type;
    return list;
}

/**
 * Checks if a linked-list is empty.
 * Do so by checking if the head is null.
 * 
 * @param list a pointer to the list that should be checked
 * @return 1 if the list is empty, 0 otherwise
 */
int list_empty(LinkedList *list) {
    return list->head == NULL;
}

/**
 * Checks if a linked-list contains an item represented by a given name.
 * Does so by going over every node on the list and checking if its name is is equal to the given name.
 * 
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
 * Checks if a linked-list contains given integer (as content).
 * Does so by going over every node on the list and checking if its content's integer value if equal to
 * the given integer.
 * 
 * @param list a pointer to the linked-list to be checked
 * @param num the integer to be found
 * @return 1 if the list includes num, 0 otherwise
 */
int list_contains_int(LinkedList *list, int num) {
    Node *node = list->head;
    while (node != NULL) {
        if (node->content.num == num) {
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
 * 
 * @param list a pointer to the linked-list that the content should be retrieved from
 * @param name the name to look for
 * @return a pointer to the content associated with the given name
 */
Content *list_get(LinkedList *list, char *name) {
    Node *node = list->head;
    while (node != NULL) {
        if (equal(node->name, name)) {
            return &node->content;
        }
        node = node->next;
    }
    /* this part of the code should not be reached, if it is then a bug exists in the code */
    fprintf(stderr, "Code Error: Name not found in data structure!\n");
    return NULL;
}


/**
 * Looks for a name in a linked-list and retrieves the macro content associated with that name.
 * Does so by getting the content associated with the name, and retrieving its macro content.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * 
 * @param list a pointer to the linked-list that the macro should be retrieved from
 * @param name the name to look for
 * @return a pointer to the macro content associated with the given name
 */
MacroContent *list_get_macro(LinkedList *list, char *name) {
    return &list_get(list, name)->macro;
}

/**
 * Looks for a name in a linked-list and retrieves the symbol content associated with that name.
 * Does so by getting the content associated with the name, and retrieving its symbol content.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * 
 * @param list a pointer to the linked-list that the symbol should be retrieved from
 * @param name the name to look for
 * @return a pointer to the symbol content associated with the given name
 */
SymbolContent *list_get_symbol(LinkedList *list, char *name) {
    return &list_get(list, name)->symbol;
}

/**
 * Adds a content with a given name to a linked-list.
 * Does so by creating a new node, setting its content to be the given content, setting it to point to the current 
 * head of the list as its next node, and finally setting it to be the new head of the list.
 * 
 * @param list    a pointer to the list that the value should be added to
 * @param name    the name associated with the value
 * @param content the content to be added
 */
void list_add(LinkedList *list, char *name, Content content) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating node\n");
        set_alloc_failure();
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
 * 
 * @param list          a pointer to the list that the macro should be added to
 * @param name          the name associated with the macro
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
 * 
 * @param list           a pointer to the list that the symbol should be added to
 * @param name           the name associated with the symbol
 * @param symbol_content the content of the symbol to be added
 */
void list_add_symbol(LinkedList *list, char *name, SymbolContent symbol_content) {
    Content content;
    content.symbol = symbol_content;
    list_add(list, name, content);
}

/**
 * Adds an integer to a linked-list.
 * Does so by creating a new Content struct with its num field being the given integer and its name being NULL (since it
 * is irrelevant), and adding it to the list using list_Add.
 * 
 * @param list          a pointer to the list that the integer should be added to
 * @param num           the integer that should be added
 */
void list_add_int(LinkedList *list, int num) {
    Content content;
    content.num = num;
    list_add(list, NULL, content);
}

/**
 * Frees a linked-list and all of its  items' names and contents from the memory.
 * Does so by going over every node in the list, getting the next node and freeing the current one and its contents,
 * then doing the same with the next node.
 * Finally frees the list pointer.
 * 
 * @param list a pointer to the list that should be freed
 */
void deep_free_list(LinkedList *list) {
    Node *node = list->head;
    Node *next;
    while (node != NULL) {
        /* if the content is a symbol, its name and list of appearances are allocated on the heap and should be freed */
        if (list->content_type == SYMBOL) {
            deep_free_list(node->content.symbol.appearances);
            free(node->name);
        }
        /* if the content is a macro, its name and content are allocated on the heap and should be freed */
        if (list->content_type == MACRO) {
            free(node->content.macro);
            free(node->name);
        }
        next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

/**
 * Frees a linked-list and its nodes from the memory, without freeing the nodes' names contents.
 * Does so by going over every node in the list, getting the next node and freeing the current one,
 * then doing the same with the next node.
 * Finally frees the list pointer.
 * 
 * @param list a pointer to the list that should be freed
 */
void shallow_free_list(LinkedList *list) {
    Node *node = list->head;
    Node *next;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

/**
 * Adds a given integer to the value of every symbol in a hash-linked-list that meets a given condition.
 * Does so by going over every item on the list, and if applying the condition to its symbol content yields 1,
 * adding the given integer to its value.
 * Assumes that that the content of every item in the list is a symbol.
 * 
 * @param list      a pointer to the list whose items' values should be changed
 * @param to_add    the integer to add to the values
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void list_add_to_all_that_apply(LinkedList *list, int to_add, int (*condition)(SymbolContent symbol)) {
    Node *node = list->head;
    while (node != NULL) {
        if (condition(node->content.symbol))
            node->content.symbol.value += to_add;
        node = node->next;
    }
}

/**
 * Adds every symbol in a given linked-list that meets a given condition to another given linked-list.
 * Importantly, copies of the symbols are added, rather then references to the same symbols, and copies of the pointers
 * representing the names are added.
 * Does so by going over every item on the first list, and if applying the condition to its symbol content yields 1,
 * adding its name and a copy of its content and the pointer representing the name to the second list.
 * Assumes every item on the first list is a symbol.
 * 
 * @param list1     the list whose symbols should be added to the other list
 * @param list2     the list that the symbols should be added to
 * @param condition a pointer to a function that accepts a symbol and returns 1 if it meets the wanted condition and 0
 *                  otherwise
 */
void list_add_matching_to_list(LinkedList *list1, LinkedList *list2, int (*condition)(SymbolContent symbol)) {
    Node *node = list1->head;
    while (node != NULL) {
        if (condition(node->content.symbol))
            list_add(list2, node->name, node->content);
        node = node->next;
    }
}
