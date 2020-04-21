# Makefile for tutorial 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.3 2010/05/18 10:34:13 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================

p=simple
p=io
p=delaunay
p=BF_output
p=one
p=output_prec
p=IO_prec

TARGETS= delaunay io 

MORE_TARGETS= prog1 prog2 prog3 prog4 prog5 prog6 prog7 \
	prog10 prog11 prog12 \
	one simple 

OBSOLETE_TARGETS= prog8 prog9 BF_output

default: $(TARGETS)

test: $(TARGETS)
	./delaunay 7
	./io

moretest: $(MORE_TARGETS)
	./prog1
	./prog2
	./prog3
	./prog4
	./prog5
	./prog6
	./prog7
	./prog10
	./prog11
	./prog12
	./simple
	./one

progs: $(MORE_TARGETS)

p: $(p).cpp
	${CXX} -c $(CXXFLAGS) $(CORE_INC) ${p}.cpp -o ${p}.o
	${CXX} $(LDFLAGS) ${p}.o $(CORE_LIB) -o ${p}

time: $(TARGETS)

simple: simple.o

Prog1: Prog1.o

io: io.o

io1: io1.o

delaunay: delaunay.o

prog1: prog1.o

prog2: prog2.o

prog3: prog3.o

prog4: prog4.o

prog5: prog5.o

prog6: prog6.o

prog7: prog7.o

prog8: prog8.o

prog9: prog9.o

prog10: prog10.o

prog10: prog11.o

prog10: prog12.o

BF_output: BF_output.o

output_prec: output_prec.o

IO_prec: IO_prec.o

one: one.o


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
MORE_EXEPROGS=$(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "$(MORE_EXEPROGS)" || rm -f $(MORE_EXEPROGS)

