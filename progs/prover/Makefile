# Makefile for prover 
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:25 exact Exp $

# Choose the VARiant library:
# VAR=

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= prover_level3 

MORE_TARGETS= prover_level1

default: $(TARGETS)

more: $(MORE_TARGETS)

test: $(TARGETS)
	./prover_level3 inputs/pappus 10

moretest: $(MORE_TARGETS) $(TARGETS) test_false
	./prover_level1 inputs/pappus 10
	./prover_level3 inputs/simson 2
	./prover_level1 inputs/simson 2
	./prover_level3 inputs/simsonBad 3

test_false: $(TARGETS)
	./prover_level3 inputs/pappusBad 5
	./prover_level1 inputs/pappusBad 5

test_long: $(TARGETS)
	./prover_level3 inputs/pappus 10
	./prover_level3 inputs/simsonBad 3
	./prover_level3 inputs/pascal 2
	./prover_level3 inputs/butterfly 2

# ONLY TRUE THEOREMS, UNDER LEVEL 3 ARE TESTED HERE:
test_true: $(TARGETS)
	./prover_level3 inputs/simson 2
	./prover_level3 inputs/pappus 10
	./prover_level3 inputs/butterfly 2

%_level1: %_level1.o terms_level1.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

%_level3: %_level3.o terms_level3.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

#=================================================
# Rules for CORE_LEVEL 1 and CORE_LEVEL 3
#=================================================
%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

