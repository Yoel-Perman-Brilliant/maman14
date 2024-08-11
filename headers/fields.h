/**
 * Includes macros and prototypes for functions that have to do with identifying, verifying and handling various fields
 * in the input, except for operators (see operators.h).
 * All functions that get a string as a parameter assume that it is not null and ends with a terminating 0.
 */

#ifndef MAMAN14_LEGAL_NAMES_H
#define MAMAN14_LEGAL_NAMES_H

/**
 * The keyword stating the start of a macro definition.
 */
#define MACRO_DEFINITION "macr"
/**
 * The keyword stating the end of a macro definition.
 */
#define MACRO_END "endmacr"

#define COMMENT_START ';'

/**
 * The first field of a line (excluding the label) is considered a directive if it starts with a period.
 */
#define DIRECTIVE_START '.'

/** The four legal directives **/
#define DATA_DIRECTIVE ".data"
#define STRING_DIRECTIVE ".string"
#define EXTERN_DIRECTIVE ".extern"
#define ENTRY_DIRECTIVE ".entry"

/**
 * Separates between .data arguments.
 */
#define DATA_SEPARATOR ","

/**
 * Separates between operands in instructions.
 */
#define OPERAND_SEPARATOR ","

/**
 * A string consisting of whitespace characters that can appear in a line.
 */
#define BLANKS " \t"

/**
 * The start and end of .string arguments.
 */
#define STRING_START_AND_END '"'

/**
 * The maximum value that a word in the memory can hold.
 */
#define MAX_WORD_SIZE (short)(pow(2, WORD_SIZE_BITS - 1) - 1)

/**
 * The minimum value that a word in the memory can hold.
 */
#define MIN_WORD_SIZE (short)(-(pow(2, WORD_SIZE_BITS - 1)))

/**
 * The first character of an operand in the immediate address method.
 */
#define IMMEDIATE_ADDRESS_START '#'

/**
 * The first character of an operand in the indirect register address method.
 */
#define INDIRECT_REGISTER_ADDRESS_START '*'

/**
 * Represents the possible address methods for operands, with each constant's value being as defined in the task.
 * Also includes a "no operand" address method, which is used as the address method of a non-existent operand
 * when given as a parameter to certain functions.
 */
typedef enum {
    IMMEDIATE_ADDRESS = 0, DIRECT_ADDRESS = 1, INDIRECT_REGISTER_ADDRESS = 2, DIRECT_REGISTER_ADDRESS = 3,
    NO_OPERAND = 4
} AddressMethod;

/**
 * Checks if a given field is the name of a register.
 * 
 * @param field the field to be checked
 * @return 1 if field is a register, 0 otherwise
 */
int is_register(char *field);

/**
 * Determines if a label name is legal.
 * 
 * @param name the label name to be checked
 * @return 1 if the given name is legal, 0 otherwise
 */
int legal_label_name(char *name);

/**
 * Determines if a macro name is legal.
 * 
 * @param name the macro name to be checked
 * @return 1 if the given name is legal, 0 otherwise
 */
int legal_macro_name(char *name);

/**
 * Determines if a given field is a label.
 * Should be used on the first field of the line.
 * 
 * @param field the field to be checked
 * @return 1 if the field is a label, 0 otherwise
 */
int is_label(char *field);

/**
 * Transforms a given label (including the colon) into the appropriate symbol it represents (without the colon).
 * 
 * @param label the label to be transformed
 */
void label_to_symbol(char *label);

/**
 * Checks if a given field is a legal directive. 
 * 
 * @param field the field to be checked
 * @return 1 if the field is a legal directive, 0 otherwise
 */
int is_directive(char *field);

/**
 * Finds the label of a line with a given pointer to it and changes the pointer's value to not include the label.
 * 
 * @param line             a pointer to the line being read
 * @param label_name       a pointer to a string whose value should be the label if there is one, or NULL if there isn't
 */
void find_label(char **line, char **label_name);

/**
 * Finds the address method of a given operand.
 * 
 * @param operand the operand to be checked
 * @return the address method of the operand
 */
AddressMethod get_address_method(char *operand);

#endif
