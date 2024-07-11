#include "../headers/operators.h"
#include "../headers/util/string_ops.h"

int is_legal_source_method(Operator op, AddressMethod method) {
    return (op.legal_source_methods >> method) & 1;
}

int is_legal_destination_method(Operator op, AddressMethod method) {
    return (op.legal_destination_methods >> method) & 1;
}

int has_source(Operator op) {
    return op.legal_source_methods;
}

int has_destination(Operator op) {
    return op.legal_destination_methods;
}

Operator create_operator(char *name, char *legal_source_methods, char *legal_destination_methods) {
    Operator op;
    op.name = name;
    op.legal_source_methods = binary_string_to_number(legal_source_methods);
    op.legal_destination_methods = binary_string_to_number(legal_destination_methods);
    return op;
}

Operator *operators() {
    static Operator operators[NUMBER_OF_OPERATORS + 1];
    operators[0] = create_operator("mov", "1111", "1110");
    operators[1] = create_operator("cmp", "1111", "1111");
    operators[2] = create_operator("add", "1111", "1110");
    operators[3] = create_operator("sub", "1111", "1110");
    operators[4] = create_operator("lea", "0010", "1110");
    operators[5] = create_operator("clr", "0000", "1110");
    operators[6] = create_operator("not", "0000", "1110");
    operators[7] = create_operator("inc", "0000", "1110");
    operators[8] = create_operator("dec", "0000", "1110");
    operators[9] = create_operator("jmp", "0000", "0110");
    operators[10] = create_operator("bne", "0000", "0110");
    operators[11] = create_operator("red", "0000", "1110");
    operators[12] = create_operator("prn", "0000", "1111");
    operators[13] = create_operator("jsr", "0000", "0110");
    operators[14] = create_operator("rts", "0000", "0000");
    operators[15] = create_operator("stop", "0000", "0000");
    /* Illegal operator */
    operators[16] = create_operator(ILLEGAL_OPERATOR_NAME, "0000", "0000");
    return operators;
}

int get_opcode(char *operator_name) {
    int i;
    for (i = 0; i < NUMBER_OF_OPERATORS; i++) {
        if (equal(operators()[i].name, operator_name)) return i;
    }
    return NUMBER_OF_OPERATORS;
}

Operator get_operator(char *operator_name) {
    int i;
    for (i = 0; i < NUMBER_OF_OPERATORS; i++) {
        if (equal(operators()[i].name, operator_name)) return operators()[i];
    }
    return operators()[NUMBER_OF_OPERATORS];
}

int is_operator(char *name) {
    return !equal((get_operator(name)).name, ILLEGAL_OPERATOR_NAME);
}
