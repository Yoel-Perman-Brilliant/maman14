FLAGS = -Wall -ansi -pedantic -g
ALL_OBJECT_FILES = object/hash_table.o object/linked_list.o object/string_ops.o object/temp_main.o object/field_names.o

test: $(ALL_OBJECT_FILES)
	gcc $(FLAGS) $(ALL_OBJECT_FILES) -o test

object/temp_main.o: src/temp_main.c headers/string_ops.h headers/linked_list.h
	gcc -c $(FLAGS) src/temp_main.c -o object/temp_main.o

object/field_names.o: src/field_names.c headers/field_names.h headers/string_ops.h
	gcc -c $(FLAGS) src/field_names.c -o object/field_names.o

object/hash_table.o: src/hash_table.c headers/hash_table.h
	gcc -c $(FLAGS) src/hash_table.c -o object/hash_table.o
	
object/linked_list.o: src/linked_list.c headers/linked_list.h headers/string_ops.h headers/exit_codes.h
	gcc -c $(FLAGS) src/linked_list.c -o object/linked_list.o

object/string_ops.o: src/string_ops.c headers/string_ops.h
	gcc -c $(FLAGS) src/string_ops.c -o object/string_ops.o
