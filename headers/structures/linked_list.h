#ifndef MAMAN14_LINKED_LIST_H
#define MAMAN14_LINKED_LIST_H

typedef union Value {
    int ival;
    char *sval;
} Value;

typedef struct Node {
    char *name;
    Value value;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
} LinkedList;

LinkedList *create_list();

int list_contains(LinkedList *list, char *name);

Value list_get(LinkedList *list, char *name);

int list_get_int(LinkedList *list, char *name);

char *list_get_string(LinkedList *list, char *name);

void list_add(LinkedList *list, char *name, Value value);

void list_add_int(LinkedList *list, char *name, int value);

void list_add_string(LinkedList *list, char *name, char *value);

void free_list(LinkedList *list);

void print_int_list(LinkedList *list);

#endif
