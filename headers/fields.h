#ifndef MAMAN14_LEGAL_NAMES_H
#define MAMAN14_LEGAL_NAMES_H

#define MACRO_DEFINITION "macr"
#define MACRO_END "endmacr"

#include "structures/hash_table.h"

int legal_label_name(char *name);

int legal_macro_name(char *name);

int is_label_definition(char *field);

#endif
