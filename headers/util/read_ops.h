#ifndef MAMAN14_READ_OPS_H
#define MAMAN14_READ_OPS_H

#include "stdio.h"

#define MAX_LINE_LENGTH 80

int read_line(FILE *file, char *file_name, int line_number, char s[]);

#endif
