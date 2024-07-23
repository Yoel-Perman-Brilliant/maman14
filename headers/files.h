/**
 * Includes prototypes for functions that handle getting the names and opening files in different formats.
 */
#ifndef MAMAN14_FILES_H
#define MAMAN14_FILES_H

#include "stdio.h"
#include "requirements.h"

/**
 * Gets a file name without an extension and returns the name of the input file with the .as extension.
 * 
 * @param file_name the file name without the extension (as given as command line argument)
 * @return the name of the input file with the extension, or NULL if a memory allocation failure occurred
 */
char *get_input_file_name(char file_name[]);

/**
 * Gets a file name without an extension and returns the name of the parsed input file with the .am extension.
 * 
 * @param file_name the file name without the extension (as given as command line argument)
 * @return the name of the parsed file with the extension, or NULL if a memory allocation failure occurred
 */
char *get_parsed_file_name(char file_name[]);

/**
 * Returns a pointer to the input file with a read permission based on the extensionless file name.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the now-open file, or NULL if the file could not be opened
 */
FILE *get_input_file(char file_name[]);

/**
 * Returns a pointer to a new file with an append permission based on the extensionless file name, which should act
 * as the parsed macro-less file.
 * Before opening the file, deletes any exsisting parsed file with the same to avoid writing to an existing file,
 * and instead generate a new one.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_parsed_file_append(char file_name[]);

/**
 * Returns a pointer to the parsed, macro-less file with a read permission based on the extensionless file name.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_parsed_file_read(char file_name[]);

FILE *get_object_file(char file_name[]);

FILE *get_extern_file(char file_name[]);

FILE *get_entry_file(char file_name[]);

void remove_output_files(char file_name[]);

#endif
