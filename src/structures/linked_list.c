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
 * Checks if a linked-list contains a value represented by a given name.
 * Does so by going over every node on the list and checking if its name is is equal to the given name.
 * @param list a pointer to the linked-list to be checked
 * @param name the name to be checked
 * @return 1 if the list contains a value with the given name, 0 otherwise
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
 * Looks for a name in a linked-list and retrieves the value associated with that name.
 * Does so by going over every item on the list, and if its name is equal to the given name, retrieving its value.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
Value list_get(LinkedList *list, char *name) {
    Node *node = list->head;
    while (node != NULL) {
        if (equal(node->name, name)) {
            return node->value;
        }
        node = node->next;
    }
    /* this part of the code should not be reached, if it is then a bug exists in the code */
    fprintf(stderr, "Code Error: Name not found in data structure!");
    exit(NAME_NOT_FOUND);
}

/**
 * Looks for a name in a linked-list and retrieves the integer value associated with that name.
 * Does so by getting the value associated with the given name, and retrieving its integer field.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
int list_get_int(LinkedList *list, char *name) {
    return list_get(list, name).ival;
}

/**
 * Looks for a name in a linked-list and retrieves the string value associated with that name.
 * Does so by getting the value associated with the given name, and retrieving its string field.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
char *list_get_string(LinkedList *list, char *name) {
    return list_get(list, name).sval;
}

/**
 * Adds a value with a given name to a linked-list.
 * Does so by creating a new node, setting its value to be the given value, setting it to point to the current head of
 * the list as its next node, and finally setting it to be the new head of the list.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void list_add(LinkedList *list, char *name, Value value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating node");
        return;
    }
    node->name = name;
    node->value = value;
    node->next = list->head;
    list->head = node;
}

/**
 * Adds an integer value with a given name to a linked-list.
 * Does so by creating a new value struct with its integer field being the given value, and adding it to the list using
 * list_add.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void list_add_int(LinkedList *list, char *name, int value) {
    Value value_holder;
    value_holder.ival = value;
    list_add(list, name, value_holder);
}

/**
 * Adds a string value with a given name to a linked-list.
 * Does so by creating a new value struct with its string field being the given value, and adding it to the list using
 * list_add.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void list_add_string(LinkedList *list, char *name, char *value) {
    Value value_holder;
    value_holder.sval = value;
    list_add(list, name, value_holder);
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

/**
 * Prints a linked-list's items, assuming their values are all integers.
 * Does so by going over every item on the list and printing its name and integer value with appropriate formatting.
 * @param list a pointer to the list that should be printed
 */
void print_int_list(LinkedList *list) {
    Node *node = list->head;
    printf("[");
    while (node != NULL) {
        printf("name: %s, value: %d;  ", node->name, node->value.ival);
        node = node->next;
    }
    printf("]\n");
}
