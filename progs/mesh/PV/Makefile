# Makefile for mesh/PV 
# 
# Author: Shuxing Lu (April 2009)
# Core Library, $Id: Makefile,v 1.9 2010/07/14 16:03:05 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

# file extension
ext= 
ext=.c
ext=.cc
ext=.cpp
#CHOOSE THE VARIANT Core Library you need
#VAR=GCC31
#VAR=Debug
#VAR=
#VAR=Opt

CORE_PATH=../../..
include ../Make.options

#=================================================
# VARIABLES 
#=================================================

DEPEND=
TARGETS= PV

n=1
n=

p=PV
# program version is "v"
v=
# program is "pv"
pv=$(p)$(v)
ifile=plot

verbose=1
verbose=

LOCALLDFLAGS=-L/opt/local/lib

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test: $(TARGETS)
	-@echo "We compile ./PV but not run it in automatic tests,"
	-@echo "    because the program is interactive."
	-@echo "You may directly call \"make eg1\" or \"make eg2\", etc."

moretest: $(TARGETS)
	-@echo "We compile ./PV but not run it in automatic tests,"
	-@echo "    because the program is interactive."
	-@echo "You may directly call \"make eg12\" or \"make eg20\", etc."


# The following eq is the taylor expansion of "sin(X^2)-cos(X^2) to the first three terms."
# But current bipoly string constructor loses all divisions.
eg1 example1:  $(TARGETS)
		./PV "x^2 - x^4 - y^2 + 1" -1.5 -1.5 1.5 1.5 100000 0.0001 0.01
eg2 example2:  $(TARGETS)
		./PV "x^2 - x^6/6 + x^10/120 - (1 - y^4/2 + y^8/24)" -2 -2 2 2 1    0.001 -0.1 1

eg10 example10: $(TARGETS)
		./PV "x^2 - x^6/6 + x^10/120 - (1 - y^4/2 + y^8/24)" -2 -2 2 2 0.02 0.001 -0.1 0

p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}


# Compile with different number types

pC PV: PV.o

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $(LOCALLDFLAGS) $< $(CORE_LIB) -o $@

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
ALL=$(TARGETS) $(MORE_TARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f tCurves${EXETYPE}

#=================================================
# Housekeeping
#=================================================

tar:
	-@tar cvf curves.tar \
		*cpp Makefile README

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes

