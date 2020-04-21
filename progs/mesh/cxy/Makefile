# Makefile for mesh/cxy
#
# WHAT THIS CODE DOES:
# 	It is an implementation of the Cxy Algorithm from Lin and Yap 
# 	(SoCG 2009).
#
# 	This code computes a piecewise-linear approximation of an implicitly given
# 	curve f(x,y)=0 restricted to a specifed box region B.
#
# 	Here f is a polynomial, but the theory could accept more general functions.
# 	We guarantee the correct topology of the output provided
# 	(1) f has no singularities inside B, and
# 	(2) f intersects the boundary of B transversally. 
#
#  	The curve may look zagged, but it is
# 	really a continuous line -- you must zoom into the image to see this.
# 	You can improve its closeness to the actual curve by specifying smaller
# 	epsilons.
#
# (1) COMPILING THE PROGRAM:
#	You can compile the program at various levels (level 1 is fast,
#	but possibly incorrect for difficult cases, level 3 is always
#	correct, but can be very slow).
#
# To make the program "cxy" at level n, do:
#
# 	> make veryclean	-- removes out previously compiled objects
# 	> make cxy LEV=n	-- n=1, 2 or 3
# 	> make eg10		-- run example 10
#
# To compile all three levels of accuracy in one shot, do:
#
# 	> make all		-- compiles cxy_1, cxy_2, cxy_3 (3 levels)
# 	> make testall		-- runs eg1 in all 3 levels
# 	> make LEV=2 eg4	-- runs eg4 in level 2
#
# (2) RUNNING THE PROGRAM:
#
# There are several simple examples (with optional arguments) 
# which you could try:
#
# 	> make eg 
# 	> make eg1 [xmin=-1.5] [xmax=-1.5] [showbox=0] ...
# 	> make eg2
# 	> ...
#
#  Currently, you can do "make eg<n>" (where <n>=1-6, 10-12).
#  Note that you can pan and zoom when viewing the displayed curve:
#  Feel free to create more targets of your own.
#
# (3) TODO:  We should create examples that exceed the
# 	accuracy of Level 1, so that Level 2 is
# 	really needed.
#
# --Chee (May 2010)
#
# Since Core Library Version 2.0
# 	$Id: Makefile,v 1.31 2011/02/09 22:41:14 exact Exp $

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
# use Make.options2 instead of Make.options:
include ../Make.options2

#=================================================
# RUNTIME PARAMETERS 
#=================================================

SUFFIX=

ifdef LEV
	SUFFIX=_$(LEV)
else
	LEV=2
	LEV=3
	LEV=1
endif

DEPEND=
TARGETS= cxy

CORE_LEVEL=2
CORE_LEVEL=3
CORE_LEVEL=1

p=cxy

ifile=plot

verbose=1
verbose=

xmin=-1.5
ymin=-1.5
xmax=1.5
ymax=1.5
maxsize=0.01
minsize=0.001
offset=0
showbox=0
showbox=1
display=1

#=================================================
# TARGETS
#=================================================

# We just compile, but do not run the tests because these
#     programs are interactive, NOT suitable for automatic testing
default: all

test: all
	-@echo "We compile ./cxy but not run it in automatic tests,"
	-@echo "    because the program is interactive."
	-@echo "You may directly call \"make eg1\" or \"make eg2\", etc."

moretest: all
	-@echo "We compile ./cxy but not run it in automatic tests,"
	-@echo "    because the program is interactive."
	-@echo "You may directly call \"make eg12\" or \"make eg20\", etc."

# These are interactive tests... not suitable for automatic testing!
no=11
no=2
no=1
testall: cxy1 cxy2 cxy3
	make LEV=1 eg$(no)
	make LEV=2 eg$(no)
	make LEV=3 eg$(no)

# ********************************************************************** 
# THESE FOUR TESTS USES THE SAME CURVE:
# 		x^2 -3x^4 -2y^2 +1	(a modified ellipse)
#
# COMMENTS: if the curve looks "wriggly" it is not an issue, as
# we introduce vertices in the middle of box edges.  We could
# improve this by simply linear interpolation.  But the isotopy
# type of the curve is guaranteed correct!
# You can zoom into the curve to see how the wriggly-ness arise.
# ********************************************************************** 
eg: eg4

# eg1 -- show the subdiv boxes (default), refined to 0.2
eg1:  $(TARGETS)
	./cxy$(SUFFIX) "(x^2 - 3x^4 - 2y^2 + 1)(x^3-xy)" \
		$(xmin) $(ymin) $(xmax) $(ymax) \
		$(maxsize) $(minsize) $(offset) $(showbox) $(display)

# eg2 -- do not show the subdiv boxes, refined to 0.2
eg2:  $(TARGETS)
	./cxy$(SUFFIX) "(x^2 - 3x^4 - 2y^2 + 1)(x^3-xy)" \
		$(xmin) $(ymin) $(xmax) $(ymax) \
		$(maxsize) $(minsize) $(offset) 0 $(display)

# eg3 -- max size is 10000 (this basically stops the subdivision when isotopy is achieved)
# 	Thus, this should be the fastest
eg3:  $(TARGETS)
	./cxy$(SUFFIX) "(x^2 - 3x^4 - 2y^2 + 1)(x^3 -xy)" \
		$(xmin) $(ymin) $(xmax) $(ymax) \
		$(maxsize) $(minsize) $(offset) $(showbox) 1

# eg4 -- like eg1, but min size is 0.01 (this forces subdivision until at least 0.01)
# 	This can be slow unless in Level 1!
# 	On the platform in the README file, takes 379 ms.
# 	You can zoom into the image to see the fine details.
eg4:  $(TARGETS)
	./cxy$(SUFFIX) "(x^2 - 3x^4 - 2y^2 + 1)(x^3 -xy)" \
		$(xmin) $(ymin) $(xmax) $(ymax) \
		0.01 $(minsize) $(offset) $(showbox) 1

# eg5 -- like eg1, but min size is 0.0001 (this forces subdivision until at least 0.0001)
# 	This can be very slow even in Level 1! 
# 	On the platform in the README file, it takes 20892 ms.
# 	You can zoom into the image to see the fine details.
eg5:  $(TARGETS)
	./cxy$(SUFFIX) "(x^2 - 3x^4 - 2y^2 + 1)(x^3 -xy)" \
		$(xmin) $(ymin) $(xmax) $(ymax) \
		$(maxsize) 0.0001 $(offset) $(showbox) 1

# ********************************************************************** 


# ********************************************************************** 
# THE NEXT CURVE IS the curve 0 = f(x,y) = sin (x^2) - cos (y^2).
#    BUT we do the TAYLOR EXPANSION to the third term:
#	f(x,y)  =  x^2 - x^6/6 + x^10/120 - (1 - y^4/2 + y^8/24)
#    But our parser accepts only integer coefficients, so we multiply by 120:
#
# NOTE:  Our Core 2 Design Paper used these images
# ********************************************************************** 
eg10:  $(TARGETS)
	./cxy$(SUFFIX) "120 x^2 - 20 x^6 + x^10 - (120 - 60 y^4 + 5 y^8)" \
		 -2 -2 2 2 1000 0.001 0 1
eg11:  $(TARGETS)
	./cxy$(SUFFIX) "120 x^2 - 20 x^6 + x^10 - (120 - 60 y^4 + 5 y^8)" \
		 -2 -2 2 2 0.02 0.001 0 0
# eg12: it simply ignores all the divisions in the coefficients,
# 	so computes a completely different curve.
eg12:  $(TARGETS)
	./cxy$(SUFFIX) "x^2 - x^6/6 + x^10/120 - (1 - y^4/2 + y^8/24)" \
		 -2 -2 2 2 0.01 0.001 0.8 1
# ********************************************************************** 

# ********************************************************************** 
# Example from Krawcyzk test for intersecting two curves
eg20:	$(TARGETS)
	./cxy$(SUFFIX) "(x^3 - x)(x^2 y - 7)" \
		 -2 -2 10 10 0.01 0.001 0 1
eg21:	$(TARGETS)
	./cxy$(SUFFIX) "x^2 y = 7" \
		 -2 -2 10 10 0.01 0.001 0 1
eg22:	$(TARGETS)
	./cxy$(SUFFIX) "x^3 - x = 0" \
		 -2 -2 10 10 0.01 0.001 0 1

# ********************************************************************** 

# ********************************************************************** 
# Example FP2.2 from Paper of Duracz and Konecny (actually, they credit it to
# 	the thesis of Vu (Ref.[15]).  From discussion of Chee and Luke Ong.
# 	This example has two underlying curves: a quartic curve
#	   (x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3 = 0
#	and a circle
#	   x^2+y^2 - 2 = 0
#
# 	Eg31 and 32 are correspond to FP2.2, but Eg30 is a partial version (where
# 	only the quartic curve is shown).
#
eg30:	$(TARGETS)
	./cxy$(SUFFIX) "(x^2 + y^2 +12x + 9)^2 = 4 (2 x + 3)^3" \
		 -2 -2 2 2 0.02 0.001 0 1
# NOTES on eg31 and eg32:
# 	The difference between these 2 examples is that the last argument
# 	(the 9-th argument, "draw_box" boolean flag)
# 	is 1 in eg31, and 0 in eg32. 
#	Using the re-factored version of Long/Shuxing code,
#	we run out of memory on eg31, but eg32 runs for 23 minutes and still did not
#	stop. This does not seem right, because its memory
#	remained stuck at 77M throughout most of its computation time.  
#	Does it mean that it keeps constructing the SAME boxes because Level 1
#	resolution is exhausted?   Only way to find out is to use Level 2. 
#	Eventually, we fixed the code, and we could run these examples quite reasonably:
#	eg31 takes 13723 ms.   A newer implementation by Narayan improves this to 805 ms
#	(17 times faster).
eg31:	$(TARGETS)
	./cxy$(SUFFIX) "((x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3)(x^2+y^2 -2)" \
		 -2 -2 2 2 0.03 0.001 0 1
eg32:	$(TARGETS)
	./cxy$(SUFFIX) "((x^2 + y^2 +12x + 9)^2 - 4 (2 x + 3)^3)(x^2+y^2 -2)" \
		 -2 -2 2 2 0.03 0.001 0 0
#
# Example FP2.3 from Paper of Duracz and Konecny (actually, they credit it to
# 	the thesis of Vu (Ref.[15]).
# 	This example has three curves:
#		x^3 + y^3 - 3xy = 0
#		x^2 + y^2 - 0.1 = 0
#		(x^2 + y^2)(y^2 + x(x+1)) - 4xy^2 = 0
#	On the cxy-new, eg33 takes 6183 ms.
eg33: $(TARGETS)
	./cxy$(SUFFIX) "(x^3 + y^3 - 3xy)(x^2 + y^2 - 0.1)((x^2 + y^2)(y^2 + x(x+1)) - 4xy^2)" \
		 -2 -2 2 2 0.03 0.001 0 1

# ********************************************************************** 
# The following examples are based on looking of complex and imaginary
# curves of univariate polynomials (over z).  If p(z)= u(x,y)+ i v(x,y),
# then we consider the curve uv(x,y)= u(x,y)*v(x,y).
# 
# eg41 is the golden ratio p(z)=z^2-z-1:
###################################################
#	In each case, we have corresponding targets to show
#	the u-, v-, and uv-curves in complex plane.
#
eg41: $(TARGETS)
	./cxy$(SUFFIX) "xy(-y + x^2-x-1)" \
		 -3 -3 3 3 0.01 0.0001 0 1 &
eg41a: $(TARGETS)
	./cxy$(SUFFIX) "xy(-y + x^2-x-1)" \
		 $(xmin) $(ymin) $(xmax) $(ymax) \
		 $(maxsize) $(minsize) $(offset) $(showbox)  &
eg41u: $(TARGETS)
	./cxy$(SUFFIX) \
		"xy(-y^2 + (x^2 - x - 1))" \
		 -3 -3 3 3 0.01 0.0001 0 1 &
eg41v: $(TARGETS)
	./cxy$(SUFFIX) \
		"x(2x - 1)y" \
		 -3 -3 3 3 0.01 0.0001 0 1 &
eg41uv: $(TARGETS)
	./cxy$(SUFFIX) \
		"x(y^3(-2x + 1) + (2x^3 - 3x^2 - x + 1)y)" \
		 -3 -3 3 3 0.01 0.0001 0 1 &

###################################################
# eg42 is lehmer's polynomial
# 	z^10 - z^9 + z^7 - z^6 + z^5 - z^4 + z^3 - z + 1
###################################################
#
# u-polynomial :
# -y^10 + (45x^2 - 9x)y^8 + (-210x^4 + 84x^3 - 7x + 1)y^6 + (210x^6 - 126x^5 + 35x^3 - 15x^2 + 5x - 1)y^4 + (-45x^8 + 36x^7 - 21x^5 + 15x^4 - 10x^3 + 6x^2 - 3x)y^2 + (x^10 - x^9 + x^7 - x^6 + x^5 - x^4 + x^3 - x + 1)
#
# v-polynomial :
# y^9(10x - 1) + (-120x^3 + 36x^2 - 1)y^7 + (252x^5 - 126x^4 + 21x^2 - 6x + 1)y^5 + (-120x^7 + 84x^6 - 35x^4 + 20x^3 - 10x^2 + 4x - 1)y^3 + (10x^9 - 9x^8 + 7x^6 - 6x^5 + 5x^4 - 4x^3 + 3x^2 - 1)y
#
# uv-polynomial :
# y^19(-10x + 1) + (570x^3 - 171x^2 + 9x + 1)y^17 + (-7752x^5 + 3876x^4 - 408x^3 - 136x^2 + 32x - 2)y^15 + (38760x^7 - 27132x^6 + 4284x^5 + 2380x^4 - 1120x^3 + 210x^2 - 21x + 1)y^13 + (-83980x^9 + 75582x^8 - 15912x^7 - 12376x^6 + 8736x^5 - 2730x^4 + 546x^3 - 78x^2 - 6x + 3)y^11 + (83980x^11 - 92378x^10 + 24310x^9 + 24310x^8 - 22880x^7 + 10010x^6 - 3003x^5 + 715x^4 + 110x^3 - 165x^2 + 45x - 3)y^9 + (-38760x^13 + 50388x^12 - 15912x^11 - 19448x^10 + 22880x^9 - 12870x^8 + 5148x^7 - 1716x^6 - 396x^5 + 990x^4 - 540x^3 + 108x^2 - 4x - 1)y^7 + (7752x^15 - 11628x^14 + 4284x^13 + 6188x^12 - 8736x^11 + 6006x^10 - 3003x^9 + 1287x^8 + 396x^7 - 1386x^6 + 1134x^5 - 378x^4 + 28x^3 + 21x^2 - 9x + 2)y^5 + (-570x^17 + 969x^16 - 408x^15 - 680x^14 + 1120x^13 - 910x^12 + 546x^11 - 286x^10 - 110x^9 + 495x^8 - 540x^7 + 252x^6 - 28x^5 - 35x^4 + 30x^3 - 20x^2 + 8x - 1)y^3 + (10x^19 - 19x^18 + 9x^17 + 17x^16 - 32x^15 + 30x^14 - 21x^13 + 13x^12 + 6x^11 - 33x^10 + 45x^9 - 27x^8 + 4x^7 + 7x^6 - 9x^5 + 10x^4 - 8x^3 + 3x^2 + x - 1)y
eg42: $(TARGETS)
	./cxy$(SUFFIX) "xy(-y + x^10 - x^9 + x^7 - x^6 + x^5 - x^4 + x^3 - x + 1)" \
		 -3 -3 3 3 0.01 0.00001 0 0 &

eg42a: $(TARGETS)
	./cxy$(SUFFIX) "-y^10 + (45x^2 - 9x)y^8 + (-210x^4 + 84x^3 - 7x + 1)y^6 + (210x^6 - 126x^5 + 35x^3 - 15x^2 + 5x - 1)y^4 + (-45x^8 + 36x^7 - 21x^5 + 15x^4 - 10x^3 + 6x^2 - 3x)y^2 + (x^10 - x^9 + x^7 - x^6 + x^5 - x^4 + x^3 - x + 1)"
		 -3 -3 3 3 0.03 0.001 0 1

###################################################
# eg43 is 5th root of unity polynomial
# 	(z-1)^5
###################################################
eg43: $(TARGETS)
	./cxy$(SUFFIX) "xy(y- (x-1)^5)" \
		 -3 -3 3 3 0.01 0.0001 0 1 &
eg43a: $(TARGETS)
	./cxy$(SUFFIX) "y^3(-2x + 1) + (2x^3 - 3x^2 - x + 1)y" \
		 -3 -3 3 3 $(maxsize) $(minsize) $(offset) $(showbox)  &

###################################################
# eg50 is a simple example on the intersection of two curves (illustration of
# 	curve arrangement paper of Yap and Sharma:
# 	f= x^2+y^2-1,   and g= y-x^2.
###################################################
eg50: $(TARGETS)
	./cxy$(SUFFIX) "(x^2 + y^2-1)(y- x^2)" \
		 -3 -3 3 3 \
		 0.01 0.0001 0 1 &
eg50a: $(TARGETS)
	./cxy$(SUFFIX) "(x^2 + y^2-1)(y- x^2)" \
		$(xmin) $(ymin) $(xmax) $(ymax) \
		$(maxsize) $(minsize) $(offset) $(showbox) $(display)

# ********************************************************************** 

p: $(p).o $(DEPEND)
	${CXX} $(LDFLAGS)  $< $(CORE_LIB) -o ${p}


# Compile with different number types

pC cxy: cxy.o

new cxy-new: cxy-new.o

cxy: box.o display.o
	${CXX} $(LDFLAGS) -DCORE_LEVEL=${LEV} \
	   box.o display.o cxy.o $(CORE_LIB) -o ${p}

#=================================================
# Rules for CORE_LEV 1, 2, and 3
#=================================================
cxy1: box_1.o display_1.o cxy_1

cxy2: box_2.o display_2.o cxy_2

cxy3: box_3.o display_3.o cxy_3

all: 
	make cxy1
	make cxy2
	make cxy3

CXY_DEP_lev1= box_1.o display_1.o
CXY_DEP_lev2= box_2.o display_2.o
CXY_DEP_lev3= box_3.o display_3.o

OPT_CXXFLAGS=$(CXXFLAGS)
# Include this flag if you want to run gcc with the
# highest level of optimization. This makes the compile
# slightly slower, but the code 2x or 3x faster
# OPT_CXXFLAGS=-O3 $(CXXFLAGS)

%_1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(OPT_CXXFLAGS) $(CORE_INC) $< -o $@

%_2.o: %.cpp
	${CXX} -c -DCORE_LEVEL=2 $(OPT_CXXFLAGS) $(CORE_INC) $< -o $@

%_3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(OPT_CXXFLAGS) $(CORE_INC) $< -o $@

%_1: %_1.o $(CXY_DEP_1)
	${CXX} $(LDFLAGS) $? $(CXY_DEP_lev1) \
	    $(CORE_LIB) $(COREX_LIB_LEVEL1) -o $@

%_2: %_2.o $(CXY_DEP_2)
	${CXX} $(LDFLAGS) $? $(CXY_DEP_lev2) \
	    $(CORE_LIB) $(COREX_LIB_LEVEL1) -o $@

%_3: %_3.o $(CXY_DEP_3)
	${CXX} $(LDFLAGS) $? $(CXY_DEP_lev3) \
	    $(CORE_LIB) $(COREX_LIB_LEVEL3) -o $@

	
t:	t.o
	${CXX} $(LDFLAGS) t.o $(CORE_LIB) -o $@
	
t1:	t
	./t "(x-1)^5" "data/fifthRootUnity" 1
	cp data/fifthRootUnity* ../../data/complexCurve

t2:	t
	./t "3x^3 -2 + 2x^2"  "data/cubic1" 1
	cp data/cubic1* ../../data/complexCurve

t3:	t
	./t "1-x+x^3-x^4+x^5-x^6+x^7-x^9+x^10" "data/lehmer" 1
	cp data/lehmer* ../../data/complexCurve

#=================================================
#	DEMOS:
#=================================================
-include make.demo

#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(OPT_CXXFLAGS) -DCORE_LEVEL=${LEV} \
	   $(CORE_INC) $< -o $@


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
	-@rm -f cxy${EXETYPE} cxy_1 cxy_2 cxy_3

#=================================================
# Housekeeping
#=================================================

tar:
	-@tar cvf curves.tar \
		*cpp Makefile README

save:
	-@ci -l -m"make save" *.cpp *.tcc *.h Makefile README Notes

