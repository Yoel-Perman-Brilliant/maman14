/**
 * This file is include a prototype for the function responsible for the pre-assembly process.
 * The function is pre-assemble, which takes in a file name without the .as extension and parses it into a new .am file
 * with the same name. If a .am file with this name already exists, deletes it and  creates a new file. 
 * If any errors are found, the .am file is not created, but the program keeps analyzing the input file in order to find
 * more errors. Assumes that the definition of every macro comes before its usage, that there are no nested macro
 * definitions, that a macro cannot be defined if a macro with the same name has already been defined, 
 * and that a macro definition and ending cannot have labels.
 */

#ifndef MAMAN14_PRE_ASSAMBLER_H
#define MAMAN14_PRE_ASSAMBLER_H

#include "structures/hash_map.h"
#include "requirements.h"

/**
 * Reads an input file and parses all of its macros. Creates a new parsed file with the same name and a .am extension.
 * If a .am file with the same name already exists, deletes it and creates a new file.
 * If any error is found during the pre-assembling, will not create a parsed file, but will continue parsing the input
 * file and reporting errors, as long as the error does not prevent that.
 * Assumes that the definition of every macro comes before its usage, that there are no nested macro definitions, that
 * a macro cannot be defined if a macro with the same name has already been defined, and that a macro definition and
 * ending cannot have labels.
 * 
 * @param file_name   the name of the input file without the .as extension
 * @return 1 if an error was found, 0 if the file was parsed successfully
 */
int pre_assemble(char file_name[], Requirements *requirements);

#endif
