#ifndef MAMAN14_STRING_OPS_H
#define MAMAN14_STRING_OPS_H

#include "stdlib.h"

int equal(char *s1, char *s2);

int exists(char *string, char c);

char *find_token(char *string, char *separators, char **rest);

char is_line_blank(char *string);

/**
* Removes any heading and trailing spaces or tabs from a string.
* @param string a string which should be trimmed
* @return a new trimmed version of the original string
*/
char *trim(char string[]);

int is_integer(char string[]);

int to_integer(char string[]);

char first_non_blank(char string[]);

char last_non_blank(char string[]);

char *remove_all_blanks(char string[]);

int number_of_blanks(char string[]);

int includes_consecutive(char string[], char c);

#endif
