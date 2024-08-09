/**
 * This file acts as an allocation failure handler, and includes a flag that indicates whether an allocation failure has
 * occurred, as well as functions that allow for interacting with the flag.
 */

/**
 * A global flag which indicates whether an allocation failure has occurred - starts off as 0 and
 * changes to 1 if necessary.
 */
static unsigned allocation_failure = 0;

/**
 * Notifies the handler that a memory allocation failure has occurred.
 * Does so by setting the allocation_failure flag on.
 */
void set_alloc_failure() {
    allocation_failure = 1;
}

/**
 * Returns whether a memory allocation failure has occurred. 
 * Does so by getting the value of the allocation_failure flag.
 * 
 * @return 1 if an allocation failure has occurred, 0 otherwise
 */
unsigned is_alloc_failure() {
    return allocation_failure;
}