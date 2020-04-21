# Makefile for hyperGeom 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:54:38 exact Exp $

CORE_PATH=../../..
include ../../Make.options

PREC 	= 500

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= log2 pi

all: $(TARGETS)

test:
	./log2 $(PREC)
	./pi $(PREC)
	
log2: log2.o

pi: pi.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -I../ $< -o $@

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

rcs:
	-@ci -l -m"make rcs" *.cpp *.h Makefile README
