#ifndef MAMAN14_REQUIREMENTS_H
#define MAMAN14_REQUIREMENTS_H

#include "structures/hash_table.h"

#define MEMORY_SIZE_WORDS 4096

typedef struct {
    HashTable *symbol_table;
} Requirements; 

Requirements *create_requirements();

void free_requirements(Requirements *requirements);

#endif
