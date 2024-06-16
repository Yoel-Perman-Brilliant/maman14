#include "../headers/hash_table.h"
#include "stdlib.h"
#include "stdio.h"

unsigned hash(char *s) {
    unsigned hash_val;
    for (hash_val = 0; *s != '\0'; s++) {
        hash_val = *s + HASH_MULTIPLIER * hash_val;
    }
    return hash_val % HASH_TABLE_SIZE;
}

HashTable *create_table() {
    HashTable *table = malloc(sizeof(HashTable));
    int i;
    if (table == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating table");
        return NULL;
    }
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        table->lists[i] = create_list();
    }
    return table;
}

Value table_get(HashTable *table, char *name) {
    return list_get(table->lists[hash(name)], name);
}

int table_get_int(HashTable *table, char *name) {
    return table_get(table, name).ival;
}

char *table_get_string(HashTable *table, char *name) {
    return table_get(table, name).sval;
}

void table_add(HashTable *table, char *name, Value value) {
    list_add(table->lists[hash(name)], name, value);
}

void table_add_int(HashTable *table, char *name, int value) {
    list_add_int(table->lists[hash(name)], name, value);
}

void table_add_string(HashTable *table, char *name, char *value) {
    list_add_string(table->lists[hash(name)], name, value);
}

void free_table(HashTable *table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        free_list(table->lists[i]);
    }
    free(table);
}