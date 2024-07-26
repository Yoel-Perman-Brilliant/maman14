/**
 * Includes macros and prototypes for functions that handle conversions from fields in the assembly files to binary
 * words in the memory. All macros and functions that return a word return a short unsigned, whose leftmost bit is 
 * 0 and the rest are the bits representing the word.
 */
#ifndef MAMAN14_CONVERSIONS_H
#define MAMAN14_CONVERSIONS_H

#include "math.h"
#include "operators.h"
#include "symbols.h"

/**
 * The number of bits in a memory word.
 */
#define WORD_SIZE_BITS 15

#define BITS_IN_BYTE 8

/**
 * Opcode representing an illegal operator.
 */
#define UNDEFINED_OPCODE 16

/**
 * The unsigned value of a word in the memory representing an integer in the 2's complement method.
 * Returns it as an unsigned short whose leftmost bit is 0.
 */
#define DATA_NUM_TO_WORD(x) (((unsigned short)((x) > 0 ? (x) : (pow(2, WORD_SIZE_BITS)) - (-(x)))))

/**
 * The size in bits of a number given in the immediate address method.
 */
#define IMMEDIATE_VALUE_SIZE_BITS 12

/**
 * The maximum value that a number given in the immediate address method can have.
 */
#define IMMEDIATE_VALUE_MAX (pow(2, (IMMEDIATE_VALUE_SIZE_BITS - 1)) - 1)

/**
 * The minimum value that a number given in the immediate address method can have.
 */
#define IMMEDIATE_VALUE_MIN (-pow(2, (IMMEDIATE_VALUE_SIZE_BITS - 1)))

void print_binary(unsigned short num);

/**
 * Creates the first memory word representing an instruction.
 * 
 * @param op                 the instruction's operator
 * @param source_method      the address method for the source operand
 * @param destination_method the address method for the destination operand
 * @return the first word of the instruction's binary value, padded with a zero on the left so it can be returned
 * as an unsigned short.
 */
short unsigned build_instruction_first_word(Operator op, AddressMethod source_method, AddressMethod destination_method);

/**
 * Checks if the second and third words of an instruction should be combined to a single word.
 * Should only for two-operand instructions.
 * 
 * @param source_method      the address method for the source operand
 * @param destination_method the address method for the destination operand
 * @return 1 if the additional words should be combined, 0 otherwise
 */
int should_combine_additional_words(AddressMethod source_method, AddressMethod destination_method);

/**
 * Creates a memory word representing a source register (a source operand in either one of the register
 * address methods).
 * Assumes the operand is a legal register.
 * 
 * @param reg a string containing precisely the register that should be represented by the word
 * @return the word representing the register as a source operand
 */
short unsigned create_source_register_word(char *reg);

/**
 * Creates a memory word representing a destination register (a destination operand in either one of the register
 * address methods).
 * Assumes the operand is a legal register.
 * 
 * @param reg a string containing precisely the register that should be represented by the word
 * @return the word representing the register as a destination operand
 */
short unsigned create_destination_register_word(char *reg);

/**
 * Creates a memory word representing both a source register and a destination register (operands represented in either
 * one of the register address methods).
 * Assumes the operands are legal registers.
 * 
 * @param source_reg      a string containing precisely the source register that should be represented by the word
 * @param destination_reg a string containing precisely the destination register that should be represented by the word
 * @return the combined word representing both register as their appropriate operands
 */
short unsigned create_combined_register_word(char *source_reg, char *destination_reg);

/**
 * Creates a memory word representing an immediate value (an operand given in the immediate address method).
 * Assumes the value is legal and is no more than 12 bits long.
 * 
 * @param num the number that should be represented by the word
 * @return the word representing the number
 */
short unsigned create_immediate_address_word(short num);

/**
 * Creates a memory word representing the value of an operand given in the direct address method.
 * 
 * @param symbol_value the value of the symbol given as the operand
 * @param symbol_type the type of the symbol given as the operand (regular/entry/external)
 * @return the word representing the operand
 */
short unsigned create_direct_address_word(int symbol_value, SymbolType symbol_type);

#endif
