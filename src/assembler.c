#include "stdio.h"
#include "../headers/files.h"
#include "../headers/pre_assembler.h"
#include "../headers/first_pass.h"
#include "../headers/second_pass.h"
#include "../headers/output_creator.h"
#include "../headers/exit_codes.h"

int assemble(char file_name[]) {
    Requirements *requirements = create_requirements();
    int failure;
    remove_output_files(file_name);
    
    failure = pre_assemble(file_name);
    if (!failure) printf("%s: Pre-assembly completed successfully\n", file_name);
    else return 1;
    
    failure = first_pass(file_name, requirements);
    if (!failure) printf("%s: First pass completed successfully\n", file_name);
    failure |= second_pass(file_name, requirements);
    if (!failure) printf("%s: Second pass completed successfully\n", file_name);
    else return 1;
    
    failure = create_files(file_name, requirements);
    if (!failure) printf("%s: Output files creation completed successfully\n", file_name);
    else return 1;
    
    free_requirements(requirements);
    return failure;
}

int main(int argc, char **argv) {
    int failure;
    int i;
    if (argc == 1) {
        printf("No file names given to assembler\n");
        return NO_FILES_GIVEN;
    }
    for (i = 1; i < argc; i++) {
        failure |= assemble(argv[i]);
    }
    if (failure) return ASSEMBLY_FAILURE;
    return SUCCESS;
}
