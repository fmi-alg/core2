# Makefile for testIO 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.14 2010/07/12 09:50:56 exact Exp $

CORE_PATH=../..
CORE_DIR=../..
include $(CORE_DIR)/progs/Make.options

#=================================================
# variables
#=================================================

verbose=1
prec=52
oprec=20
n=20

args=$(verbose) $(prec) $(oprec) $(n)

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= dagstuhl-2017

MORE_TARGETS= $(TARGETS) \
	testIO testMpfr testBigFloat testSqrt \
	testCompatible testCompatibleNew \
	noGarbage


ALL= $(MORE_TARGETS) t 

default: $(TARGETS)

all: $(TARGETS) $(MORE_TARGETS)

t test: $(TARGETS)
	./dagstuhl-2017  ${args}

moretest: $(MORE_TARGETS)
	./testIO
	./testMpfr 0
	./testSqrt 997 10 
	./testCompatible 
	./testCompatibleNew
	./noGarbage 
	./testSqrt 997 10 $(verbose)
	./testCompatible 777.888 777888e-3 1

testIO: testIO.o

testMpfr: testMpfr.o

testSqrt: testSqrt.o

noGarbage: noGarbage.o

sqrt: sqrt.o

t1: t1.o

t: t.o

testCompatible: testCompatible.o

testCompatibleNew: testCompatibleNew.o

testBigFloat: testBigFloat.o

dagstuhl-2017: dagstuhl-2017.o

cur: testBigFloat

include $(CORE_DIR)/progs/Make.rules

