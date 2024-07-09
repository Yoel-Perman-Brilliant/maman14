#ifndef MAMAN14_CONVERSIONS_H
#define MAMAN14_CONVERSIONS_H

#include "math.h"

#define WORD_SIZE_BITS 15
#define BITS_IN_BYTE 8
#define INSTRUCTION_COUNT 16
#define UNDEFINED_OPCODE 16

#define NUM_TO_WORD(x) (((unsigned short)((x) > 0 ? (x) : (pow(2, WORD_SIZE_BITS)) - (-x))) \
                            << (sizeof(short) * BITS_IN_BYTE - WORD_SIZE_BITS))

void print_binary(unsigned short num);

unsigned char get_opcode(char *operator);

#endif
