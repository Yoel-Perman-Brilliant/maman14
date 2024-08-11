/**
 * Includes prototypes for miscellaneous utility functions. 
 */
#ifndef GENERAL_UTIL_H
#define GENERAL_UTIL_H

#include "stdarg.h"
#include "stdio.h"

/**
 * The maximum length of a line that can be read using read_line.
 */
#define MAX_LINE_LENGTH 80

/**
 * Reads a line from a file into a given character array as long as it is at most 80 characters long.
 * 
 * @param file        the file that should be read from
 * @param file_name   the name of the file (used for error reporting)
 * @param line_number the number of the line being read (used for error reporting)
 * @param s           an array that should contain the output
 * @return 0 if the line was read successfully, 1 otherwise
 */
int read_line(FILE *file, char *file_name, int line_number, char s[]);

/**
 * Frees the given arguments from memory.
 * 
 * @param num the number of pointers to be freed
 * @param ... a variable-length list of pointers to be freed.
 */
void free_all(int num, ...);

#endif
