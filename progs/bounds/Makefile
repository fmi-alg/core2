# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.5 2010/06/10 16:14:28 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= tEscapeBound tCutOffBound

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	time ./tEscapeBound 2000
	time ./tEscapeBound 5000
	time ./tCutOffBound 200
	time ./tCutOffBound 700

moretest:

all: $(TARGETS) 

tEscapeBound : tEscapeBound.o

tCutOffBound : tCutOffBound.o

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
ifeq ($(PLATFORM), mac)
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@
else
	${CXX} -c $(CXXFLAGS) -ffriend-injection $(CORE_INC) $< -o $@
endif

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

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

