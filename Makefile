FLAGS = -Wall -ansi -pedantic -g
ALL_OBJECT_FILES = object/hash_map.o object/linked_list.o object/string_ops.o object/assembler.o object/fields.o \
		 		   object/pre_assembler.o object/general_util.o object/requirements.o object/files.o \
		 		   object/conversions.o object/first_pass.o object/operators.o object/set.o object/second_pass.o \
		 		   object/output_creator.o

assembler: $(ALL_OBJECT_FILES)
	gcc $(FLAGS) $(ALL_OBJECT_FILES) -o assembler

object/pre_assembler.o: src/pre_assembler.c headers/pre_assembler.h headers/structures/hash_map.h \
						headers/util/string_ops.h headers/util/general_util.h headers/files.h headers/exit_codes.h \
						headers/structures/linked_list.h headers/requirements.h
	gcc -c $(FLAGS)  src/pre_assembler.c -o object/pre_assembler.o

object/assembler.o: src/assembler.c headers/files.h headers/pre_assembler.h headers/requirements.h \
					headers/first_pass.h headers/second_pass.h headers/output_creator.h headers/exit_codes.h
	gcc -c $(FLAGS) src/assembler.c -o object/assembler.o

object/operators.o: src/operators.c headers/operators.h headers/util/string_ops.h headers/fields.h
		gcc -c $(FLAGS) src/operators.c -o object/operators.o

object/conversions.o: src/conversions.c headers/conversions.h headers/operators.h headers/util/string_ops.h \
					  headers/symbols.h
	gcc -c $(FLAGS) src/conversions.c -o object/conversions.o

object/first_pass.o: src/first_pass.c headers/first_pass.h headers/files.h headers/requirements.h \
 					 headers/util/string_ops.h headers/conversions.h headers/operators.h headers/util/general_util.h \
 					 headers/fields.h headers/structures/hash_map.h headers/structures/set.h \
 					 headers/symbols.h
	gcc -c $(FLAGS) src/first_pass.c -o object/first_pass.o

object/second_pass.o: src/second_pass.c headers/second_pass.h headers/util/string_ops.h headers/fields.h \
					  headers/requirements.h headers/structures/hash_map.h headers/structures/set.h \
					  headers/operators.h headers/conversions.h headers/files.h headers/util/general_util.h \
					  headers/symbols.h
	gcc -c $(FLAGS) src/second_pass.c -o object/second_pass.o

object/output_creator.o: src/output_creator.c headers/output_creator.h headers/requirements.h headers/files.h \
						 headers/structures/linked_list.h headers/structures/hash_map.h
	gcc -c $(FLAGS) src/output_creator.c -o object/output_creator.o

object/files.o: src/files.c headers/files.h headers/exit_codes.h headers/requirements.h headers/util/general_util.h
	gcc -c $(FlAGS) src/files.c -o object/files.o

object/requirements.o: src/requirements.c headers/requirements.h headers/exit_codes.h headers/structures/set.h \
					   headers/structures/hash_map.h headers/structures/linked_list.h
	gcc -c $(FlAGS) src/requirements.c -o object/requirements.o

object/fields.o: src/fields.c headers/fields.h headers/util/string_ops.h headers/operators.h
	gcc -c $(FLAGS) src/fields.c -o object/fields.o

object/set.o: src/structures/set.c headers/structures/set.h headers/exit_codes.h src/structures/linked_list.c
	gcc -c $(FLAGS) src/structures/set.c -o object/set.o

object/hash_map.o: src/structures/hash_map.c headers/structures/hash_map.h headers/exit_codes.h \
 					 src/structures/linked_list.c headers/symbols.h
	gcc -c $(FLAGS) src/structures/hash_map.c -o object/hash_map.o
	
object/linked_list.o: src/structures/linked_list.c headers/structures/linked_list.h headers/util/string_ops.h \
					  headers/exit_codes.h headers/symbols.h
	gcc -c $(FLAGS) src/structures/linked_list.c -o object/linked_list.o

object/string_ops.o: src/util/string_ops.c headers/util/string_ops.h headers/exit_codes.h
	gcc -c $(FLAGS) src/util/string_ops.c -o object/string_ops.o

object/general_util.o: src/util/general_util.c headers/util/general_util.h
	gcc -c $(FLAGS) src/util/general_util.c -o object/general_util.o


clean:
	rm object/*