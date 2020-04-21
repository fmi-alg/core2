# Makefile for pi 
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:25 exact Exp $

# Set parameters before calling Make.options

VAR=Debug
VAR=

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================


TARGETS= newPi 

MORETARGETS= pi brent

EXTRATARGETS= pi2 pi3

default: $(TARGETS)

test: $(TARGETS) 
	./newPi 6644 0
	./newPi 6644 1

moretest: $(MORETARGETS)
	./pi 3500 0 0
	./brent 6644 1

extratest: $(EXTRATARGETS)
	./pi2 6644
	./pi3 6644

testbrent: brent
	./brent 100

time: $(TARGETS) $(MORETARGETS)
	time ./pi 6644 0 0 >/dev/null
	time ./pi2 6644 >/dev/null
	time ./pi3 6644 >/dev/null
	time ./brent 6644 >/dev/null
	time ./brent 6644 1 >/dev/null
	

bigtest: pi 
	./pi 15000 0 0

pi: pi.o

newPi: newPi.o

pi2: pi2.o

pi3: pi3.o

brent: brent.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

