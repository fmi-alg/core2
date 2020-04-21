# Makefile for Radical
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.4 2010/06/16 14:56:35 exact Exp $

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

CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= tRadical tMoreRadicals
MORE_TARGETS= tUC tRadicalALL

p=tMoreRadicals

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

more: $(MORE_TARGETS)

test:  $(TARGETS)
	./tRadical -1
	./tMoreRadicals 

moretest: $(MORE_TARGETS)
	./tUC
	./tRadicalRat -1
	./tRadicalBF -1
	./tRadicalExp -1
	./tRadicalInt -1

$(p) p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}

all: $(TARGETS) $(MORE_TARGETS) 

tMoreRadicals: tMoreRadicals.o

tCubic: tCubic.o

tUC: tUC.o

tCubic: tCubic.o

t: t.o

# Compiling tRadical with various number types (int, BigInt, Exp)

tRadicalALL: tRadical tRadicalRat tRadicalInt tRadicalExp tRadicalBF

tRadical: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)    tRadical.cpp -o tRadical.o
	${CXX} $(LDFLAGS) tRadical.o $(CORE_LIB) -o tRadical

tRadicalRat: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_RAT tRadical.cpp -o tRadicalRat.o
	${CXX} $(LDFLAGS) tRadicalRat.o $(CORE_LIB) -o tRadicalRat

tRadicalBF: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BF tRadical.cpp -o tRadicalBF.o
	${CXX} $(LDFLAGS) tRadicalBF.o $(CORE_LIB) -o tRadicalBF

tRadicalInt: tRadical.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_INT tRadical.cpp -o tRadicalInt.o
	${CXX} $(LDFLAGS) tRadicalInt.o $(CORE_LIB) -o tRadicalInt

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
ALL=$(TARGETS) $(MORE_TARGETS) \
	tRadicalRat tRadicalInt tRadicalExp tRadicalBF \
	tMoreRadicals
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

