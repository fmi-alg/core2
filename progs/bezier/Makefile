# Makefile for bezier 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:23 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=shared
LINKAGE=static

#CHOOSE THE VARIANT Core Library you need
VAR=GCC31
VAR=Debug
VAR=

CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

ifile=plot
DEPEND=
TARGETS= tBezier
p=tBezier

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)

p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}

all: $(TARGETS) $(MORE_TARGETS) 

testall: test moretest

tBernstRat: tBernstRat.o

tBernstBF: tBernstBF.o

s show:
	../../ext/graphics/drawcurve.exe $(verbose) < data/${ifile}

s2 show2:
	../../ext/graphics/drawcurve.exe $(verbose) < data/${ifile}2


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
ALL=$(TARGETS) $(MORE_TARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# Housekeeping
#=================================================

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes


