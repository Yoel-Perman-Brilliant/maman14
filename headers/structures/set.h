/**
 * Includes a set data structure, which includes unordered integers, non of which appear more than once.
 * It is based on a hash-table which consists of linked-lists whose nodes hold integer values.
 * In addition, includes prototypes for functions that allow for interacting with sets.
 * All functions assumes that the given pointer to a set is not null.
 */
#ifndef MAMAN14_SET_H
#define MAMAN14_SET_H
#include "linked_list.h"

/**
 * The number of slots in the hash-table that the set is based on.
 */
#define SET_HASH_TABLE_SIZE 101

/**
 * A set data structure, which includes unordered integers, non of which appear more than once.
 * It is based on a hash-table which consists of linked-lists whose nodes hold integer values.
 */
typedef struct  {
    LinkedList *lists[SET_HASH_TABLE_SIZE];
} Set;

/**
 * Creates a new, empty set.
 * 
 * @return a pointer to the new set
 */
Set *create_set();

/**
 * Frees a set and all of its contents from the memory.
 * 
 * @param set the set to be freed
 */
void free_set(Set *set);

/**
 * Checks if a set contains a given integer.
 * 
 * @param set a pointer to the set to be checked
 * @param num the integer to be checked
 * @return 1 if the set contains the given integer, 0 otherwise
 */
int set_contains(Set *set, int num);

/**
 * Adds an integer to a given set.
 * 
 * @param set a pointer to the set that the number should be added to
 * @param num the integer to be added
 * @return 1 if the addition was successful (the number did not already appear in the set), 0 otherwise
 */
int set_add(Set *set, int num);

void set_print(Set *set);

#endif
