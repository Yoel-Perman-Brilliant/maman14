/**
 * Includes a linked-list data structure, which consists of nodes that can hold either a string or
 * an integer value, each associated with a certain name.
 * In addition, includes prototype for functions that allow for interacting with linked-lists.
 */
#ifndef MAMAN14_LINKED_LIST_H
#define MAMAN14_LINKED_LIST_H

/**
 * Represents a possible value of a node in a linked list. Can be either an integer or a string.
 */
typedef union Value {
    int ival;
    char *sval;
} Value;

/**
 * Represents an item in a linked-list. Includes a value, a name associated with the value and a pointer to the next
 * item on the list.
 */
typedef struct Node {
    char *name;
    Value value;
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
 * Checks if a linked-list contains a value represented by a given name.
 * @param list a pointer to the linked-list to be checked
 * @param name the name to be checked
 * @return 1 if the list contains a value with the given name, 0 otherwise
 */
int list_contains(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the value associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
Value list_get(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the integer value associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
int list_get_int(LinkedList *list, char *name);

/**
 * Looks for a name in a linked-list and retrieves the string value associated with that name.
 * Should only be used after verifying that the name exists in the list using list_contains.
 * @param list a pointer to the linked-list that the value should be retrieved from
 * @param name the name to look for
 * @return the value associated with the given name
 */
char *list_get_string(LinkedList *list, char *name);

/**
 * Adds a value with a given name to a linked-list.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void list_add(LinkedList *list, char *name, Value value);

/**
 * Adds an integer value with a given name to a linked-list.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void list_add_int(LinkedList *list, char *name, int value);

/**
 * Adds an string value with a given name to a linked-list.
 * @param list a pointer to the list that the value should be added to
 * @param name the name associated with the value
 * @param value the value to be added
 */
void list_add_string(LinkedList *list, char *name, char *value);

/**
 * Frees a linked-list and all of its contents from the memory.
 * @param list a pointer to the list that should be freed
 */
void free_list(LinkedList *list);

/**
 * Prints a linked-list's items, assuming their values are all integers.
 * @param list a pointer to the list that should be printed
 */
void print_int_list(LinkedList *list);

#endif
