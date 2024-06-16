#include "../headers/field_names.h"
#include "../headers/string_ops.h"
#include "ctype.h"

int is_instruction(char *name) {
    char *instructions[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
                            "dec", "jmp", "bne", "red", "prn","jsr", "rts", "stop"};
    int i;
    for (i = 0; i < sizeof(instructions) / sizeof (char *); i++) {
        if (equal(instructions[i], name)) return 1;
    }
    return 0;
}

int is_directive(char *name) {
    char *directives[] = {".data", ".string", ".entry", ".extern"};
    int i;
    for (i = 0; i < sizeof(directives) / sizeof (char *); i++) {
        if (equal(directives[i], name)) return 1;
    }
    return 0;
}

int is_register(char *name) {
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "PSW"};
    int i;
    for (i = 0; i < sizeof(registers) / sizeof (char *); i++) {
        if (equal(registers[i], name)) return 1;
    }
    return 0;
}

int legal_macro_and_label_name(char *name) {
    int i;
    if (is_instruction(name) || is_directive(name) || is_register(name)) return 0;
    if (!isalpha(name[0])) return 0;
    for (i = 0; name[i] != '\0'; i++) {
        if (!isalnum(name[i])) return 0;
    }
    return 1;
}
