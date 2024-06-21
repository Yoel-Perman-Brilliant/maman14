#ifndef MAMAN14_GENERAL_UTIL_H
#define MAMAN14_GENERAL_UTIL_H

#include "stdarg.h"
#include "stdio.h"

#define MAX_LINE_LENGTH 80

int read_line(FILE *file, char *file_name, int line_number, char s[]);

void free_all(int num, ...);

#endif
