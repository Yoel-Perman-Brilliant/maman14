#ifndef MAMAN14_HASH_TABLE_H
#define MAMAN14_HASH_TABLE_H

#include "linked_list.h"

#define HASH_TABLE_SIZE 101
#define HASH_MULTIPLIER 31

typedef struct {
    LinkedList *lists[HASH_TABLE_SIZE];
} HashTable;

unsigned hash(char *s);

HashTable *create_table();

int table_contains(HashTable *table, char *name);

Value table_get(HashTable *table, char *name);

int table_get_int(HashTable *table, char *name);

char *table_get_string(HashTable *table, char *name);

void table_add(HashTable *table, char *name, Value value);

void table_add_int(HashTable *table, char *name, int value);

void table_add_string(HashTable *table, char *name, char *value);

void free_table(HashTable *table);

#endif
