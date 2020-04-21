# Makefile for complex 
#
# (template Makefile for simple targets)
#
# Chee, Dec 2016: 
#	Be sure sure to choose Core Compiler Option of
#		VAR=Debug
#	in order for the asserts to be effective.
#	Note that VAR="" or VAR=Opt includes compiler flag "-DNDEBUG"
#	which turns off assertions.
#
# Core Library, $Id: Makefile,v 1.1 2010/06/22 16:25:15 exact Exp $



# turn assertions off
#VAR=Opt
# turn assertions on (this is the normal mode for this folder)
 VAR=Debug

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = tComplex

all: $(TARGETS)

test: $(TARGETS)
	./tComplex

moretest: $(TARGETS)
# nothing to do

tComplex: tComplex.o


#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

vclean: clean
	-@test -z "tComplex" || rm -f tComplex
	-@test -z "tComplex.exe" || rm -f tComplex.exe
#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

