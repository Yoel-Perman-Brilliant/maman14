#include "../headers/conversions.h"
#include "stdio.h"
#include "../headers/fields.h"
#include "../headers/util/string_ops.h"

#define RIGHTMOST_BIT(x) (x & 1)

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
