/**
 * This file is responsible for the pre-assembly process. The pre-assembler takes an input file (a .as file) and parses
 * all of its macros using a hash-tables that includes each macro's name and content. The table is updated as the file
 * is being read.
 * The main function in the file is pre-assemble, which takes in a file name without the .as extension and parses it
 * into a new .am file with the same name. If a .am file with this name already exists, deletes it and 
 * creates a new file. If any errors are found, the .am file is not created, but the program keeps analyzing the 
 * input file in order to find more errors.
 * The function does so by reading the input file line by line. For each line, if a macro usage is detected (the first field
 * appears in the macro table) writes its content into the parsed file. Else, if a macro definition keyword is found as
 * the first field of the line, sees the second field of the line as the macro's name, and updates the macro's content
 * line by line until a macro end keyword is found, at which point the macro table is updated with the new macro.
 * If neither of these criteria is met, copies the file from the input file to the parsed file.
 * Assumes that the definition of every macro comes before its usage, that there are no nested macro definitions, that
 * a macro cannot be defined if a macro with the same name has already been defined, and that a macro definition and
 * ending cannot have labels.
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../headers/pre_assembler.h"
#include "../headers/util/string_ops.h"
#include "../headers/fields.h"
#include "../headers/util/general_util.h"
#include "../headers/files.h"
#include "../headers/exit_codes.h"

#define BLANKS " \t"

/**
 * Reads the first, second and thirds fields of a given line, as well as the rest of the line,
 * into given pointers to strings. Separates fields by whitespace characters.
 * Does so by using the find_token method to find the first token of the string (seperated by whitespaces), then does
 * the same twice more, each time with the rest of the string (the part after the token that was just found).
 * 
 * @param line         the line whose fields should be found
 * @param first_field  a pointer to a string that should hold the first field of the line
 * @param second_field a pointer to a string that should hold the second field of the line
 * @param third_field  a pointer to a string that should hold the third field of the line
 * @param rest         a pointer to a string that should hold the rest of the line (the part after the third field)
 */
void get_fields(char *line, char **first_field, char **second_field, char **third_field, char **rest) {
    *first_field = find_token(line, BLANKS, rest);
    *second_field = find_token(*rest, BLANKS, rest);
    *third_field = find_token(*rest, BLANKS, rest);
}

/**
 * Writes a macro's content into a file (should be the parsed file).
 * Assumes that the macro exists in the macro table.
 * 
 * @param macro       the name of the macro
 * @param macro_table a pointer to the macro table
 * @param parsed_file a pointer to the parsed file that the macro content should be written to.
 */
void handle_macro_usage(char *macro, HashTable *macro_table, FILE *parsed_file) {
    MacroContent macro_content = *table_get_macro(macro_table, macro);
    fprintf(parsed_file, "%s", macro_content);
}

/**
 * Checks if a line in the input file includes a macro usage (must be the first field of the line), and if it does, 
 * writes the macro content into the parsed file. Also makes sure that there is no label before the macro usage, and
 * if there is, reports an error.
 * 
 * @param macro_table     a pointer to the macro table
 * @param first_field     the first field of the line
 * @param second_field    the second field of the line
 * @param parsed_file     a pointer to the parsed file
 * @param line_count      the number of the line being checked in the input file (used for error reporting)
 * @param input_file_name the name of the input file (used for error reporting)
 * @param error_found     a pointer to an integer value that should hold whether an error has occurred
 * @return 1 if a macro usage was found, 0 otherwise
 */
int check_and_handle_macro_usage(HashTable *macro_table, char *first_field, char *second_field,
                                 FILE *parsed_file, int line_count, char *input_file_name, int *error_found) {
    /* if the first field is a known macro name, writes its content into the parsed file. only does so if no error
     * was found */
    if (table_contains(macro_table, first_field) && !(*error_found)) {
        if (!is_line_blank(second_field)) {
            printf("Input Error: Extra characters after macro usage in line %d of file %s\n",
                   line_count, input_file_name);
            *error_found = 1;
            return 1;
        }
        handle_macro_usage(first_field, macro_table, parsed_file);
        return 1;
    }
        /* if a label appears before the macro usage, reports an errors and updates the error_found pointer */
    else if (is_label(first_field) && table_contains(macro_table, second_field)) {
        printf("Input Error: Label used before macro usage in line %d of file %s\n", line_count, input_file_name);
        *error_found = 1;
        return 1;
    }
    return 0;
}

/**
 * Checks if a currently-read macro definition has ended, and if it has, inserts it into the macro table.
 * Does so by checking if the first field of the line is the macro end keyword, and making sure that it is the
 * only field (there is no label and there are no characters after the keyword). If so, inserts the macro name and
 * content to the table.
 * 
 * @param macro_table     a pointer to the macro table
 * @param macro_name      the name of the macro whose definition is being read
 * @param line            the current line being analyzed
 * @param error_found     a pointer to an integer value that should hold whether an error has occurred
 * @param line_count      the number of the line being checked in the input file (used for error reporting)
 * @param input_file_name the name of the input file (used for error reporting)
 * @param macro_content   the content of the macro being defined
 * @return 1 if the macro end has been found or an error has occurred, 0 otherwise
 */
int check_and_handle_macro_end(HashTable *macro_table, char *macro_name, char *line, int *error_found, int line_count,
                               char *input_file_name, MacroContent macro_content) {
    /* the first, second and third fields of the line */
    char *first_field;
    char *second_field;
    char *third_field;
    /* the part of the line after the third field */
    char *rest;
    /* reads the fields into the variables */
    get_fields(line, &first_field, &second_field, &third_field, &rest);
    /* checks if the macro end keyword has been found */
    if (equal(first_field, MACRO_END)) {
        /* makes sure there are no extra characters after the macro end keyword */
        if (!is_line_blank(second_field)) {
            printf("Input error: Line %d in file %s includes extra characters after macro end declaration\n",
                   line_count, input_file_name);
            *error_found = 1;
            free_all(3, first_field, second_field, third_field);
            return 1;
        }
        /* adds the macro whose definition just ended to the macro table */
        table_add_macro(macro_table, macro_name, macro_content);
        return 1;
    }
        /* checks if the macro end includes a label, reports an error if yes */
    else if (is_label(first_field) && equal(second_field, MACRO_END)) {
        printf("Input error: Line %d in file %s includes a label before macro end declaration\n",
               line_count, input_file_name);
        *error_found = 1;
        free_all(3, first_field, second_field, third_field);
        return 1;
    }
    free_all(2, first_field, third_field);
    return 0;
}

/**
 * Reads a macro definition and inserts it to the macro table.
 * Does so by reading the lines after the definition title one by one and updating the macro's content accordingly
 * until the macro's end is found, and then inserting the macro to the macro table.
 * Assumes that the definition's first line has already been read.
 * 
 * @param macro_table     a pointer to the macro table
 * @param macro_name      the name of the macro whose definition is being read
 * @param post_macro_name the part of the definition's first line after the macro name
 * @param input_file_name the name of the input file (used for error reporting)
 * @param input_file      a pointer to the input file
 * @param line_count      a pointer to a variable representing the number of the line being checked in the 
 *                        input file (used for error reporting)
 * @param error_found a pointer to an integer value that should hold whether an error has occurred
 */
void handle_macro_definition(HashTable *macro_table, char *macro_name, char *post_macro_name, char *input_file_name,
                             FILE *input_file, int *line_count, int *error_found) {
    /* the line being read */
    char line[MAX_LINE_LENGTH + 1];
    /* the macro's content */
    MacroContent macro_content = (MacroContent) calloc(1, 1);
    if (macro_content == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying macro content\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    /* makes sure no macro with the same name has already been defined */
    if (table_contains(macro_table, macro_name)) {
        printf("Input error: Macro defined in line %d in file %s has already been defined\n", *line_count,
               input_file_name);
        *error_found = 1;
    }
    /* makes sure the macro name is not empty */
    if (is_line_blank(macro_name)) {
        printf("Input error: Macro defined in line %d in file %s has no name\n", *line_count, input_file_name);
        *error_found = 1;
    }
        /* makes sure the macro name is legal */
    else if (!legal_macro_name(macro_name)) {
        printf("Input error: Macro defined in line %d in file %s has an illegal name\n",
               *line_count, input_file_name);
        *error_found = 1;
    }
        /* makes sure there are no extra characters after the macro name */
    else if (!is_line_blank(post_macro_name)) {
        printf("Input error: Line %d in file %s includes extra characters after macro name\n",
               *line_count, input_file_name);
        *error_found = 1;
    }

    /* reads the lines from the input one by one until a macro end is found */
    (*line_count)++;
    if (read_line(input_file, input_file_name, *line_count, line)) *error_found = 1;
    while (1) {
        /* if a macro end is found, inserts it to the macro table and quits reading the definition */
        if (check_and_handle_macro_end(macro_table, macro_name, line, error_found, *line_count,
                                       input_file_name, macro_content)) {
            break;
        }
        /* if a macro end is not found, updates the macro's content */
        else {
            /* reallocates the macro content to a new string that has enough spaces for the next line */
            macro_content = realloc(macro_content, strlen(macro_content) + strlen(line) + 1);
            if (macro_content == NULL) {
                fprintf(stderr, "Memory Error: Memory allocation failure when copying macro content\n");
                exit(MEMORY_ALLOCATION_FAILURE);
            }
            /* adds the next line (with a line break) to the macro's content */
            strcat(macro_content, line);
            strcat(macro_content, "\n");
        }
        /* reads the next line */
        (*line_count)++;
        if (read_line(input_file, input_file_name, *line_count, line)) *error_found = 1;
    }
}

/**
 * Checks if a macro definition starts in the given line. If it does, keeps reading until the macro's end and inserts
 * it to the macro table.
 * Does so by checking if the first field of the line is the macro definition keyword, and that it has no label. If it
 * is, sees the second field as the macro name, and handles the macro definition using handle_macro_definition.
 * 
 * @param macro_table     a pointer to the macro table
 * @param line            the current line being analyzed
 * @param input_file_name the name of the input file (used for error reporting)
 * @param input_file      a pointer to the input file
 * @param line_count      a pointer to a variable representing the number of the line being checked in the 
 *                        input file (used for error reporting)
 * @param error_found     a pointer to an integer value that should hold whether an error has occurred
 * @return 1 if a macro definition was found, 0 otherwise
 */
int check_and_handle_macro_definition(HashTable *macro_table, char *line, char *input_file_name, FILE *input_file,
                                      int *line_count, int *error_found) {
    /* the first, second and third fields of the line */
    char *first_field;
    char *second_field;
    char *third_field;
    /* the part of the line after the third field */
    char *rest;
    /* reads the fields into the variables */
    get_fields(line, &first_field, &second_field, &third_field, &rest);
    /* if a macro definition keyword exists has a label, reports an error */
    if (is_label(first_field) && equal(second_field, MACRO_DEFINITION)) {
        printf("Input Error: Label used before macro definition in line %d of file %s\n", *line_count,
               input_file_name);
        free_all(3, first_field, second_field, third_field);
        return 1;
    }
    /* if the first field is a macro definition keyword, sees the second field as the macro name and keeps reading
     * and handling the macro definition */
    if (equal(first_field, MACRO_DEFINITION)) {
        handle_macro_definition(macro_table, second_field, third_field, input_file_name,
                                input_file, line_count, error_found);
        free_all(1, first_field);
        return 1;
    }
    /* no macro definition was found */
    free_all(3, first_field, second_field, third_field);
    return 0;
}

/**
 * Reads an input file and parses all of its macros. Creates a new parsed file with the same name and a .am extension.
 * If a .am file with the same name already exists, deletes it and creates a new file.
 * If any error is found during the pre-assembling, will not create a parsed file, but will continue parsing the input
 * file and reporting errors, as long as the error does not prevent that.
 * Does so by reading the input file line by line. For each line, if a macro usage is detected (the first field
 * appears in the macro table) writes its content into the parsed file. Else, if a macro definition keyword is found as
 * the first field of the line, sees the second field of the line as the macro's name, and updates the macro's content
 * line by line until a macro end keyword is found, at which point the macro table is updated with the new macro.
 * If neither of these criteria is met, copies the file from the input file to the parsed file.
 * Assumes that the definition of every macro comes before its usage, that there are no nested macro definitions, that
 * a macro cannot be defined if a macro with the same name has already been defined, and that a macro definition and
 * ending cannot have labels.
 * 
 * @param file_name   the name of the input file without the .as extension
 * @param macro_table a pointer to the macro table.
 * @return 1 if an error was found, 0 if the file was parsed successfully
 */
int pre_assemble(char file_name[]) {
    /* a hash-table where macro names are mapped to their contents */
    HashTable *macro_table = create_table();
    /* a pointer to the input .as file */
    FILE *input_file;
    /* a pointer to the parsed .am file */
    FILE *parsed_file;
    /* the name of the input file (including the extension) */
    char *input_file_name;
    /* the name of the parsed file (including the extension) */
    char *parsed_file_name;
    /* the number of the line being read */
    int line_count;
    /* the line being read */
    char line[MAX_LINE_LENGTH + 1];
    /* the first, second and third fields of the line */
    char *first_field;
    char *second_field;
    char *third_field;
    /* the portion of the line after the first field */
    char *rest;
    /* whether an error has occurred */
    int error_found = 0;

    /* gets the name of the input file and a pointer to the file, if the file can't be read
     * the pre-assembly process is stopped for the file */
    input_file_name = get_input_file_name(file_name);
    input_file = get_input_file(file_name);
    /* if the input file is null */
    if (!input_file) {
        free(input_file_name);
        return 1;
    }

    /* gets the name of the parsed file and a pointer to the file, the parsed file is null then an error is reported and
     * the error_found flag is set to 1 */
    parsed_file_name = get_parsed_file_name(file_name);
    parsed_file = get_parsed_file_append(file_name);
    if (!parsed_file) error_found = 1;

    /* reads the input file line by line, and checks for macro usage and definition */
    line_count = 0;
    while (!feof(input_file)) {
        line_count++;
        /* if an error has occurred while reading the line, the error_found flag is set to 1 */
        if (read_line(input_file, input_file_name, line_count, line)) error_found = 1;
        /* puts the first, second and thirds fields of the line into the variables, as well as the portion after the
         * third field */
        get_fields(line, &first_field, &second_field, &third_field, &rest);
        /* if a macro usage is detected, its content are written to the parsed file, and the loop moves to the
         * next line */
        if (check_and_handle_macro_usage(macro_table, first_field, second_field, parsed_file,
                                         line_count, input_file_name, &error_found)) {
            continue;
        }
        /* if a macro definition is detected, it is inserted to the macro table, and the loop moves to the
         * line after the macro's end */
        if (check_and_handle_macro_definition(macro_table, line, file_name,
                                              input_file, &line_count, &error_found)) {
            continue;
        }
        /* if no special case is detected and no error has occurred so far, copies the line to the parsed file */
        if (!error_found) fprintf(parsed_file, "%s\n", line);
    }
    /* closes the files */
    fclose(input_file);
    fclose(parsed_file);
    /* if an error has been found, removes the parsed file since the parsing cannot be correct */
    if (error_found) remove(parsed_file_name);
    free_all(5, first_field, second_field, third_field, input_file_name, parsed_file_name);
    free_table(macro_table, 0);
    return error_found;
}
