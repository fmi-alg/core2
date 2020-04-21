# Makefile for root isolation benchmarks.
#
# USING THIS FILE:
#
#	This directory runs the root isolation algorithms found in
#		../ceval and ../krawczyk.
# 	There are various demos: try
#
#		> make egN
#
# 	where N=0,1,2,3, etc.
#
#	Authors:
#	Narayan Kamath, Chee Yap, Irian Voiculescu (2010-2012)
#
#	Since Core 2.1
#
# Core Library, $Id: Makefile,v 1.10 2010/11/09 04:44:05 exact Exp $

CORE_PATH=../../..
include ../Make.options

#=================================================
# Define variables
#=================================================
# -lprofiler has problems on various platforms:
PROFILE=-lprofiler
PROFILE=

DATA=../../data/polynomials
FRISCO=../../data/frisco

VERB=
VERB=-v
v=1
verbose=$(v)

friscoCollection=../../data/Collections/friscoRealIntegerMini
friscoDir=../../data/frisco/

friscoFile=kam4.pol	# real sparse int
friscoFile=lar1.pol

###################################################
# arguments for ceval:
# frisco polynomial for target eg:
fpoly=chrmc23

xmin=1.318
ymin=1.632
xmax=1.320
ymax=1.634
minboxsize=0.000001

# compiling the two versions of predicates.cpp:
###################################################
ORIG=0

DEBUG=
DEBUG=-DDEBUG


#=================================================
# Targets:
#=================================================
#=================================================
# Standard CORE targets:
#=================================================
TARGETS = main_ceval main_newt

all: $(TARGETS)

test: $(TARGETS)
	python ./run_tests.py

tf test_frisco: $(TARGETS)
	python ./run_frisco.py $(friscoCollection)

tf1 test_frisco1: $(TARGETS)
	python ./run_frisco.py ../../data/Collections/friscoRealIntegerMini

tf2 test_frisco2: $(TARGETS)
	python ./run_frisco.py ../../data/Collections/friscoRealFloatMini

moretest: $(TARGETS)
	python ./run_moretests.py

help: main_ceval
	./main_ceval --help

#=================================================
# Demo Targets:
#=================================================
# The following is for visualization and interactive mode:
#  NOTE that you can
#  	zoom-in (i) or zoom-out (k or o), pan (w,a,s,d) 
#  in viewing the display.

# eg0 -- similar to eg, but using user specified bounding box
eg0: main_ceval
	./main_ceval --poly ./data/$(fpoly).pol --display \
		--x_max $(xmax) --x_min $(xmin) --y_min $(ymin) --y_max $(ymax) \
		--min_box_size $(minboxsize) 

# eg -- similar to eg0, but using computed root bounds to determine bounding box
eg: main_ceval
	./main_ceval --poly ./data/$(fpoly).pol --display --use_root_bounds

# eg1 (20 roots of unity)
eg1: main_ceval
	./main_ceval --poly ./data/nroots20.pol --display --use_root_bounds

# eg2
#
eg2 : main_ceval
	./main_ceval --poly ./data/somepoly6.pol --display --use_root_bounds

# 
# eg3 (laguerre20 polynomial) gives explicit bounding box to search for roots:
eg3 : main_ceval
	./main_ceval --poly ./data/laguerre20.pol --display \
		--x_min 0 --x_max 70  \
		--y_min -1 --y_max 1

# eg4 (laguerre20 polynomial) uses root bound calculations that
# ensure that all roots are found:
eg4 : main_ceval
	./main_ceval --poly ./data/laguerre20.pol --display --use_root_bounds

eg5 : main_ceval
	./main_ceval --poly ./data/chrma22.pol --display --use_root_bounds

# eg6 -- chrmc23.pol from the frisco Suite has 22 complex roots.  
# 	The default arguments for main_ceval in Level 1 only
# 	isolates 12 of these roots, leaving 72 ambiguous boxes, as this output shows:
#
# 	BOX STATISTICS: 
# 	   iters=43637,   includes=24,   splits=10909,
# 	      ambiguous=72,   exc_c0=32632,   exc_c1=0,time=270989
# 	      Operated on Bounding box : [-64 + (-64)i],[64 + (64)i]
#
# 	      --------------------------
# 	      Number of roots:12
# 	      m= [2.41699 + (-1.5459)i], r= 0.00585938
# 	      m= [2.93652 + (-0.754883)i], r= 0.00585938
# 	      m= [1.20801 + (-0.668945)i], r= 0.00585938
# 	      m= [1.23193 + (-0.252441)i], r= 0.00292969
# 	      m= [-0.391602 + (-1.68457)i], r= 0.00585938
# 	      m= [2.93652 + (0.754883)i], r= 0.00585938
# 	      m= [2.41699 + (1.5459)i], r= 0.00585938
# 	      m= [1.19092 + (0.000488281)i], r= 0.00292969
# 	      m= [1.23193 + (0.252441)i], r= 0.00292969
# 	      m= [1.20801 + (0.668945)i], r= 0.00585938
# 	      m= [0.999512 + (0.000488281)i], r= 0.00292969
# 	      m= [-0.391602 + (1.68457)i], r= 0.00585938
# 	      -------------Graphic----------------
#
#
# 	The ambiguous boxes are clustered in two regions, and the following  
# 	demo (eg6) begins with a bounding box that encloses the ambiguous
# 	boxes in one of these regions. 
# 	Unfortunately, LEVEL 1 accuracy will
# 	erroneously say there are NO roots (and NO ambiguous boxes)
eg6:	./main_ceval
	./main_ceval --poly ./data/chrmc23.pol --display \
		--x_min 1.318 --y_min 1.632 --x_max 1.320 --y_max 1.634 \
		--min_box_size 0.000001

# eg6a: THIS uses a smaller bounding box than eg6.   
# 	LEVEL 1 accuracy will erroneously say there are NO roots
# 	(and 4 ambiguous boxes).
eg6a:	./main_ceval
	./main_ceval --poly ./data/chrmc23.pol --display \
		--x_min 1.31942 --y_min 1.63314 --x_max 1.31948 --y_max 1.63320 \
		--min_box_size 0.000001

eg7 : main_ceval
	./main_ceval --poly ./data/wilk20.pol --display --use_root_bounds



##################################################
##################################################
main_ceval : main_ceval.o benchmark.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o 
	${CXX} $(LDFLAGS) benchmark.o main_ceval.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o $(CORE_LIB) -o $@

main_newt : main_newt.o benchmark.o ../krawczyk/tmp-display.o
	${CXX} $(LDFLAGS) benchmark.o main_newt.o \
		../krawczyk/tmp-display.o $(CORE_LIB) $(PROFILE) -o $@

complexCurves c: complexCurves.o benchmark.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o 
	${CXX} $(LDFLAGS) benchmark.o complexCurves.o ../ceval/algorithm.o \
		../ceval/predicates.o ../ceval/tmp-display.o $(CORE_LIB) -o $@

# Examples for complexCurves
c1:	complexCurves
	complexCurves --poly $(DATA)/simplePolynomials/rootsOfUnity2.pol $(VERB)

c2:	complexCurves
	complexCurves --poly $(DATA)/simplePolynomials/identity.poL $(VERB)

c3:	complexCurves
	complexCurves --poly $(FRISCO)/hermite20.pol $(VERB)

c4:	complexCurves
	complexCurves --poly $(DATA)/specialPolynomials/goldenratio.pol $(VERB)

c5:	complexCurves
	complexCurves --poly ./data/hermite20.pol  $(VERB)

c6:	complexCurves
	complexCurves --poly $(DATA)/simplePolynomials/rootsOfUnity3.pol $(VERB)

cv:	convertPoly.o 
	${CXX} $(LDFLAGS) convertPoly.o $(CORE_LIB) -o $@

cv0:	convertPoly
	./convertPoly "FRISCO" "$(friscoDir)$(friscoFile)" "output/friscoOutput" 1

cv1:	convertPoly
	./convertPoly "DIRECT" "(x-1)^5" "output/fifthRootUnity" 1
	cp output/fifthRootUnity* ../../data/complexCurve

cv2:	convertPoly
	./convertPoly "DIRECT" "1-x+x^3-x^4+x^5-x^6+x^7-x^9+x^10" "output/lehmer" 1
	cp output/lehmer* ../../data/complexCurve

cv3:	convertPoly
	./convertPoly "HUMAN_READABLE" "data2/fifthRootUnity.poly" "output/fifthRootUnity" 1
	cp output/fifthRootUnity* ../../data/complexCurve

cv4:	convertPoly
	./convertPoly "HUMAN_READABLE" "data2/lehmer.poly" "output/lehmer" 1
	cp output/lehmer* ../../data/complexCurve

cv5:	convertPoly
	./convertPoly "HUMAN_READABLE" "data2/golden.poly" "output/golden" 1
	cp output/golden* ../../data/complexCurve

cv6:	convertPoly
	./convertPoly "FRISCO" "data/somepoly6.pol" "output/somepoly6.pol" 1
	cp output/somepoly6* ../../data/complexCurve

cv7:	convertPoly
	./convertPoly "FRISCO" "data/hermite20.pol" "output/hermite20.pol" 1
	cp output/hermite20* ../../data/complexCurve
	#
# this reads in a sparse polynomial with floating point coefficients
cv8:	convertPoly
	./convertPoly "FRISCO" "../../data/frisco/lar1.pol" "output/lar1.pol" 1


t:	t.o
	${CXX} $(LDFLAGS) t.o $(CORE_LIB) -o $@

t1:	t
	./t "(x-1)^5" "output/fifthRootUnity" 1
	cp output/fifthRootUnity* ../../data/complexCurve

t2:	t
	./t "3x^3 -2 + 2x^2"  "output/cubic1" 1
	cp output/cubic1* ../../data/complexCurve

t3:	t
	./t "1-x+x^3-x^4+x^5-x^6+x^7-x^9+x^10" "data/lehmer" 1
	cp data/lehmer* ../../data/complexCurve

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -DORIGINAL=$(ORIG) $(DEBUG) -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

#old:	${CXX} -c $(CXXFLAGS) -O3 $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
	-@test -z "../ceval/*.o" || rm -f ../ceval/*.o
	-@test -z "../krawczyk/*.o" || rm -f ../krawczyk/*.o

m:
	gvim Makefile

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "t" || rm -f t
	-@test -z "convertPoly" || rm -f convertPoly
	-@test -z "cv" || rm -f cv

#=================================================
# End
#=================================================

