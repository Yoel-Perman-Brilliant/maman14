/**
 * Includes functions that have to do with identifying, verifying and handling various fields in the input,
 * except for operators (see operators.c).
 */

#include "../headers/fields.h"
#include "../headers/util/string_ops.h"
#include "ctype.h"
#include "string.h"
#include "stdio.h"

#define MAX_MACRO_AND_LABEL_LENGTH 31
#define LABEL_END ':'

/**
 * Determines if a field name is the name of an instruction.
 * Does so by going over every instruction name and comparing it to the field name.
 * 
 * @param name the name to be checked
 * @return 1 if the field has the name of an instruction, 0 otherwise
 */
int is_instruction(char *field) {
    char *instructions[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                            "dec", "jmp", "bne", "red", "prn","jsr", "rts", "stop"};
    int i;
    for (i = 0; i < sizeof(instructions) / sizeof (char *); i++) {
        if (equal(instructions[i], field)) return 1;
    }
    return 0;
}

/**
 * Determines if a field name is the name of an directive.
 * Does so by going over every directive name and comparing it to the field name.
 * 
 * @param name the name to be checked
 * @return 1 if the field has the name of a directive, 0 otherwise
 */
int is_known_directive(char *field) {
    char *directives[] = {".data", ".string", ".entry", ".extern"};
    int i;
    for (i = 0; i < sizeof(directives) / sizeof (char *); i++) {
        if (equal(directives[i], field)) return 1;
    }
    return 0;
}

/**
 * Determines if a field name is the name of a register.
 * Does so by going over every register name and comparing it to the field name.
 * 
 * @param name the name to be checked
 * @return 1 if the field has the name of a register, 0 otherwise
 */
int is_register(char *field) {
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i;
    for (i = 0; i < sizeof(registers) / sizeof (char *); i++) {
        if (equal(registers[i], field)) return 1;
    }
    return 0;
}

/**
 * Determines if a field name is the name of a keyword that doesn't belong to any other checked category.
 * Does so by comparing the field name to every possible other.
 * 
 * @param name the name to be checked
 * @return 1 if the field has the name of one of the checked keywords, 0 otherwise
 */
int is_other_keyword(char *field) {
    return equal(field, MACRO_DEFINITION) || equal(field, MACRO_END);
}

/**
 * Determines if a macro name is legal.
 * Does so by making sure that it isn't a keyword, that it is within the length limit, that the first character is a
 * latin letter and that the following characters are printable (not whitespaces).
 * 
 * @param name the macro name to be checked
 * @return 1 if the given name is legal, 0 otherwise
 */
int legal_macro_name(char *name) {
    int i;
    if (is_instruction(name) || is_known_directive(name) || is_register(name) || is_other_keyword(name)) return 0;
    if (strlen(name) > MAX_MACRO_AND_LABEL_LENGTH) return 0;
    if (!isalpha(name[0])) return 0;
    for (i = 0; name[i] != '\0'; i++) {
        if (isspace(name[i])) return 0;
    }
    return 1;
}

/**
 * Determines if a label name is legal.
 * Does so by making sure that it isn't a keyword, that it is within the length limit, that the first character is a
 * latin letter and that the following characters are either letters or numbers.
 * 
 * @param name the label name to be checked
 * @return 1 if the given name is legal, 0 otherwise
 */
int legal_label_name(char *name) {
    int i;
    if (is_instruction(name) || is_known_directive(name) || is_register(name) || is_other_keyword(name)) return 0;
    if (strlen(name) > MAX_MACRO_AND_LABEL_LENGTH) return 0;
    if (!isalpha(name[0])) return 0;
    for (i = 0; name[i] != '\0'; i++) {
        if (!isalnum(name[i])) return 0;
    }
    return 1;
}

/**
 * Determines if a given field is a label.
 * Does so by checking if its last character is a colon, as well as making sure that it isn't a comment by checking
 * that the first character is not a comma.
 * Should be used on the first field of the line.
 * 
 * @param field the field to be checked
 * @return 1 if the field is a label, 0 otherwise
 */
int is_label(char *field) {
    return field[strlen(field) - 1] == LABEL_END && field[0] != COMMENT_START;
}

/**
 * Transforms a given label (including the colon) into the appropriate symbol it represents (without the colon).
 * Does so by changing its last character to be 0, this ending it one character early and removing the colon.
 * 
 * @param label the label to be transformed
 */
void label_to_symbol(char *label) {
    label[strlen(label) - 1] = '\0';
}

int is_directive(char *field) {
    return first_non_blank(field) == DIRECTIVE_START;
}


/**
 * Finds the label of a line with a given pointer to it and changes the pointer's value to not include the label.
 * 
 * Does so by finding the first field of the line and setting the value of the the pointer to the label to it,
 * then setting the value of the pointer to the line to the part after the first field.
 * 
 * @param line             a pointer to the line being read
 * @param label_name       a pointer to a string whose value should be the label if there is one, or NULL if there isn't
 */
void find_label(char **line, char **label_name) {
    /* the part of the line after the label */
    char *rest;
    /* the first field of the line */
    char *first_field = find_token(*line, BLANKS, &rest);
    /* if the first field is a label */
    if (is_label(first_field)) {
        /* removes the colon from the label to find the symbol */
        label_to_symbol(first_field);
        /* sets the value of the label_name pointer to the symbol */
        *label_name = first_field;
        /* sets the value of the line pointer to the part after the label */
        *line = rest;
    }
    /* if the first field is not a label, sets the value of the label_name pointer to NULL */
    else {
        free(first_field);
        *label_name = NULL;
    }
}

/**
 * Gets the address method of a given operand.
 * 
 * Does so by checking if it starts with a pound - if it does, it must be immediate address. If it starts with an
 * asterisk, it must be indirect register address. If it's a register, then it is direct register. Otherwise, assumes
 * it's a symbol and therefore it is direct address.
 * 
 * @param operand the operand to check
 * @return the address method of operand
 */
AddressMethod get_address_method(char *operand) {
    if (operand[0] == IMMEDIATE_ADDRESS_START) return IMMEDIATE_ADDRESS;
    if (operand[0] == INDIRECT_REGISTER_ADDRESS_START) return INDIRECT_REGISTER_ADDRESS;
    if (is_register(operand)) return DIRECT_REGISTER_ADDRESS;
    return DIRECT_ADDRESS;
}

