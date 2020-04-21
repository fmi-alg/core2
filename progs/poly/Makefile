# Makefile for Poly 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.12 2010/07/21 07:11:25 exact Exp $

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

CORE_PATH=../..
include ${CORE_PATH}/progs/Make.options

#=================================================
# TARGET FILES
#=================================================

DEPEND=

n=1

# if default is NT is not assigned, default is NT=BigInt
# NT=
# NT=BigFloat
# NT=Expr
# NT=int
# NT=BigRat

p=tPoly
p=tt
p=solve
p=tNewton
p=tSturm${n}
p=tSturm
p=tDescartes
p=pisot
p=tRootOf
p=tPseudo
p=tGCD
p=readpoly
p=readFile

TARGETS= tNewton tSturm tDescartes tSturmBF tDescartes tDescartesBF tPolyALL

# Dec'2018, Chee: removed from MORE_TARGETS the following:
#		tSturmLon, tSturmInt, tDescartesLon, tDescartesInt
#	because ToBigFloat2(ht) is not yet defined for these types.
MORE_TARGETS = tPolyALL \
    tSturmRat tSturmExp \
    tWilk20 tWilk20deriv tKameny4 tGCD tRootOf wein \
    tDescartesRat tDescartesExp \
    fileReader readPoly parsepoly

OTHER_TARGETS = solve pisot parsepoly

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test:  $(TARGETS)
	./tSturmBF
	./tDescartesBF
	./tPoly
	./tPolyInt
	./tPolyRat
	./tPolyBF
	./tPolyExp

moretest: $(MORE_TARGETS)
	./tPoly
	./tPolyInt
	./tPolyLon
	./tPolyRat
	./tPolyExp
	./tPolyBF
	./tSturmExp
	./tSturmRat
	./tSturmInt
	./tSturmLon
	./tDescartesExp
	./tDescartesRat
	./tDescartesInt
	./tDescartesLon
	./tWilk20
	./tWilk20deriv
	./tKameny4
	./tGCD
	./tRootOf
	./wein
	./tNewton 123
	./tNewton 12345678901234

othertest: $(OTHER_TARGETS)
	./pisot
	./solve

p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}

all: $(TARGETS) $(MORE_TARGETS) 

testall: test moretest

testmore: tNewton tSturm
	tNewton 4
	tNewton 8
	tNewton 123456
	tSturm 200 moreTest
	tSturmInt
#	tSturmLon

a:
	touch tPoly.cpp
	make tPoly
	./tPoly

testPoly:
	./tPoly
	./tPolyInt
	./tPolyLon
	./tPolyRat
	./tPolyExp
	./tPolyBF

testNewton:
	./tNewton
	./tNewtonBF

testSturm:
	./tSturm
	./tSturmBF
	./tSturmExp

timetmp:  tSturm tDescartes \
		tSturmBF tDescartesBF \
		tSturmRat tDescartesRat 
	time tSturm	2>&1 | tee tmp/time_tSturm
	time tDescartes	2>&1 | tee tmp/time_tDescartes
	time tSturmBF	2>&1 | tee tmp/time_tSturmBF
	time tDescartesBF 2>&1 | tee tmp/time_tDescartesBF
	time tSturmRat	2>&1 | tee tmp/time_tSturmRat
	time tDescartesRat 2>&1 | tee tmp/time_tDescartesRat

time:  tPoly tNewton tSturm tSturmBF tDescartes tDescartesBF
	time tPoly 	2>&1 | tee tmp/time_tPoly
	time tNewton	2>&1 | tee tmp/time_tNewton
	time tSturm	2>&1 | tee tmp/time_tSturm
	time tSturmBF	2>&1 | tee tmp/time_tSturmBF
	time tDescartes	2>&1 | tee tmp/time_tDescartes
	time tDescartesBF 2>&1 | tee tmp/time_tDescartesBF

# Compiling Polynomial class with various number types (int, BigInt, Exp)
tPolyALL: tPoly tPolyRat tPolyInt tPolyExp tPolyBF tPolyLon

tPoly: tPoly.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC)    tPoly.cpp -o tPoly.o 
	${CXX} $(LDFLAGS) tPoly.o $(CORE_LIB) -o tPoly

tPolyRat: tPoly.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_RAT tPoly.cpp -o tPolyRat.o
	${CXX} $(LDFLAGS) tPolyRat.o $(CORE_LIB) -o tPolyRat

tPolyBF: tPoly.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BF tPoly.cpp -o tPolyBF.o
	${CXX} $(LDFLAGS) tPolyBF.o $(CORE_LIB) -o tPolyBF

tPolyInt: tPoly.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_INT tPoly.cpp -o tPolyInt.o
	${CXX} $(LDFLAGS) tPolyInt.o $(CORE_LIB) -o tPolyInt

tPolyLon: tPoly.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_LON tPoly.cpp -o tPolyLon.o
	${CXX} $(LDFLAGS) tPolyLon.o $(CORE_LIB) -o tPolyLon

tPolyExp: tPoly.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_EXP tPoly.cpp -o tPolyExp.o
	${CXX} $(LDFLAGS) tPolyExp.o $(CORE_LIB) -o tPolyExp

exp:	tPoly.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_EXP tPoly.cpp -o tPolyExp.o
	${CXX} $(LDFLAGS) tPolyExp.o $(CORE_LIB) -o tPolyExp

# testing Newton iteration with various number types (int, BigInt, Exp)
tNewtonALL: tNewton tNewtonBF 

tNewton: tNewton.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BINT tNewton.cpp -o tNewton.o
	${CXX} $(LDFLAGS) tNewton.o $(CORE_LIB) -o tNewton

tNewtonBF: tNewton.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BF tNewton.cpp -o tNewtonBF.o
	${CXX} $(LDFLAGS) tNewtonBF.o $(CORE_LIB) -o tNewtonBF

tGCD: tGCD.o

tRootOf: tRootOf.o

tWilk20: tWilk20.o

tWilk20deriv: tWilk20deriv.o

wein: wein.o

# Compile tSturm with NT=BigInt (default), or NT=BigFloat, NT=Expr,
#                NT=BigRat, NT=int, or NT=long:
# tSturmALL: tSturm tSturmBF tSturmExp tSturmInt tSturmRat tSturmLon
tSturmALL: tSturm tSturmBF tSturmExp tSturmInt tSturmRat tSturmLon

tSturm: tSturm.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tSturm.cpp -o tSturm.o
	${CXX} $(LDFLAGS) tSturm.o $(CORE_LIB) -o tSturm

tSturmBF: tSturm.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BF tSturm.cpp -o tSturmBF.o
	${CXX} $(LDFLAGS) tSturmBF.o $(CORE_LIB) -o tSturmBF

tSturmExp: tSturm.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_EXP tSturm.cpp -o tSturmExp.o
	${CXX} $(LDFLAGS) tSturmExp.o $(CORE_LIB) -o tSturmExp

tSturmInt: tSturm.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_INT tSturm.cpp -o tSturmInt.o
	${CXX} $(LDFLAGS) tSturmInt.o $(CORE_LIB) -o tSturmInt

tSturmLon: tSturm.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_LONG tSturm.cpp -o tSturmLon.o
	${CXX} $(LDFLAGS) tSturmLon.o $(CORE_LIB) -o tSturmLon

tSturmRat: tSturm.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_RAT tSturm.cpp -o tSturmRat.o
	${CXX} $(LDFLAGS) tSturmRat.o $(CORE_LIB) -o tSturmRat

# Compile tDescartes with NT=BigInt (default), or NT=BigFloat, NT=Expr,
#                NT=BigRat, NT=int, or NT=long:
# tDescartesALL: tDescartes tDescartesBF tDescartesExp tDescartesInt tDescartesRat tDescartesLon
tDescartesALL: tDescartes tDescartesBF tDescartesExp tDescartesInt tDescartesRat tDescartesLon

tDescartes: tDescartes.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) tDescartes.cpp -o tDescartes.o
	${CXX} $(LDFLAGS) tDescartes.o $(CORE_LIB) -o tDescartes

tDescartesBF: tDescartes.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_BF tDescartes.cpp -o tDescartesBF.o
	${CXX} $(LDFLAGS) tDescartesBF.o $(CORE_LIB) -o tDescartesBF

tDescartesExp: tDescartes.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_EXP tDescartes.cpp -o tDescartesExp.o
	${CXX} $(LDFLAGS) tDescartesExp.o $(CORE_LIB) -o tDescartesExp

tDescartesInt: tDescartes.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_INT tDescartes.cpp -o tDescartesInt.o
	${CXX} $(LDFLAGS) tDescartesInt.o $(CORE_LIB) -o tDescartesInt

tDescartesLon: tDescartes.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_LONG tDescartes.cpp -o tDescartesLon.o
	${CXX} $(LDFLAGS) tDescartesLon.o $(CORE_LIB) -o tDescartesLon

tDescartesRat: tDescartes.cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -D_NT_RAT tDescartes.cpp -o tDescartesRat.o
	${CXX} $(LDFLAGS) tDescartesRat.o $(CORE_LIB) -o tDescartesRat

timeNewton: timeNewton.o

solve: solve.o

pisot: pisot.o

tKameny4: tKameny4.o

tPseudo: tPseudo.o

parsepoly: parsepoly.o

fileReader: fileReader.h fileReader.cpp fileReader.o

readPoly: readPoly.h readPoly.cpp readPoly.o

t:	t.o

wein:	wein.o

tt:	tt.cpp 
	${CXX} -c $(CXXFLAGS) $(CORE_INC)            tt.cpp -o tt.o
	${CXX} $(LDFLAGS) tt.o $(CORE_LIB) -o tt

tt1:	tt1.o

tt2:	tt2.o

tt3:	tt3.o

tBug:	tBug.o

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

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f tDescartes$(EXETYPE) tDescartesExp$(EXETYPE) tDescartesBF$(EXETYPE) \
		tDescartesInt$(EXETYPE) tDescartesLon$(EXETYPE) tDescartesRat$(EXETYPE)
	-@rm -f tSturm$(EXETYPE) tSturmExp$(EXETYPE) tSturmBF$(EXETYPE) \
		tSturmInt$(EXETYPE) tSturmLon$(EXETYPE) tSturmRat$(EXETYPE)
	-@rm -f tNewton$(EXETYPE) tNewtonBF$(EXETYPE) 
	-@rm -f tPoly$(EXETYPE) tPolyExp$(EXETYPE) \
	          tPolyInt$(EXETYPE) tPolyRat$(EXETYPE) tPolyBF$(EXETYPE) tPolyLon$(EXETYPE)
	-@rm -f tt$(EXETYPE) tt1$(EXETYPE) tt2$(EXETYPE)

#=================================================
# Housekeeping
#=================================================

tar:
	-@tar cvf poly.tar \
		Poly.h Poly.tcc \
		tPoly.cpp tt.cpp \
		Newton.h tNewton.cpp \
		Sturm.h tSturm.cpp\
	        solve.cpp \
	        tRoots.cpp \
		Makefile README

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes


