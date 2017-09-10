SRC:=src/
INC:=include/
COM:=build/
INC_FLAGS:=-I $(INC) -I ../lib/include/
TEST_INC_FLAGS:=$(INC_FLAGS) -I ../lib/test/include/

$(COM)pet_test.out_dep:=pet.o tag.o category.o

.SECONDEXPANSION:

.PRECIOUS: $(COM)%.o
$(COM)%.o: $(SRC)%.c $(INC)%.h | $(COM)
	$(COMPILE.c) $(INC_FLAGS) $(OUTPUT_OPTION) $<

$(COM)%.out: test/%.c $$(addprefix $$(COM), $$($$@_dep))
	$(LINK.c) $^ ../lib/build/swagger_error.o $(TEST_INC_FLAGS) -ljansson $(OUTPUT_OPTION)

.PRECIOUS: %/
%/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(COM)
