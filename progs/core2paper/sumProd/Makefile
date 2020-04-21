# Makefile for benchmark/sumProduct
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.4 2010/06/14 16:17:28 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=shared
LINKAGE=static

#CHOOSE THE VARIANT Core Library you need
#VAR=GCC31
#VAR=Debug
#VAR=

CORE_PATH=../../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

TARGETS= tSumAll tProdAll
MORE_TARGETS= tSumAll tProdAll

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

more: $(MORE_TARGETS)

test:  $(TARGETS)
	time ./tSum1 2000 
	time ./tSum2 2000 
	time ./tProd1 2000 
	time ./tProd2 2000
	time ./tProdConverge1 2000
	time ./tProdConverge2 2000

moretest: $(MORE_TARGETS)
	time ./tSum1 4000
	time ./tSum1 10000
	time ./tSum2 4000
	time ./tSum2 10000
	time ./tSum2 100000
	time ./tSum2 200000
	time ./tProd1 400
	time ./tProd1 1000
	time ./tProd1 10000
	time ./tProd2 400
	time ./tProd2 1000
	time ./tProd2 10000
	time ./tProd2 20000
	time ./tProdConverge1 10000
	time ./tProdConverge2 10000

bench: moretest

all: $(TARGETS) $(MORE_TARGETS) 

# Compiling tSum with(without) using summation operator various number types

tSumAll: tSum1 tSum2

sum: tSumAll

tSum1: tSum.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  tSum.cpp -o tSum1.o
	${CXX} $(LDFLAGS) --stack,83886080 tSum1.o $(CORE_LIB) -o tSum1

tSum2: tSum.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_USE_SUM tSum.cpp -o tSum2.o
	${CXX} $(LDFLAGS) tSum2.o $(CORE_LIB) -o tSum2

tHarmonicAll: tHarmonic1 tHarmonic2

harmonic: tHarmonicAll

tHarmonic1: tHarmonic.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  tHarmonic.cpp -o tHarmonic1.o
	${CXX} $(LDFLAGS) --stack,83886080 tHarmonic1.o $(CORE_LIB) -o tHarmonic1

tHarmonic2: tHarmonic.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_USE_SUM tSum.cpp -o tHarmonic2.o
	${CXX} $(LDFLAGS) tHarmonic2.o $(CORE_LIB) -o tHarmonic2

experiment1: tSum1 tSum2
	tSum1 1000
	tSum2 1000
	tSum1 10000
	tSum2 10000
	tSum1 100000
	tSum2 100000
	tSum1 1000000
	tSum2 1000000

experiment2: tProd1 tProd2
	tProd1 1000
	tProd2 1000
	tProd1 10000
	tProd2 10000
	tProd1 100000
	tProd2 100000
	tProd1 1000000
	tProd2 1000000

tProdConverge : tProdConverge1

tProd : tProd1

tProdAll: tProd1 tProd2

prod: tProdAll

tProdConverge: tProdConverge.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  tProdConverge.cpp -o tProdConverge.o
	${CXX} $(LDFLAGS) --stack,83886080 tProdConverge.o $(CORE_LIB) -o tProdConverge

tProdAll: tProd1 tProd2 tProdConverge1 tProdConverge2

tProdConverge1: tProdConverge.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  tProdConverge.cpp -o tProdConverge1.o
	${CXX} $(LDFLAGS) --stack,83886080 tProdConverge1.o $(CORE_LIB) -o tProdConverge1

tProdConverge2: tProdConverge.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  -D_USE_PROD tProdConverge.cpp -o tProdConverge2.o
	${CXX} $(LDFLAGS) --stack,83886080 tProdConverge2.o $(CORE_LIB) -o tProdConverge2

tProd1: tProd.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)  tProd.cpp -o tProd1.o
	${CXX} $(LDFLAGS) --stack,83886080 tProd1.o $(CORE_LIB) -o tProd1

tProd2: tProd.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_USE_PROD tProd.cpp -o tProd2.o
	${CXX} $(LDFLAGS) tProd2.o $(CORE_LIB) -o tProd2
	
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
ALL=$(TARGETS) $(MORE_TARGETS) tSum1 tSum2 tProd1 tProd2 tHarmonic1 tHarmonic2
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

