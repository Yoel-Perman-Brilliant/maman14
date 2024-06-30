#ifndef MAMAN14_FILES_H
#define MAMAN14_FILES_H

#include "stdio.h"

char *get_input_file_name(char file_name[]);

char *get_parsed_file_name(char file_name[]);

FILE *get_input_file(char file_name[]);

FILE *get_parsed_file_append(char file_name[]);

FILE *get_parsed_file_read(char file_name[]);

#endif