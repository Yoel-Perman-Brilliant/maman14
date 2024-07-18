/**
 * Includes the requirements structure, whose members are necessary data structures that are used for the encoding of
 * each file.
 * Also includes prototypes for functions that allow for interacting with the requirements.
 */
#ifndef MAMAN14_REQUIREMENTS_H
#define MAMAN14_REQUIREMENTS_H

#include "structures/hash_table.h"
#include "structures/set.h"

#define MEMORY_SIZE 4096
#define IC_START 100

/**
 * The requirement data structure. Exactly one of which must be created for the assembly of each file.
 */
typedef struct {
    HashTable *symbol_table;
    Set *faulty_instructions;
    /**
     * The file's data portion of the memory image.
     */
    unsigned short *data_array;
    /**
     * The file's instruction portion of the memory image.
     */
    unsigned short *instruction_array;
    /** 
     * The number of data words in the memory.
     */
    int dc;
    /** 
     * The number of instruction words in the memory, plus a constant value that should be the first in
     * the memory image.
     */
    int ic;
    
    unsigned extern_found : 1;
    unsigned entry_found : 1;
} Requirements; 

/**
 * Creates a new instance of Requirements to be used for the assembly of one file.
 * 
 * @return a pointer to new Requirements
 */
Requirements *create_requirements();

/**
 * Frees a pointer to an instance of Requirements and all of its members.
 * 
 * @param requirements a pointer to the requirements to be freed
 */
void free_requirements(Requirements *requirements);

/**
 * Inserts a word into the Requirement's instruction array while advancing its instruction counter.
 * 
 * @param requirements     the requirements of the file
 * @param instruction      the word to be added, padded with a 0 on the left
 * @param line_count       the number of the line in the parsed file whose portion is being inserted to
 *                         the memory (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @return 0 if the insertion was successful, 1 otherwise
 */
int memory_insert_instruction(Requirements *requirements, unsigned short instruction, int line_count, char *parsed_file_name);

/**
 * Inserts a word into the Requirement's data array while advancing its data counter.
 * 
 * @param requirements     the requirements of the file
 * @param data             the word to be added, padded with a 0 on the left
 * @param line_count       the number of the line in the parsed file whose portion is being inserted to
 *                         the memory (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @return 0 if the insertion was successful, 1 otherwise
 */
int memory_insert_data(Requirements *requirements, unsigned short data, int line_count, char *parsed_file_name);

#endif
