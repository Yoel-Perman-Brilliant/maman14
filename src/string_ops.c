#include "../headers/string_ops.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

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
