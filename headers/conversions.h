/**
 * Includes macros and prototypes for functions that handle conversions from fields in the assembly files to binary
 * words in the memory. All macros and functions that return a word return a short unsigned, whose leftmost bit is 0 and the
 * rest are the bits representing the word.
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
#define IMMEDIATE_VALUE_SIZE_BITS 12
#define IMMEDIATE_VALUE_MAX (pow(2, (IMMEDIATE_VALUE_SIZE_BITS - 1)) - 1)
#define IMMEDIATE_VALUE_MIN (-pow(2, (IMMEDIATE_VALUE_SIZE_BITS - 1)))

void print_binary(unsigned short num);

/**
 * Returns the first memory word representing an instruction.
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

#endif
