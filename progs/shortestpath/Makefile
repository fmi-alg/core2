# Makefile for shortestpath 
#
# Core Library -- since Core 2.1 (Jan 2010)
#
#
# 	$Id: Makefile,v 1.1 2010/01/17 13:31:39 exact Exp $

include ../Make.options

default dijkstra:

#=================================================
# VARIABLES
#=================================================
# take defaults from top level:
CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#CHOOSE LEVEL
Level=1
Level=3

#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

#CHOOSE THE VARIANT Core Library you need
#VAR=GCC31
#VAR=Debug
#VAR=
#VAR=Opt

#=================================================
# DEFAULT TARGETS
#=================================================

TARGETS= dijkstra 
MORE_TARGETS= 
p=t
p=dijkstra

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

more: $(MORE_TARGETS)

test:  $(TARGETS)
	./dijkstra < data/graph1

moretest: $(MORE_TARGETS)

$(p) p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}

all: $(TARGETS) $(MORE_TARGETS) 

dijkstra: dijkstra.o

tRadicalRat: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_RAT tRadical.cpp -o tRadicalRat.o
	${CXX} $(LDFLAGS) tRadicalRat.o $(CORE_LIB) -o tRadicalRat

tRadicalBF: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BF tRadical.cpp -o tRadicalBF.o
	${CXX} $(LDFLAGS) tRadicalBF.o $(CORE_LIB) -o tRadicalBF

tRadicalExp: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_EXP tRadical.cpp -o tRadicalExp.o
	${CXX} $(LDFLAGS) tRadicalExp.o $(CORE_LIB) -o tRadicalExp


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
ALL=$(TARGETS) $(MORE_TARGETS) tRadicalRat tRadicalInt 
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# END
#=================================================
