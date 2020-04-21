# Makefile for delaunay 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:23 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= dt2

MORETARGETS= dt4

default: $(TARGETS)

all: $(TARGETS) $(MORETARGETS)

test: $(TARGETS)
	./dt2    < inputs/i10    > outputs/i10.ps
	./dt2    < inputs/circ12 > outputs/circ12.ps
	./dt2 -l < inputs/i10    > outputs/i10.leda
	./dt2    < inputs/i100   > outputs/i100.ps

moretest: $(MORETARGETS)
	./dt4    < inputs/circ12 > outputs/circ20-dt4.ps
	./dt4    < inputs/i10    > outputs/i10-dt4.ps
	./dt4    < inputs/circ12 > outputs/circ12-dt4.ps

dt2: dt2.o

dt4: dt4.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

