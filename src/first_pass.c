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
#define MAX_WORD_SIZE (short)(pow(2, WORD_SIZE_BITS) - 1)
#define MIN_WORD_SIZE (short)(-(pow(2, WORD_SIZE_BITS - 1)))

void insert_data(char *rest, char *parsed_file_name, int line_count, Requirements *requirements, int *errors_found) {
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
        requirements->data_array[requirements->dc] = NUM_TO_WORD(value);
        (requirements->dc)++;
        arg = find_token(rest, DATA_SEPARATOR, &rest);
        trimmed_arg = trim(arg);
        free(arg);
    }
    free(trimmed_arg);
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
        insert_data(rest, parsed_file_name, line_count, requirements, error_found);
        return 1;
    }
    else if (equal(directive, STRING_DIRECTIVE)) {
        /* @TODO add */
        insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                      line_count, parsed_file_name);
        return 1;
    }
    else if (equal(directive, EXTERN_DIRECTIVE)) {
        /* @TODO add */
    }
    else if (equal(directive, ENTRY_DIRECTIVE)) {
        /* @TODO add */
    }
    return 0;
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

int first_pass(char file_name[], Requirements *requirements) {
    int errors_found = 0;
    int i;
    char *label_name = NULL;
    SymbolContent symbol_content;
    char *line1 = "HELLO: .data   7, 32767, +17  ,  -9           ";
    find_label(&line1, &label_name, 1, "th", &errors_found);
    check_and_handle_directive(line1, label_name, 1, "th", &errors_found, requirements);
    char *line2 = "HELLOW : .data   7, 32767, +17  ,  -9           ";
    find_label(&line2, &label_name, 2, "th", &errors_found);
    check_and_handle_directive(line2, label_name, 2, "th", &errors_found, requirements);
    for (i = 0; i < 12; i++) {
        printf("i: %d, value: %u\n", i, requirements->data_array[i]);
    }
    symbol_content = table_get_symbol(requirements->symbol_table, "HELLOW");
    return 0;
}