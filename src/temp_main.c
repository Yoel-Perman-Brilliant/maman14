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
    printf("%s", get_operator_list()[0]);
    printf("%s", get_operator_list()[2]);
    return 0;
}