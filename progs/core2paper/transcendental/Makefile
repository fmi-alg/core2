# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2010/05/14 09:09:37 exact Exp $

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

TARGETS= tConst tElemFun 
MORETARGETS= $(TARGETS) tZimmermann

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	./tConst 1000 1
	./tElemFun 

moretest: $(MORETARGETS)
	./tConst 10000
	./tConst 100000
	./tZimmermann

time:
	time ./tConst 10000

all: $(TARGETS) 

t: t.o
	
bench: moretest

tConst: tConst.o

tElemFun: tElemFun.o

tManyDigits: tManyDigits.o

tZimmermann : tZimmermann.o

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
ALL=$(TARGETS) $(MORE_TARGETS) tConst tElemFun tZimmermann
EXEPROGS=$(ALL:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

