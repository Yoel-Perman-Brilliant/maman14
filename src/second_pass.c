#include "../headers/second_pass.h"
#include "../headers/util/string_ops.h"
#include "../headers/util/general_util.h"
#include "../headers/fields.h"
#include "../headers/requirements.h"
#include "../headers/files.h"
#include "stdio.h"
#include "stdlib.h"
#include "../headers/operators.h"
#include "../headers/conversions.h"

#define OPERAND_SEPARATOR ","

void second_pass_handle_instruction(char *line, int line_count, char *parsed_file_name, int *error_found,
                                    Requirements *requirements);

void second_pass_handle_two_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements);

void second_pass_handle_one_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements);

void check_and_handle_entry(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                            Requirements *requirements);

int validate_operand(char *operand, AddressMethod address_method, int line_count, char *parsed_file_name,
                     int *error_found, Requirements *requirements);

short unsigned create_single_operand_word(char *operand, AddressMethod address_method, Requirements *requirements,
                                          int is_source);

short unsigned create_combined_operand_word(char *source_operand, char *destination_operand,
                                            AddressMethod source_method, AddressMethod destination_method);

void check_and_handle_external_symbol(char *symbol_name, Requirements *requirements);

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
    requirements->ic = IC_START;
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
            second_pass_handle_instruction(line, line_count, parsed_file_name, &error_found, requirements);
        } else check_and_handle_entry(line, label, line_count, parsed_file_name, &error_found, requirements);
    }
    fclose(parsed_file);
    free(parsed_file_name);
    return error_found;
}

/**
 * Checks if a given directive line is a .entry directive, and if so, handles it.
 * Does so by finding the directive itself (the first field of the line) and checking if it is .entry.
 * If it is, finds the argument (and makes sure there is exactly one), makes sure it is a defined, non-external symbol,
 * and if it is, finds it in the symbol table and changes its type to ENTRY.
 * 
 * @param line             the line being read (excluding the label, if there is one)
 * @param label_name       the name of the line's label, or null if there isn't one
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
void check_and_handle_entry(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                            Requirements *requirements) {
    /* the part of the line after .entry */
    char *rest;
    char *directive = find_token(line, BLANKS, &rest);
    /* makes sure the directive is .entry */
    if (equal(directive, ENTRY_DIRECTIVE)) {
        /* the name of the symbol given as argument for .extern */
        char *argument;
        /* the content of the symbol being given as an argument */
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
        /* makes sure the argument symbol is defined */
        if (!table_contains(requirements->symbol_table, argument)) {
            printf("Input Error: Symbol \"%s\" given as argument for .entry directive in line %d of file %s is "
                   "undefined in that file\n", argument, line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        /* finds the reference to the symbol in the symbol table */
        symbol = table_get_symbol(requirements->symbol_table, argument);
        /* makes sure the symbol is not external */
        if (symbol->type == EXTERNAL) {
            printf("Input Error: Symbol \"%s\" given as argument for .entry directive in line %d of file %s is"
                   "already defined in that file as external\n", argument, line_count, parsed_file_name);
            *error_found = 1;
            return;
        }
        /* @TODO check for defining something as .entry twice */
        /* changes the symbol's type to ENTRY */
        symbol->type = ENTRY;
    }
}

void second_pass_handle_instruction(char *line, int line_count, char *parsed_file_name, int *error_found,
                                    Requirements *requirements) {
    char *rest;
    char *operator_name;
    Operator op;
    if (set_contains(requirements->faulty_instructions, line_count)) return;
    operator_name = find_token(line, BLANKS, &rest);
    op = get_operator(operator_name);
    free(operator_name);
    requirements->ic++;
    if (has_source(op)) {
        second_pass_handle_two_operand_instruction(rest, line_count, parsed_file_name, error_found, requirements);
    } else if (has_destination(op)) {
        second_pass_handle_one_operand_instruction(rest, line_count, parsed_file_name, error_found, requirements);
    }
}

void second_pass_handle_two_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements) {
    char *source_operand;
    char *trimmed_source_operand;
    char *destination_operand;
    char *trimmed_destination_operand;
    AddressMethod source_method;
    AddressMethod destination_method;
    source_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    destination_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    trimmed_source_operand = trim(source_operand);
    trimmed_destination_operand = trim(destination_operand);
    free_all(2, source_operand, destination_operand);
    source_method = get_address_method(trimmed_source_operand);
    destination_method = get_address_method(trimmed_destination_operand);
    if (!validate_operand(trimmed_source_operand, source_method, line_count, parsed_file_name,
                          error_found, requirements)
        || !validate_operand(trimmed_destination_operand, destination_method, line_count,
                             parsed_file_name, error_found, requirements)) {
        return;
    }
    if (should_combine_additional_words(source_method, destination_method)) {
        short unsigned word = create_combined_operand_word(trimmed_source_operand,
                                                           trimmed_destination_operand,
                                                           source_method, destination_method);
        memory_insert_instruction(requirements, word, line_count, parsed_file_name);
    } else {
        short unsigned source_word = create_single_operand_word(trimmed_source_operand, source_method,
                                                                requirements, 1);
        short unsigned destination_word = create_single_operand_word(trimmed_destination_operand, destination_method,
                                                                     requirements, 0);
        if (source_method == DIRECT_ADDRESS) {
            check_and_handle_external_symbol(trimmed_source_operand, requirements);
        }
        memory_insert_instruction(requirements, source_word, line_count, parsed_file_name);
        if (destination_method == DIRECT_ADDRESS) {
            check_and_handle_external_symbol(trimmed_destination_operand, requirements);
        }
        memory_insert_instruction(requirements, destination_word, line_count, parsed_file_name);
    }
    free_all(2, trimmed_source_operand, trimmed_destination_operand);
}

void second_pass_handle_one_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements) {
    char *destination_operand = find_token(rest, BLANKS, &rest);
    short unsigned destination_word;
    AddressMethod destination_method = get_address_method(destination_operand);
    if (!validate_operand(destination_operand, destination_method, line_count, parsed_file_name, error_found,
                          requirements)) {
        return;
    }
    if (destination_method == DIRECT_ADDRESS) {
        check_and_handle_external_symbol(destination_operand, requirements);
    }
    destination_word = create_single_operand_word(destination_operand,
                                                  destination_method, requirements, 0);
    memory_insert_instruction(requirements, destination_word, line_count, parsed_file_name);
    free(destination_operand);
}

int validate_immediate_address_operand(char *operand, int line_count, char *parsed_file_name, int *error_found) {
    int value;
    if (!is_integer(operand + 1)) {
        printf("Input Error: In operand \"%s\" given in the immediate address method in line %d of file %s, "
               "%s is not an integer\n", operand, line_count, parsed_file_name, operand + 1);
        *error_found = 1;
        return 0;
    }
    value = to_integer(operand + 1);
    if (value > IMMEDIATE_VALUE_MAX || value < IMMEDIATE_VALUE_MIN) {
        printf("Input Error: In operand \"%s\" given in the immediate address method in line %d of file %s, "
               "%d is not in the allowed range\n", operand, line_count, parsed_file_name, value);
        *error_found = 1;
        return 0;
    }
    return 1;
}

int validate_direct_address_operand(char *operand, int line_count, char *parsed_file_name, int *error_found,
                                    Requirements *requirements) {
    if (!table_contains(requirements->symbol_table, operand)) {
        printf("Input Error: Operand \"%s\" given in the direct address method in line %d of file %s is not"
               " a defined symbol\n", operand, line_count, parsed_file_name);
        *error_found = 1;
        return 0;
    }
    return 1;
}

int validate_indirect_register_address_operand(char *operand, int line_count, char *parsed_file_name,
                                               int *error_found) {
    if (!is_register(operand + 1)) {
        printf("Input Error: In operand \"%s\" given in the indirect register address method in line %d of file %s,"
               " %s is not valid register\n", operand, line_count, parsed_file_name, operand + 1);
        *error_found = 1;
        return 0;
    }
    return 1;
}

int validate_operand(char *operand, AddressMethod address_method, int line_count, char *parsed_file_name,
                     int *error_found, Requirements *requirements) {
    if (address_method == IMMEDIATE_ADDRESS) {
        return validate_immediate_address_operand(operand, line_count, parsed_file_name, error_found);
    }
    if (address_method == INDIRECT_REGISTER_ADDRESS) {
        return validate_indirect_register_address_operand(operand, line_count, parsed_file_name, error_found);
    }
    if (address_method == DIRECT_ADDRESS) {
        return validate_direct_address_operand(operand, line_count, parsed_file_name, error_found, requirements);
    }
    return 1;
}

char *get_reg(char *operand, AddressMethod addressMethod) {
    if (addressMethod == DIRECT_REGISTER_ADDRESS) return operand;
    return operand + 1;
}

short unsigned create_single_operand_word(char *operand, AddressMethod address_method, Requirements *requirements,
                                          int is_source) {
    if (address_method == IMMEDIATE_ADDRESS) return create_immediate_address_word(to_integer(operand + 1));
    if (address_method == DIRECT_ADDRESS) {
        SymbolContent symbol = *table_get_symbol(requirements->symbol_table, operand);
        return create_direct_address_word(symbol.value, symbol.type);
    }
    if (address_method == INDIRECT_REGISTER_ADDRESS && is_source) {
        return create_source_register_word(operand + 1);
    }
    if (address_method == INDIRECT_REGISTER_ADDRESS) {
        return create_destination_register_word(operand + 1);
    }
    if (is_source) return create_source_register_word(operand);
    return create_destination_register_word(operand);
}

short unsigned create_combined_operand_word(char *source_operand, char *destination_operand,
                                            AddressMethod source_method, AddressMethod destination_method) {
    if (source_method == INDIRECT_REGISTER_ADDRESS) source_operand++;
    if (destination_method == INDIRECT_REGISTER_ADDRESS) destination_operand++;
    return create_combined_register_word(source_operand, destination_operand);
}

void check_and_handle_external_symbol(char *symbol_name, Requirements *requirements) {
    SymbolContent *symbol_content = table_get_symbol(requirements->symbol_table, symbol_name);
    if (symbol_content->type == EXTERNAL) {
        list_add_line_number(symbol_content->appearances, requirements->ic);
        requirements->extern_found = 1;
    }
}
