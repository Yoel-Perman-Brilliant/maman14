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
    if (is_alloc_failure()) exit(MEMORY_ALLOCATION_FAILURE);
    
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
    
    /* executes the first pass over the macro-less .am file, before this failure is necessarily 0 */
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
