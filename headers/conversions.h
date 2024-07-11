#ifndef MAMAN14_CONVERSIONS_H
#define MAMAN14_CONVERSIONS_H

#include "math.h"
#include "operators.h"

#define WORD_SIZE_BITS 15
#define BITS_IN_BYTE 8
#define INSTRUCTION_COUNT 16
#define UNDEFINED_OPCODE 16

#define NUM_TO_WORD(x) (((unsigned short)((x) > 0 ? (x) : (pow(2, WORD_SIZE_BITS)) - (-(x)))))

void print_binary(unsigned short num);

short unsigned build_instruction_first_word(Operator op, AddressMethod source_method, AddressMethod destination_method);

int should_combine_additional_words(AddressMethod source_method, AddressMethod destination_method);

#endif
