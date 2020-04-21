# Makefile for rootIsol
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.6 2010/11/23 04:28:34 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= t 
TARGETS= testSturm 
TARGETS= tDescartes 
TARGETS= tRootIsol 

MORETARGETS= testSturm tDescartes tRootIsol

all: $(TARGETS)

test: $(TARGETS)
	./tRobust.sh

moretest: $(MORETARGETS)

eval: tEval
	./tEval

# help
help:
	-@echo "root isolation targets:"
	-@echo "Possible targets:"
	-@echo "    eval, t, tmr, tm"
	-@echo "Generic targets:"
	-@echo "    all (=testSturm, tDescartes, tRootIsol), test"

# nothing to do

t: t.o  

tmr: tMultRoots
	./tMultRoots

tm: tMult
	./tMult

tMultRoots: tMultRoots.o

tMult: tMult.o

tDescartes:tDescartes.o

testSturm:testSturm.o

tRootIsol:tRootIsol.o

tEval:tEval.o

tt: tt.o
	./tt

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

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
ALL=$(TARGETS) $(MORETARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

