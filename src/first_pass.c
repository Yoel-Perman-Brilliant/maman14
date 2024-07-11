#include "../headers/first_pass.h"
#include "../headers/files.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "../headers/requirements.h"
#include "../headers/conversions.h"
#include "../headers/util/string_ops.h"
#include "../headers/util/general_util.h"
#include "../headers/operators.h"

#define DATA_SEPARATOR ","
#define DOUBLE_DATA_SEPARATOR ",,"
#define OPERAND_SEPARATOR ","
#define DOUBLE_OPERAND_SEPARATOR ",,"
#define BLANKS " \t"
#define STRING_START_AND_END '"'
#define MAX_WORD_SIZE (short)(pow(2, WORD_SIZE_BITS) - 1)
#define MIN_WORD_SIZE (short)(-(pow(2, WORD_SIZE_BITS - 1)))
#define IMMEDIATE_ADDRESS_START '#'
#define INDIRECT_REGISTER_ADDRESS_START '*'


void insert_data_numbers(char *rest, char *parsed_file_name, int line_count, Requirements *requirements,
                         int *error_found);

void insert_symbol(char *symbol, SymbolType type, SymbolLocation location, Requirements *requirements,
                   int *error_found, int line_count, char *parsed_file_name);

int check_and_handle_directive(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                               Requirements *requirements);

void find_label(char **line, char **label_name, int line_count, char *parsed_file_name, int *error_found);

void handle_extern(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                   Requirements *requirements);

void insert_string(char *rest, int line_count, char *parsed_file_name, int *error_found, Requirements *requirements);

AddressMethod get_address_method(char *operand);

void handle_instruction(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                        Requirements *requirements);

void handle_two_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements);

void handle_one_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements);

void handle_zero_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                     int *error_found, Requirements *requirements);

int has_comment_start(char *line, int line_count, char *parsed_file_name, int *error_found);

int first_pass(char file_name[], Requirements *requirements) {
    char *parsed_file_name = get_parsed_file_name(file_name);
    FILE *parsed_file = get_parsed_file_read(file_name);
    int error_found = 0;
    /* the number of the line being read */
    int line_count = 0;
    /* the line being read */
    char line_read[MAX_LINE_LENGTH + 1];
    char *label;
    if (!parsed_file) {
        free(parsed_file_name);
        return 1;
    }
    while (!feof(parsed_file)) {
        char *line;
        line_count++;
        read_line(parsed_file, parsed_file_name, line_count, line_read);
        line = line_read;
        label = NULL;
        find_label(&line, &label, line_count, parsed_file_name, &error_found);
        if (is_line_blank(line)) continue;
        if (line[0] == COMMENT_START) continue;
        if (has_comment_start(line, line_count, parsed_file_name, &error_found)) continue;
        if (check_and_handle_directive(line, label, line_count, parsed_file_name, &error_found,
                                       requirements)) {
            continue;
        }
        handle_instruction(line, label, line_count, parsed_file_name, &error_found, requirements);
    }
    table_add_to_all_apply(requirements->symbol_table, requirements->ic, is_data_symbol);
    fclose(parsed_file);
    free(parsed_file_name);
    return error_found;
}

void insert_data_numbers(char *rest, char *parsed_file_name, int line_count,
                         Requirements *requirements, int *error_found) {
    char *arg = NULL;
    char *trimmed_arg;
    if (first_non_blank(rest) == *DATA_SEPARATOR) {
        printf("Input Error: .data directive in line %d of file %s starts with an illegal comma\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (last_non_blank(rest) == *DATA_SEPARATOR) {
        printf("Input Error: .data directive in line %d of file %s ends with an illegal comma\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (includes_consecutive(rest, *DATA_SEPARATOR)) {
        printf("Input Error: .data directive in line %d of file %s includes multiple consecutive commas\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (is_line_blank(rest)) {
        printf("Input Error: .data directive in line %d of file %s has no arguments\n", line_count, parsed_file_name);
        *error_found = 1;
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
            *error_found = 1;
            return;
        }
        if (strpbrk(trimmed_arg, BLANKS)) {
            printf("Input Error: Missing comma in .data directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        if (!is_integer(trimmed_arg)) {
            printf("Input Error: argument \"%s\" of .data directive in line %d of file %s is not an integer\n",
                   trimmed_arg, line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        value = to_integer(trimmed_arg);
        if (value > MAX_WORD_SIZE || value < MIN_WORD_SIZE) {
            printf("Input Error: argument \"%d\" of .data directive in line %d of file %s is not "
                   "within the machine's memory cell bounds\n", value, line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        *error_found |= memory_insert_data(requirements, value, line_count, parsed_file_name);
        arg = find_token(rest, DATA_SEPARATOR, &rest);
        trimmed_arg = trim(arg);
        free(arg);
    }
    free(trimmed_arg);
}

/* consider better error reporting */
void insert_string(char *rest, int line_count, char *parsed_file_name, int *error_found, Requirements *requirements) {
    char *trimmed_rest;
    int trimmed_rest_length;
    int i;
    if (first_non_blank(rest) != STRING_START_AND_END) {
        printf("Input Error: Argument for .string directive in line %d of file %s does not start with "
               "double quotation marks\n", line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (last_non_blank(rest) != STRING_START_AND_END) {
        printf("Input Error: Argument for .string directive in line %d of file %s does not start with "
               "double quotation marks\n", line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    trimmed_rest = trim(rest);
    trimmed_rest_length = strlen(trimmed_rest);
    for (i = 1; i < trimmed_rest_length - 1; i++) {
        unsigned short value = NUM_TO_WORD(trimmed_rest[i]);
        *error_found |= memory_insert_data(requirements, value, line_count, parsed_file_name);
    }
    *error_found |= memory_insert_data(requirements, 0, line_count, parsed_file_name);
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
    else if (location == DATA) content.value = requirements->dc;
    else content.value = 0;
    table_add_symbol(requirements->symbol_table, symbol, content);
}

int check_and_handle_directive(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                               Requirements *requirements) {
    char *rest;
    char *directive = find_token(line, BLANKS, &rest);
    if (directive[0] != DIRECTIVE_START) return 0;
    if (equal(directive, DATA_DIRECTIVE)) {
        if (label_name != NULL) {
            insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                          line_count, parsed_file_name);
        }
        insert_data_numbers(rest, parsed_file_name, line_count, requirements, error_found);
        return 1;
    } else if (equal(directive, STRING_DIRECTIVE)) {
        if (label_name != NULL) {
            insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                          line_count, parsed_file_name);
        }
        insert_string(rest, line_count, parsed_file_name, error_found, requirements);
        return 1;
    } else if (equal(directive, EXTERN_DIRECTIVE)) {
        handle_extern(rest, label_name, line_count, parsed_file_name, error_found, requirements);
        return 1;
    } else if (equal(directive, ENTRY_DIRECTIVE)) {
        return 1;
    } else {
        printf("Input Error: Illegal directive \"%s\" in line %d of file %s", directive, line_count, parsed_file_name);
        *error_found = 1;
        return 0;
    }
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

void handle_instruction(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                        Requirements *requirements) {
    char *rest;
    char *operator_name = find_token(line, BLANKS, &rest);
    Operator op;
    if (label_name != NULL)
        insert_symbol(label_name, REGULAR, CODE, requirements,
                      error_found, line_count, parsed_file_name);
    if (!is_operator(operator_name)) {
        printf("Input Error: Illegal instruction name \"%s\" in line %d of file %s\n",
               operator_name, line_count, parsed_file_name);
        return;
    }
    op = get_operator(operator_name);
    if (has_source(op)) {
        handle_two_operand_instruction(op, rest, line_count, parsed_file_name, error_found, requirements);
    } else if (has_destination(op)) {
        handle_one_operand_instruction(op, rest, line_count, parsed_file_name, error_found, requirements);
    } else {
        handle_zero_operand_instruction(op, rest, line_count, parsed_file_name, error_found, requirements);
    }
    free(operator_name);
}

void handle_two_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements) {
    char *source_operand;
    char *destination_operand;
    char *trimmed_source_operand;
    char *trimmed_destination_operand;
    AddressMethod source_address_method;
    AddressMethod destination_address_method;
    short unsigned first_word;
    if (first_non_blank(rest) == *OPERAND_SEPARATOR) {
        printf("Input Error: Operand list in line %d of file %s starts with an illegal comma\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (last_non_blank(rest) == *OPERAND_SEPARATOR) {
        printf("Input Error: Operand list in line %d of file %s ends with an illegal comma\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (includes_consecutive(rest, *OPERAND_SEPARATOR)) {
        printf("Input Error: Operand list in line %d of file %s includes multiple consecutive commas\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    source_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    destination_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    trimmed_source_operand = trim(source_operand);
    trimmed_destination_operand = trim(destination_operand);
    free_all(2, source_operand, destination_operand);
    if (is_line_blank(trimmed_source_operand)) {
        printf("Input Error: Missing source operand in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (strpbrk(trimmed_source_operand, BLANKS)) {
        printf("Input Error: Missing comma between operands in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (is_line_blank(trimmed_destination_operand)) {
        printf("Input Error: Missing destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (strpbrk(trimmed_destination_operand, BLANKS) || !is_line_blank(rest)) {
        printf("Input Error: Extra characters after destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    source_address_method = get_address_method(trimmed_source_operand);
    destination_address_method = get_address_method(trimmed_destination_operand);
    if (!is_legal_source_method(op, source_address_method)) {
        printf("Input Error: Illegal source address method in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (!is_legal_destination_method(op, destination_address_method)) {
        printf("Input Error: Illegal destination address method in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    free_all(2, trimmed_source_operand, trimmed_destination_operand);
    first_word = build_instruction_first_word(op, source_address_method, destination_address_method);
    memory_insert_instruction(requirements, first_word, line_count, parsed_file_name);
    if (should_combine_additional_words(source_address_method, destination_address_method)) {
        requirements->ic++;
    } else requirements->ic += 2;
}

void handle_one_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements) {
    char *destination_operand = find_token(rest, BLANKS, &rest);
    AddressMethod destination_address_method;
    short unsigned first_word;

    if (first_non_blank(destination_operand) == *OPERAND_SEPARATOR ||
        last_non_blank(destination_operand) == *OPERAND_SEPARATOR) {
        printf("Input Error: Illegal comma in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (is_line_blank(destination_operand)) {
        printf("Input Error: Missing destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (exists(destination_operand, *OPERAND_SEPARATOR)) {
        printf("Input Error: Too many operands for operator \"%s\" in line %d of file %s\n",
               op.name, line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    if (!is_line_blank(rest)) {
        printf("Input Error: Extra characters after destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    destination_address_method = get_address_method(destination_operand);
    if (!is_legal_destination_method(op, destination_address_method)) {
        printf("Input Error: Illegal destination address method in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    free(destination_operand);
    first_word = build_instruction_first_word(op, NO_OPERAND, destination_address_method);
    memory_insert_instruction(requirements, first_word, line_count, parsed_file_name);
    requirements->ic++;
}

void handle_zero_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                     int *error_found, Requirements *requirements) {
    unsigned short first_word;
    if (!is_line_blank(rest)) {
        printf("Input Error: Extra characters after instruction in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    first_word = build_instruction_first_word(op, NO_OPERAND, NO_OPERAND);
    memory_insert_instruction(requirements, first_word, line_count, parsed_file_name);
}

AddressMethod get_address_method(char *operand) {
    if (operand[0] == IMMEDIATE_ADDRESS_START) return IMMEDIATE_ADDRESS;
    if (operand[0] == INDIRECT_REGISTER_ADDRESS_START) return INDIRECT_REGISTER_ADDRESS;
    if (is_register(operand)) return DIRECT_REGISTER_ADDRESS;
    return DIRECT_ADDRESS;
}

int has_comment_start(char *line, int line_count, char *parsed_file_name, int *error_found) {
    if (exists(line, COMMENT_START)) {
        printf("Input Error: Semicolon signifying a comment appears after the first character in line %d of file "
               "%s\n", line_count, parsed_file_name);
        *error_found = 1;
        return 1;
    }
    return 0;
}
