/**
 * Includes functions that allow for interacting with the requirements structure.
 */

#include "../headers/requirements.h"
#include "../headers/exit_codes.h"
#include "stdlib.h"
#include "stdio.h"
#include "../headers/util/general_util.h"

/**
 * Creates a new instance of Requirements to be used for the assembly of one file.
 * Does so by allocating memory for it and for the arrays it includes, creating the symbol table and initializing the
 * instruction and data counters.
 * 
 * @return a pointer to new Requirements
 */
Requirements *create_requirements() {
    Requirements *requirements = malloc(sizeof(Requirements));
    if (requirements == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating requirements\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    requirements->macro_table = create_map(MACRO);
    requirements->symbol_table = create_map(SYMBOL);
    requirements->data_array = calloc(MEMORY_SIZE, sizeof(short));
    if (requirements->data_array == NULL) {
        printf("Memory Error: Memory allocation failure when creating data array\n");
        set_alloc_failure();
    }
    requirements->faulty_instructions = create_set();
    requirements->instruction_array = calloc(MEMORY_SIZE, sizeof(short));
    if (requirements->instruction_array == NULL) {
        printf("Memory Error: Memory allocation failure when creating instruction array\n");
        set_alloc_failure();
    }
    if (is_alloc_failure()) {
        free_requirements(requirements);
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    requirements->ic = IC_START;
    requirements->dc = 0;
    requirements->extern_found = 0;
    return requirements;
}

/**
 * Frees a pointer to an instance of Requirements and all of its members.
 * 
 * @param requirements a pointer to the requirements to be freed
 */
void free_requirements(Requirements *requirements) {
    free_map(requirements->macro_table);
    free_map(requirements->symbol_table);
    free_set(requirements->faulty_instructions);
    free(requirements->data_array);
    free(requirements->instruction_array);
    free(requirements);
}

/**
 * Inserts a word into the Requirement's instruction array while advancing its instruction counter.
 * If the sum of the instruction and data counters is larger than the size of the memory, then there is no more
 * space for the new instruction in the memory image, so an error is thrown.
 * 
 * @param requirements     the requirements of the file
 * @param instruction      the word to be added, padded with a 0 on the left
 * @param line_count       the number of the line in the parsed file whose portion is being inserted to
 *                         the memory (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @return 0 if the insertion was successful, 1 otherwise
 */
int memory_insert_instruction(Requirements *requirements, unsigned short instruction, int line_count, char *parsed_file_name) {
    if (requirements->ic + requirements->dc >= MEMORY_SIZE) {
        printf("Input Error: Not enough space in the memory image (Error occurred in line %d of file %s)\n",
               line_count, parsed_file_name);
        return 1;
    }
    requirements->instruction_array[requirements->ic++] = instruction;
    return 0;
}

/**
 * Inserts a word into the Requirement's data array while advancing its data counter.
 * If the sum of the instruction and data counters is larger than the size of the memory, then there is no more
 * space for the new data in the memory image, so an error is thrown.
 * 
 * @param requirements     the requirements of the file
 * @param data             the word to be added, padded with a 0 on the left
 * @param line_count       the number of the line in the parsed file whose portion is being inserted to
 *                         the memory (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @return 0 if the insertion was successful, 1 otherwise
 */
int memory_insert_data(Requirements *requirements, unsigned short data, int line_count, char *parsed_file_name) {
    if (requirements->ic + requirements->dc >= MEMORY_SIZE) {
        printf("Input Error: Not enough space in the memory image (Error occurred in line %d of file %s)\n",
               line_count, parsed_file_name);
        return 1;
    }
    requirements->data_array[requirements->dc++] = data;
    return 0;
}