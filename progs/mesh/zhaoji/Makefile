# Makefile for Zhaoji (modified from Gaussion)
#
# This Makefile illustrates some techniques for compiling your program
# into all three levels.
#
# NOTE: Being able to do all three levels is new to Core 2.0
#
# --Chee (May 2010)
#
# Core Library, $Id: Makefile,v 1.9 2010/06/11 17:49:29 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= SubdivSolver_level3 SubdivSolver_level2 SubdivSolver_level1 

default: $(TARGETS)

test: $(TARGETS)
	./subdivSolver_level3 inputs/MatHilbert6 3
	./subdivSolver_level2 inputs/MatHilbert6 3
	./subdivSolver_level1 inputs/MatHilbert6 3

moretest: $(TARGETS) $(MORETARGETS)
	./gaussian_level3 inputs/gregA 3
	./gaussian_level2 inputs/gregA 3
	./gaussian_level1 inputs/gregA 3
	./gaussian_level3 inputs/gregE 3
	./gaussian_level2 inputs/gregE 3
	./gaussian_level1 inputs/gregE 3

t: gaussian_level3 gaussian_level2 gaussian_level1
	./gaussian_level3 inputs/MatHilbert6 3
	./gaussian_level2 inputs/MatHilbert6 3
	./gaussian_level1 inputs/MatHilbert6 3


simpletest: $(TARGETS)
	./gaussian_level3 inputs/MatHilbert6 1
	./gaussian_level2 inputs/MatHilbert6 1
	./gaussian_level1 inputs/MatHilbert6 1

#=================================================
# Rules for CORE_LEVEL 1, 2, and 3
#=================================================
%_level1: %_level1.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL1) -o $@

%_level2: %_level2.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL2) -o $@

%_level3: %_level3.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) $(COREX_LIB_LEVEL3) -o $@

%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level2.o: %.cpp
	${CXX} -c -DCORE_LEVEL=2 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

m:
	gvim Makefile
#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# End
#=================================================
