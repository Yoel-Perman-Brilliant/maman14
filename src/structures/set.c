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
        set->lists[i] = create_list(INTEGER);
    }
    return set;
}

void free_set(Set *set) {
    int i;
    for (i = 0; i < SET_SIZE; i++) {
        deep_free_list(set->lists[i]);
    }
    free(set);
}

int set_hash(int num) {
    return num % SET_SIZE;
}

int set_contains(Set *set, int num) {
    return list_contains_int(set->lists[set_hash(num)], num);
}

int set_add(Set *set, int num) {
    if (set_contains(set, num)) return 0;
    list_add_int(set->lists[set_hash(num)], num);
    return 1;
}

void set_print(Set *set) {
    int i;
    for (i = 0; i < SET_SIZE; i++) {
        list_print_numbers(set->lists[i]);
    }
}
