# Makefile for hyperGeom 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:24 exact Exp $

include ../Make.options

PREC 	= 300
f	= sin
f	= cos
f	= erf
f	= log

#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS= po1

default: $(TARGETS)

po1: po1.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# HOUSEKEEPING
#=================================================

clean:
	-@test -z "*.o" || rm -f *.o

EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

rcs:
	-@ci -l -m"make rcs" *.cpp *.h Makefile README

tar:
	-@tar cvf T.tar *.cpp Makefile TODO *.h timing README const other
