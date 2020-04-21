# Makefile for mesh/krawczyk
#
# This code gives implementation of the
# 	Krawczyk operator
# which can be used to find simple isolated roots for any 
# nonlinear system.  Currently, it is mostly used for
# polynomials.
#
# You can try interesting examples by typing
# 	
# 		> make eg1
# 		> make eg2
#		> ... 		
#
# TODO:
# (1) We ought to make this Makefile automatically
# 	compile in all three levels,
# 	and to do timing tests.
# (2) We should create examples that exceed the
# 	accuracy of Level 1, so that Level 2 is
# 	really needed.
# --Chee (May 2010)
#
#  AUTHOR: Narayan Kamath (Oxford)
#  Date: May, 2010
#  Since Core 2.0
#
# Core Library, $Id: Makefile,v 1.7 2010/07/14 14:50:25 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=static
LINKAGE=shared

# file extension
ext= 
ext=.c
ext=.cc
ext=.cpp

CORE_PATH=../../..
include ../Make.options

#=================================================
# VARIABLES 
#=================================================

DEPEND=
TARGETS=krawczyk

n=1

p=krawczyk
# program version is "v"
v=
# program is "pv"
pv=$(p)$(v)
ifile=plot

verbose=1


#=================================================
# TARGETS
#=================================================

default: $(TARGETS)

test: $(TARGETS)
	-@echo "We compile ./krawczyk but not run it in automatic tests,"
	-@echo "    because the program is interactive."
	-@echo "You may directly call \"make eg1\" or \"make eg2\", etc."

moretest: $(TARGETS)
	-@echo "We compile ./krawczyk but not run it in automatic tests,"
	-@echo "    because the program is interactive."
	-@echo "You may directly call \"make eg11\" or \"make eg20\", etc."

#=================================================
# Simple intersection.
#=================================================
# eg1 -- show the subdiv boxes (default), refined to 0.2
eg1:  $(TARGETS)
	./krawczyk x^3-x x^2y-7 -2 9 -2 9 0.001 0.2 
# eg2 -- show the subdiv boxes (default), refined to 0.2
eg2:  $(TARGETS)
	./krawczyk x^3-x x^2y-7 -2 10 -2 10 0.001 0.2

# eg2 -- do not show the subdiv boxes, refined to 0.2
eg3:  $(TARGETS)
	./krawczyk x^3-x x^2y-7 -2 10 -2 10 0.001 0.2

#=================================================
# A more complex intersection.
#=================================================
eg11: $(TARGETS)
	./krawczyk "((x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3)" "(x^2+y^2 -2)" -2 2 -2 2 0.0001 0.3 
eg11N: $(TARGETS)
	./krawczyk "((x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3)" "(x^2+y^2 -2)" -2 2 -2 2 0.0001 0.3 N
eg11K: $(TARGETS)
	./krawczyk "((x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3)" "(x^2+y^2 -2)" -2 2 -2 2 0.0001 0.3 K

eg20: $(TARGETS)
	./krawczyk "(-1 + x^5) + (-10*x^3)*y^2 + (5x)* y^4" "(5*x^4)*y^1 + (-10*x^2)*y^3 + y^5" -2 2 -2 2 0.0001 0.3
	
eg20N: $(TARGETS)
	./krawczyk "(-1 + x^5) + (-10*x^3)*y^2 + (5x)* y^4" "(5*x^4)*y^1 + (-10*x^2)*y^3 + y^5" -2 2 -2 2 0.0001 0.3 N
eg20K: $(TARGETS)
	./krawczyk "(-1 + x^5) + (-10*x^3)*y^2 + (5x)* y^4" "(5*x^4)*y^1 + (-10*x^2)*y^3 + y^5" -2 2 -2 2 0.0001 0.3 K

	

p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}


# Compile with different number types

pC cxy: cxy.o

krawczyk: krawczyk.o tmp-display.o
	${CXX} $(LDFLAGS) krawczyk.o tmp-display.o $(CORE_LIB) -o ${p}

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
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
ALL=$(TARGETS) $(MORE_TARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f tCurves${EXETYPE}

#=================================================
# Housekeeping
#=================================================

tar:
	-@tar cvf curves.tar \
		*cpp Makefile README

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes
