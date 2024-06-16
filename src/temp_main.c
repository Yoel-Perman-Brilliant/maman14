#include "stdio.h"
#include "../headers/string_ops.h"
#include "../headers/linked_list.h"
#include "stdlib.h"
int main() {
    LinkedList *list = create_list();
    list_add_int(list, "two", 2);
    list_add_int(list, "three", 3);
    list_add_int(list, "negative one", -1);
    print_int_list(list);
    printf("%d\n", list_contains(list, "three"));
    printf("%d\n", list_get_int(list, "two"));
    return 0;
}