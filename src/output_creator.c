/**
 * Includes the function create_files, which is responsible for creating the output files (.ob, .ext and .ent)
 * for an assembly file based on its filled requirements, as well as helper functions for create_files.
 */

#include "../headers/requirements.h"
#include "../headers/files.h"
#include "../headers/output_creator.h"

/** PROTOTYPES FOR FUNCTIONS DEFINED LATER IN THE FILE **/
/** FOR DOCUMENTATION, SEE DEFINITIONS **/

static int is_extern(SymbolContent symbol);

static int is_entry(SymbolContent symbol);

static int write_object_file(char file_name[], Requirements *requirements);

static int write_extern_file(char file_name[], LinkedList *extern_list);

static int write_entry_file(char file_name[], LinkedList *entry_list);

/**
 * Creates the output files for an assembly file based on its filled requirements.
 * Will only create .ext and .ent files if they will not be empty.
 * 
 * Does so by first creating the object file based on the requirements.
 * Next, fills two linked-lists with symbols - one with the external symbols and one with the entry symbols.
 * If the extern_found field in the requirements is on, then an extern symbol is used, so a .ext file is created.
 * If the entry list is not empty, then a symbol is defined as entry, and therefore a .ent file is created.
 * Finally, frees the lists without freeing their contents, since it is freed later when freeing the requirements.
 * 
 * @param file_name    the extensionless file name
 * @param requirements a pointer to the requirements of the file
 * @return 1 if an error has occurred, 0 otherwise
 */
int create_files(char file_name[], Requirements *requirements) {
    /* whether an error has occurred */
    int error_found = 0;
    /* list of external symbols */
    LinkedList *extern_list = create_list(SYMBOL);
    /* list of entry symbols */
    LinkedList *entry_list = create_list(SYMBOL);
    
    /* if a memory allocation failure has occurred, stops and returns 1 to signify error */
    if (extern_list == NULL || entry_list == NULL) return 1;
    
    /* fills the symbol lists based on the symbol table */
    map_add_matching_to_list(requirements->symbol_table, extern_list, is_extern);
    map_add_matching_to_list(requirements->symbol_table, entry_list, is_entry);
    
    /* writes the object file and updates error_found */
    error_found |= write_object_file(file_name, requirements);
    
    /* creates a .ext file if an external symbol is used */
    if (requirements->extern_found) error_found |= write_extern_file(file_name, extern_list);
    /* creates a .ent file if an entry symbol is defined */
    if (!list_empty(entry_list)) error_found |= write_entry_file(file_name, entry_list);
    
    /* shallow-frees the lists since their contents are freed later */
    shallow_free_list(extern_list);
    shallow_free_list(entry_list);
    return error_found;
}

/**
 * Creates and writes the object file based on the file's requirements.
 * 
 * Does so by first creating the file and printing the instruction count (minus its start value) and the data count to
 * its first line.
 * Then, prints the instructions and their addresses one by one, and then the data and its addresses one by one,
 * based on the memory image in the requirements.
 * 
 * @param file_name the extensionless file name
 * @param requirements the file's requirements
 * @return 1 if an error has occurred, 0 otherwise
 */
static int write_object_file(char file_name[], Requirements *requirements) {
    FILE *file = get_object_file(file_name);
    int i;
    if (file == NULL) return 1;
    /* prints the instruction count (minus its start value) and the data count to its first line */
    fprintf(file, "  %d %d\n", requirements->ic - IC_START, requirements->dc);
    /* prints the instructions and their addresses one by one */
    for (i = IC_START; i < requirements->ic; i++) {
        /* the address always takes 4 digits, the instruction is printed in octal and always takes 5 digits */
        fprintf(file, "%04d %05o\n", i, requirements->instruction_array[i]);
    }
    /* prints the data and its addresses one by one */
    for (i = 0; i < requirements->dc; i++) {
        /* the address always takes 4 digits, the data is printed in octal and always takes 5 digits */
        fprintf(file, "%04d %05o\n", i + requirements->ic, requirements->data_array[i]);
    }
    fclose(file);
    return 0;
}

/**
 * Creates and writes the extern file based on the external symbol list.
 * 
 * Does so by going over every item on the list, and for each item, printing the number of every instruction in the
 * symbol's appearances list.
 * 
 * @param file_name the extensionless file name
 * @param requirements the file's requirements
 * @return 1 if an error has occurred, 0 otherwise
 */
static int write_extern_file(char file_name[], LinkedList *extern_list) {
    FILE *file = get_extern_file(file_name);
    /* the item on the external symbol list */
    Node *node;
    if (file == NULL) return 1;
    node = extern_list->head;
    /* for every item on the external symbol list */
    while (node != NULL) {
        /* an item on the symbol's appearances list */
        Node *appearance = node->content.symbol.appearances->head;
        /* for every appearance */
        while (appearance != NULL) {
            /* prints the symbol's name and the appearance to the file, the appearance's address is 4 digit long */
            fprintf(file, "%s %04d\n", node->name, appearance->content.num);
            appearance = appearance->next;
        }
        node = node->next;
    }
    fclose(file);
    return 0;
}


/**
 * Creates and writes the entry file based on the entry symbol list.

 * Does so by going over every item on the list and printing its name and value to the file.
 * 
 * @param file_name the extensionless file name
 * @param requirements the file's requirements
 * @return 1 if an error has occurred, 0 otherwise
 */
static int write_entry_file(char file_name[], LinkedList *entry_list) {
    FILE *file = get_entry_file(file_name);
    /* the item on the entry symbol list */
    Node *node;
    if (file == NULL) return 1;
    node = entry_list->head;
    /* for every item on the entry list */
    while (node != NULL) {
        /* prints the symbol's name and value to the file */
        fprintf(file, "%s %d\n", node->name, node->content.symbol.value);
        node = node->next;
    }
    fclose(file);
    return 0;
}

/**
 * Checks if a given symbol is external.
 * Used as a condition to copy symbols to the external symbol list.
 * 
 * @param symbol the symbol to be checked
 * @return 1 if the symbol is external, 0 otherwise
 */
static int is_extern(SymbolContent symbol) {
    return symbol.type == EXTERNAL;
}

/**
 * Checks if a given symbol is an entry.
 * Used as a condition to copy symbols to the entry symbol list.
 * 
 * @param symbol the symbol to be checked
 * @return 1 if the symbol is an entry, 0 otherwise
 */
static int is_entry(SymbolContent symbol) {
    return symbol.type == ENTRY;
}
