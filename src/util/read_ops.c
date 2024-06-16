#include "../../headers/util/read_ops.h"
#include "stdio.h"

/**
 * Reads a line from a file into a given character array as long as it is at most 80 characters long.
 * @param file the file that should be read from
 * @param file_name the name of the file (used for error reporting)
 * @param line_number the number of the line being read (used for error reporting)
 * @param s an array that should contain the output
 * @return 0 if the line was read successfully, 1 otherwise
 */
int read_line(FILE *file, char *file_name, int line_number, char s[]) {
    char c = getc(file);
    int count = 0;
    int error = 0;
    while (c != EOF && c != '\n') {
        if (count == MAX_LINE_LENGTH) {
            printf("Input error: Line %d in file %s is too long!\n", line_number, file_name);
            error = 1;
        }
        if (!error) s[count] = c;
        count++;
        c = getc(file);
    }
    s[count] = '\0';
    return error;
}