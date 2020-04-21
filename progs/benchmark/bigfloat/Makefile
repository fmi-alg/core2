# Makefile for benchmark/bigfloat
#
# This is to perform basic bigfloat speed test
# 	(comparing Core 1 with Core 2 and with Mpfr)
#
# Since Core 2.0
# Zilin (2007)
# Core Library, $Id: Makefile,v 1.4 2010/07/07 15:42:05 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

#CHOOSE THE VARIANT Core Library you need
VAR=GCC31
VAR=Debug
VAR=

CORE_PATH=../../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= bm_bigfloat 
MORETARGETS= $(TARGETS)

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test bench:  $(TARGETS)
	./bm_bigfloat 100 1000 
	./bm_bigfloat 100 10000 
	./bm_bigfloat 100 100000 

moretest: $(MORETARGETS)
	./bm_bigfloat 10 1000000

bm_bigfloat: bm_bigfloat.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  bm_bigfloat.cpp -o bm_bigfloat.o
	${CXX} $(LDFLAGS) bm_bigfloat.o $(CORE_LIB) -o bm_bigfloat

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
ALL=$(TARGETS) $(MORE_TARGETS) bm_bigfloat
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

