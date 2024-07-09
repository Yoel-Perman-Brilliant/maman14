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

int memory_insert_instruction(Requirements *requirements, unsigned short instruction, int line_count, char *parsed_file_name);

int memory_insert_data(Requirements *requirements, unsigned short data, int line_count, char *parsed_file_name);


#endif
