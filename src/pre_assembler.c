#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../headers/pre_assembler.h"
#include "../headers/util/string_ops.h"
#include "../headers/fields.h"
#include "../headers/util/general_util.h"

#define INPUT_EXTENSION ".as"
#define PARSED_EXTENSION ".am"
#define BLANKS " \t"

char *get_input_file_name(char file_name[]) {
    char *input_file_name = calloc(strlen(file_name) + strlen(INPUT_EXTENSION) + 1, 1);
    if (input_file_name == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying file name\n");
        return NULL;
    }
    strcat(input_file_name, file_name);
    strcat(input_file_name, INPUT_EXTENSION);
    return input_file_name;
}

char *get_parsed_file_name(char file_name[]) {
    char *parsed_file_name = calloc(strlen(file_name) + strlen(PARSED_EXTENSION) + 1, 1);
    if (parsed_file_name == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying file name\n");
        return NULL;
    }
    strcat(parsed_file_name, file_name);
    strcat(parsed_file_name, PARSED_EXTENSION);
    return parsed_file_name;
}

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

FILE *get_parsed_file(char file_name[]) {
    FILE *parsed_file;
    char *parsed_file_name = get_parsed_file_name(file_name);
    remove(parsed_file_name);
    parsed_file = fopen(parsed_file_name, "a");
    if (parsed_file == NULL) {
        printf("Error: Can't create file %s\n", parsed_file_name);
        return NULL;
    }
    free(parsed_file_name);
    return parsed_file;
}

int get_fields(char *line, char **first_field, char **second_field, char **third_field, char **rest, int *error_found) {
    *first_field = find_token(line, BLANKS, rest);
    if (*first_field == NULL) {
        *error_found = 1;
        return 1;
    }
    *second_field = find_token(*rest, BLANKS, rest);
    if (*second_field == NULL) {
        *error_found = 1;
        return 1;
    }
    *third_field = find_token(*rest, BLANKS, rest);
    if (*second_field == NULL) {
        *error_found = 1;
        return 1;
    }
    return 0;
}

void handle_macro_usage(char *macro, HashTable *macro_table, FILE *parsed_file) {
    char *macro_content = table_get_string(macro_table, macro);
    fprintf(parsed_file, "%s", macro_content);
}

int check_and_handle_macro_usage(HashTable *macro_table, char *first_field, char *second_field,
                                 FILE *parsed_file, int line_count, char *input_file_name, int *error_found) {
    if (table_contains(macro_table, first_field)) {
        handle_macro_usage(first_field, macro_table, parsed_file);
        return 1;
    } else if (is_label(first_field) && table_contains(macro_table, second_field)) {
        printf("Input Error: Label used before macro usage in line %d of file %s\n", line_count, input_file_name);
        *error_found = 1;
        return 1;
    }
    return 0;
}

int check_and_handle_macro_end(HashTable *macro_table, char *macro_name, char *line, int *error_found, int line_count,
                               char *input_file_name, char *macro_content) {
    char *first_field;
    char *second_field;
    char *third_field;
    char *rest;
    if (get_fields(line, &first_field, &second_field, &third_field, &rest, error_found)) {
        printf("Error: File %s could not be parsed due to a memory error\n", input_file_name);
        free_all(3, first_field, second_field, third_field);
        return 1;
    }
    if (equal(first_field, MACRO_END)) {
        if (!is_line_blank(second_field)) {
            printf("Input error: Line %d in file %s includes extra characters after macro end declaration\n",
                   line_count, input_file_name);
            *error_found = 1;
            free_all(3, first_field, second_field, third_field);
            return 1;
        }
        table_add_string(macro_table, macro_name, macro_content);
        return 1;
    } else if (is_label(first_field) && equal(second_field, MACRO_END)) {
        printf("Input error: Line %d in file %s includes a label before macro end declaration\n",
               line_count, input_file_name);
        *error_found = 1;
        free_all(3, first_field, second_field, third_field);
        return 1;
    }
    free_all(2, first_field, third_field);
    return 0;
}

void handle_macro_definition(HashTable *macro_table, char *macro_name, char *post_macro_name, char *input_file_name, 
                             FILE *input_file, int *line_count, int *error_found) {
    char *macro_content = malloc(0);
    char line[MAX_LINE_LENGTH + 1];
    if (table_contains(macro_table, macro_name)) {
        printf("Input error: Macro defined in line %d in file %s has already been defined\n", *line_count,
               input_file_name);
        *error_found = 1;
    }
    if (is_line_blank(macro_name)) {
        printf("Input error: Macro defined in line %d in file %s has no name\n", *line_count, input_file_name);
        *error_found = 1;
    }
    else if (!legal_macro_name(macro_name)) {
        printf("Input error: Macro defined in line %d in file %s has an illegal name\n",
               *line_count, input_file_name);
        *error_found = 1;
    }
    else if (!is_line_blank(post_macro_name)) {
        printf("Input error: Line %d in file %s includes extra characters after macro name\n",
               *line_count, input_file_name);
        *error_found = 1;
    }
    

    (*line_count)++;
    if (read_line(input_file, input_file_name, *line_count, line)) *error_found = 1;
    while (1) {
        if (check_and_handle_macro_end(macro_table, macro_name, line, error_found, *line_count,
                                       input_file_name, macro_content)) {
            break;
        } else {
            macro_content = realloc(macro_content, strlen(macro_content) + strlen(line) + 1);
            if (macro_content == NULL) {
                fprintf(stderr, "Memory Error: Memory allocation failure when copying macro content");
            }
            strcat(macro_content, line);
            strcat(macro_content, "\n");
        }
        (*line_count)++;
        if (read_line(input_file, input_file_name, *line_count, line)) *error_found = 1;
    }
}

int check_and_handle_macro_definition(HashTable *macro_table, char *line, char *input_file_name, FILE *input_file,
                                      int *line_count, int *error_found) {
    char *first_field;
    char *second_field;
    char *third_field;
    char *rest;
    if (get_fields(line, &first_field, &second_field, &third_field, &rest, error_found)) {
        printf("Error: File %s could not be parsed due to a memory error\n", input_file_name);
        free_all(3, first_field, second_field, third_field);
        return 0;
    }
    if (is_label(first_field) && equal(second_field, MACRO_DEFINITION)) {
        printf("Input Error: Label used before macro definition in line %d of file %s\n", *line_count,
               input_file_name);
        free_all(3, first_field, second_field, third_field);
        return 1;
    }
    if (equal(first_field, MACRO_DEFINITION)) {
        handle_macro_definition(macro_table, second_field, third_field, input_file_name, 
                                input_file, line_count,error_found);
        free_all(1, first_field);
        return 1;
    }
    free_all(3, first_field, second_field, third_field);
    return 0;
}

int pre_assemble(char file_name[], HashTable *macro_table) {
    FILE *input_file;
    FILE *parsed_file;
    char *input_file_name;
    char *parsed_file_name;
    int line_count;
    char line[MAX_LINE_LENGTH + 1];
    char *first_field;
    char *second_field;
    char *third_field;
    char *rest;
    int error_found = 0;

    input_file_name = get_input_file_name(file_name);
    input_file = get_input_file(file_name);
    if (!input_file) {
        free(input_file_name);
        return 1;
    }

    parsed_file_name = get_parsed_file_name(file_name);
    parsed_file = get_parsed_file(file_name);
    if (!parsed_file) error_found = 1;
    line_count = 0;
    while (!feof(input_file)) {
        line_count++;
        if (read_line(input_file, input_file_name, line_count, line)) error_found = 1;
        if (get_fields(line, &first_field, &second_field, &third_field, &rest, &error_found)) {
            printf("Error: File %s could not be parsed due to a memory error", input_file_name);
            free_all(5, first_field, second_field, third_field, input_file_name, parsed_file_name);
            return 1;
        };
        if (check_and_handle_macro_usage(macro_table, first_field, second_field, parsed_file,
                                         line_count, input_file_name, &error_found)) {
            continue;
        }
        if (check_and_handle_macro_definition(macro_table, line, file_name,
                                              input_file, &line_count, &error_found))
            continue;
        fprintf(parsed_file, "%s\n", line);
    }
    fclose(input_file);
    fclose(parsed_file);
    if (error_found) remove(parsed_file_name);
    free_all(5, first_field, second_field, third_field, input_file_name, parsed_file_name);
    return error_found;
}
