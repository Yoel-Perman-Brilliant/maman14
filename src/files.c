#include "../headers/files.h"
#include "../headers/exit_codes.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define INPUT_EXTENSION ".as"
#define PARSED_EXTENSION ".am"

char *get_file_name_with_extension(char file_name[], char extension[]) {
    char *name_with_extension = calloc(strlen(file_name) + strlen(extension) + 1, 1);
    if (name_with_extension == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying file name\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    /* copies the extensionless file name and the extension to the new string */
    strcat(name_with_extension, file_name);
    strcat(name_with_extension, extension);
    return name_with_extension;
}

/**
 * Gets a file name without an extension and returns the name of the input file with the .as extension.
 * @param file_name the file name without the extension (as given as command line argument)
 * @return the name of the input file with the extension, or NULL if a memory allocation failure occurred
 */
char *get_input_file_name(char file_name[]) {
    return get_file_name_with_extension(file_name, INPUT_EXTENSION);
}

/**
 * Gets a file name without an extension and returns the name of the parsed input file with the .am extension.
 * @param file_name the file name without the extension (as given as command line argument)
 * @return the name of the parsed file with the extension, or NULL if a memory allocation failure occurred
 */
char *get_parsed_file_name(char file_name[]) {
    return get_file_name_with_extension(file_name, PARSED_EXTENSION);
}

/**
 * Returns a pointer to the input file with a read permission based on the extensionless file name.
 * @param file_name the name of the input file without the extension
 * @return a pointer to the now-open file, or NULL if the file could not be opened
 */
FILE *get_input_file(char file_name[]) {
    FILE *input_file;
    char *input_file_name = get_input_file_name(file_name);
    input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        printf("Error: Can't open file %s\n", input_file_name);
        return NULL;
    }
    free(input_file_name);
    return input_file;
}

/**
 * Returns a pointer to a new file with an append permission based on the extensionless file name, which should act
 * as the parsed macro-less file.
 * Before opening the file, deletes any exsisting parsed file with the same to avoid writing to an existing file,
 * and instead generate a new one.
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_parsed_file_append(char file_name[]) {
    FILE *parsed_file;
    char *parsed_file_name = get_parsed_file_name(file_name);
    /* removes any already existing file with the same name */
    remove(parsed_file_name);
    /* since no file with the name exists, creates a new file */
    parsed_file = fopen(parsed_file_name, "a");
    if (parsed_file == NULL) {
        printf("Error: Can't create file %s\n", parsed_file_name);
        return NULL;
    }
    free(parsed_file_name);
    return parsed_file;
}

FILE *get_parsed_file_read(char file_name[]) {
    FILE *parsed_file;
    char *parsed_file_name = get_parsed_file_name(file_name);
    parsed_file = fopen(parsed_file_name, "r");
    if (parsed_file == NULL) {
        printf("Error: Can't open file %s\n", parsed_file_name);
        return NULL;
    }
    free(parsed_file_name);
    return parsed_file;
}
