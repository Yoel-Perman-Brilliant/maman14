/**
 * Handles the first pass of the assembler over the parsed, macro-less file.
 * The first pass handles the encoding of everything that never depends on the values of any symbol: That includes 
 * the .data, .string and .extern directives and the first word of every instruction.
 * In addition, the first pass checks for the legality of everything it encodes, 
 * as well as the legality of the syntax of an instruction (does not check that the content of the operands is legal,
 * but checks everything else in the instruction). It also builds the symbol table, and updates the faulty instructions
 * set - a set of line numbers that represent faulty instructions that shouldn't be encoded in the second pass.
 * The main function in this file is first_pass, which executes the assembler's first pass over a given file.
 * Assumes that the input .as file has already been parsed to a macro-less .am file.
 * Also assumes that a symbol can be defined as .extern more than once, since it doesn't not interfere with any
 * part of the assembly process.
 */

#include "../headers/first_pass.h"
#include "../headers/files.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "../headers/conversions.h"
#include "../headers/util/string_ops.h"
#include "../headers/util/general_util.h"

/** PROTOTYPES FOR FUNCTIONS DEFINED LATER IN THE FILE **/
/** FOR DOCUMENTATION, SEE DEFINITIONS **/

static void insert_data_numbers(char *rest, char *parsed_file_name, int line_count, Requirements *requirements,
                         int *error_found);

static void insert_symbol(char *symbol, SymbolType type, SymbolLocation location, Requirements *requirements,
                   int *error_found, int line_count, char *parsed_file_name);

static int check_and_handle_directive(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                               Requirements *requirements);

static void handle_extern(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                   Requirements *requirements);

static void insert_string(char *rest, int line_count, char *parsed_file_name, int *error_found, Requirements *requirements);

static void first_pass_handle_instruction(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                        Requirements *requirements);

static void first_pass_handle_two_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements);

static void first_pass_handle_one_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements);

static void handle_zero_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                     int *error_found, Requirements *requirements);

static int has_comment_start(char *line, int line_count, char *parsed_file_name, int *error_found);

static int blank_after_label(char *line, int line_count, char *parsed_file_name, int *error_found);

static int is_data_symbol(SymbolContent symbol);


/**
 * Executes the first pass of the assembler over a parsed, macro-less file.
 * Handles the encoding of everything that never depends on the values of any symbol: That includes 
 * the .data, .string and .extern directives and the first word of every instruction.
 * In addition, the first pass checks for the legality of everything it encodes, 
 * as well as the legality of the syntax of an instruction (does not check that the content of the operands is legal,
 * but checks everything else in the instruction).
 * Assumes that the input .as file has already been parsed to a macro-less .am file.
 * 
 * The function reads the file's lines one by one.
 * For each one, skips it if it's blank or a comment and throws an error if it a comment starts mid-line.
 * It then finds the label, and makes sure the part after the label is not empty. Then it checks if the line is a
 * directive, and if so, handles it and inserts its label into the symbol table if necessary.
 * Otherwise, handles it as an instruction and inserts its label into the symbol table if necessary.
 * After reading the entire file, increases the value of every data symbol by IC.
 * 
 * @param file_name    the extension-less file name
 * @param requirements a pointer to the requirements for the assembly of the file
 * @return 1 if any error in the file was found, 0 otherwise
 */
int first_pass(char file_name[], Requirements *requirements) {
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
    /* for each line */
    while (!feof(parsed_file)) {
        /* a pointer version of line_read that can have a pointer reference it */
        char *line;
        line_count++;
        read_line(parsed_file, parsed_file_name, line_count, line_read);
        line = line_read;
        /* if the line is blank, skips to the next line */
        if (is_line_blank(line)) continue;
        /* if the line is a comment, skips to the next line */
        if (line[0] == COMMENT_START) continue;
        /* if the line has a comment starting mid-line, skips to the next line and throws an error */
        if (has_comment_start(line, line_count, parsed_file_name, &error_found)) continue;
        /* default value in case there is no label */
        label = NULL;
        /* finds the label and changes line to be the part after the label */
        find_label(&line, &label);
        /* makes sure that the line is not a blank line with a label */
        if (label != NULL && blank_after_label(line, line_count, parsed_file_name, &error_found)) {
            free(label);
            continue;
        }
        /* checks if the line is a directive and handles it if it is */
        if (check_and_handle_directive(line, label, line_count, parsed_file_name, &error_found,
                                       requirements)) {
            continue;
        }
        /* otherwise the line must be an instruction (if it is valid) */
        first_pass_handle_instruction(line, label, line_count, parsed_file_name, &error_found, requirements);
    }
    /* increases the value of every data symbol by IC */
    map_add_to_all_that_apply(requirements->symbol_table, requirements->ic, is_data_symbol);
    fclose(parsed_file);
    free(parsed_file_name);
    /* if no error was found, error_found has not been changed, therefore returns 0. otherwise its value is 1 */
    return error_found;
}

/**
 * Reads the data values from a .data directive and inserts them into the memory image while finding errors.
 * 
 * Does so by splitting the string based on commas to find the arguments, trimming each one to remove whitespaces at the
 * start and the end, finding the integer value of each one, verifying it, and inserting it to the memory.
 * 
 * @param rest             the part of the line that should contain only the values to insert (the part after .data)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param requirements     a pointer to the requirements for the file
 * @param error_found      a pointer to a value that represents whether an error has been found
 */
static void insert_data_numbers(char *rest, char *parsed_file_name, int line_count,
                         Requirements *requirements, int *error_found) {
    /* the current argument for the directive */
    char *arg = NULL;
    /* the trimmed version of the argument (without whitespaces at the start and the end */
    char *trimmed_arg;
    /* verifies that the argument list is not empty */
    if (is_line_blank(rest)) {
        printf("Input Error: .data directive in line %d of file %s has no arguments\n", line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* verifies that the argument list does not start with a comma */
    if (first_non_blank(rest) == *DATA_SEPARATOR) {
        printf("Input Error: .data directive in line %d of file %s starts with an illegal comma\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* verifies that the argument list does not end with a comma */
    if (last_non_blank(rest) == *DATA_SEPARATOR) {
        printf("Input Error: .data directive in line %d of file %s ends with an illegal comma\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* verifies that the argument list does not include multiple consecutive commas, including ones with whitespaces
     * between them */
    if (includes_consecutive(rest, *DATA_SEPARATOR)) {
        printf("Input Error: .data directive in line %d of file %s includes multiple consecutive commas\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* finds and times the argument */
    arg = find_token(rest, DATA_SEPARATOR, &rest);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (arg == NULL) {
        *error_found = 1;
        return;
    }
    trimmed_arg = trim(arg);
    free(arg);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (trimmed_arg == NULL) {
        *error_found = 1;
        return;
    }
    /* for each argument (until the argument is not empty) */
    while (!is_line_blank(trimmed_arg)) {
        /* the int value of the argument */
        int value;
        /* if the argument includes whitespaces (which are necessarily not the start or the end), it must be made of
         * two arguments without a comma between them */
        if (strpbrk(trimmed_arg, BLANKS)) {
            printf("Input Error: Missing comma in .data directive in line %d of file %s\n",
                   line_count, parsed_file_name);
            *error_found = 1;
            free(trimmed_arg);
            return;
        }
        /* verifies that the argument is an integer */
        if (!is_integer(trimmed_arg)) {
            printf("Input Error: argument \"%s\" of .data directive in line %d of file %s is not an integer\n",
                   trimmed_arg, line_count, parsed_file_name);
            *error_found = 1;
            free(trimmed_arg);
            return;
        }
        value = atoi(trimmed_arg);
        /* verifies that the argument's integer value is within the limits of the machine */
        if (value > MAX_WORD_SIZE || value < MIN_WORD_SIZE) {
            printf("Input Error: argument \"%s\" of .data directive in line %d of file %s is not "
                   "within the machine's memory cell bounds\n", trimmed_arg, line_count, parsed_file_name);
            *error_found = 1;
            free(trimmed_arg);
            return;
        }
        value = DATA_NUM_TO_WORD(value);
        /* inserts the data to the memory image while updating the value of error_found to 1 if an error is found
         * in the inserting process */
        *error_found |= memory_insert_data(requirements, value, line_count, parsed_file_name);
        free(trimmed_arg);
        /* the next argument */
        arg = find_token(rest, DATA_SEPARATOR, &rest);
        /* if a memory allocation failure has occurred, updates the error flag and stops */
        if (arg == NULL) {
            *error_found = 1;
            return;
        }
        trimmed_arg = trim(arg);
        free(arg);
        /* if a memory allocation failure has occurred, updates the error flag and stops */
        if (trimmed_arg == NULL) {
            *error_found = 1;
            return;
        }
    }
    free(trimmed_arg);
}

/**
 * Reads the string from a .string directive and inserts its characters into the memory image while finding errors.
 * Assumes that the string is defined as anything that resides between the most external double quotes.
 * For example, if the directive is: .string "hell"o world", then the characters of hell"o world (including the double
 * quotes) would be inserted into the memory. 
 * Also ignores escape characters and treats each character as individual.
 *
 * Does so by verifying that the directive has an argument, that the first and last non-whitespace characters of the 
 * argument are double quotes, and for every character except for these, inserts the ascii value of the 
 * character to memory.
 * 
 * @param rest             the part of the line after .string
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void insert_string(char *rest, int line_count, char *parsed_file_name, int *error_found, Requirements *requirements) {
    /* the part of the line after .string without heading and trailing whitespaces */
    char *trimmed_rest;
    /* the length of the part after .string without heading and trailing whitespaces */
    int trimmed_rest_length;
    /* the index of the character being read in trimmed_rest */
    int i;
    /* verifies that there is an argument */
    if (is_line_blank(rest)) {
        printf("Input Error: Missing argument for .string directive in line %d of"
               "file %s\n", line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* verifies that the first non-whitespace character of the argument is double quotes */
    if (first_non_blank(rest) != STRING_START_AND_END) {
        printf("Input Error: Argument for .string directive in line %d of file %s does not start with "
               "double quotation marks\n", line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* verifies that the last non-whitespace character of the argument is double quotes */
    if (last_non_blank(rest) != STRING_START_AND_END) {
        printf("Input Error: Argument for .string directive in line %d of file %s does not end with "
               "double quotation marks\n", line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* trims the argument and finds its length */
    trimmed_rest = trim(rest);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (trimmed_rest == NULL) {
        *error_found = 1;
        return;
    }
    trimmed_rest_length = strlen(trimmed_rest);
    /* verifies that the argument is not a single set of quotation marks, which would pass the previous checks */
    if (trimmed_rest_length == 1) {
        printf("Input Error: Argument for .string directive in line %d of file %s is not wrapped by two "
               "sets of quotation marks\n", line_count, parsed_file_name);
        *error_found = 1;
        free(trimmed_rest);
        return;
    }
    /* foe every character in the argument besides the first and last (the quotation marks), inserts their
     * unsigned short ascii value to the memory, and changes the value of error_found to 1 if an error is found
     * in the process */
    for (i = 1; i < trimmed_rest_length - 1; i++) {
        unsigned short value = DATA_NUM_TO_WORD(trimmed_rest[i]);
        *error_found |= memory_insert_data(requirements, value, line_count, parsed_file_name);
    }
    *error_found |= memory_insert_data(requirements, 0, line_count, parsed_file_name);
    free(trimmed_rest);
}

/**
 * Inserts a symbol to the symbol table while finding errors.
 * Assumes that a symbol can be defined as .extern more than once, since it doesn't not interfere with any
 * part of the assembly process.
 * 
 * Does so by making sure that the symbol is not already defined, setting its value to either the instruction counter
 * or data counter if the location is known, or 0 if it's external, then adding it to the symbol table with the given
 * parameters.
 * 
 * @param symbol           the name of the symbol (without a colon)
 * @param type             the type of the symbol (regular, external or entry)
 * @param location         the location of the symbol (code or data)
 * @param requirements     a pointer to the requirements for the file
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 */
static void insert_symbol(char *symbol, SymbolType type, SymbolLocation location, Requirements *requirements,
                   int *error_found, int line_count, char *parsed_file_name) {
    /* the content of the symbol to be added */
    SymbolContent content;
    /* makes sure the symbol's name is legal */
    if (!legal_label_name(symbol)) {
        printf("Input Error: Label in line %d of file %s has an illegal name\n",
               line_count, parsed_file_name);
        *error_found = 1;
        return;
    }
    /* makes sure that the symbol is not already defined in the file, unless it's a double-definition of an external
     * symbol, which is assumed to be legal */
    if (map_contains(requirements->symbol_table, symbol)) {
        if (type == EXTERNAL && map_get_symbol(requirements->symbol_table, symbol)->type != EXTERNAL) {
            printf("Input Error: Symbol \"%s\" given as a parameter for .extern in line %d of file %s is "
                   "already defined in the file\n", symbol, line_count, parsed_file_name);
            *error_found = 1;
            free(symbol);
            return;
        } else if (type != EXTERNAL) {
            printf("Input Error: Label %s in line %d of file %s is already defined\n",
                   symbol, line_count, parsed_file_name);
            *error_found = 1;
            free(symbol);
            return;
        }
    }
    /* makes sure the symbol has not already been defined as a macro */
    if (map_contains(requirements->macro_table, symbol)) {
        if (type == EXTERNAL) {
            printf("Input Error: Symbol \"%s\" given as a parameter for .extern in line %d of file %s was "
                   "already defined as a macro\n", symbol, line_count, parsed_file_name);
        }
        else {
            printf("Input Error: Label %s in line %d of file %s was already defined as a macro\n",
                   symbol, line_count, parsed_file_name);
        }
        *error_found = 1;
        free(symbol);
        return;
    }
    /* sets the attributes of the symbol based on the given parameters */
    content.type = type;
    content.location = location;
    /* sets the value to ic, dc or 0 based on the known or unknown location in the memory image */
    if (location == CODE) content.value = requirements->ic;
    else if (location == DATA) content.value = requirements->dc;
    else content.value = 0;
    content.appearances = create_list(INTEGER);
    /* adds the symbol to the symbol table */
    map_add_symbol(requirements->symbol_table, symbol, content);
}

/**
 * Checks if the line being read is a directive, and if so, handles it while finding errors (unless it's .entry,
 * which is handled in the second pass). Also inserts the line's label (if it exists) into the symbol table if 
 * it's a .data or .string directive.
 * Considers a line whose first field starts with a period as a directive.
 * 
 * Does so by first checking that the directive itself starts with a period, then checks what directive it is.
 * If it's .data or .string, inserts the line's label (if there is one) to the symbol table, then inserts the
 * directive's data. If it's .extern, handles the directive. If it's .entry, does nothing as it is left for the second
 * pass. Otherwise it's an illegal directive.
 * 
 * @param line             the line being read, not including the label (if there is one)
 * @param label_name       the name of line's label if there is one, or NULL if there isn't
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 * @return 1 if the line is a directive, 0 otherwise
 */
static int check_and_handle_directive(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                               Requirements *requirements) {
    /* the part of the line after the directive */
    char *rest;
    /* the first field of the line (which is passed without the label) */
    char *directive = find_token(line, BLANKS, &rest);
    /* if a memory allocation failure has occurred, frees variables and stops. returns 1 in order to not proceed to
     * the instruction check */
    if (directive == NULL) {
        free(directive);
        free(label_name);
        return 1;
    }
    /* checks that it is a directive */
    if (!is_directive(directive)) {
        free(directive);
        return 0;
    }
    /* if it's .data */
    if (equal(directive, DATA_DIRECTIVE)) {
        /* inserts the label to the symbol table if there is one */
        if (label_name != NULL) {
            insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                          line_count, parsed_file_name);
        }
        insert_data_numbers(rest, parsed_file_name, line_count, requirements, error_found);
        free(directive);
        return 1;
    }
    /* if it's .string */
    else if (equal(directive, STRING_DIRECTIVE)) {
        /* inserts the label to the symbol table if there is one */
        if (label_name != NULL) {
            insert_symbol(label_name, REGULAR, DATA, requirements, error_found,
                          line_count, parsed_file_name);
        }
        insert_string(rest, line_count, parsed_file_name, error_found, requirements);
        free(directive);
        return 1;
    }
    /* if it's .extern */
    else if (equal(directive, EXTERN_DIRECTIVE)) {
        handle_extern(rest, label_name, line_count, parsed_file_name, error_found, requirements);
        free(directive);
        return 1;
    }
    /* if it's .entry */
    else if (equal(directive, ENTRY_DIRECTIVE)) {
        free(directive);
        free(label_name);
        return 1;
    }
    /* any other directive is illegal */
    else {
        printf("Input Error: Illegal directive \"%s\" in line %d of file %s\n",
               directive, line_count, parsed_file_name);
        *error_found = 1;
        free(directive);
        free(label_name);
        return 1;
    }
}

/**
 * Analyzes and handles a .extern directive while finding errors.
 * Assumes .extern may only get one parameter.
 * 
 * Does so by inserting the field after .extern to the symbol table if it's legal.
 * 
 * @param rest             the part of the line after .extern
 * @param label_name       the name of line's label if there is one, or NULL if there isn't
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void handle_extern(char *rest, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                   Requirements *requirements) {
    /* the symbol given as argument for .extern */
    char *symbol;
    /* if the line has a label, issues a warning */
    if (label_name != NULL) {
        printf("Warning: Label found before .extern directive in line %d of file %s\n",
               line_count, parsed_file_name);
    }
    /* the argument is the field directly after .extern */
    symbol = find_token(rest, BLANKS, &rest);
    /* if a memory allocation failure has occurred, updates the error flag, frees label_name and stops */
    if (symbol == NULL) {
        *error_found = 1;
        free(label_name);
        return;
    }
    /* makes sure the argument field is not empty */
    if (is_line_blank(symbol)) {
        printf("Input Error: No argument given to .extern directive in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        free(label_name);
        free(symbol);
        return;
    }
    /* makes sure the part of the line after the argument is empty */
    if (!is_line_blank(rest)) {
        printf("Input Error: Extra characters after the argument for .extern directive in line %d of file %s\n",
               line_count, parsed_file_name);
        *error_found = 1;
        free(label_name);
        free(symbol);
        return;
    }
    /* inserts the symbol to the symbol table */
    insert_symbol(symbol, EXTERNAL, UNDEFINED, requirements, error_found, line_count, parsed_file_name);
    free(label_name);
}


/**
 * Handles an instruction line while finding errors and inserting the line's label (if it exists) into the symbol table.
 * This function validates the syntax of the line and the operator and inserts the instruction's first word into 
 * the memory image. It does not, however, check the content of the operands and does not insert the additional words
 * into the memory image. These are kept for the second pass.
 * Considers "instruction" to be anything that is not a blank line, a comment line or a directive. If it's non of these
 * but also not a valid instruction (as far as the function checks), the problem would be found and reported.
 * If the instruction is legal, enters the line number to the faulty instructions set.
 * 
 * Does so by first inserting the label (if there is one) to the symbol table, finding the operator, verifying it, and
 * find the amount of operands that the operator requires. Then analyzes the rest of the line based on the number
 * of expected operators.
 * 
 * @param line             the part of the line after the label
 * @param label_name       the name of the line's label if there is one, or NULL if there isn't
 * @param line_count       the number of the line in the file that is being analyzed
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void first_pass_handle_instruction(char *line, char *label_name, int line_count, char *parsed_file_name, int *error_found,
                        Requirements *requirements) {
    /* the part of the line after the operator */
    char *rest;
    /* the name of the operator must be the first field of the line */
    char *operator_name = find_token(line, BLANKS, &rest);
    /* the instruction's operator */
    Operator op;
    /* if there is a label, inserts it to the symbol table */ 
    if (label_name != NULL) {
        insert_symbol(label_name, REGULAR, CODE, requirements,
                      error_found, line_count, parsed_file_name);
    }
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (operator_name == NULL) {
        *error_found = 1;
        return;
    }
    /* makes sure that the operator is legal */
    if (!is_operator(operator_name)) {
        printf("Input Error: Illegal instruction name \"%s\" in line %d of file %s\n",
               operator_name, line_count, parsed_file_name);
        *error_found = 1;
        set_add(requirements->faulty_instructions, line_count);
        free(operator_name);
        return;
    }
    op = get_operator(operator_name);
    /* handles the instruction based on the expected number of operands */
    if (has_source(op)) {
        first_pass_handle_two_operand_instruction(op, rest, line_count, parsed_file_name, error_found, requirements);
    } else if (has_destination(op)) {
        first_pass_handle_one_operand_instruction(op, rest, line_count, parsed_file_name, error_found, requirements);
    } else {
        handle_zero_operand_instruction(op, rest, line_count, parsed_file_name, error_found, requirements);
    }
    free(operator_name);
}

/**
 * Handles an instruction line that should have two operands while finding errors.
 * This function validates the syntax of the line and inserts the instruction's first word into 
 * the memory image. It does not, however, check the content of the operands and does not insert the additional words
 * into the memory image. These are kept for the second pass.
 * If the instruction is legal, enters the line number to the faulty instructions set.
 * 
 * Does so by verifying the syntax (makes sure there are two operands split by a single comma and any amount of
 * whitespaces), then finding the address method of each operand, building the memory word, verifying it, inserting
 * it to the memory image, and advancing the instruction counter to account for the additional words.
 * 
 * @param op               the instruction's operator
 * @param rest             the part of the line after the operator
 * @param line_count       the number of the line in the file that is being analyzed
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void first_pass_handle_two_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements) {
    char *source_operand;
    char *destination_operand;
    /* the source operand without heading or trailing whitespaces */
    char *trimmed_source_operand;
    /* the destination operand without heading or trailing whitespaces */
    char *trimmed_destination_operand;
    AddressMethod source_address_method;
    AddressMethod destination_address_method;
    /* the instruction's first word in the memory */
    short unsigned first_word;
    /* makes sure there is no comma before the first operand */
    if (first_non_blank(rest) == *OPERAND_SEPARATOR) {
        printf("Input Error: Operand list in line %d of file %s starts with an illegal comma\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        return;
    }
    /* makes sure there is no after the last operand */
    if (last_non_blank(rest) == *OPERAND_SEPARATOR) {
        printf("Input Error: Operand list in line %d of file %s ends with an illegal comma\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        return;
    }
    /* makes sure there are no multiple consecutive commas (including ones with only whitespaces between them) */
    if (includes_consecutive(rest, *OPERAND_SEPARATOR)) {
        printf("Input Error: Operand list in line %d of file %s includes multiple consecutive commas\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        return;
    }
    /* the source operand is the first field of the part after the operator when separating by commas,
     * may include whitespaces */
    source_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    /* the source operand is the first field of the part after the source operand when separating by commas,
     * may include whitespaces */
    destination_operand = find_token(rest, OPERAND_SEPARATOR, &rest);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (source_operand == NULL || destination_operand == NULL) {
        *error_found = 1;
        return;
    }
    /* removes heading and trailing whitespaces from the operands */
    trimmed_source_operand = trim(source_operand);
    trimmed_destination_operand = trim(destination_operand);
    free_all(2, source_operand, destination_operand);
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (trimmed_source_operand == NULL || trimmed_destination_operand == NULL) {
        *error_found = 1;
        return;
    }
    /* makes sure there is a source operand */
    if (is_line_blank(trimmed_source_operand)) {
        printf("Input Error: Missing source operand in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    /* if the trimmed source operand includes blank spaces, then it must be made of two operands without a 
     * comma between them */ 
    if (strpbrk(trimmed_source_operand, BLANKS)) {
        printf("Input Error: Missing comma between operands in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    /* makes sure there is a destination operand */
    if (is_line_blank(trimmed_destination_operand)) {
        printf("Input Error: Missing destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    /* if the trimmed destination operand includes blank spaces, then it must be made of two operands without a comma
     * between them, which would classify as extra characters after the second operand (the destination);
     * if the part after the field which represents the destination operand (when split by commas) is not a blank line,
     * then there are also extra characters after the destination operand */
    if (strpbrk(trimmed_destination_operand, BLANKS) || !is_line_blank(rest)) {
        printf("Input Error: Extra characters after destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    /* the address methods of the operands */
    source_address_method = get_address_method(trimmed_source_operand);
    destination_address_method = get_address_method(trimmed_destination_operand);
    /* makes sure the source operand's address method is legal */
    if (!is_legal_source_method(op, source_address_method)) {
        printf("Input Error: Illegal source address method in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    /* makes sure the destination operand's address method is legal */
    if (!is_legal_destination_method(op, destination_address_method)) {
        printf("Input Error: Illegal destination address method in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free_all(2, trimmed_source_operand, trimmed_destination_operand);
        return;
    }
    free_all(2, trimmed_source_operand, trimmed_destination_operand);
    /* build's the instruction first memory word based on the operator and address methods */
    first_word = build_instruction_first_word(op, source_address_method,
                                              destination_address_method);
    /* inserts the word into the memory while updating the value of error_found to 1 if there
     * are any errors in the process */
    *error_found |= memory_insert_instruction(requirements, first_word, line_count, parsed_file_name);
    /* advances the instruction counter based on the number of additional words */
    if (should_combine_additional_words(source_address_method, destination_address_method)) {
        requirements->ic++;
    } else requirements->ic += 2;
}

/**
 * Handles an instruction line that should have one operand while finding errors.
 * This function validates the syntax of the line and inserts the instruction's first word into 
 * the memory image. It does not, however, check the content of the operand and does not insert the additional word
 * into the memory image. These are kept for the second pass.
 * If the instruction is legal, enters the line number to the faulty instructions set.
 * 
 * Does so by verifying the syntax of the line (only one operand and no commas), then finding the operand's address
 * method, verifying it, building the instruction's first memory word, inserting it to the memory and advancing the
 * instruction counter to account for the additional word.
 * 
 * @param op               the instruction's operator
 * @param rest             the part of the line after the operator
 * @param line_count       the number of the line in the file that is being analyzed
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void first_pass_handle_one_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                    int *error_found, Requirements *requirements) {
    /* the destination operand is the next field after the operator */
    char *destination_operand = find_token(rest, BLANKS, &rest);
    /* the destination operand's address method */
    AddressMethod destination_address_method;
    /* the instruction's first word in the memory */
    short unsigned first_word;
    /* if a memory allocation failure has occurred, updates the error flag and stops */
    if (destination_operand == NULL) {
        *error_found = 1;
        return;
    }
    /* makes sure that the destination operand is not empty */
    if (is_line_blank(destination_operand)) {
        printf("Input Error: Missing destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free(destination_operand);
        return;
    }
    /* if the operand starts or ends with a comma, it is illegal */
    if (first_non_blank(destination_operand) == *OPERAND_SEPARATOR ||
        last_non_blank(destination_operand) == *OPERAND_SEPARATOR) {
        printf("Input Error: Illegal comma in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free(destination_operand);
        return;
    }
    /* if the operand includes a comma, then it is made of two operands */
    if (exists(destination_operand, *OPERAND_SEPARATOR)) {
        printf("Input Error: Too many operands for operator \"%s\" in line %d of file %s\n",
               op.name, line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free(destination_operand);
        return;
    }
    /* makes sure the part of the line after the operand is empty */
    if (!is_line_blank(rest)) {
        printf("Input Error: Extra characters after destination operand in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free(destination_operand);
        return;
    }
    destination_address_method = get_address_method(destination_operand);
    /* makes sure the operand's address method is legal based on the operator */
    if (!is_legal_destination_method(op, destination_address_method)) {
        printf("Input Error: Illegal destination address method in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        free(destination_operand);
        return;
    }
    free(destination_operand);
    /* builds the instruction's first word in the memory */
    first_word = build_instruction_first_word(op, NO_OPERAND, destination_address_method);
    /* inserts the word into the memory while updating the value of error_found to 1 if there
     * are any errors in the process */
    *error_found |= memory_insert_instruction(requirements, first_word, line_count, parsed_file_name);
    /* advances the instruction counter to account for the additional word */
    requirements->ic++;
}

/**
 * Handles an instruction line that should have no operands while finding errors.
 * This function validates the syntax of the line and inserts the instruction's first word into 
 * the memory image.
 * If the instruction is legal, enters the line number to the faulty instructions set.
 * 
 * Does so by verifying the syntax of the line (only includes the operator), then builds the instruction's first memory
 * word and inserts it to the memory.
 * 
 * @param op               the instruction's operator
 * @param rest             the part of the line after the operator
 * @param line_count       the number of the line in the file that is being analyzed
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @param requirements     a pointer to the requirements for the file
 */
static void handle_zero_operand_instruction(Operator op, char *rest, int line_count, char *parsed_file_name,
                                     int *error_found, Requirements *requirements) {
    /* the instruction's first word in the memory */
    unsigned short first_word;
    /* makes sure there are no extra characters after the operator */
    if (!is_line_blank(rest)) {
        printf("Input Error: Extra characters after instruction in line %d of file %s\n",
               line_count, parsed_file_name);
        set_add(requirements->faulty_instructions, line_count);
        *error_found = 1;
        return;
    }
    /* builds the instruction's first word in the memory */
    first_word = build_instruction_first_word(op, NO_OPERAND, NO_OPERAND);
    /* inserts the word into the memory while updating the value of error_found to 1 if there
     * are any errors in the process */
    *error_found |= memory_insert_instruction(requirements, first_word, line_count, parsed_file_name);
}

/**
 * Checks if a line includes a semicolon. Should be used when knowing that the first character is 
 * not a semicolon and therefore the line is not a comment. If a semicolon is found, throws an error.
 * 
 * @param line             the line to be checked
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @return 1 if the line includes a semicolon, 0 otherwise
 */
static int has_comment_start(char *line, int line_count, char *parsed_file_name, int *error_found) {
    /* makes sure the line (that is known to not be a comment line) doesn't have a semicolon, which would be illegal */
    if (exists(line, COMMENT_START)) {
        printf("Input Error: Semicolon signifying a comment appears after the first character in line %d of file "
               "%s\n", line_count, parsed_file_name);
        *error_found = 1;
        return 1;
    }
    return 0;
}

/**
 * Checks if a line is blank, and should only be used if a label appears before the line, which would cause a blank
 * line to have a label which is illegal. If the line is blank, throws and appropriate error.
 * 
 * @param line             the line to be checked (the part after the label)
 * @param line_count       the number of the line in the file that is being analyzed (used for error reporting)
 * @param parsed_file_name the name of the parsed file that is being read (used for error reporting)
 * @param error_found      a pointer to a value that represents whether an error has been found
 * @return 1 if the line is blank, 0 otherwise 
 */
static int blank_after_label(char *line, int line_count, char *parsed_file_name, int *error_found) {
    /* checks that the line after the label is not blank, which would be illegal */
    if (is_line_blank(line)) {
        printf("Input Error: Line %d of file %s is empty but has a label\n", line_count, parsed_file_name);
        *error_found = 1;
        return 1;
    }
    return 0;
}

/**
 * Checks if a given symbol represents a word in the data portion.
 * @param symbol the content of the symbol to be checked
 * 
 * @return 1 if symbol is a data symbol, 0 otherwise
 */
static int is_data_symbol(SymbolContent symbol) {
    return symbol.location == DATA;
}
