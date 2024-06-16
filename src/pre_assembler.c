#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdlib.h"
#include "../headers/pre_assembler.h"
#include "../headers/util/read_ops.h"
#include "../headers/util/string_ops.h"
#include "../headers/field_names.h"


#define INPUT_EXTENSION ".as"
#define PARSED_EXTENSION ".am"
#define BLANKS " \t"

/*
 * @TODO
 * 1. Handle labels
 * 2. handle file already existing
 * 3. check for errors (illegal input)
 * 4. free pointers
 * 5. check for macro names with underscores
 * 6. possibly split into sub-functions cuz this shit is ugly as balls
 */
int pre_assemble(char file_name[], HashTable *macro_table) {
    FILE *input_file;
    FILE *parsed_file;
    char *input_file_name;
    char *parsed_file_name;
    int line_count;
    char line[MAX_LINE_LENGTH + 1];
    char *first_field;
    char *rest;
    char *macro_name;

    input_file_name = malloc(strlen(file_name) + strlen(INPUT_EXTENSION) + 1);
    if (input_file_name == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying file name\n");
        return 1;
    }

    parsed_file_name = malloc(strlen(file_name) + strlen(PARSED_EXTENSION) + 1);
    if (parsed_file_name == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when copying file name\n");
        return 1;
    }

    strcat(input_file_name, file_name);
    strcat(input_file_name, INPUT_EXTENSION);
    strcat(parsed_file_name, file_name);
    strcat(parsed_file_name, PARSED_EXTENSION);

    input_file = fopen(input_file_name, "r");
    parsed_file = fopen(parsed_file_name, "a");

    line_count = 0;
    while (!feof(input_file)) {
        line_count++;
        if (read_line(input_file, input_file_name, line_count, line)) return 1;
        first_field = find_token(line, BLANKS, &rest);
        if (table_contains(macro_table, first_field)) {
            /*if (!is_line_blank(rest)) {
                printf("Input error: Line %d in file %s includes extra characters after macro name!", 
                       line_count, input_file_name);
                return 1;
            }*/
            char *macro_content = table_get_string(macro_table, first_field);
            fprintf(parsed_file, "%s", macro_content);
            continue;
        }
        else if (equal(first_field, MACRO_DEFINITION)) {
            char *macro_content = malloc(0);
            macro_name = find_token(rest, BLANKS, &rest);
            if (macro_name == NULL) return 1;
            if (is_line_blank(macro_name)) {
                printf("Input error: Macro defined in line %d in file %s has no name\n", line_count, file_name);
                return 1;
            }
            if (!is_line_blank(rest)) {
                printf("Input error: Line %d in file %s includes extra characters after macro name\n",
                       line_count, input_file_name);
                return 1;
            }
            if (!legal_macro_and_label_name(macro_name)) {
                printf("Input error: Macro defined in line %d in file %s has an illegal name\n",
                       line_count, input_file_name);
                return 1;
            }
            
            line_count++;
            if (read_line(input_file, input_file_name, line_count, line)) return 1;
            while (1) {
                first_field = find_token(line, BLANKS, &rest);
                if (first_field == NULL) return 1;
                if (equal(first_field, MACRO_END)) {
                    if (!is_line_blank(rest)) {
                        printf("Input error: Line %d in file %s includes extra characters after macro name\n",
                               line_count, input_file_name);
                        return 1;
                    }
                    table_add_string(macro_table, macro_name, macro_content);
                    break;
                }
                else {
                    macro_content = realloc(macro_content, strlen(macro_content) + strlen(line) + 1);
                    strcat(macro_content, line);
                    strcat(macro_content, "\n");
                }
                line_count++;
                if (read_line(input_file, input_file_name, line_count, line)) return 1;
            }
        }
        else fprintf(parsed_file, "%s\n", line);
    }
    return 0;
}
