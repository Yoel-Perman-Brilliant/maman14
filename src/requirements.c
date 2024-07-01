#include "../headers/requirements.h"
#include "stdlib.h"

Requirements *create_requirements() {
    Requirements *requirements = malloc(sizeof(requirements));
    requirements->symbol_table = create_table();
    requirements->data_array = calloc(MEMORY_SIZE, sizeof(short));
    requirements->instruction_array = calloc(MEMORY_SIZE, sizeof(short));
    requirements->ic = 0;
    requirements->dc = 0;
    return requirements;
}

void free_requirements(Requirements *requirements) {
    free_table(requirements->symbol_table);
    free(requirements->data_array);
    free(requirements->instruction_array);
    free(requirements);
}
