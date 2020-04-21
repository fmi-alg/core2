# Makefile for third_party 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.9 2010/11/01 02:59:47 exact Exp $

CORE_PATH=../..
include Make.options

#=================================================
# Variables
#=================================================

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = tclap

all: $(TARGETS)

test: $(TARGETS)
	

moretest: $(TARGETS)
# nothing to do

#=================================================
# TCLAP:
#=================================================
tclap: tclap-example
	./tclap-example --help

tclap1: tclap-example
	./tclap-example -n Yap

tclap2: tclap-example
	./tclap-example -n Yap -r

tclap-example: tclap-example.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "tclap-example" || rm -f tclap-example

