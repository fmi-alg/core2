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

verbose=
verbose=1

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= testIO testMpfr testSqrt testCompatible testCompatibleNew \
	testBigFloat noGarbage

MORE_TARGETS= $(TARGETS)

ALL= $(MORE_TARGETS) t 

default: $(TARGETS)

all: $(TARGETS) $(MORE_TARGETS)

test: $(TARGETS)
	./testIO
	./testMpfr 0
	./testSqrt 997 10 
	./testCompatible 
	./testCompatibleNew
	./testBigFloat 
	./noGarbage 

moretest: $(MORE_TARGETS)
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

cur: testBigFloat

include $(CORE_DIR)/progs/Make.rules

