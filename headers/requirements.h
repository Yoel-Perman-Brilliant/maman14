#ifndef MAMAN14_REQUIREMENTS_H
#define MAMAN14_REQUIREMENTS_H

#include "structures/hash_table.h"

#define MEMORY_SIZE 4096

typedef struct {
    HashTable *symbol_table;
    unsigned short *data_array;
    unsigned short *instruction_array;
    int dc;
    int ic;
} Requirements; 

Requirements *create_requirements();

void free_requirements(Requirements *requirements);

#endif
