/**
 * This file includes prototypes for functions that allow for knowing if an allocation failure has occurred,
 * which is necessary in order to a void memory-related errors.
 */

#ifndef MAMAN14_ALLOC_FAILURE_HANDLER_H
#define MAMAN14_ALLOC_FAILURE_HANDLER_H

/**
 * Notifies the handler that a memory allocation failure has occurred.
 */
void set_alloc_failure();

/**
 * Returns whether a memory allocation failure has occurred. 
 * @return 1 if an allocation failure has occurred, 0 otherwise
 */
unsigned is_alloc_failure();

#endif
