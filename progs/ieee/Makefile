# Makefile for ieeeFormat 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:24 exact Exp $

#=================================================
# Define variables
#=================================================

include ../Make.options

ifndef lev
	lev=3
endif

ifdef DEBUG
	XFLAGS = -DmyDEBUG
endif

p=bigRand
p=coreEps
p=fpline
p=frexp
p=ieee
p=ieeeFormat
p=next
p=nextafter
p=tOverflow
p=tDoubleValue
p=fpOverflow

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= tDoubleValue

default: $(TARGETS)

MORETARGETS= ieeeFormat 

ALLTARGETS= bigRand coreEps \
	fpline frexp \
	ieee ieeeFormat \
	next nextafter \
	tDoubleValue \
	tOverflow fpOverflow 

test: $(TARGETS)
	./tDoubleValue

moretest: $(MORETARGETS)
	./ieeeFormat

ieeeFormat: ieeeFormat.o

tDoubleValue: tDoubleValue.o

p $(p): $(p).o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

fpline13: fpline1 fpline3

fpline3 3: fpline3.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level3 $(CORE_LIB) -o fpline3
	
fpline3.o: fpline.cpp
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=3 $(CORE_INC) $< -o fpline3.o

fpline1 1: fpline1.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level1 $(CORE_LIB) -o fpline1
	
fpline1.o: fpline.cpp
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=1 $(CORE_INC) $< -o fpline1.o

fpline: fpline.o

line:
	./fpline > out

coreEps: coreEps.o
	${CXX} -c $(CXXFLAGS)  $(XFLAGS) $(CORE_INC) $< -o $@

bigRand: bigRand.o

frexp: frexp.o

tOverflow: tOverflow.o

# Compiling fpOverflow.cpp the wrong way:
# 	The executable produced by this target does not work properly!!!
#		> fpOverflow 10
#			Overflow flag before: 0
#			Underflow flag before: 0
#			Overflow flag after: 0
#			Underflow flag after: 0
# 	This wrong output is wrong!
fpOverflow: fpOverflow.o

# Alternative (correct) way to compile fpOverflow.cpp:
# 	Call the target below to compile fpOverflow (it has no -O3 flag).
# 	The executable produced by this target works as expected:
#		> fpOverflow 10
#			Overflow flag before: 0
#			Underflow flag before: 0
#			Overflow flag after: 1
#			Underflow flag after: 1
# 	This is the correct output!
fpOverflowX: fpOverflow.o
	g++ -c -std=c++11 -Wfatal-errors -Wall  -I. -I../../inc \
	    -I../../mpfr/include -I../../gmp/include \
	    fpOverflow.cpp -o fpOverflow.o
	g++  fpOverflow.o -L../../lib -o fpOverflow
	
tt: tt.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(XFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))
ALLEXEPROGS=$(ALLTARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z p$(EXETYPE) || rm -f p$(EXETYPE)
	-@test -z "$(ALLEXEPROGS)" || rm -f $(ALLEXEPROGS)

