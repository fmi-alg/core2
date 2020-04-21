# Makefile for manydigits
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2010/01/21 18:44:11 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

CORE_PATH=../../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= manydigits 

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	./manydigits

moretest:  $(TARGETS)
	./manydigits 10000 0

time:
	time ./manydigits

all: $(TARGETS) 
	
manydigits: manydigits.o

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

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
ALL=$(TARGETS) $(MORE_TARGETS) tConst tElemFun
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

