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

void add_to_external(SymbolContent *content) {
    if (content->type == EXTERNAL) content->value += 100;
}

void print(SymbolContent *content) {
    printf("%d\n", content->value);
}

int main() {
    LinkedList *list = create_list();
    SymbolContent content1;
    content1.value = 2;
    content1.location = DATA;
    content1.type = EXTERNAL;
    SymbolContent content2;
    content2.value = 3;
    content2.location = DATA;
    content2.type = REGULAR;
    list_add_symbol(list, "hello", content1);
    list_add_symbol(list, "hg", content2);
    list_foreach_symbol(list, add_to_external);
    list_foreach_symbol(list, print);
    return 0;
}