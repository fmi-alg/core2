# Makefile for graham 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.3 2010/01/07 14:57:59 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= graham 
MORETARGETS= smallsum 

all: $(TARGETS)

test: $(TARGETS)
	./graham

moretest: $(MORETARGETS)
	./smallsum 

smalltest:
	./orourke
	./smallsum 

graham: graham.o

smallsum: smallsum.o

orourke: orourke.o

sum: sum.o

t: t.o

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

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

