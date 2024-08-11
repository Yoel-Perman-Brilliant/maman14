/**
 * Includes the prototype for second_pass, which is responsible for encoding the operands of instructions,
 * updating symbols in the symbol table that are declared as .entry, and updating the list of appearances of external
 * symbols as operands. By the end of the second pass, the requirements of the file should be completely filled with
 * the information necessary to create the output files.
 */
#ifndef MAMAN14_SECOND_PASS_H
#define MAMAN14_SECOND_PASS_H

#include "requirements.h"

/**
 * Executes the second pass over the parsed .am file, which is responsible for encoding the operands of instructions,
 * updating symbols in the symbol table that are declared as .entry, and updating the list of appearances of external
 * symbols as operands.
 * 
 * @param file_name the extensionless file name
 * @param requirements a pointer to the requirements of the file
 * @return 1 if an error has occurred, 0 otherwise
 */
int second_pass(char file_name[], Requirements *requirements);

#endif
