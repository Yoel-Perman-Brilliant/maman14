/**
 * Includes the function second_pass, which is responsible for encoding the operands of instructions,
 * updating symbols in the symbol table that are declared as .entry, and updating the list of appearances of external
 * symbols as operands. By the end of the second pass, the requirements of the file should be completely filled with
 * the information necessary to create the output files.
 * Also includes helper functions for second_pass.
 * Assumes that a symbol can be defined as .entry more than once, since it doesn't not interfere with any
 * part of the assembly process.
 */
#include "../headers/second_pass.h"
#include "../headers/util/string_ops.h"
#include "../headers/util/general_util.h"
#include "../headers/files.h"
#include "stdio.h"
#include "stdlib.h"
#include "../headers/operators.h"
#include "../headers/conversions.h"

/** PROTOTYPES FOR FUNCTIONS DEFINED LATER IN THE FILE **/
/** FOR DOCUMENTATION, SEE DEFINITIONS **/

static void second_pass_handle_instruction(char *line, int line_count, char *parsed_file_name, int *error_found,
                                    Requirements *requirements);

static void second_pass_handle_two_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements);

static void second_pass_handle_one_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements);

static void check_and_handle_entry(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                            Requirements *requirements);

static int validate_operand(char *operand, AddressMethod address_method, int line_count, char *parsed_file_name,
                     int *error_found, Requirements *requirements);

static short unsigned create_single_operand_word(char *operand, AddressMethod address_method, Requirements *requirements,
                                          int is_source);

static short unsigned create_combined_operand_word(char *source_operand, char *destination_operand,
                                            AddressMethod source_method, AddressMethod destination_method);

static void check_and_handle_external_symbol(char *symbol_name, Requirements *requirements);


/**
 * Executes the second pass over the parsed .am file, which is responsible for encoding the operands of instructions,
 * updating symbols in the symbol table that are declared as .entry, and updating the list of appearances of external
 * symbols as operands.
 * 
 * Does so by going over every line, and if it is not blank or has a comment, checks if it is a directive. If it is not,
 * then it is an instruction and it is handled. Otherwise, checks if it is a .entry directive and handles it if it is
 * (all other directives have already been handled in the first pass).
 * 
 * @param file_name the extensionless file name
 * @param requirements a pointer to the requirements of the file
 * @return 1 if an error has occurred, 0 otherwise
 */
int second_pass(char file_name[], Requirements *requirements) {
    char *parsed_file_name = get_parsed_file_name(file_name);
    FILE *parsed_file = get_parsed_file_read(file_name);
    int error_found = 0;
    /* the number of the line being read */
    int line_count = 0;
    /* the line being read */
    char line_read[MAX_LINE_LENGTH + 1];
    /* the line's label */
    char *label;
    /* makes sure the parsed file name is not null (no allocation failure occurred) */
    if (parsed_file_name == NULL) {
        return 1;
    }
    /* makes sure the parsed file is not null, if it is then the file can't be encoded */
    if (!parsed_file) {
        free(parsed_file_name);
        return 1;
    }
    /* since the instructions are gone over again, resets the instruction counter */
    requirements->ic = IC_START;
    /* for each line */
    while (!feof(parsed_file)) {
        /* a pointer version of line_read that can have a pointer reference it */
        char *line;
        line_count++;
        read_line(parsed_file, parsed_file_name, line_count, line_read);
        line = line_read;
        /* if the line is blank or a has a comment, skips to the next line.
         * Even if the comment starts mid-line, this has already been reported in the first pass, and the line
         * does not need to be encoded in the second pass */
        if (is_line_blank(line) || exists(line, COMMENT_START)) continue;
        find_label(&line, &label);
        /* checks that the part after the label is not blank */
        if (is_line_blank(line)) {
            free(label);
            continue;
        }
        /* if the line is not a directive, it's an instruction */
        if (!is_directive(line)) {
            free(label);
            second_pass_handle_instruction(line, line_count, parsed_file_name, &error_found, requirements);
            
        }
        /* otherwise, checks if the line is a .entry directive and handles it if necessary.All other directives
         * have already been handled in the first pass */
        else check_and_handle_entry(line, label, line_count, parsed_file_name, &error_found, requirements);
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
 * Assumes that a symbol can be defined as .entry more than once, since it doesn't not interfere with any
 * part of the assembly process.
 * 
 * @param line             the line being read (excluding the label, if there is one)
 * @param label_name       the name of the line's label, or null if there isn't one
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void check_and_handle_entry(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                            Requirements *requirements) {
    /* the part of the line after .entry */
    char *rest;
    char *directive = find_token(line, BLANKS, &rest);
    /* if a memory allocation failure has occurred, updates the error flag, frees the label name and stops */
    if (directive == NULL) {
        *error_found = 1;
        free(label_name);
        return;
    }
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
        /* if a memory allocation failure has occurred, updates the error flag, frees variables and stops */
        if (argument == NULL) {
            *error_found = 1;
            free_all(2, directive, label_name);
            return;
        }
        /* makes sure the argument field is not empty */
        if (is_line_blank(argument)) {
            printf("Input Error: No argument given to .entry directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            free_all(3, argument, directive, label_name);
            return;
        }
        /* makes sure the part of the line after the argument is empty */
        if (!is_line_blank(rest)) {
            printf("Input Error: Extra characters after the argument for .entry directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            free_all(3, argument, directive, label_name);
            return;
        }
        /* makes sure the argument symbol is defined */
        if (!map_contains(requirements->symbol_table, argument)) {
            printf("Input Error: Symbol \"%s\" given as argument for .entry directive in line %d of file %s is "
                   "undefined in that file\n", argument, line_count, parsed_file_name);
            *error_found = 1;
            free_all(3, argument, directive, label_name);
            return;
        }
        /* finds the reference to the symbol in the symbol table */
        symbol = map_get_symbol(requirements->symbol_table, argument);
        /* makes sure the symbol is not external */
        if (symbol->type == EXTERNAL) {
            printf("Input Error: Symbol \"%s\" given as argument for .entry directive in line %d of file %s is "
                   "already defined in that file as external\n", argument, line_count, parsed_file_name);
            *error_found = 1;
            free_all(3, directive, label_name, argument);
            return;
        }
        free(argument);
        /* changes the symbol's type to ENTRY */
        symbol->type = ENTRY;
    }
    free(directive);
    free(label_name);
}

/**
 * Verifies and encodes an instruction's operands and enters them into the memory image.
 * 
 * Does so by first making sure that the line doesn't contain any errors that were found and reported during the first
 * pass, by checking if it is in the faulty instructions set. If no such errors were found, finds the operator, and
 * based on what operator it is, handles the instruction either as a one-operator or two-operator instruction. If it is
 * a zero-operand instruction, there are no operators to encode.
 * 
 * @param line             the line being read (excluding the label, if there is one)
 * @param line_count       the number of the line in the file that is being analyzed
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void second_pass_handle_instruction(char *line, int line_count, char *parsed_file_name, int *error_found,
                                    Requirements *requirements) {
    /* the part of the line after the operator */
    char *rest;
    char *operator_name;
    Operator op;
    /* if the line was found to be faulty in the first pass, the operators can't be encoded */
    if (set_contains(requirements->faulty_instructions, line_count)) return;
    operator_name = find_token(line, BLANKS, &rest);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (operator_name == NULL) {
        *error_found = 1;
        return;
    }
    op = get_operator(operator_name);
    free(operator_name);
    /* increments the instruction count, so ic now refers to the slot in the memory where the first operand should go */ 
    requirements->ic++;
    
    /* Since the instruction was found to have no syntax errors, the number of operands in teh instruction is as it
     * should be based on the operator. */
    
    /* if the instruction has two operands */
    if (has_source(op)) {
        second_pass_handle_two_operand_instruction(rest, line_count, parsed_file_name, error_found, requirements);
    } 
    /* if the instruction has one operand */
    else if (has_destination(op)) {
        second_pass_handle_one_operand_instruction(rest, line_count, parsed_file_name, error_found, requirements);
    }
}

/**
 * Verifies and encodes the operands of a two-operand instruction and inserts them into the memory image.
 * 
 * Does so by splitting the part of the line after the operator based on commas to find the operands, finding their
 * address methods, verifying the operand's legality, deciding whether they should be combined into a single word,
 * and finally encoding them. Also, if wither of the operands is found to be an external symbol, its slot in the memory
 * is added to the symbol's appearances list.
 * 
 * 
 * @param line             the part of the line after the operator
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void second_pass_handle_two_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements) {
    /* the field that represents the source operand */
    char *source_operand;
    /* the field that represents the source operand, without heading or trailing whitespaces */
    char *trimmed_source_operand;
    /* the field that represents the destination operand */
    char *destination_operand;
    /* the field that represents the destination operand, without heading or trailing whitespaces */
    char *trimmed_destination_operand;
    
    /* the address method for the source operand */
    AddressMethod source_method;
    /* the address method for the destination operand */
    AddressMethod destination_method;
    
    source_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    destination_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (source_operand == NULL || destination_operand == NULL) {
        *error_found = 1;
        return;
    }
    trimmed_source_operand = trim(source_operand);
    trimmed_destination_operand = trim(destination_operand);
    free_all(2, source_operand, destination_operand);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (trimmed_source_operand == NULL || trimmed_destination_operand == NULL) {
        *error_found = 1;
        return;
    }
    
    source_method = get_address_method(trimmed_source_operand);
    destination_method = get_address_method(trimmed_destination_operand);
    
    /* makes sure that the operands are legal, with respect to their address methods */
    if (!validate_operand(trimmed_source_operand, source_method, line_count, parsed_file_name,
                          error_found, requirements)
        || !validate_operand(trimmed_destination_operand, destination_method, line_count,
                             parsed_file_name, error_found, requirements)) {
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    /* if the operands should be combined into a single word, creates that word and inserts it to the memory */ 
    if (should_combine_additional_words(source_method, destination_method)) {
        short unsigned word = create_combined_operand_word(trimmed_source_operand,
                                                           trimmed_destination_operand,
                                                           source_method, destination_method);
        memory_insert_instruction(requirements, word, line_count, parsed_file_name);
    } 
    /* if the operands should not be combined into a single word */
    else {
        short unsigned source_word = create_single_operand_word(trimmed_source_operand,
                                                                source_method, requirements,
                                                                1);
        short unsigned destination_word = create_single_operand_word(trimmed_destination_operand, 
                                                                     destination_method, requirements,
                                                                     0);
        /* if the source operand is a symbol, checks if it is external */
        if (source_method == DIRECT_ADDRESS) {
            check_and_handle_external_symbol(trimmed_source_operand, requirements);
        }
        memory_insert_instruction(requirements, source_word, line_count, parsed_file_name);
        
        /* if the destination operand is a symbol, checks if it is external */
        if (destination_method == DIRECT_ADDRESS) {
            check_and_handle_external_symbol(trimmed_destination_operand, requirements);
        }
        memory_insert_instruction(requirements, destination_word, line_count, parsed_file_name);
    }
    free_all(2, trimmed_source_operand, trimmed_destination_operand);
}

/**
 * Verifies and encodes the operand of a one-operand instruction and inserts it into the memory image.
 * 
 * Does so by finding the field after the operator, verifying it, checking if it is an external symbol and adding
 * the slot in the memory to its appearances list if necessary, and finally encoding the operand and adding to the
 * memory image.
 * 
 * @param line             the part of the line after the operator
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void second_pass_handle_one_operand_instruction(char *rest, int line_count, char *parsed_file_name, int *error_found,
                                                Requirements *requirements) {
    /* the only field after the operator is the destination operand */
    char *destination_operand = find_token(rest, BLANKS, &rest);
    /* the word in the memory representing the destination operand */
    short unsigned destination_word;
    
    /* the destination operand's address method */
    AddressMethod destination_method;
    
    if (destination_operand ==  NULL) {
        *error_found = 1;
        return;
    }

    destination_method = get_address_method(destination_operand);
    
    /* makes sure that the operand is legal, with respect to its address method */
    if (!validate_operand(destination_operand, destination_method, line_count, parsed_file_name, error_found,
                          requirements)) {
        free(destination_operand);
        return;
    }
    /* if the operand is a symbol, checks if it is external and handles it appropriately */
    if (destination_method == DIRECT_ADDRESS) {
        check_and_handle_external_symbol(destination_operand, requirements);
    }
    /* creates the memory word for the operands and inserts it to the memory */
    destination_word = create_single_operand_word(destination_operand,
                                                  destination_method, requirements, 0);
    memory_insert_instruction(requirements, destination_word, line_count, parsed_file_name);
    free(destination_operand);
}

/**
 * Checks if a given operand given in the immediate address method is legal.
 * 
 * Does so by checking sure that the part after the pound is an integer, and that it within the bounds for a
 * signed 12 bit integer in the 2's complement method.
 * 
 * @param operand          the operand to be checked
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @return 1 if the operand is legal, 0 otherwise
 */
static int validate_immediate_address_operand(char *operand, int line_count, char *parsed_file_name, int *error_found) {
    /* the value represented by the operand */
    int value;
    /* makes sure the part of the operand after the starting pound is an integer */
    if (!is_integer(operand + 1)) {
        printf("Input Error: In operand \"%s\" given in the immediate address method in line %d of file %s, "
               "%s is not an integer\n", operand, line_count, parsed_file_name, operand + 1);
        *error_found = 1;
        return 0;
    }
    value = atoi(operand + 1);
    /* makes sure the operand's value is within the bounds for signed 12 bit integer in the 2's complement method */
    if (value > IMMEDIATE_VALUE_MAX || value < IMMEDIATE_VALUE_MIN) {
        printf("Input Error: In operand \"%s\" given in the immediate address method in line %d of file %s, "
               "%d is not in the allowed range\n", operand, line_count, parsed_file_name, value);
        *error_found = 1;
        return 0;
    }
    return 1;
}

/**
 * Checks if a given operand given in the direct address method is legal.
 * 
 * Does so by checking if it is in fact a defined symbol.
 * 
 * @param operand          the operand to be checked
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements of the file
 * @return 1 if the operand is legal, 0 otherwise
 */
static int validate_direct_address_operand(char *operand, int line_count, char *parsed_file_name, int *error_found,
                                    Requirements *requirements) {
    /* makes sure the operand is a defined symbol */
    if (!map_contains(requirements->symbol_table, operand)) {
        printf("Input Error: Operand \"%s\" given in the direct address method in line %d of file %s is not"
               " a defined symbol\n", operand, line_count, parsed_file_name);
        *error_found = 1;
        return 0;
    }
    return 1;
}

/**
 * Checks if a given operand given in the indirect register address method is legal.
 * 
 * Does so by checking if the part after the starting asterisk is a legal register.
 * 
 * @param operand          the operand to be checked
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @return 1 if the operand is legal, 0 otherwise
 */
static int validate_indirect_register_address_operand(char *operand, int line_count, char *parsed_file_name,
                                               int *error_found) {
    if (!is_register(operand + 1)) {
        printf("Input Error: In operand \"%s\" given in the indirect register address method in line %d of file %s,"
               " %s is not valid register\n", operand, line_count, parsed_file_name, operand + 1);
        *error_found = 1;
        return 0;
    }
    return 1;
}

/**
 * Makes sure that a given operand is legal in respect to its address method.
 * 
 * @param operand          the operand to be checked
 * @param address_method   the operand's address method
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements of the file
 * @return 1 if the operand is legal, 0 otherwise
 */
static int validate_operand(char *operand, AddressMethod address_method, int line_count, char *parsed_file_name,
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
    /* an operand is considered to be in the direct register address if and only if it is one of eight defined
     * keywords (the registers), which means it necessarily legal */
    return 1;
}

/**
 * Creates a memory word representing a single operand.
 * Assumes the operand is valid.
 * 
 * Does so by building the correct type of word based on the operand's address method, as well as whether it is a source
 * or destination operand if the address method is one of the register address methods, and whether it is an external
 * symbol if the address method is direct address.
 * 
 * @param operand        the operand to be represented
 * @param address_method the operand's address method
 * @param requirements   the requirements of the file
 * @param is_source      1 if the operand is a source operand, 0 if it is a destination operand
 * @return the memory word
 */
static short unsigned create_single_operand_word(char *operand, AddressMethod address_method, Requirements *requirements,
                                          int is_source) {
    /* if the address method is immediate address, the immediate value is the part after the starting '#',
     * builds the word based on this value */
    if (address_method == IMMEDIATE_ADDRESS) {
        return create_immediate_address_word(atoi(operand + 1));
    }
    /* if the address method is direct address, gets the symbol's value and type and builds the word */
    if (address_method == DIRECT_ADDRESS) {
        SymbolContent symbol = *map_get_symbol(requirements->symbol_table, operand);
        return create_direct_address_word(symbol.value, symbol.type);
    }
    /* if the address method is indirect register address, then the register is the part after the starting '*'.
     * The word is different for source and destination operands */
    if (address_method == INDIRECT_REGISTER_ADDRESS && is_source) {
        return create_source_register_word(operand + 1);
    }
    /* the address method is indirect register address, but it is necessarily not a source operand */
    if (address_method == INDIRECT_REGISTER_ADDRESS) {
        return create_destination_register_word(operand + 1);
    }
    /* the address method is necessarily direct register address, so the register is the whole operand.
     * The word changes based on whether it is a source or destination operand */
    if (is_source) return create_source_register_word(operand);
    return create_destination_register_word(operand);
}

/**
 * Creates a memory word representing two operands in the either direct or indirect register address method.
 * Assumes both operands are valid and are given in one of the mentioned methods.
 * 
 * Does so by getting the register part of the operands based on their address methods, and creating the memory
 * word based on the registers.
 * 
 * @param source_operand      the source operand
 * @param destination_operand the destination operand
 * @param source_method       the source operand's address method
 * @param destination_method  the destination operand's address method
 * @return the memory word
 */
static short unsigned create_combined_operand_word(char *source_operand, char *destination_operand,
                                            AddressMethod source_method, AddressMethod destination_method) {
    /* if the address method for either of the operands is indirect register address, then the register is the part
     * immediately after the starting asterisk, so the operand variable changes to it.
     * Otherwise, it's the whole operand */
    if (source_method == INDIRECT_REGISTER_ADDRESS) source_operand++;
    if (destination_method == INDIRECT_REGISTER_ADDRESS) destination_operand++;
    return create_combined_register_word(source_operand, destination_operand);
}

/**
 * Checks if a symbol used aas an operand is an external symbol, and if it does, inserts the instruction counter (the
 * index of the operand's memory word) to the symbol's appearances list, and updating the requirements to know that
 * a .ext file needs to be created.
 * Assumes that the symbol name given represents an existing symbol.
 * 
 * @param symbol_name the name of the symbol to be checked
 * @param requirements a pointer to the requirements of the file
 */
static void check_and_handle_external_symbol(char *symbol_name, Requirements *requirements) {
    SymbolContent *symbol_content = map_get_symbol(requirements->symbol_table, symbol_name);
    if (symbol_content->type == EXTERNAL) {
        list_add_int(symbol_content->appearances, requirements->ic);
        requirements->extern_found = 1;
    }
}
