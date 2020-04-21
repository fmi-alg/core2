# Makefile for testIdent 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:26 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= testIdent 
MORETARGETS= testBound

all: $(TARGETS)

test: $(TARGETS)
	./testIdent

moretest: $(TARGETS) $(MORETARGETS)
	./testBound
	./testIdent 1
	./testBound 1

testIdent: testIdent.o

testBound: testBound.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

