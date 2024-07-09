#include "../headers/requirements.h"
#include "../headers/exit_codes.h"
#include "stdlib.h"
#include "stdio.h"

Requirements *create_requirements() {
    Requirements *requirements = malloc(sizeof(requirements));
    if (requirements == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating requirements\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    requirements->symbol_table = create_table();
    requirements->data_array = calloc(MEMORY_SIZE, sizeof(short));
    if (requirements->data_array == NULL) {
        printf("Memory Error: Memory allocation failure when creating data array\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    requirements->instruction_array = calloc(MEMORY_SIZE, sizeof(short));
    if (requirements->instruction_array == NULL) {
        printf("Memory Error: Memory allocation failure when creating instruction array\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    requirements->ic = 0;
    requirements->dc = 0;
    return requirements;
}

void free_requirements(Requirements *requirements) {
    free_table(requirements->symbol_table);
    free(requirements->data_array);
    free(requirements->instruction_array);
    free(requirements);
}

int memory_insert_instruction(Requirements *requirements, unsigned short instruction, int line_count, char *parsed_file_name) {
    if (requirements->ic + requirements->dc >= MEMORY_SIZE) {
        printf("Input Error: Not enough space in the memory image (Error occurred in line %d of file %s)\n",
               line_count, parsed_file_name);
        return 1;
    }
    requirements->instruction_array[requirements->ic++] = instruction;
    return 0;
}

int memory_insert_data(Requirements *requirements, unsigned short data, int line_count, char *parsed_file_name) {
    if (requirements->ic + requirements->dc >= MEMORY_SIZE) {
        printf("Input Error: Not enough space in the memory image (Error occurred in line %d of file %s)\n",
               line_count, parsed_file_name);
        return 1;
    }
    requirements->data_array[requirements->dc++] = data;
    return 0;
}