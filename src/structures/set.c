#include "../../headers/structures/set.h"
#include "../../headers/exit_codes.h"
#include "stdlib.h"
#include "stdio.h"

Set *create_set() {
    Set *set = (Set *)malloc(sizeof(Set));
    int i;
    if (set == NULL) {
        printf("Memory Error: Memory allocation failure when creating set\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    for (i = 0; i < SET_SIZE; i++) {
        set->lists[i] = create_list();
    }
    return set;
}

void free_set(Set *set) {
    int i;
    for (i = 0; i < SET_SIZE; i++) {
        free_list(set->lists[i], 0);
    }
    free(set);
}

int set_hash(LineNumber line_number) {
    return line_number % SET_SIZE;
}

int set_contains(Set *set, LineNumber line_number) {
    return list_contains_line_number(set->lists[set_hash(line_number)], line_number);
}

int set_add(Set *set, LineNumber line_number) {
    if (set_contains(set, line_number)) return 0;
    list_add_line_number(set->lists[set_hash(line_number)], line_number);
    return 1;
}

void set_print(Set *set) {
    int i;
    for (i = 0; i < SET_SIZE; i++) {
        list_print_numbers(set->lists[i]);
    }
}
