#include "../../headers/util/string_ops.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

#define BLANKS " \t"

int equal(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

int exists(char *string, char chars) {
    return strchr(string, chars) != NULL;
}

char *find_token(char *string, char *separators, char **rest) {
    char *output;
    int output_length = 0;
    if (*string == '\0') return string;
    while (exists(separators, *string)) {
        string++;
        if (*string == '\0') return string;
    }
    while (!exists(separators, *string) && *string != '\0') {
        string++;
        output_length++;
    }
    output = (char *) malloc(output_length + 1);
    if (output == NULL) {
        fprintf(stderr, "Memory Error: Memory allocation failure when creating string");
        return NULL;
    }
    strncpy(output, string - output_length, output_length);
    output[output_length] = '\0';
    *rest = string;
    return output;
}

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
    strcpy(output, string + head_length);
    /* finds the first character (from the end) which isn't a blank space, and sets it to be the last character */
    i = strlen(output) - 1;
    while (i >= 0 && (isspace(output[i]))) {
        i--;
    }
    output[i + 1] = '\0';
    return output;
}

