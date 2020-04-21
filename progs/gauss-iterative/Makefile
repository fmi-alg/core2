# Makefile for bcir
#
# Core Library, $Id: Makefile,v 1.4 2008/12/11 21:24:18 exact Exp $

include ../Make.options


#=================================================
# Define target files (put all your files here)
#=================================================
LEV=2
TARGETS= bcir_level2

# MORETARGETS= 

default: $(TARGETS)

test: $(TARGETS)
	./bcir_level2 inputs/matrix_d4_1 inputs/vector_d4_1 200
	./bcir_level2 inputs/matrix_d4_2 inputs/vector_d4_2 200
	./bcir_level2 inputs/matrix_d10_1 inputs/vector_d10_1 200
#	./bcir inputs/matrix_d10_2 inputs/vector_d10_2 200
	./bcir_level2 inputs/matrix_d30_1 inputs/vector_d30_1 200
	./bcir_level2 inputs/matrix_d100_1 inputs/vector_d100_1 2000


moretest: $(TARGETS) $(MORETARGETS)

simpletest: $(TARGETS)


%_level2: %_level2.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL2) -o $@

%_level2: %_level2.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL2) -o $@

%_level3: %_level3.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL3) -o $@


#=================================================
# Rules for CORE_LEVEL 1 and CORE_LEVEL 3
#=================================================


%_level2.o: %.cpp
	${CXX} -c -DCORE_LEVEL=2 $(CXXFLAGS) $(CORE_INC) $< -o $@


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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

