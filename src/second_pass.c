#include "../headers/second_pass.h"
#include "../headers/string_ops.h"
#include "../headers/fields.h"
#include "../headers/requirements.h"
#include "stdio.h"

void handle_instruction(char *line, int line_count, char *parsed_file_name, int *error_found,
                        Requirements *requirements);

void check_and_handle_entry(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                            Requirements *requirements);

int second_pass(char file_name[], Requirements *requirements) {
    char *parsed_file_name = get_parsed_file_name(file_name);
    FILE *parsed_file = get_parsed_file_read(file_name);
    int error_found = 0;
    /* the number of the line being read */
    int line_count = 0;
    /* the line being read */
    char line_read[MAX_LINE_LENGTH + 1];
    /* the lin's label */
    char *label;
    /* makes sure the parsed file is not null, if it does then an error is found */
    if (!parsed_file) {
        free(parsed_file_name);
        return 1;
    }
    /* for each line */
    while (!feof(parsed_file)) {
        /* a pointer version of line_read that can have a pointer reference it */
        char *line;
        line_count++;
        read_line(parsed_file, parsed_file_name, line_count, line_read);
        line = line_read;
        if (is_line_blank(line) || exists(line, COMMENT_START)) continue;
        find_label(&line, &label, line_count, parsed_file_name, &error_found);
        /* checks that the part after the label is not blank */
        if (is_line_blank(line)) continue;
        if (!is_directive(line)) {
            handle_instruction(line, line_count, parsed_file_name, &error_found, requirements);
        } else check_and_handle_entry(line, label, line_count, parsed_file_name, &error_found, requirements);
    }
    fclose(parsed_file);
    free(parsed_file_name);
    return error_found;
}

void check_and_handle_entry(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                            Requirements *requirements) {
    char *rest;
    char *directive = find_token(line, BLANKS, &rest);
    if (equal(directive, ENTRY_DIRECTIVE)) {
        /* the symbol given as argument for .extern */
        char *argument;
        SymbolContent *symbol;
        /* if the line has a label, issues a warning */
        if (label_name != NULL) {
            printf("Warning: Label found before .entry directive in line %d of file %s\n",
                   line_count, parsed_file_name);
        }
        /* the argument is the field directly after .entry */
        argument = find_token(rest, BLANKS, &rest);
        /* makes sure the argument field is not empty */
        if (is_line_blank(argument)) {
            printf("Input Error: No argument given to .entry directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        /* makes sure the part of the line after the argument is empty */
        if (!is_line_blank(rest)) {
            printf("Input Error: Extra characters after the argument for .entry directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        if (!table_contains(requirements->symbol_table, argument)) {
            printf("Input Error: Symbol \"%s\" given as argument for .entry directive in line %d of file %s is"
                   "undefined in that file\n", argument, line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        symbol = table_get_symbol(requirements->symbol_table, argument);
        if (symbol->type == EXTERNAL) {
            printf("Input Error: Symbol \"%s\" given as argument for .entry directive in line %d of file %s is"
                   "already defined in that file as external\n", argument, line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        /* @TODO check for defining something as .entry twice */
        symbol->type = ENTRY;
        requirements->entry_found = 1;
    }
}