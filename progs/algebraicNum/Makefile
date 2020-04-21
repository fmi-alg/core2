# Makefile for algebraicNum
#
#
# Core Library, $Id: Makefile,v 1.2 2006/12/20 16:01:02 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

#CHOOSE THE VARIANT Core Library you need
#VAR=GCC31
#VAR=Debug
#VAR=

ifndef CORE_PATH 
	CORE_PATH=../..
endif

include ${CORE_PATH}/progs/Make.options

#=================================================
# VARIABLES 
#=================================================


DEPEND= 
TARGETS_20= algebraicNum
OTHER_TARGETS = 
TARGETS= $(TARGETS_20)

p=tt

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

more: $(MORE_TARGETS)

checklib: 
	make -C $(CORE_PATH) clean
	make -C $(CORE_PATH)

testi test_intervals: $(TARGETS_20)
	./algebraicNum interval 2 3 1 8
	./algebraicNum interval 2 3 2 4

testc test_compose : $(TARGETS_20)
	echo "poly with poly"
	./algebraicNum compose pop  "0" "0"
	./algebraicNum compose pop  "1" "0"
	./algebraicNum compose pop  "0" "1"
	./algebraicNum compose pop  "1" "1"
	./algebraicNum compose pop  "x" "x"
	./algebraicNum compose pop  "x^2" "x+1"
	echo "poly with bipoly"
	./algebraicNum compose pob  "0" "0"
	./algebraicNum compose pob  "1" "0"
	./algebraicNum compose pob  "0" "1"
	./algebraicNum compose pob  "1" "1"
	./algebraicNum compose pob  "x" "x"
	./algebraicNum compose pob  "x^2" "x+1"
	echo "poly with bipoly,bipoly"
	./algebraicNum compose bobb  "0" "0" "0"
	./algebraicNum compose bobb  "1" "0" "0"
	./algebraicNum compose bobb  "0" "1" "1"
	./algebraicNum compose bobb  "1" "1" "x"
	./algebraicNum compose bobb  "x" "x" "1"
	./algebraicNum compose bobb  "x^2" "x+1" "x-2"
	./algebraicNum compose bobb  "y^2" "x+1" "x-2"

time testtime : $(TARGETS_20)
	time ./algebraicNum compose popNaive "(x-1)(x-2)(x-3)(x-5)" "(x+2)(x+3)(x+4)(x+5)" 200
	time ./algebraicNum compose pop "(x-1)(x-2)(x-3)(x-5)" "(x+2)(x+3)(x+4)(x+5)" 200

	time ./algebraicNum compose pobNaive "(x-1)(3x-1)(x^15+1)" "(x-1)(y-2)(x-y-3)(x^5-5)" 1
	time ./algebraicNum compose pob "(x-1)(3x-1)(x^15+1)" "(x-1)(y-2)(x-y-3)(x^5-5)" 1

	time ./algebraicNum compose bobbNaive "(x-1)(x-2)(y-3)(y-5)" "(x+2)(x+3)(x+4)(x^9+5)" "(y+x)(y-2x)(x+1)" 1
	time ./algebraicNum compose bobb "(x-1)(x-2)(y-3)(y-5)" "(x+2)(x+3)(x+4)(x^9+5)" "(y+x)(y-2x)(x+1)" 1

test test_math:  $(TARGETS_20)
	./algebraicNum rootbound "(x^2 -x -1)(x+1)" -2 2
	./algebraicNum isolate   "(x^2 -x -1)(x+1)" -2 2
	./algebraicNum algebra   "(x-3)" 2.1, 4.5 "(x+5)" -6.2 -3.2
	./algebraicNum algebra   "(x^2 -x -1)(x+1)" 0 2 "(3x^2-4)(x-1)" 1.1 2.0

all: $(TARGETS) $(MORE_TARGETS) 


#so that it recognizes .h deps
algebraicNum_DEPEND  = descartes.h composePoly.h composeBiPoly.h AlgebraicNum.h
#system files
algebraicNum_DEPEND += ${CORE_PATH}/inc/CORE/poly/Poly.h 
algebraicNum_DEPEND += ${CORE_PATH}/inc/CORE/poly/Curves.h 
algebraicNum_DEPEND += ${CORE_PATH}/inc/CORE/poly/Curves.tcc 
algebraicNum_DEPEND += ${CORE_PATH}/inc/CORE/poly/PolyFun.h
algebraicNum_DEPEND += ${CORE_PATH}/inc/CORE/poly/PolyBase.h

DEPEND=$(algebraicNum_DEPEND)


#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o: $(DEPEND)
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@


#=================================================
# Rules
#=================================================
algebraicNum : algebraicNum.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@
#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

spotless: 
	rm -f *~ *.o *.exe *.stackdump *#

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
	-@tar cvf algebraicNum.tar \
		*cpp Makefile README


