#ifndef MAMAN14_STRING_OPS_H
#define MAMAN14_STRING_OPS_H

int equal(char *s1, char *s2);

char *find_token(char *string, char *separators, char **rest);

char is_line_blank(char *string);

/**
* Removes any heading and trailing spaces or tabs from a string.
* @param string a string which should be trimmed
* @return a new trimmed version of the original string
*/
char *trim(char string[]);

#endif
