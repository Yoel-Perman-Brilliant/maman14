#include "../headers/requirements.h"
#include "stdlib.h"

Requirements *create_requirements() {
    Requirements *requirements = malloc(sizeof(requirements));
    requirements->symbol_table = create_table();
    return requirements;
}

void free_requirements(Requirements *requirements) {
    free_table(requirements->symbol_table);
    free(requirements);
}
