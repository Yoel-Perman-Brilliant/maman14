#include "../headers/first_pass.h"
#include "../headers/files.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "../headers/requirements.h"
#include "../headers/conversions.h"
#include "../headers/util/string_ops.h"

#define DATA_SEPARATOR ","
#define DOUBLE_DATA_SEPARATOR ",,"
#define BLANKS " \t"
#define STRING_START_AND_END '"'
#define MAX_WORD_SIZE (short)(pow(2, WORD_SIZE_BITS) - 1)
#define MIN_WORD_SIZE (short)(-(pow(2, WORD_SIZE_BITS - 1)))


void insert_data_numbers(char *rest, char *parsed_file_name, int line_count, Requirements *requirements, int *errors_found);

void insert_symbol(char *symbol, SymbolType type, SymbolLocation location, Requirements *requirements,
                   int *error_found, int line_count, char *parsed_file_name);

int check_and_handle_directive(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                               Requirements *requirements);

void find_label(char **line, char **label_name, int line_count, char *parsed_file_name, int *error_found);

void handle_extern(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                   Requirements *requirements);

void insert_string(char *rest, int line_count, char *parsed_file_name, int *error_found, Requirements *requirements);

int first_pass(char file_name[], Requirements *requirements) {
    int errors_found = 0;
    int i;
    char *label_name = NULL;
    SymbolContent symbol_content;
    char *line1 = "HELLO: .data   7, 32767, +17  ,  -9           ";
    find_label(&line1, &label_name, 1, "th", &errors_found);
    check_and_handle_directive(line1, label_name, 1, "th", &errors_found, requirements);
    char *line3 = ".extern POO";
    find_label(&line3, &label_name, 3, "th", &errors_found);
    check_and_handle_directive(line3, label_name, 3, "th", &errors_found, requirements);
    char *line2 = "HELLOW: .data   7, 32767, +17  ,  -9           ";
    find_label(&line2, &label_name, 2, "th", &errors_found);
    check_and_handle_directive(line2, label_name, 2, "th", &errors_found, requirements);
    char *line4 = "JJJ: .string \"peewop\" ";
    find_label(&line4, &label_name, 4, "th", &errors_found);
    check_and_handle_directive(line4, label_name, 4, "th", &errors_found, requirements);
    char *line5 = "JJJK: .string \"peewop\" ";
    find_label(&line5, &label_name, 5, "th", &errors_found);
    check_and_handle_directive(line5, label_name, 5, "th", &errors_found, requirements);
    for (i = 0; i < 20; i++) {
        printf("i: %d, value: %u\n", i, requirements->data_array[i]);
    }
    symbol_content = table_get_symbol(requirements->symbol_table, "JJJK");
    printf("symbol value: %d\n", symbol_content.value);
    printf("symbol location: %d\n", symbol_content.location);
    printf("symbol type: %d\n", symbol_content.type);
    return 0;
}

void insert_data_numbers(char *rest, char *parsed_file_name, int line_count, Requirements *requirements, int *errors_found) {
    char *arg = NULL;
    char *trimmed_arg;
    if (first_non_blank(rest) == *DATA_SEPARATOR) {
        printf("Input Error: .data directive in line %d of file %s starts with an illegal comma\n",
               line_count, parsed_file_name);
        *errors_found = 1;
        return;
    }
    if (last_non_blank(rest) == *DATA_SEPARATOR) {
        printf("Input Error: .data directive in line %d of file %s ends with an illegal comma\n",
               line_count, parsed_file_name);
        *errors_found = 1;
        return;
    }
    if (includes_consecutive(rest, *DATA_SEPARATOR)) {
        printf("Input Error: .data directive in line %d of file %s includes multiple consecutive commas\n",
               line_count, parsed_file_name);
        *errors_found = 1;
        return;
    }
    if (is_line_blank(rest)) {
        printf("Input Error: .data directive in line %d of file %s has no arguments\n", line_count, parsed_file_name);
        *errors_found = 1;
        return;
    }
    arg = find_token(rest, DATA_SEPARATOR, &rest);
    trimmed_arg = trim(arg);
    free(arg);
    while (!is_line_blank(trimmed_arg)) {
        int value;
        if (is_line_blank(trimmed_arg)) {
            printf("Input Error: .data directive in line %d of file %s has multiple consecutive commas\n",
                   line_count, parsed_file_name);
            *errors_found = 1;
            return;
        }
        if (strpbrk(trimmed_arg, BLANKS)) {
            printf("Input Error: Missing comma in .data directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *errors_found = 1;
            return;
        }
        if (!is_integer(trimmed_arg)) {
            printf("Input Error: argument \"%s\" of .data directive in line %d of file %s is not an integer\n",
                   trimmed_arg, line_count, parsed_file_name);
            *errors_found = 1;
            return;
        }
        value = to_integer(trimmed_arg);
        if (value > MAX_WORD_SIZE || value < MIN_WORD_SIZE) {
            printf("Input Error: argument \"%d\" of .data directive in line %d of file %s is not "
                   "within the machine's memory cell bounds\n", value, line_count, parsed_file_name);
            *errors_found = 1;
            return;
        }
        *errors_found |= memory_insert_data(requirements, value, line_count, parsed_file_name);
        arg = find_token(rest, DATA_SEPARATOR, &rest);
        trimmed_arg = trim(arg);
        free(arg);
    }
    free(trimmed_arg);
}

/* consider better error reporting */
void insert_string(char *rest, int line_count, char *parsed_file_name, int *errors_found, Requirements *requirements) {
    char *trimmed_rest;
    int trimmed_rest_length;
    int i;
    if (first_non_blank(rest) != STRING_START_AND_END) {
        printf("Input Error: Argument for .string directive in line %d of file %s does not start with "
               "double quotation marks\n", line_count, parsed_file_name);
        *errors_found = 1;
        return;
    }
    if (last_non_blank(rest) != STRING_START_AND_END) {
        printf("Input Error: Argument for .string directive in line %d of file %s does not start with "
               "double quotation marks\n", line_count, parsed_file_name);
        *errors_found = 1;
        return;
    }
    trimmed_rest = trim(rest);
    trimmed_rest_length = strlen(trimmed_rest);
    for (i = 1; i < trimmed_rest_length - 1; i++) {
        unsigned short value = NUM_TO_WORD(trimmed_rest[i]);
        *errors_found |= memory_insert_data(requirements, value, line_count, parsed_file_name);
    }
    *errors_found |= memory_insert_data(requirements, 0, line_count, parsed_file_name);
    free(trimmed_rest);
}

void insert_symbol(char *symbol, SymbolType type, SymbolLocation location, Requirements *requirements,
                   int *error_found, int line_count, char *parsed_file_name) {
    SymbolContent content;
    if (table_contains(requirements->symbol_table, symbol)) {
        if (type == EXTERNAL) {
            printf("Input Error: Symbol \"%s\" given as a parameter for .extern in line %d of file %s is "
                   "already defined in the file\n", symbol, line_count, parsed_file_name);
        } else {
            printf("Input Error: Label %s in line %d of file %s is already defined\n",
                   symbol, line_count, parsed_file_name);
        }
        *error_found = 1;
        return;
    }
    content.type = type;
    content.location = location;
    if (location == CODE) content.value = requirements->ic;
    else if (location == DATA) content.value= requirements->dc;
    else content.value = 0;
    table_add_symbol(requirements->symbol_table, symbol, content);
}

int check_and_handle_directive(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                               Requirements *requirements) {
    char *rest;
    char *directive = find_token(line, BLANKS, &rest);
    if (equal(directive, DATA_DIRECTIVE)) {
        if (label_name != NULL) {
            insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                          line_count, parsed_file_name);
        }
        insert_data_numbers(rest, parsed_file_name, line_count, requirements, error_found);
        return 1;
    }
    else if (equal(directive, STRING_DIRECTIVE)) {
        if (label_name != NULL) {
            insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                          line_count, parsed_file_name);
        }
        insert_string(rest, line_count, parsed_file_name, error_found, requirements);
        return 1;
    }
    else if (equal(directive, EXTERN_DIRECTIVE)) {
        handle_extern(rest, label_name, line_count, parsed_file_name, error_found, requirements);
        return 1;
    }
    else if (equal(directive, ENTRY_DIRECTIVE)) {
        /* @TODO add */
    }
    return 0;
}

void handle_extern(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                  Requirements *requirements) {
    char *symbol;
    if (label_name != NULL) {
        printf("Warning: Label found before .extern directive in line %d of file %s\n",
               line_count, parsed_file_name);
    }
    symbol = find_token(rest, BLANKS, &rest);
    if (is_line_blank(symbol)) {
        printf("Input Error: No argument given to .extern directive in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (!is_line_blank(rest)) {
        printf("Input Error: Extra characters after the argument for .extern directive in line %d of file %s\n",
               line_count, parsed_file_name); 
        *error_found = 1;
        return;
    }
    if (!legal_label_name(symbol)) {
        printf("Input Error: Symbol given as argument to .extern in line %d of file %s has an illegal name\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    insert_symbol(symbol, EXTERNAL, UNDEFINED, requirements, error_found, line_count, parsed_file_name);
}

void find_label(char **line, char **label_name, int line_count, char *parsed_file_name, int *error_found) {
    char *rest;
    char *first_field = find_token(*line, BLANKS, &rest);
    if (first_field == NULL) {
        *error_found = 1;
        return;
    }
    if (is_label(first_field)) {
        label_to_symbol(first_field);
        if (!legal_label_name(first_field)) {
            printf("Input Error: Label in line %d of file %s has an illegal name\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        *label_name = first_field;
        *line = rest;
    }
}