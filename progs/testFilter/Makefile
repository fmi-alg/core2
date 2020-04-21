# Makefile for testFilter 
#
# Core Library, $Id: Makefile,v 1.3 2007/03/20 14:36:27 exact Exp $

VAR=

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS= testFilter testmatrix

MORE_TARGETS= testFilter morematrix

TIME_TARGETS=timeFilter timeFilter1 

p=tt
p=t

default: $(TARGETS)

test: $(TARGETS) 
	./testFilter inputs/200x3x10matrices
	./testFilter inputs/200x4x20matrices
	./testFilter inputs/200x5x30matrices 

moretest: $(MORE_TARGETS) 
	./testFilter inputs/1000x2x10matrices
	./testFilter inputs/1000x3x10matrices
	./testFilter inputs/1000x4x10matrices
	./testFilter inputs/500x5x10matrices
	./testFilter inputs/500x6x10matrices
	./testFilter inputs/500x7x10matrices
	./testFilter inputs/500x8x10matrices	

bench: $(MORE_TARGETS) 
	./testFilter inputs/1000x3x10matrices
	./testFilter inputs/1000x4x10matrices
	./testFilter inputs/500x5x10matrices
	./testFilter inputs/500x6x10matrices
	./testFilter inputs/500x7x10matrices
	./testFilter inputs/500x8x10matrices	

debug:
	./tt inputs/200x3x10matrices

bigtest: $(TARGETS) bigmatrix
	./testFilter inputs/200x10x100matrices 
	./testFilter inputs/200x20x100matrices 

time${v}: timeFilter${v} 
	./timeFilter${v} inputs/1000x2x10matrices
	./timeFilter${v} inputs/1000x3x10matrices
	./timeFilter${v} inputs/1000x4x10matrices
	./timeFilter${v} inputs/500x5x10matrices
	./timeFilter${v} inputs/500x6x10matrices
	./timeFilter${v} inputs/500x7x10matrices
	./timeFilter${v} inputs/500x8x10matrices

testmatrix: createMatrices
	./createMatrices inputs/200x3x10matrices 200 3 10
	./createMatrices inputs/200x4x20matrices 200 4 20
	./createMatrices inputs/200x5x30matrices 200 5 30

morematrix: createMatrices
	./createMatrices inputs/1000x2x10matrices 1000 2 10
	./createMatrices inputs/1000x3x10matrices 1000 3 10
	./createMatrices inputs/1000x4x10matrices 1000 4 10
	./createMatrices inputs/500x5x10matrices 500 5 10
	./createMatrices inputs/500x6x10matrices 500 6 10
	./createMatrices inputs/500x7x10matrices 500 7 10
	./createMatrices inputs/500x8x10matrices 500 8 10
		
bigmatrix: createMatrices
	./createMatrices inputs/200x10x100matrices 200 10 100
	./createMatrices inputs/200x20x100matrices 200 20 100

testFilter: testFilter.o

timeFilter: timeFilter.o

timeFilter1: timeFilter1.o

createMatrices: createMatrices.o

p ${p}: ${p}.o
	${CXX} $(LDFLAGS) ${p}.o $(CORE_LIB) -o ${p}

#=================================================
# Rules
#=================================================
%: %.o
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
EXEPROGS=$(TARGETS:=$(EXETYPE)) createMatrices$(EXETYPE)

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

