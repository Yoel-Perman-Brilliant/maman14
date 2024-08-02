#include "../../headers/util/string_ops.h"
#include "../../headers/exit_codes.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

#define BLANKS " \t"

/**
 * Checks if two strings are identical.
 * Does so by checking if their strcmp value is 0.
 * 
 * @param s1 the first string
 * @param s2 the second string
 * @return 1 if the strings are identical, 0 otherwise
 */
int equal(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

/**
 * Checks if a given character appears in a given string.
 * Does so by checking if the pointer to the first occurrence of the character in the string is not null.
 * 
 * @param string the string to be checked
 * @param c      the character to be checked
 * @return 1 if c appears in string, 0 otherwise.
 */
int exists(char *string, char c) {
    return strchr(string, c) != NULL;
}

/**
 * Returns the first maximum substring of a given string that doesn't include any of the given separators.
 * For example, if the function would be called with a string parameter of "  x Hello  xx World   " and a separators
 * parameter of " x", the string "Hello" would be returned.
 * If there is no such substring, returns an empty string.
 * Returns the output as a new string that has been allocated on the heap.
 * Places the pointer to the character immediately after the token as the value of rest.
 * Does not assume that the string is not null.
 * 
 * Does so by advancing a pointer throughout the string until it points to a character that is not a separator.
 * This character marks the start of the token.
 * It then starts counting the number of characters that the token should include while advancing the pointer, and stops
 * when a the pointer points to a separator character, a terminating 0 - the character immediately after the
 * token.
 * Then, allocates memory in the size of the characters counted, plus one for the terminating 0.
 * Next, copies N characters from to the new string starting from the pointer minus N, where N is the number of
 * characters counted.
 * Finally, sets the value of the rest pointer to the pointer that now points to the part after the token.
 * 
 * @param string     the string whose first token should be found
 * @param separators a string made of the characters that the token should be split by
 * @param rest       a pointer to a string whose value will be changed to the string that starts immediately after 
 *                   the token (as a pointer to the character in the original string, not as a new string)
 * @return a new string with the first maximum substring of a given string that doesn't include any of the
 *         given separators - essentially the first token when splitting by the separators, an empty string if no
 *         such substring exists, or null if string is null
 */
char *find_token(char *string, char *separators, char **rest) {
    /* the token that should be returned */
    char *output;
    /* the size of the token (not including the termination 0) */
    int output_length = 0;
    /* checks for nul string */
    if (string == NULL) return NULL;
    /* if the string is empty, returns an empty token */
    if (*string == '\0') {
        output = malloc(1);
        if (output == NULL) {
            printf("Memory Error: Memory allocation failure when creating string token\n");
            exit(MEMORY_ALLOCATION_FAILURE);
        }
        output[0] = '\0';
        return output;
    }
    /* advances the string pointer until its value is not a separator */
    while (exists(separators, *string)) {
        string++;
        /* if the pointer's value is 0, then the string only includes separators and an empty token is returned */
        if (*string == '\0') {
            output = malloc(1);
            if (output == NULL) {
                printf("Memory Error: Memory allocation failure when creating string token\n");
                exit(MEMORY_ALLOCATION_FAILURE);
            }
            output[0] = '\0';
            return output;
        }
    }
    /* advances the string pointer and increments output_length until string
     * points to a non-separator character or the end of the string */
    while (!exists(separators, *string) && *string != '\0') {
        string++;
        output_length++;
    }
    /* allocates memory for the token based on the characters counted */
    output = (char *) malloc(output_length + 1);
    if (output == NULL) {
        printf("Memory Error: Memory allocation failure when creating string token\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    /* string now points to the character after the token, so string - output_length points to the token's first
     * character, which means the first output_length characters of this pointer represent the token, and thus
     * they are copied */
    strncpy(output, string - output_length, output_length);
    output[output_length] = '\0';
    /* changes the value of the rest pointer to string, which now points to the character after the token */
    *rest = string;
    return output;
}

/**
 * Checks if a string is made of only whitespace characters, or if it is empty.
 * Does so by checking if every character in the string is a whitespace.
 * 
 * @param string the string to be checked
 * @return 1 if string is made only of whitespace characters or it is empty, 0 otherwise
 */
char is_line_blank(char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        if (!isspace(string[i])) return 0;
    }
    return 1;
}

/**
* Removes any heading and trailing spaces or tabs from a string.
* Does so by finding the number of blank spaces at the start and creating a copy of the string that starts after
* those blank spaces, and then placing the closing '\0' after the first non-blank character from the end.
*
* @param string a string which should be trimmed
* @return a new trimmed version of the original string
*/
char *trim(char string[]) {
    char *output;
    /* the number of blank spaces at the start of the string */
    int head_length = strspn(string, BLANKS);
    /* index of character to check in the string */
    int i;
    /* the output string, length is at most the length of the input without the blank spaces at the start */
    output = (char *) malloc(strlen(string) - head_length + 1);
    if (output == NULL) {
        printf("Memory Error: Memory allocation failure when creating trimmed string\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    strcpy(output, string + head_length);
    /* finds the first character (from the end) which isn't a blank space, and sets it to be the last character */
    i = strlen(output) - 1;
    while (i >= 0 && (isspace(output[i]))) {
        i--;
    }
    output[i + 1] = '\0';
    return output;
}

/**
 * Checks if a given string represents an integer (that may start with one + or -).
 * Assumes the string is not empty.
 * D
 * 
 * @param string the string to be checked - not empty
 * @return 1 if string represents an integer, 0 otherwise
 */
int is_integer(char string[]) {
    int i = 1;
    if (!(isdigit(string[0]) || (string[0] == '+' && string[1] != '\0') || (string[0] == '-' && string[1] != '\0'))) {
        return 0;
    } 
    while (string[i] != '\0') {
        if (!isdigit(string[i])) return 0;
        i++;
    }
    return 1;
}

char first_non_blank(char string[]) {
    int i;
    for (i = 0; string[i] != '\0';) {
        if (!isspace(string[i])) return string[i];
        i++;
    }
    return 0;
}

char last_non_blank(char string[]) {
    int i;
    for (i = strlen(string) - 1; i >= 0;) {
        if (!isspace(string[i])) {
            return string[i];
        }
        i--;
    }
    return 0;
}

/**
 * Finds the number of blank spaces (spaces or tabs) in a given string.
 * 
 * @param string the string whose number of blank spaces should be found
 * @return the number of blank spaces in the string 
 */
int number_of_blanks(char string[]) {
    int i;
    int length = strlen(string);
    int amount = 0;
    for (i = 0; i < length; i++) {
        if (isspace(string[i])) amount++;
    }
    return amount;
}

/**
 * Returns a string that is the same as a given string, without any blank spaces. Does so by creating a new character
 * array and copying to it each character from the given string that is not a whitespace.
 * 
 * @param string a string whose spaces should be removed
 * @return the given string without blank spaces
 */
char *remove_all_blanks(char string[]) {
    /* size of the input string */
    int input_length;
    /* size of the input string without any blank spaces */
    int output_length;
    char *output;
    /* index of the current character in the input string */
    int output_index;
    /* index of the current character in the output string */
    int input_index;
    input_length = strlen(string);
    output_length = input_length - number_of_blanks(string);
    output = (char *) malloc(output_length + 1);
    if (output == NULL) {
        printf("Memory Error: Memory allocation failure when creating string without whitespaces\n");
        exit(MEMORY_ALLOCATION_FAILURE);
    }
    /* goes over every character in the input string, and if it is not a blank space, enters it into the anext available
     * slot in the output string */
    output_index = 0;
    for (input_index = 0; input_index <= input_length; input_index++) {
        if (!isspace(string[input_index])) {
            *(output + output_index) = string[input_index];
            output_index++;
        }
    }
    return output;
}

int includes_consecutive(char string[], char c) {
    char *app1 = strchr(string, c);
    char *app2;
    if (app1 == NULL) return 0;
    app2 = strchr(app1 + 1, c);
    while (app2 != NULL) {
        char *p;
        for (p = app1 + 1; p < app2; p++) {
            if (!isspace(*p)) {
                break;
            }
        }
        if (p == app2) return 1;
        app1 = app2;
        app2 = strchr(app2 + 1, c);
    }
    return 0;
}

unsigned binary_string_to_number(char string[]) {
    unsigned num = 0;
    int i;
    for (i = 0; string[i] != 0; i++) {
        num <<= 1;
        if (string[i] == '1') num |= 1;
    }
    return num;
}

