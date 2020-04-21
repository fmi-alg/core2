# Makefile for voronoi 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:54:37 exact Exp $

CORE_PATH=../../..
include ../../Make.options

TARGETS= pythagorean
MORETARGETS= 

ifeq ($(PLATFORM), mingw)
  OBJS+=getopt.o
endif

#=================================================
# Define target files (put all your files here)
#=================================================

default: $(TARGETS)

test: $(TARGETS)
	./pythagorean  10 3 4 5 2

moretest: $(MORETARGETS)
	./pythagorean  10 5 12 13 
	./pythagorean  10 5 12 13 


pythagorean: pythagorean.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

	
#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

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
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

