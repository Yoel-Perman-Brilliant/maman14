/**
 * Includes macros and prototypes for functions that have to do with identifying and verifying various fields in the input.
 */

#ifndef MAMAN14_LEGAL_NAMES_H
#define MAMAN14_LEGAL_NAMES_H

#define MACRO_DEFINITION "macr"
#define MACRO_END "endmacr"
#define COMMENT_START ';'

#include "structures/hash_table.h"

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

#endif
