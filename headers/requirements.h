#ifndef MAMAN14_REQUIREMENTS_H
#define MAMAN14_REQUIREMENTS_H

#include "structures/hash_table.h"

#define MEMORY_SIZE 4096

typedef struct {
    HashTable *symbol_table;
    unsigned short data_array[MEMORY_SIZE / 2];
    unsigned short instruction_array[MEMORY_SIZE / 2];

} Requirements; 

Requirements *create_requirements();

void free_requirements(Requirements *requirements);

#endif
