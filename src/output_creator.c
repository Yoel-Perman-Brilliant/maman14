#include "../headers/requirements.h"
#include "../headers/files.h"
#include "../headers/structures/linked_list.h"
#include "../headers/structures/hash_map.h"
#include "../headers/output_creator.h"

#define COUNTER_WIDTH 4
#define VALUE_WITH 5
#define NUMBER_OF_INSTRUCTIONS_PADDING 3

int is_extern(SymbolContent symbol);

int is_entry(SymbolContent symbol);

int write_object_file(char file_name[], Requirements *requirements);

int write_extern_file(char file_name[], LinkedList *extern_list);

int write_entry_file(char file_name[], LinkedList *entry_list);

int create_files(char file_name[], Requirements *requirements) {
    int error_found = 0;
    LinkedList *extern_list = create_list();
    LinkedList *entry_list = create_list();
    error_found |= write_object_file(file_name, requirements);
    map_add_matching_to_list(requirements->symbol_table, extern_list, is_extern);
    map_add_matching_to_list(requirements->symbol_table, entry_list, is_entry);
    if (requirements->extern_found) error_found |= write_extern_file(file_name, extern_list);
    if (!list_empty(entry_list)) error_found |= write_entry_file(file_name, entry_list);
    return error_found;
}

int write_object_file(char file_name[], Requirements *requirements) {
    FILE *file = get_object_file(file_name);
    int i;
    if (file == NULL) return 1;
    fprintf(file, "   %d %d\n", requirements->ic - IC_START, requirements->dc);
    for (i = IC_START; i < requirements->ic; i++) {
        fprintf(file, "%04d %05o\n", i, requirements->instruction_array[i]);
    }
    for (i = 0; i < requirements->dc; i++) {
        fprintf(file, "%04d %05o\n", i + requirements->ic, requirements->data_array[i]);
    }
    fclose(file);
    return 0;
}

int write_extern_file(char file_name[], LinkedList *extern_list) {
    FILE *file = get_extern_file(file_name);
    Node *node;
    if (file == NULL) return 1;
    node = extern_list->head;
    while (node != NULL) {
        Node *appearance = node->content.symbol.appearances->head;
        while (appearance != NULL) {
            fprintf(file, "%s %d\n", node->name, appearance->content.line_number);
            appearance = appearance->next;
        }
        node = node->next;
    }
    fclose(file);
    return 0;
}

int write_entry_file(char file_name[], LinkedList *entry_list) {
    FILE *file = get_entry_file(file_name);
    Node *node;
    if (file == NULL) return 1;
    node = entry_list->head;
    while (node != NULL) {
        fprintf(file, "%s %d\n", node->name, node->content.symbol.value);
        node = node->next;
    }
    fclose(file);
    return 0;
}

int is_extern(SymbolContent symbol) {
    return symbol.type == EXTERNAL;
}

int is_entry(SymbolContent symbol) {
    return symbol.type == ENTRY;
}
