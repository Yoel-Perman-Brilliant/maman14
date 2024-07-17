#ifndef MAMAN14_SET_H
#define MAMAN14_SET_H
#include "linked_list.h"

#define SET_SIZE 101

typedef struct  {
    LinkedList *lists[SET_SIZE];
} Set;

Set *create_set();

void free_set(Set *set);

int set_contains(Set *set, LineNumber line_number);

int set_add(Set *set, LineNumber line_number);

void set_print(Set *set);

#endif
