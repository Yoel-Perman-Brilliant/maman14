/**
 * Includes macros and prototypes for functions that have to do with identifying, verifying and handling various fields
 * in the input, except for operators (see operators.h).
 */

#ifndef MAMAN14_LEGAL_NAMES_H
#define MAMAN14_LEGAL_NAMES_H

#define MACRO_DEFINITION "macr"
#define MACRO_END "endmacr"
#define COMMENT_START ';'
/**
 * The first field of a line (excluding the label) is considered a directive if it starts with a period.
 */
#define DIRECTIVE_START '.'
#define DATA_DIRECTIVE ".data"
#define STRING_DIRECTIVE ".string"
#define EXTERN_DIRECTIVE ".extern"
#define ENTRY_DIRECTIVE ".entry"

/**
 * Represents the location of a symbol's value (code or data).
 * Undefined is used for external symbols whose location is not yet known.
 */
typedef enum SymbolLocation {
    CODE, DATA, UNDEFINED
} SymbolLocation;

/**
 * Represents the type of a symbol - external or entry if it is given as parameter for an appropriate directive,
 * and regular otherwise.
 */
typedef enum SymbolType {
    EXTERNAL, ENTRY, REGULAR
} SymbolType;

/**
 * Represents a symbol's content - its value, location and type.
 */
typedef struct SymbolContent {
    int value;
    SymbolLocation location;
    SymbolType type;
} SymbolContent;

/**
 * Represents a macro's content, which is effectively one string.
 */
typedef char *MacroContent;

/**
 * Checks if a given field is the name of a register.
 * @param field the field to be checked
 * @return 1 if field is a register, 0 otherwise
 */
int is_register(char *field);

/**
 * Determines if a label name is legal.
 * @param name the label name to be checked
 * @return 1 if the given name is legal, 0 otherwise
 */
int legal_label_name(char *name);

/**
 * Determines if a macro name is legal.
 * @param name the macro name to be checked
 * @return 1 if the given name is legal, 0 otherwise
 */
int legal_macro_name(char *name);

/**
 * Determines if a given field is a label.
 * Should be used on the first field of the line.
 * @param field the field to be checked
 * @return 1 if the field is a label, 0 otherwise
 */
int is_label(char *field);

/**
 * Transforms a given label (including the colon) into the appropriate symbol it represents (without the colon).
 * @param label the label to be transformed
 */
void label_to_symbol(char *label);

/**
 * Checks if a given symbol represents a word in the data portion.
 * @param symbol the content of the symbol to be checked
 * @return 1 if symbol is a data symbol, 0 otherwise
 */
int is_data_symbol(SymbolContent symbol);

#endif
