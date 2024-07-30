/**
 * Includes functions that allow for interacting with sets.
 */
#include "../../headers/structures/set.h"
#include "../../headers/exit_codes.h"
#include "stdlib.h"
#include "stdio.h"

/**
 * Creates a new, empty set.
 * Does so by allocating the necessary memory in the heap, and creating a new list for every slot.
 * 
 * @return a pointer to the new set
 */
Set *create_set() {
    Set *set = (Set *)malloc(sizeof(Set));
    int i;
    if (set == NULL) {
        printf("Memory Error: Memory allocation failure when creating set\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    for (i = 0; i < SET_HASH_TABLE_SIZE; i++) {
        /* sets may only include integers, so it is the type of every list */
        set->lists[i] = create_list(INTEGER);
    }
    return set;
}

/**
 * Frees a set and all of its contents from the memory.
 * Does so by freeing the list in every slot and their items' names and contents, and then 
 * freeing the pointer to the set.
 * 
 * @param set a pointer to the set that should be freed
 */
void free_set(Set *set) {
    int i;
    for (i = 0; i < SET_HASH_TABLE_SIZE; i++) {
        deep_free_list(set->lists[i]);
    }
    free(set);
}

/**
 * Generates a hash value for a given integer, that represents the index of the list that the integer
 * should be entered into.
 * Does so by taking the modulo 101 of the value. Since the distribution of the numbers that get entered into the set
 * should not favour any reminder of 101 over another, this should result in an even distribution of hash values.
 * 
 * @param num the integer that should be hashed
 * @return the hash value for the given integer
 */
int set_hash(int num) {
    return num % SET_HASH_TABLE_SIZE;
}

/**
 * Checks if a set contains a given integer.
 * Does so by checking if the list at the index of the integer's hash-value contains the given integer.
 * 
 * @param set a pointer to the set to be checked
 * @param num the integer to be checked
 * @return 1 if the set contains the given integer, 0 otherwise
 */
int set_contains(Set *set, int num) {
    return list_contains_int(set->lists[set_hash(num)], num);
}

/**
 * Adds an integer to a given set.
 * Does so by first checking if the set already contains the item, and if it doesn't, adding it to the list at the
 * index of the integer's hash-value.
 * 
 * @param set a pointer to the set that the number should be added to
 * @param num the integer to be added
 * @return 1 if the addition was successful (the number did not already appear in the set), 0 otherwise
 */
int set_add(Set *set, int num) {
    if (set_contains(set, num)) return 0;
    list_add_int(set->lists[set_hash(num)], num);
    return 1;
}

void set_print(Set *set) {
    int i;
    for (i = 0; i < SET_HASH_TABLE_SIZE; i++) {
        list_print_numbers(set->lists[i]);
    }
}
