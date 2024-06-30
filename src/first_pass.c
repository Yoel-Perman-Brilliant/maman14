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
#define MAX_WORD_SIZE (short)(pow(2, WORD_SIZE_BITS) - 1)
#define MIN_WORD_SIZE (short)(-(pow(2, WORD_SIZE_BITS - 1)))

void insert_data(char *rest, int *dc, char *parsed_file_name, int line_count, Requirements *requirements,
                 int *errors_found) {
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
        requirements->data_array[*dc] = NUM_TO_WORD(value);
        (*dc)++;
        arg = find_token(rest, DATA_SEPARATOR, &rest);
        trimmed_arg = trim(arg);
        free(arg);
    }
    free(trimmed_arg);
}

int first_pass(char file_name[], Requirements *requirements) {
    int dc = 0;
    int errors_found = 0;
    int i;
    insert_data("   7, 32767, +17  ,  9           ", &dc, "th", 1, requirements, &errors_found);
    for (i = 0; i < 6; i++) {
        printf("i: %d, value: %u\n", i, requirements->data_array[i]);
    }
    return 0;
}