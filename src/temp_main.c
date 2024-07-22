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
#include "../headers/second_pass.h"
#include "../headers/fields.h"
#include "../headers/operators.h"
#include "../headers/structures/set.h"

int is_external(SymbolContent symbolContent) {
    return symbolContent.type == EXTERNAL;
}

int main() {
    int i;
    Requirements *requirements = create_requirements();
    pre_assemble("bodingo");
    first_pass("bodingo", requirements);
    second_pass("bodingo", requirements);
    printf("\033[1;33mINSTRUCTIONS:\n\033[0m");
    for (i = 98; i < 150; i++) {
        printf("i: %d ", i);
        print_binary((requirements->instruction_array)[i]);
    }
    printf("\033[1;33mDATA:\n\033[0m");
    for (i = 0; i < 20; i++) {
        printf("i: %d ", i);
        print_binary((requirements->data_array)[i]);
    }
    table_print_symbols(requirements->symbol_table);
    /*printf("\033[1;33mFAULTY INSTRUCTIONS:\n\033[0m");
    set_print(requirements->faulty_instructions);*/
    /*table_add_matching_to_list(requirements->symbol_table, externals, is_external);
    free(requirements);
    list_print_symbols(externals);*/
    return 0;
}

