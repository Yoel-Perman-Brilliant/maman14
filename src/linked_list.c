#include "../headers/linked_list.h"
#include "stdlib.h"
#include "../headers/string_ops.h"
#include "../headers/exit_codes.h"
#include "stdio.h"

#define VALUE_NOT_FOUND_EXIT_CODE 1

LinkedList *create_list() {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating list");
        return NULL;
    }
    list->head = NULL;
    return list;
}

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

/* SHOULD ONLY BE USED AFTER LIST_CONTAINS */
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

int list_get_int(LinkedList *list, char *name) {
    return list_get(list, name).ival;
}

char *list_get_string(LinkedList *list, char *name) {
    return list_get(list, name).sval;
}

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

void list_add_int(LinkedList *list, char *name, int value) {
    Value value_holder;
    value_holder.ival = value;
    list_add(list, name, value_holder);
}

void list_add_string(LinkedList *list, char *name, char *value) {
    Value value_holder;
    value_holder.sval = value;
    list_add(list, name, value_holder);
}

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

void print_int_list(LinkedList *list) {
    Node *node = list->head;
    printf("[");
    while (node != NULL) {
        printf("name: %s, value: %d;  ", node->name, node->value.ival);
        node = node->next;
    }
    printf("]\n");
}
