/**
 * Includes the Operator structure, which represents one of the 16 possible operators for an instruction.
 * Also includes an enum which represents the different possible address methods for the operands, and prototypes for 
 * functions that handle analyzing given operators and address methods.
 */
#ifndef MAMAN14_OPERATORS_H
#define MAMAN14_OPERATORS_H

#define NUMBER_OF_OPERATORS 16
#define ILLEGAL_OPERATOR_NAME "illegal"

/**
 * Represents the possible address methods, with each constant's value being as defined in the task.
 * Also includes a "no operand" address method, which is used as the address method of a non-existent operand
 * when given as a parameter to certain functions.
 */
typedef enum {
    IMMEDIATE_ADDRESS = 0, DIRECT_ADDRESS = 1, INDIRECT_REGISTER_ADDRESS = 2, DIRECT_REGISTER_ADDRESS = 3,
    NO_OPERAND = 4
} AddressMethod;

/**
 * Represents an operator, which has a name, a list of legal source address methods and a a list of legal
 * destination address methods. The opcode is defined externally as the index of the operator in the operators array.
 */
typedef struct {
    char *name;
    
    /**
     * A 4 bit value, whose nth bit for the right (starting at 0) is on if and only if the address method n is a legal
     * source address method for the operator.
     * If the operator does not have a source operand, all bits are off.
     */
    unsigned legal_source_methods : 4;
    
    /**
     * A 4 bit value, whose nth bit for the right (starting at 0) is on if and only if the address method n is a legal
     * destination address method for the operator.
     * If the operator does not have a destination operand, all bits are off.
     */
    unsigned legal_destination_methods : 4;
} Operator;

/**
 * Returns a static list of the operators, where each operator's index is its opcode.
 * @return the list described in the function summary
 */
Operator *operators();

/**
 * Checks if a given address method is legal as source address method for the given operator.
 * @param op the operator
 * @param method the source address method
 * @return 1 if method is a legal source address method for op, 0 otherwise
 */
int is_legal_source_method(Operator op, AddressMethod method);

/**
 * Checks if a given address method is legal as destination address method for the given operator.
 * @param op the operator
 * @param method the destination address method
 * @return 1 if method is a legal destination address method for op, 0 otherwise
 */
int is_legal_destination_method(Operator op, AddressMethod method);

/**
 * Checks if a given operator requires a source operand
 * @param op the operator
 * @return 1 if op requires a source operand, 0 otherwise
 */
int has_source(Operator op);

/**
 * Checks if a given operator requires a destination operand
 * @param op the operator
 * @return 1 if op requires a destination operand, 0 otherwise
 */
int has_destination(Operator op);

/**
 * Returns the opcode of an operator based on its name.
 * @param operator_name the name of the operator
 * @return the operator's opcode
 */
int get_opcode(char *operator_name);

/**
 * Returns an operator based on its name
 * @param operator_name the name of the operator
 * @return the operator
 */
Operator get_operator(char *operator_name);

/**
 * Checks if a given name is a valid operator name.
 * @param name the name to be checked
 * @return 1 if the name is the name of a legal operator, 0 otherwise
 */
int is_operator(char *name);

#endif
