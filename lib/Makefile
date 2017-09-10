SRC:=src/
INC:=include/
COM:=build/
INC_FLAGS:=-I $(INC)
TEST_INC_FLAGS:=$(INC_FLAGS) -I test/include/

$(COM)date_time_tests.out_dep:=date_time.o swagger_error.o

.SECONDEXPANSION:

.PRECIOUS: $(COM)%.o
$(COM)%.o: $(SRC)%.c $(INC)%.h | $(COM)
	$(COMPILE.c) $(INC_FLAGS) $(OUTPUT_OPTION) $<

$(COM)%.out: test/%.c $$(addprefix $$(COM), $$($$@_dep))
	$(LINK.c) $^ $(TEST_INC_FLAGS) -ljansson $(OUTPUT_OPTION)

.PRECIOUS: %/
%/:
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) -r $(COM)
