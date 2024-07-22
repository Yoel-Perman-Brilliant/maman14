/**
 * Includes functions that handle conversions from fields in the assembly files to binary  words in the memory.
 * All functions that return a word return a short unsigned, whose leftmost bit is 0 and the
 * rest are the bits representing the word.
 */

#include "../headers/conversions.h"
#include "stdio.h"
#include "../headers/util/string_ops.h"
#include "string.h"

#define RIGHTMOST_BIT(x) (x & 1)
#define OPCODE_SHIFT 11
#define SOURCE_METHOD_SHIFT 7
#define DESTINATION_METHOD_SHIFT 3
#define SOURCE_REGISTER_NUMBER_SHIFT 6
#define DESTINATION_REGISTER_NUMBER_SHIFT 3
#define FIRST_WORD_ARE_STRING "100" 
#define REGISTER_WORD_ARE_STRING "100"
#define IMMEDIATE_VALUE_WORD_ARE_STRING "100"
#define IMMEDIATE_VALUE_NUM_SHIFT 3
#define DIRECT_ADDRESS_WORD_RELOCATABLE_ARE_STRING "010"
#define DIRECT_ADDRESS_WORD_EXTERNAL_ARE_STRING "001"
#define DIRECT_ADDRESS_WORD_VALUE_SHIFT 3

void print_binary(unsigned short num) {
    /* For every i in the range [0, INT_SIZE - 1], prints the last bit of the number without its last i bits, as in the
    ith bit of the number from the right. i starts as INT_SIZE - 1 and decreases, so the bits are printed properly
    from left to right. */
    int i;
    for (i = WORD_SIZE_BITS; i >= 0; i--) {
        printf("%d", RIGHTMOST_BIT(num >> i));
    }
    printf("\n");
}

/**
 * Returns the first memory word representing an instruction.
 * Does so by creating 4 unsigned short values, each representing a portion of the word with the rest
 * of its bits being 0, and ORing them all.
 * 
 * @param op                 the instruction's operator
 * @param source_method      the address method for the source operand
 * @param destination_method the address method for the destination operand
 * @return the first word of the instruction's binary value, padded with a zero on the left so it can be returned
 * as an unsigned short.
 */
short unsigned build_instruction_first_word(Operator op, AddressMethod source_method, 
                                            AddressMethod destination_method) {
    /* the bits representing the opcode, with the correct shift from the right */
    short unsigned opcode_bits =  get_opcode(op.name) << OPCODE_SHIFT;
    /* the bits representing the source address method, with the correct shift from the right */
    short unsigned source_method_bits;
    /* the bits representing the destination address method, with the correct shift from the right */
    short unsigned destination_method_bits;
    /* the bits representing the A,R,E (which is always the same in the first word),
     * with the correct shift from the right */
    short unsigned are = binary_string_to_number(FIRST_WORD_ARE_STRING);;
    /* if there is no source, then no bits in the source address method are on */
    if (source_method == NO_OPERAND) source_method_bits = 0;
    /* otherwise, the 1 representing the address method is shifted to the left based on which bit should be on, plus the
     * shift to the left of the first bit of the source address method portion */
    else source_method_bits = 1 << (source_method + SOURCE_METHOD_SHIFT);
    /* if there is no destination, then no bits in the destination address method are on */
    if (destination_method == NO_OPERAND) destination_method_bits = 0;
    /* otherwise, the 1 representing the address method is shifted to the left based on which bit should be on, plus the
     * shift to the left of the first bit of the destination address method portion */
    else destination_method_bits = 1 << (destination_method + DESTINATION_METHOD_SHIFT);
    /* ORs all the values (with the correct shifting already applied) */
    return opcode_bits | source_method_bits | destination_method_bits | are;
}

/**
 * Checks if the second and third words of an instruction should be combined to a single word.
 * Does so by checking if both the source address method and the destination address method are
 * either direct register address or indirect register address.
 * Should only for two-operand instructions.
 * @param source_method the address method for the source operand
 * @param destination_method the address method for the destination operand
 * @return 1 if the additional words should be combined, 0 otherwise
 */
int should_combine_additional_words(AddressMethod source_method, AddressMethod destination_method) {
    return (source_method == DIRECT_REGISTER_ADDRESS || source_method == INDIRECT_REGISTER_ADDRESS) &&
            (destination_method == DIRECT_REGISTER_ADDRESS || destination_method == INDIRECT_REGISTER_ADDRESS);
}

short unsigned create_source_register_word(char *reg) {
    int get_register_number(char *reg);
    int reg_number = get_register_number(reg);
    short unsigned source_register_bits = reg_number << SOURCE_REGISTER_NUMBER_SHIFT;
    short unsigned are = binary_string_to_number(REGISTER_WORD_ARE_STRING);
    return source_register_bits | are;
}

short unsigned create_destination_register_word(char *reg) {
    int get_register_number(char *reg);
    int reg_number = get_register_number(reg);
    short unsigned destination_register_bits = reg_number << DESTINATION_REGISTER_NUMBER_SHIFT;
    short unsigned are = binary_string_to_number(REGISTER_WORD_ARE_STRING);
    return destination_register_bits | are;
}

short unsigned create_combined_register_word(char *source_reg, char *destination_reg) {
    int get_register_number(char *reg);
    int source_reg_number = get_register_number(source_reg);
    int destination_reg_number = get_register_number(destination_reg);
    short unsigned source_register_bits = source_reg_number << SOURCE_REGISTER_NUMBER_SHIFT;
    short unsigned destination_register_bits = destination_reg_number << DESTINATION_REGISTER_NUMBER_SHIFT;
    short unsigned are = binary_string_to_number(REGISTER_WORD_ARE_STRING);
    return source_register_bits | destination_register_bits | are;
}

short unsigned create_immediate_address_word(short num) {
    short unsigned num_bits = (unsigned short)(num > 0 ? num : (pow(2, WORD_SIZE_BITS)) + num)
            << IMMEDIATE_VALUE_NUM_SHIFT;
    short unsigned are = binary_string_to_number(REGISTER_WORD_ARE_STRING);
    return num_bits | are;
}

short unsigned create_direct_address_word(int symbol_value, SymbolType symbol_type) {
    short unsigned value_bits = symbol_value << DIRECT_ADDRESS_WORD_VALUE_SHIFT;
    short unsigned are;
    if (symbol_type == EXTERNAL) {
        are = binary_string_to_number(DIRECT_ADDRESS_WORD_EXTERNAL_ARE_STRING);
    } else {
        are = binary_string_to_number(DIRECT_ADDRESS_WORD_RELOCATABLE_ARE_STRING);
    }
    return value_bits | are;
}

/* assumes reg is a valid register */
int get_register_number(char *reg) {
    return atoi(reg + 1);
}

