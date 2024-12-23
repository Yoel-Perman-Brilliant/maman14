/**
 * Includes the prototype for the first_pass function, which executes the first pass of the assembler over the parsed,
 * macro-less file.
 * The first pass handles the encoding of everything that never depends on the values of any symbol: That includes 
 * the .data, .string and .extern directives and the first word of every instruction.
 * In addition, the first pass checks for the legality of everything it encodes, 
 * as well as the legality of the syntax of an instruction (does not check that the content of the operands is legal,
 * but checks everything else in the instruction). It also builds the symbol table for the second pass.
 * Assumes that the input .as file has already been parsed to a macro-less .am file.
 * Also assumes that a symbol can be defined as .extern more than once, since it doesn't not interfere with any
 * part of the assembly process.
 */
#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "structures/hash_map.h"
#include "requirements.h"

/**
 * Executes the first pass of the assembler over a parsed, macro-less file.
 * Handles the encoding of everything that never depends on the values of any symbol: That includes 
 * the .data, .string and .extern directives and the first word of every instruction.
 * In addition, the first pass checks for the legality of everything it encodes, 
 * as well as the legality of the syntax of an instruction (does not check that the content of the operands is legal,
 * but checks everything else in the instruction).
 * Assumes that the input .as file has already been parsed to a macro-less .am file.
 * 
 * @param file_name    the extension-less file name
 * @param requirements a pointer to the requirements for the assembly of the file
 * @return 1 if any error in the file was found, 0 otherwise
 */
int first_pass(char file_name[], Requirements *requirements);

#endif
