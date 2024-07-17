/**
 * Includes functions that handle retrieving and analyzing operators and address methods.
 */

#include "../headers/operators.h"
#include "../headers/util/string_ops.h"

/**
 * Checks if a given address method is legal as source address method for the given operator.
 * Does so by checking if the nth bit from the right of the operator's legal source address methods is 1, where n is the
 * given address method's value.
 * 
 * @param op     the operator
 * @param method the source address method
 * @return 1 if method is a legal source address method for op, 0 otherwise
 */
int is_legal_source_method(Operator op, AddressMethod method) {
    return (op.legal_source_methods >> method) & 1;
}

/**
 * Checks if a given address method is legal as destination address method for the given operator.
 * Does so by checking if the nth bit from the right of the operator's legal destination address methods is 1, where n
 * is the given address method's value.
 * 
 * @param op     the operator
 * @param method the destination address method
 * @return 1 if method is a legal destination address method for op, 0 otherwise
 */
int is_legal_destination_method(Operator op, AddressMethod method) {
    return (op.legal_destination_methods >> method) & 1;
}

/**
 * Checks if a given operator requires a source operand.
 * Does so by returning the operator's legal source address methods' bit representation, which is 0 if and only if all
 * bits are off, which means it has no legal source address methods.
 * 
 * @param op the operator
 * @return a non-zero value if op requires a source operand, 0 otherwise
 */
int has_source(Operator op) {
    return op.legal_source_methods;
}

/**
 * Checks if a given operator requires a destination operand.
 * Does so by returning the operator's legal destination address methods' bit representation, which is 0 if and only if
 * all bits are off, which means it has no legal destination address methods.
 * 
 * @param op the operator
 * @return a non-zero value if op requires a destination operand, 0 otherwise
 */
int has_destination(Operator op) {
    return op.legal_destination_methods;
}

/**
 * Creates an operator based on the given values.
 * 
 * @param name                      the name of the operator
 * @param legal_source_methods      a string of 4 characters, where the nth character from the right is '1' if address
 *                                  method n is a legal source address method for the operator and '0' otherwise.
 * @param legal_destination_methods a string of 4 characters, where the nth character from the right is '1' if address
 *                                  method n is a legal destination address method for the operator and '0' otherwise.
 * @return the newly created operator
 */
Operator create_operator(char *name, char *legal_source_methods, char *legal_destination_methods) {
    Operator op;
    op.name = name;
    op.legal_source_methods = binary_string_to_number(legal_source_methods);
    op.legal_destination_methods = binary_string_to_number(legal_destination_methods);
    return op;
}

/**
 * Returns a static list of the operators, where each operator's index is its opcode.
 * Does so by creating an operator for every index based on the task definition.
 * Also includes an additional, illegal operator which is used for default return values of functions that look for an
 * operator on the list.
 * 
 * @return the list described in the function summary
 */
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

/**
 * Returns the opcode of an operator based on its name.
 * Does so by going over every index of the operator list (every opcode), and if the operator with a certain opcode has
 * the given name, returns the opcode. If no operator with the given name was found, returns 16.
 * 
 * @param operator_name the name of the operator
 * @return the operator's opcode, or 16 if there is no operator with the given name
 */
int get_opcode(char *operator_name) {
    int i;
    for (i = 0; i < NUMBER_OF_OPERATORS; i++) {
        if (equal(operators()[i].name, operator_name)) return i;
    }
    return NUMBER_OF_OPERATORS;
}

/**
 * Returns an operator based on its name.
 * Does so by going every operator in the operator list, and if there is an operator with the given name, returns it.
 * If no operator with the given name was found, returns the illegal operator.
 * 
 * @param operator_name the name of the operator
 * @return the operator, or the illegal operator if no such operator exists
 */
Operator get_operator(char *operator_name) {
    int i;
    for (i = 0; i < NUMBER_OF_OPERATORS; i++) {
        if (equal(operators()[i].name, operator_name)) return operators()[i];
    }
    return operators()[NUMBER_OF_OPERATORS];
}

/**
 * Checks if a given name is a valid operator name.
 * Does so by checking that the operator with the given name retrieved from the operator list is not the illegal
 * operator (checks that their names are different).
 * 
 * @param name the name to be checked
 * @return 1 if the name is the name of a legal operator, 0 otherwise
 */
int is_operator(char *name) {
    return !equal((get_operator(name)).name, ILLEGAL_OPERATOR_NAME);
}
