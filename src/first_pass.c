#include "../headers/first_pass.h"
#include "../headers/files.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int first_pass(char file_name[], HashTable *symbol_table) {
    char *parsed_file_name = get_parsed_file_name(file_name);
    FILE *parsed_file = get_parsed_file_read(file_name);
    char *line;
    while (!feof(parsed_file)) {
        
    }
    return 0;
}