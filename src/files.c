/**
 * Includes functions that handle getting the names and opening files in different formats.
 */
 
#include "../headers/files.h"
#include "../headers/exit_codes.h"
#include "../headers/util/general_util.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define INPUT_EXTENSION ".as"
#define PARSED_EXTENSION ".am"
#define OBJECT_EXTENSION ".ob"
#define EXTERN_EXTENSION ".ext"
#define ENTRY_EXTENSION ".ent"

/**
 * Creates a string that corresponds to a given file name with a given extension.
 * 
 * Does so by allocating enough memory for the new string and initializing its characters to 0, and then concatenating
 * the extensionless file name and the extension to it.
 * 
 * @param file_name the extensionless file name
 * @param extension the extension (including the period)
 * @return a string (allocated on the heap) which represents the file name with the extension
 */
static char *get_file_name_with_extension(char file_name[], char extension[]) {
    char *name_with_extension = calloc(strlen(file_name) + strlen(extension) + 1, 1);
    if (name_with_extension == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying file name\n");
        set_alloc_failure();
        return NULL;
    }
    /* copies the extensionless file name and the extension to the new string by concatenating them */
    strcat(name_with_extension, file_name);
    strcat(name_with_extension, extension);
    return name_with_extension;
}

/**
 * Gets a file name without an extension and returns the name of the input file with the .as extension.
 * 
 * @param file_name the file name without the extension (as given as command line argument)
 * @return the name of the input file with the extension, or NULL if a memory allocation failure occurred
 */
char *get_input_file_name(char file_name[]) {
    return get_file_name_with_extension(file_name, INPUT_EXTENSION);
}

/**
 * Gets a file name without an extension and returns the name of the parsed input file with the .am extension.
 * 
 * @param file_name the file name without the extension (as given as command line argument)
 * @return the name of the parsed file with the extension, or NULL if a memory allocation failure occurred
 */
char *get_parsed_file_name(char file_name[]) {
    return get_file_name_with_extension(file_name, PARSED_EXTENSION);
}

/**
 * Returns a pointer to the input file with a read permission based on the extensionless file name.
 * Does so by getting the file name with the extension and opening the file based on the name with a read permission.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the now-open file, or NULL if the file could not be opened
 */
FILE *get_input_file(char file_name[]) {
    FILE *input_file;
    char *input_file_name = get_input_file_name(file_name);
    input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        printf("Error: Can't open file %s\n", input_file_name);
        free(input_file_name);
        return NULL;
    }
    free(input_file_name);
    return input_file;
}

/**
 * Returns a pointer to a new file with an append permission based on the extensionless file name, which should act
 * as the parsed macro-less file.
 * Assumes no file with this name (including the extension) exists in the directory.
 * Does so by getting the file name with the extension and opening the file based on the name with an append permission.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_parsed_file_append(char file_name[]) {
    FILE *parsed_file;
    char *parsed_file_name = get_parsed_file_name(file_name);
    /* since no file with the name exists, creates a new file */
    parsed_file = fopen(parsed_file_name, "a");
    if (parsed_file == NULL) {
        printf("Error: Can't create file %s\n", parsed_file_name);
        free(parsed_file_name);
        return NULL;
    }
    free(parsed_file_name);
    return parsed_file;
}

/**
 * Returns a pointer to the parsed, macro-less file with a read permission based on the extensionless file name.
 * Does so by getting the file name with the extension and opening the file based on the name with a read permission.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_parsed_file_read(char file_name[]) {
    FILE *parsed_file;
    char *parsed_file_name = get_parsed_file_name(file_name);
    parsed_file = fopen(parsed_file_name, "r");
    if (parsed_file == NULL) {
        printf("Error: Can't open file %s\n", parsed_file_name);
        free(parsed_file_name);
        return NULL;
    }
    free(parsed_file_name);
    return parsed_file;
}

/**
 * Returns a pointer to the object file based on the extensionless file name.
 * Assumes no file with this name (including the extension) exists in the directory.
 * Does so by getting the file name with the extension and opening the file based on the name with an append permission.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_object_file(char file_name[]) {
    FILE *object_file;
    char *object_file_name = get_file_name_with_extension(file_name, OBJECT_EXTENSION);
    /* since no file with the name exists, creates a new file */
    object_file = fopen(object_file_name, "a");
    if (object_file == NULL) {
        printf("Error: Can't create file %s\n", object_file_name);
        free(object_file_name);
        return NULL;
    }
    free(object_file_name);
    return object_file;
}

/**
 * Returns a pointer to the extern symbols file (.ext) based on the extensionless file name.
 * Assumes no file with this name (including the extension) exists in the directory.
 * Does so by getting the file name with the extension and opening the file based on the name with an append permission.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_extern_file(char file_name[]) {
    FILE *extern_file;
    char *extern_file_name = get_file_name_with_extension(file_name, EXTERN_EXTENSION);
    /* since no file with the name exists, creates a new file */
    extern_file = fopen(extern_file_name, "a");
    if (extern_file == NULL) {
        printf("Error: Can't create file %s\n", extern_file_name);
        free(extern_file_name);
        return NULL;
    }
    free(extern_file_name);
    return extern_file;
}

/**
 * Returns a pointer to the entry symbols file (.ent) based on the extensionless file name.
 * Assumes no file with this name (including the extension) exists in the directory.
 * Does so by getting the file name with the extension and opening the file based on the name with an append permission.
 * 
 * @param file_name the name of the input file without the extension
 * @return a pointer to the new file, or NULL if the file could not be created
 */
FILE *get_entry_file(char file_name[]) {
    FILE *entry_file;
    char *entry_file_name = get_file_name_with_extension(file_name, ENTRY_EXTENSION);
    /* since no file with the name exists, creates a new file */
    entry_file = fopen(entry_file_name, "a");
    if (entry_file == NULL) {
        printf("Error: Can't create file %s\n", entry_file_name);
        free(entry_file_name);
        return NULL;
    }
    free(entry_file_name);
    return entry_file;
}

/**
 * Removes all output files (parsed, object, extern and entry) corresponding to a given extensionless file name.
 * Does so by getting the name of every every output file, removing the files based on their names, and freeing the
 * names.
 * 
 * @param file_name the name of the input file without the extension
 */
void remove_output_files(char file_name[]) {
    char *parsed_file_name = get_parsed_file_name(file_name);
    char *object_file_name = get_file_name_with_extension(file_name, OBJECT_EXTENSION);
    char *extern_file_name = get_file_name_with_extension(file_name, EXTERN_EXTENSION);
    char *entry_file_name = get_file_name_with_extension(file_name, ENTRY_EXTENSION);
    remove(parsed_file_name);
    remove(object_file_name);
    remove(extern_file_name);
    remove(entry_file_name);
    free_all(4, parsed_file_name, object_file_name, extern_file_name, entry_file_name);
}
