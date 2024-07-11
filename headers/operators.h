#ifndef MAMAN14_OPERATORS_H
#define MAMAN14_OPERATORS_H

#define NUMBER_OF_OPERATORS 16
#define ILLEGAL_OPERATOR_NAME "illegal"

typedef enum {
    IMMEDIATE_ADDRESS = 0, DIRECT_ADDRESS = 1, INDIRECT_REGISTER_ADDRESS = 2, DIRECT_REGISTER_ADDRESS = 3,
    NO_OPERAND = 4
} AddressMethod;

typedef struct {
    char *name;
    unsigned legal_source_methods : 4;
    unsigned legal_destination_methods : 4;
} Operator;

int is_legal_source_method(Operator op, AddressMethod method);

int is_legal_destination_method(Operator op, AddressMethod method);

int has_source(Operator op);

int has_destination(Operator op);

Operator *operators();

int get_opcode(char *operator_name);

Operator get_operator(char *operator_name);

int is_operator(char *name);

#endif
