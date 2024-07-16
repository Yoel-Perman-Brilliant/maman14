/**
 * Includes prototypes for functions that perform operations on strings.
 * All strings are assumed to end with a terminating 0.
 */
#ifndef MAMAN14_STRING_OPS_H
#define MAMAN14_STRING_OPS_H

#include "stdlib.h"

/**
 * Checks if two strings are identical.
 * Assumes neither string is null.
 * 
 * @param s1 the first string
 * @param s2 the second string
 * @return 1 if the strings are identical, 0 otherwise
 */
int equal(char *s1, char *s2);

/**
 * Checks if a given character appears in a given string.
 * 
 * @param string the string to be checked
 * @param c the character to be checked
 * @return 1 if c appears in string, 0 otherwise.
 */
int exists(char *string, char c);

/**
 * Returns the first maximum substring of a given string that doesn't include any of the given separators.
 * For example, if the function would be called with a string parameter of "  x Hello  xx World   " and a separators
 * parameter of " x", the string "Hello" would be returned.
 * If there is no such substring, returns an empty string.
 * Returns the output as a new string that has been allocated on the heap.
 * Places the pointer to the character immediately after the token as the value of rest.
 * 
 * @param string the string whose first token should be found
 * @param separators a string made of the characters that the token should be split by
 * @param rest a pointer to a string whose value will be changed to the string that starts immediately after the token
 *             (as a pointer to the character in the original string, not as a new string)
 * @return a new string with the first maximum substring of a given string that doesn't include any of the
 *         given separators - essentially the first token when splitting by the separators, an empty string if no
 *         such substring exists, or null if string is null
 */
char *find_token(char *string, char *separators, char **rest);

/**
 * Checks if a string is made of only whitespace characters, or if it is empty.
 * Assumes it is not null.
 * 
 * @param string the string to be checked
 * @return 1 if string is made only of whitespace characters or it is empty, 0 otherwise
 */
char is_line_blank(char *string);

/**
* Removes any heading and trailing spaces or tabs from a string.
 * 
* @param string a string which should be trimmed
* @return a new trimmed version of the original string
*/
char *trim(char string[]);

/**
 * Checks if a given string represents an integer (that may start with one + or -).
 * Assumes string is not empty and not null.
 * 
 * @param string the string to be checked - not empty
 * @return 1 if string represents an integer, 0 otherwise
 */
int is_integer(char string[]);

/**
 * Returns the integer value of a string, assumes it is known to represent an integer (that may start with one + or -).
 * 
 * @param string the string to be converted to int
 * @return the integer value of string
 */
int to_integer(char string[]);

/**
 * Returns the first non-whitespace character of a given string.
 * Assumes the string is not null.
 * 
 * @param string the string whose first non-whitespace character should be retrieved
 * @return the first non-whitespace character of string, or 0 non such character exists.
 */
char first_non_blank(char string[]);

/**
 * Returns the last non-whitespace character of a given string.
 * Assumes the string is not null.
 * 
 * @param string the string whose last non-whitespace character should be retrieved
 * @return the last non-whitespace character of string, or 0 non such character exists.
 */
char last_non_blank(char string[]);

/**
 * Returns a new string (allocated on the heap) that is identical to the given string but has no whitespaces.
 * Assumes the string is not null.
 * 
 * @param string the string whose whitespaces should be removed (in the new string)
 * @return a new string (allocated on the heap) that is identical to the given string but has no whitespaces
 */
char *remove_all_blanks(char string[]);

/**
 * Returns the number of whitespace characters in a given string.
 * Assumes it is not null.
 * 
 * @param string string to be checked
 * @return he number of whitespace characters in string
 */
int number_of_blanks(char string[]);

/**
 * Checks whether a vien string includes multiple occurrences of a given character while ignoring whitespaces.
 * For example, a call to the function with the string " hello x   x world" will return true, because the only
 * characters between the two occurrences of x are whitespaces.
 * Assumes the string is not null and the character is not a whitespace.
 * 
 * @param string the string to be checked
 * @param c the character to be checked
 * @return 1 if c appears multiple consecutive times in string while ignoring whitespaces, 0 otherwise.
 */
int includes_consecutive(char string[], char c);

/**
 * Converts a string whose characters are 0s and 1s to a non-negative number whose binary representation is depicted 
 * in the string.
 * 
 * Assumes that the string is not null and consists only of 1s and 0s.
 * @param string the string to be converted
 * @return the numeric value of the string
 */
unsigned binary_string_to_number(char string[]);

#endif
