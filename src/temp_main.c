#include "stdio.h"
#include "../headers/util/string_ops.h"
#include "../headers/structures/linked_list.h"
#include "../headers/structures/hash_table.h"
#include "stdlib.h"
#include "../headers/pre_assembler.h"

int main() {
    HashTable *table = create_table();
    pre_assemble("bodingo", table);
    return 0;
}