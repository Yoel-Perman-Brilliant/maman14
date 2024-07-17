FLAGS = -Wall -ansi -pedantic -g
ALL_OBJECT_FILES = object/hash_table.o object/linked_list.o object/string_ops.o object/temp_main.o object/fields.o \
		 		   object/pre_assembler.o object/general_util.o object/requirements.o object/files.o \
		 		   object/conversions.o object/first_pass.o object/operators.o

test: $(ALL_OBJECT_FILES)
	gcc $(FLAGS) $(ALL_OBJECT_FILES) -o test

object/pre_assembler.o: src/pre_assembler.c headers/pre_assembler.h headers/structures/hash_table.h \
						headers/util/string_ops.h headers/util/general_util.h headers/files.h headers/exit_codes.h
	gcc -c $(FLAGS)  src/pre_assembler.c -o object/pre_assembler.o

object/temp_main.o: src/temp_main.c headers/util/string_ops.h headers/structures/linked_list.h \
					headers/pre_assembler.h headers/structures/hash_table.h headers/requirements.h \
					headers/conversions.h headers/util/general_util.h headers/util/string_ops.h \
					headers/first_pass.h headers/fields.h headers/operators.h
	gcc -c $(FLAGS) src/temp_main.c -o object/temp_main.o

object/operators.o: src/operators.c headers/operators.h headers/util/string_ops.h
		gcc -c $(FLAGS) src/operators.c -o object/operators.o

object/conversions.o: src/conversions.c headers/conversions.h headers/operators.h headers/util/string_ops.h
	gcc -c $(FLAGS) src/conversions.c -o object/conversions.o

object/first_pass.o: src/first_pass.c headers/first_pass.h headers/files.h headers/requirements.h \
 					 headers/util/string_ops.h headers/conversions.h headers/operators.h headers/util/general_util.h
	gcc -c $(FLAGS) src/first_pass.c -o object/first_pass.o

object/files.o: src/files.c headers/files.h headers/exit_codes.h
	gcc -c $(FlAGS) src/files.c -o object/files.o

object/requirements.o: src/requirements.c headers/requirements.h headers/exit_codes.h
	gcc -c $(FlAGS) src/requirements.c -o object/requirements.o

object/fields.o: src/fields.c headers/fields.h headers/util/string_ops.h
	gcc -c $(FLAGS) src/fields.c -o object/fields.o

object/hash_table.o: src/structures/hash_table.c headers/structures/hash_table.h headers/exit_codes.h
	gcc -c $(FLAGS) src/structures/hash_table.c -o object/hash_table.o
	
object/linked_list.o: src/structures/linked_list.c headers/structures/linked_list.h headers/util/string_ops.h \
					  headers/exit_codes.h
	gcc -c $(FLAGS) src/structures/linked_list.c -o object/linked_list.o

object/string_ops.o: src/util/string_ops.c headers/util/string_ops.h headers/exit_codes.h
	gcc -c $(FLAGS) src/util/string_ops.c -o object/string_ops.o

object/general_util.o: src/util/general_util.c headers/util/general_util.h
	gcc -c $(FLAGS) src/util/general_util.c -o object/general_util.o


clean:
	rm object/*