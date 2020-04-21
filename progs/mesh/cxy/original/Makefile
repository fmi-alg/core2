# Makefile for Curves 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.4 2010/06/15 22:24:59 exact Exp $

#=================================================
# VARIABLES
#=================================================
#CHOOSE THE LINKAGE 
LINKAGE=shared
LINKAGE=static

# file extension
ext= 
ext=.c
ext=.cc
ext=.cpp
#CHOOSE THE VARIANT Core Library you need
#VAR=GCC31
#VAR=Debug
#VAR=
#VAR=Opt

CORE_PATH=../../../..
include ../../Make.options

#=================================================
# VARIABLES 
#=================================================

DEPEND=
TARGETS= CXY

n=1
n=

p=CXY
# program version is "v"
v=
# program is "pv"
pv=$(p)$(v)
ifile=plot

verbose=1
verbose=

#=================================================
# TARGETS
#=================================================

default: $(TARGETS)


# ********************************************************************** 
# THESE FOUR TESTS USES THE SAME CURVE:
# 		x^2 -3x^4 -2y^2 +1	(a modified ellipse)
# ********************************************************************** 
# eg1 -- show the subdiv boxes (default), refined to 0.2
eg eg1:  $(TARGETS)
	./CXY "x^2 - 3x^4 - 2y^2 + 1" -1.5 -1.5 1.5 1.5 0.1 0.0001 0 1

# eg2 -- do not show the subdiv boxes, refined to 0.2
eg2:  $(TARGETS)
	./CXY "x^2 - 3x^4 - 2y^2 + 1" -1.5 -1.5 1.5 1.5 0.1 0.0001 0 0

# eg3 -- show the subdiv boxes, just show its isotopy type
eg3:  $(TARGETS)
	./CXY "x^2 - 3x^4 - 2y^2 + 1" -1.5 -1.5 1.5 1.5 10000 0.0001 0.01 

# eg4 -- like eg1, but refine to 0.01 (slow!)
eg4:  $(TARGETS)
	./CXY "x^2 - 3x^4 - 2y^2 + 1" -1.5 -1.5 1.5 1.5 0.01 0.0001 0.01 

# eg5 -- like eg1, but refine to 0.01 (slow!)
eg5:  $(TARGETS)
	./CXY "x^2 - 3x^4 - 2y^2 + 1" -1.5 -1.5 1.5 1.5 0.01 0.0001 0.01 0
# ********************************************************************** 


# ********************************************************************** 
# THE NEXT CURVE IS the curve 0 = f(x,y) = sin (x^2) - cos (y^2).
#    BUT we do the TAYLOR EXPANSION to the third term:
#	f(x,y)  =  x^2 - x^6/6 + x^10/120 - (1 - y^4/2 + y^8/24)
#    But our parser accepts only integer coefficients, so we multiply by 120:
#
# NOTE:  Our Core 2 Paper used these images
# ********************************************************************** 
eg10:  $(TARGETS)
	./CXY "120 x^2 - 20 x^6 + x^10 - (120 - 60 y^4 + 5 y^8)" \
		 -2 -2 2 2 1000 0.001 
eg11:  $(TARGETS)
	time ./CXY "120 x^2 - 20 x^6 + x^10 - (120 - 60 y^4 + 5 y^8)" \
		 -2 -2 2 2 0.02 0.001 0 0
# eg12: it simply ignores all the divisions in the coefficients,
# 	so computes a different curve.
eg12:  $(TARGETS)
	./CXY "x^2 - x^6/6 + x^10/120 - (1 - y^4/2 + y^8/24)" \
		 -2 -2 2 2 0.01 0.001 0.8 0
# ********************************************************************** 

eg31:   $(TARGETS)
	./CXY "((x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3)(x^2+y^2 -2)" \
                 -2 -2 2 2 0.03 0.001 0 1



p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}


# Compile with different number types

pC CXY: CXY.o

new cxy-new: cxy-new.o

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
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
ALL=$(TARGETS) $(MORE_TARGETS)
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f tCurves${EXETYPE} cxy-new

#=================================================
# Housekeeping
#=================================================

tar:
	-@tar cvf curves.tar \
		*cpp Makefile README

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes

