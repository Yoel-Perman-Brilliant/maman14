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

int main() {
    /*Requirements *requirements = create_requirements();
    printf("requirements size: %lu\n", sizeof(*requirements));
    printf("table pointer size: %lu\n", sizeof(requirements->symbol_table));
    printf("in array pointer size: %lu\n", sizeof(requirements->instruction_array));
    printf("da array pointer size: %lu\n", sizeof(requirements->data_array));
    printf("ic size: %lu\n", sizeof(requirements->ic));
    printf("dc size: %lu\n", sizeof(requirements->dc));
    
    pre_assemble("bodingo");
    first_pass("bodingo", requirements);
    printf("\033[1;33mINSTRUCTIONS:\n\033[0m");
    for (i = 98; i < 200; i++) {
        printf("i: %d ", i);
        print_binary((requirements->instruction_array)[i]);
    }
    /printf("\033[1;33mDATA:\n\033[0m");
    for (i = 0; i < 20; i++) {
        printf("i: %d ", i);
        print_binary((requirements->data_array)[i]);
    }
    table_print_symbols(requirements->symbol_table);
    free_requirements(requirements);*/
    LinkedList *list = create_list();
    list_add_line_number(list, 7);
    list_add_line_number(list, 98);
    printf("7: %d\n", list_contains_line_number(list, 7));
    printf("98, %d\n", list_contains_line_number(list, 98));
    printf("101: %d\n", list_contains_line_number(list, 101));
    free_list(list);
    return 0;
}