/**
 * This is the main file for an assembler program, which converts a text-base assembly program given in a .as file to
 * machine code which resides in a newly-created .ob file, as well as additional files that are explained later.
 * 
 * The first stage is pre-assembly, which creates a parsed .am file out of the given .as file. The pre-assembly process
 * is responsible for parsing every macro in the .as file, such that the .am file is macro-less.
 * The second stage is a double-pass assembly - the assembler goes over  the .am file twice, and translates it to
 * machine code which is kept in the file's requirements structure. The first pass is responsible for encoding the first
 * word of each instruction and .data and .string directives, as well as giving a value to every symbol. The second pass
 * is responsible for the encoding of operands, as well as listing entry symbols and uses of external symbols.
 * Finally, the output files are created based on the encoding that resides in the requirements.
 * 
 * If the assembler identifies an input error in the pre-assembly stage, it will not create the .am file, won't
 * perform the assembly stage, and start the process for the next file. If it identifies an error in the first or second
 * pass stage, the output files will not be created and the assembly process will start for the next file.
 * 
 * If a memory allocation failure occurs, the program will stop as quickly as possible while freeing any
 * allocated memory.
 * 
 * The input for the program is a list of extensionless file names, each corresponding to a .as file. The extensionless
 * file names are given as command line arguments. The output for the program includes several output files for each
 * input files, each with the same name is the input file but a different extension:
 * 1. A .am file, which is the same as the .as file but all the macros are parsed.
 * 2. A .ob file, which includes the machine code.
 * 3. A .ext file, which includes a list of external symbols and the addresses in which they are used.
 * 4. A .ent file, which includes a list of entry symbols defined in the input file and their values.
 */

#include "stdio.h"
#include "../headers/files.h"
#include "../headers/pre_assembler.h"
#include "../headers/first_pass.h"
#include "../headers/second_pass.h"
#include "../headers/output_creator.h"
#include "../headers/exit_codes.h"
#include "../headers/alloc_failure_handler.h"
#include "stdlib.h"

/**
 * Executes the entire assembly process for a file.
 * 
 * Does so by first creating the file's requirements, then pre-assembling the file.
 * Then, assembles it by going over it twice and filling the requirements.
 * Finally, creates the output files using the requirements.
 * 
 * @param file_name the name of the file to be assembled without the extension
 * @return 1 if an error has occurred, 0 otherwise
 */
static int assemble(char file_name[]) {
    
    /* whether an assembly failure has occurred */
    int failure;
    
    /* creates the file's requirements, exits if a memory allocation error has occurred */
    Requirements *requirements = create_requirements();
    if (is_alloc_failure()) {
        free_requirements(requirements);
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    
    /* removes any existing output files for the given file */
    remove_output_files(file_name);
    
    /* pre-assembles the file and updates the failure flag */
    failure = pre_assemble(file_name, requirements);
    
    /* if a memory allocation error has occurred, exits the program */
    if (is_alloc_failure()) {
        free_requirements(requirements);
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    
    /* if the pre-assembly has been completed successfully, moves to the next stage */
    if (!failure) printf("%s: Pre-assembly completed successfully\n", file_name);
    
    /* if a non-memory related error has occurred in the pre-assembly, stops the assembly of this file */
    else {
        free_requirements(requirements);
        return 1;
    }
    
    /* executes the first pass over the macro-less .am file. before this, failure is necessarily 0 */
    failure = first_pass(file_name, requirements);

    /* if a memory allocation error has occurred, exits the program */
    if (is_alloc_failure()) {
        free_requirements(requirements);
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    
    /* notifies the user about a first-pass success */
    if (!failure) printf("%s: First pass completed successfully\n", file_name);
    
    /* executes the second pass even if the first pass failed in order to find errors */
    failure |= second_pass(file_name, requirements);

    /* if a memory allocation error has occurred, exits the program */
    if (is_alloc_failure()) {
        free_requirements(requirements);
        exit(MEMORY_ALLOCATION_FAILURE);
    }

    /* if the first and second pass have been completed successfully, moves to the next stage */
    if (!failure) printf("%s: Second pass completed successfully\n", file_name);
    
    /* if a non-memory related error has occurred during the first or second pass, stops the assembly of this file */
    else {
        free_requirements(requirements);
        return 1;
    }
    
    /* creates the output files */
    failure = create_files(file_name, requirements);

    /* if a memory allocation error has occurred, exits the program */
    if (is_alloc_failure()) {
        free_requirements(requirements);
        exit(MEMORY_ALLOCATION_FAILURE);
    }

    /* if the file creation was completed successfully, notifies the user and moves to the end of the function */
    if (!failure) printf("%s: Output files creation completed successfully\n", file_name);
    
    /* if a non-memory related error has occurred during the file creation, stops the assembly of this file */
    else {
        free_requirements(requirements);
        return 1;
    }
    
    free_requirements(requirements);
    return 0;
}

/**
 * Reads a list of extensions file names from the command line and assembles the corresponding .as files one by one.
 * See the documentation at the top of the file for more information about the assembly process.
 * 
 * @param argc the number of command line arguments (one plus the number of files to be assembled)
 * @param argv a list of command line arguments (the ./assembler command and the extensionless file names)
 * @return 0 if all files were assembled successfully, 1 if at least one assembly error occurred, 2 if a memory
 *         allocation failure occurred (exits with code 2 if necessary in the assemble function), or 3 if no files
 *         were given
 */
int main(int argc, char **argv) {
    /* whether an assembly error has occurred */
    int failure;
    /* index for going over the command line arguments */
    int i;
    /* no arguments were given */
    if (argc == 1) {
        printf("No file names given to assembler\n");
        return NO_FILES_GIVEN;
    }
    /* assembles every file one by one and updates the failure flag */
    for (i = 1; i < argc; i++) {
        failure |= assemble(argv[i]);
        /* prints a line break to make a distinction between messages from different files */
        printf("\n");
    }
    /* if an assembly error has occurred, exits with exit code 1, otherwise 0 */
    if (failure) return ASSEMBLY_FAILURE;
    return SUCCESS;
}
