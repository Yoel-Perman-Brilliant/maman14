#include "../headers/conversions.h"
#include "stdio.h"
#include "../headers/util/string_ops.h"

#define RIGHTMOST_BIT(x) (x & 1)
#define OPCODE_SHIFT 11
#define SOURCE_METHOD_SHIFT 7
#define DESTINATION_METHOD_SHIFT 3
#define FIRST_WORD_ARE_STRING "100" 


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

short unsigned build_instruction_first_word(Operator op, AddressMethod source_method, 
                                            AddressMethod destination_method) {
    short unsigned opcode_bits =  get_opcode(op.name) << OPCODE_SHIFT;
    short unsigned source_method_bits;
    short unsigned destination_method_bits;
    short unsigned are = binary_string_to_number(FIRST_WORD_ARE_STRING);;
    if (source_method == NO_OPERAND) source_method_bits = 0;
    else source_method_bits = 1 << (source_method + SOURCE_METHOD_SHIFT);
    if (destination_method == NO_OPERAND) destination_method_bits = 0;
    else destination_method_bits = 1 << (destination_method + DESTINATION_METHOD_SHIFT);
    /*print_binary(opcode_bits);
    print_binary(source_method_bits);
    print_binary(destination_method_bits);
    print_binary(are);*/
    return opcode_bits | source_method_bits | destination_method_bits | are;
}

int should_combine_additional_words(AddressMethod source_method, AddressMethod destination_method) {
    return (source_method == DIRECT_REGISTER_ADDRESS || source_method == INDIRECT_REGISTER_ADDRESS) &&
            (destination_method == DIRECT_REGISTER_ADDRESS || destination_method == INDIRECT_REGISTER_ADDRESS);
}

