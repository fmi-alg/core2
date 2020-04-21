# Makefile for chullGraham 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:23 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= graham 
MORETARGETS= graham4 grahamIEEE

n=1
p=graham${n}

default: $(TARGETS)

more: $(MORETARGETS)

curr: ${p}
	./${p}     < inputs/i4    > outputs/i4-${n}.ps
	./${p}   -l  < inputs/i4    > outputs/i4-${n}.leda
	./${p}     < inputs/i10        > outputs/i10-${n}.ps
	./${p}  -l < inputs/i10        > outputs/i10-${n}.leda

test: $(TARGETS)
	./graham  -l < inputs/i4         > outputs/i4.leda
	./graham     < inputs/i10        > outputs/i10.ps
	./graham  -l < inputs/i10        > outputs/i10.leda
	./graham     < inputs/i19        > outputs/i19.ps
	./graham  -l < inputs/i19        > outputs/i19.leda
	./graham     < inputs/square1000 > outputs/square1000.ps
	./graham  -l < inputs/square1000 > outputs/square1000.leda

moretest: $(MORETARGETS)
	./grahamIEEE  -l < inputs/i4         > outputs/i4-ieee.leda
	./graham4 -l < inputs/i19         > outputs/i19w.leda
	./graham4    < inputs/i19w        > outputs/i19w.ps
	./graham4    < inputs/square2000w > outputs/square2000w.ps
	./graham4 -l < inputs/square2000w > outputs/square2000w.leda
	./graham4 -l < inputs/square1000w > outputs/square1000w.leda

graham: graham.o

graham4: graham4.o

grahamIEEE: grahamIEEE.o

graham${n}: graham${n}.o

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

