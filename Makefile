FLAGS = -Wall -ansi -pedantic -g
ALL_OBJECT_FILES = object/hash_table.o object/linked_list.o object/string_ops.o object/temp_main.o object/fields.o \
		 		   object/read_ops.o object/pre_assembler.o object/general_util.o

test: $(ALL_OBJECT_FILES)
	gcc $(FLAGS) $(ALL_OBJECT_FILES) -o test

object/pre_assembler.o: src/pre_assembler.c headers/pre_assembler.h headers/structures/hash_table.h \
						headers/util/string_ops.h headers/util/read_ops.h headers/util/general_util.h
	gcc -c $(FLAGS)  src/pre_assembler.c -o object/pre_assembler.o

object/temp_main.o: src/temp_main.c headers/util/string_ops.h headers/structures/linked_list.h headers/util/read_ops.h \
					headers/pre_assembler.h headers/structures/hash_table.h
	gcc -c $(FLAGS) src/temp_main.c -o object/temp_main.o

object/fields.o: src/fields.c headers/fields.h headers/util/string_ops.h
	gcc -c $(FLAGS) src/fields.c -o object/fields.o

object/hash_table.o: src/structures/hash_table.c headers/structures/hash_table.h
	gcc -c $(FLAGS) src/structures/hash_table.c -o object/hash_table.o
	
object/linked_list.o: src/structures/linked_list.c headers/structures/linked_list.h headers/util/string_ops.h \
					  headers/exit_codes.h
	gcc -c $(FLAGS) src/structures/linked_list.c -o object/linked_list.o

object/string_ops.o: src/util/string_ops.c headers/util/string_ops.h
	gcc -c $(FLAGS) src/util/string_ops.c -o object/string_ops.o

object/read_ops.o: src/util/read_ops.c headers/util/read_ops.h
	gcc -c $(FLAGS) src/util/read_ops.c -o object/read_ops.o

object/general_util.o: src/util/general_util.c headers/util/general_util.h
	gcc -c $(FLAGS) src/util/general_util.c -o object/general_util.o


clean:
	rm object/*