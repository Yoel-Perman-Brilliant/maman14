/**
 * Includes different exit codes for the program.
 */
#ifndef EXIT_CODES_H
#define EXIT_CODES_H

/* all files were assembled successfully */
#define SUCCESS 0
/* at least one file was not successfully assembled, but the program finished running naturally */
#define ASSEMBLY_FAILURE 1
/* a memory allocation failure occurred, which caused the program to end */
#define MEMORY_ALLOCATION_FAILURE 2
/* no files were given to the assembler */
#define NO_FILES_GIVEN 3

#endif
