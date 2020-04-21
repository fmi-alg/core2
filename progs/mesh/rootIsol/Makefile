# Makefile for rootIsol
# 	Taken from Miranda folder
#
# (template Makefile for simple targets)
#
# Vikram
# April 15, 2013
#

CORE_PATH=../../..
include ../Make.options2

#=================================================
# User definition (put parameters manually here)
#=================================================
xmin = -10
xmax =  10
ymin = -10
ymax =  10
fxy  =  "y - (x-1)^2 - 1"
gxy  =  "x^2 + y^2 - 4"
minsize = 0.00001
maxsize = 0.01
maxgen  = 15 

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = mk

all: $(TARGETS)

test: $(TARGETS)
	./mk -f ${fxy} -g ${gxy} -x ${xmin} -X ${xmax} -y ${ymin} -Y ${ymax} -m ${minsize}

moretest: $(TARGETS)
# nothing to do

mk: main.o tmp-display.o
	${CXX} $(LDFLAGS) main.o tmp-display.o $(CORE_LIB) -o $@

#=================================================
# Sample examples for testing only (cited from 
# krawczyk examples)
#=================================================
eg: eg1

# Intersecting a parabola and the unit circle:
eg1: ${TARGETS}
	./mk --fxy "y-x^2" --gxy "x^2+y^2-1" \
    		--x_min -10 --x_max 10 --y_min -10 --y_max 10 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# Intersecting a parabola and the unit circle:
eg1a: ${TARGETS}
	./mk --fxy "y-x^3" --gxy "x^2+y^2-1" \
    		--x_min -10 --x_max 10 --y_min -10 --y_max 10 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# Intersecting two cubic curves:
eg2: $(TARGETS)
	./mk --fxy "x^3-x" --gxy "x^2y-7" \
    		--x_min -1.5 --x_max -0.5 --y_min 6 --y_max 8 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg3: $(TARGETS)
	./mk --fxy "((x^2 + y^2 + 12x + 9)^2 - 4(2x + 3)^3)" \
	    	--gxy "(x^2 + y^2 - 2)" \
    		--x_min -3 --x_max 3 --y_min -3 --y_max 3 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg4: ${TARGETS}
	./mk --fxy "(x^3 + y^3 - 3xy)(x^2 + y^2 - 0.1)((x^2 + y^2)(y^2 + x(x+1)) -4xy^2)" \
	    	--gxy "x^2 + y^2 - 2" \
    		--x_min -3 --x_max 3 --y_min -3 --y_max 3 \
		--minsize 0.001 --maxsize ${maxsize} --maxgen ${maxgen}

# Fifth roots of unity
eg5: ${TARGETS}
	./mk --fxy "(-1 + x^5) + (-10*x^3)*y^2 + (5x)* y^4" \
	    	--gxy "(5*x^4)*y^1 + (-10*x^2)*y^3 + y^5" \
    		--x_min -3 --x_max 3 --y_min -3 --y_max 3 \
		--minsize 0.01 --maxsize ${maxsize} --maxgen ${maxgen}

# REMARKS: laguerreN have N real positive roots (for N=6, these roots lie in [0,20])
# The following example illustrates this well:
eg6: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    		--x_min 0 --x_max 20 --y_min -10 --y_max 10 \
		--minsize 0.001 --maxsize ${maxsize} --maxgen ${maxgen}

# TO DEBUG: 
# The code get messed up if you do not provide an initial square box!
# It then reverts to the default polynomials!!!
eg6b: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    		--x_min 0 --x_max 20 --y_min -1 --y_max 1 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# This finds the one root of laguerre6 in the unit circle:
eg6c: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    		--x_min -1 --x_max 1 --y_min -1 --y_max 1 \
		--minsize 0.01 --maxsize ${maxsize} --maxgen 15
# This finds the two more root of laguerre6 (1.19 and 2.99)
eg6d: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    		--x_min 1 --x_max 3 --y_min -1 --y_max 1 \
		--minsize 0.0001 --maxsize ${maxsize} --maxgen 15
# This finds the no roots of laguerre6 
eg6e: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    		--x_min 3 --x_max 5 --y_min -1 --y_max 1 \
		--minsize 0.0002 --maxsize ${maxsize} --maxgen 15

eg7: ${TARGETS}
	./mk --poly data/somepoly6.pol \
		 --x_min -8 --x_max 8 --y_min -8 --y_max 8 \
		 --minsize 0.02 --maxsize ${maxsize} --maxgen ${maxgen}

eg8: ${TARGETS}
	./mk --poly data/rootsOfUnity10.pol \
    		--x_min -2 --x_max 2 --y_min -2 --y_max 2 \
		--minsize 0.001 --maxsize ${maxsize} --maxgen ${maxgen}

# Finding 20th roots of unity: 
eg9: ${TARGETS}
	./mk --poly data/rootsOfUnity20.pol \
		 --x_min -1.5 --x_max 1.5 --y_min -1.5 --y_max 1.5 \
		 --minsize 0.01 --maxsize ${maxsize} --maxgen ${maxgen}

eg10: ${TARGETS}
	./mk --poly data/hermite20.pol \
    		--x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} \
		--minsize 0.1 --maxsize ${maxsize} --maxgen ${maxgen}

eg11: ${TARGETS}
	./mk --poly data/chebyshev20.pol \
    		--x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg12: ${TARGETS}
	./mk --poly data/wilk20.pol \
    		--x_min -30 --x_max 30 --y_min -30 --y_max 30 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg13: ${TARGETS}
	./mk --poly data/laguerre4.pol \
    		--x_min -30 --x_max 30 --y_min -30 --y_max 30 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg14: ${TARGETS}
	./mk --poly data/laguerre5.pol \
    		--x_min -30 --x_max 30 --y_min -30 --y_max 30 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# THIS EXAMPLE HAS TWO STRAIGHT LINES, intersecting at origin and making
# 	a very acute angle (say: y=0 and 1000y=x)
# 	It stops instantly.
eg20: ${TARGETS}
	./mk --fxy "y" \
	    	--gxy "1000y - x" \
    		--x_min -4 --x_max 4 --y_min -4 --y_max 4 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}
# THIS is a perturbation of the TWO STRAIGHT LINES at the origin.
#	fxy = "y +y^2"    and    gxy = "1000y - x +x^3"
# This example is still OK (3 roots found, near (1,1), (0,0) and (-1,-1).
# If you increase the coefficient from 1000 to 10000, you get ambiguous regions.
# 	
eg20b: ${TARGETS}
	./mk --fxy "y +y^2" \
	    	--gxy "1000y - x +x^3" \
    		--x_min -4 --x_max 4 --y_min -4 --y_max 4 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# This example has two curves that are asymptotic and the Jacobian 
# coincices with one of the curves

eg21: ${TARGETS}
	./mk --fxy "xy-1" \
	    	--gxy "y" \
    		--x_min 40 --x_max 400 --y_min -1 --y_max 1 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# This example has a tangential intersection
# 	(will never terminate without cut-off)
eg22: ${TARGETS}
	./mk --fxy "y-x^2" \
	    	--gxy "y" \
    		--x_min -4 --x_max 4 --y_min -4 --y_max 4 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

temp: temp.o

m:
	gvim Makefile

#=================================================
# Rules
#=================================================
%: %.o
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
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# END Makefile
#=================================================
