#include "stdio.h"
#include "../headers/util/string_ops.h"
#include "../headers/structures/linked_list.h"
#include "../headers/structures/hash_table.h"
#include "stdlib.h"
#include "../headers/requirements.h"
#include "../headers/pre_assembler.h"
#include "../headers/conversions.h"
#include "../headers/util/general_util.h"
#include "../headers/util/string_ops.h"
#include "../headers/first_pass.h"
#include "../headers/fields.h"
#include "../headers/operators.h"

void add_to_external(SymbolContent *content) {
    if (content->type == EXTERNAL) content->value += 100;
}

void print(SymbolContent *content) {
    printf("%d\n", content->value);
}

int main() {
    int i;
    Requirements *requirements = create_requirements();
    pre_assemble("bodingo");
    first_pass("bodingo", requirements);
    /*printf("\033[1;33mINSTRUCTIONS:\n\033[0m");
    for (i = 98; i < 200; i++) {
        printf("i: %d ", i);
        print_binary((requirements->instruction_array)[i]);
    }
    printf("\033[1;33mDATA:\n\033[0m");
    for (i = 0; i < 20; i++) {
        printf("i: %d ", i);
        print_binary((requirements->data_array)[i]);
    }*/
    table_print_symbols(requirements->symbol_table);
    return 0;
}