# Makefile for Miranda-Moore-Kioustelidis Test 
#
# (template Makefile for simple targets)
#
# July 15, 2011
#

#=================================================
#	Core Library Options
#=================================================

VAR=Debug
VAR=Opt


CORE_PATH=../../..
include ../Make.options2
#include ../Make.options
CXXFLAGS += -MMD -I./cxy -fpermissive

#=================================================
# User definition (put parameters manually here)
#      Of course, you can override them at command line. E.g.,
#
#      > make eg xmin=-20
#      
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

mk: main.o display.o box.o cxy/cxy.o cxy/box.o
	${CXX} $(LDFLAGS) main.o display.o box.o cxy/cxy.o cxy/box.o $(CORE_LIB) -o $@

#=================================================
# Help
#=================================================
help:
	@echo "USAGE:  The main program is called mk.  Demos can be invoked thus:"
	@echo "         > make eg"
	@echo "         > make egX"
	@echo "     where X is replaced by 1,2,...,14, or 20, 21."
	@echo "     Some demos have variants, such as eg6a, eg6b or eg6c."
	@echo "GUI HELP:"
	@echo "     --Select boxes using mouse buttons (control-Click on MacOS)"
	@echo "        and some info will be printed on screen"
	@echo "     --Then move up and down the subdivision tree with the up/down arrow."
	@echo "     --You can also use the keyboard:"
	@echo "            a=right, d=left, s=up, w=down, i=zoom in,  o=zoom out, q=quit."
	@echo "LIST OF TARGETS:"
	@echo "	eg, eg1	-- intersecting a parabola and circle"
	@echo "	eg2	-- intersecting two cubic curves"
	@echo "	eg3     -- circle and an interesting quartic curve"
	@echo "	eg4     -- circle and an degree 9 curve"
	@echo "	eg5     -- 5th roots of unity: real and complex part of z^5-1"
	@echo "	eg6     -- Laguerre polynomial of degree 6 (6 real roots in [0,20]"


#=================================================
# Sample examples for testing only (cited from 
# krawczyk examples)
#=================================================
# we try to choose some interesting instance:
eg: eg3

# Intersecting two cubic curves:
eg0: $(TARGETS)
	./mk --fxy "x^3-x" --gxy "x^3-x+1.1" \
	    --x_min -10 --x_max 10 --y_min -2 --y_max 15 \
	    --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# Intersecting a parabola and the circle (of radius sqrt(2)):
eg1: ${TARGETS}
	./mk --fxy "y-x^2" --gxy "x^2+y^2-2" \
	    --x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} \
	    --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# Intersecting two cubic curves:
eg2: $(TARGETS)
	./mk --fxy "x^3-x" --gxy "x^2y-7" \
	    --x_min -10 --x_max 10 --y_min -2 --y_max 15 \
	    --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg3: $(TARGETS)
	./mk --fxy "((x^2 + y^2 + 12x + 9)^2 - 4(2x + 3)^3)" --gxy "(x^2 + y^2 - 2)" \
	    --x_min -3 --x_max 3 --y_min -3 --y_max 3 \
	    --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg4: ${TARGETS}
	./mk --fxy "(x^3 + y^3 - 3xy)(x^2 + y^2 - 0.1)((x^2 + y^2)(y^2 + x(x+1)) - 4xy^2)" --gxy "x^2 + y^2 - 2" \
    --x_min -3 --x_max 3 --y_min -3 --y_max 3 \
    --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg5: ${TARGETS}
	./mk --fxy "(-1 + x^5) + (-10*x^3)*y^2 + (5x)* y^4" --gxy "(5*x^4)*y^1 + (-10*x^2)*y^3 + y^5" \
    --x_min -3 --x_max 3 --y_min -3 --y_max 3 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# REMARKS: laguerreN have N real positive roots (for N=6, these roots lie in [0,20])
# The following example illustrates this well:
eg6: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    --x_min -10 --x_max 20 --y_min -10 --y_max 20 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# DEBUG:
# The code get messed up if you do not provide an initial square box!
# It then reverts to the default polynomials!!!
eg6b: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    --x_min -5 --x_max 20 --y_min -5 --y_max 5 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg6c: ${TARGETS}
	./mk --poly data/laguerre6.pol \
    --x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} --minsize ${minsize} --maxsize ${maxsize} --maxgen 15


eg7: ${TARGETS}
	./mk --poly data/somepoly6.pol \
    --x_min -8 --x_max 8 --y_min -8 --y_max 8 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg8: ${TARGETS}
	./mk --poly data/nroots10.pol \
    --x_min -2 --x_max 2 --y_min -2 --y_max 2 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg9: ${TARGETS}
	./mk --poly data/nroots20.pol \
    --x_min -1.5 --x_max 1.5 --y_min -1.5 --y_max 1.5 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg10: ${TARGETS}
	./mk --poly data/hermite20.pol \
    --x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg11: ${TARGETS}
	./mk --poly data/chebyshev20.pol \
    --x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg12: ${TARGETS}
	./mk --poly data/wilk20.pol \
    --x_min -30 --x_max 30 --y_min -30 --y_max 30 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg13: ${TARGETS}
	./mk --poly data/laguerre4.pol \
    --x_min -30 --x_max 30 --y_min -30 --y_max 30 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg14: ${TARGETS}
	./mk --poly data/laguerre5.pol \
    --x_min -30 --x_max 30 --y_min -30 --y_max 30 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}
	

# THIS is a perturbation of the TWO STRAIGHT LINES at the origin.
#	fxy = "y +y^2"    and    gxy = "1000y - x +x^3"
# This example is still OK (3 roots found, near (1,1), (0,0) and (-1,-1).
# If you increase the coefficient from 1000 to 10000, you get ambiguous regions.
# 	
eg20b: ${TARGETS}
	./mk --fxy "y +y^2" \
	    	--gxy "10000000y - x +x^3" \
    		--x_min -4 --x_max 4 --y_min -4 --y_max 4 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# This example has two curves that are asymptotic and the Jacobian 
# coincides with one of the curves

eg21: ${TARGETS}
	./mk --fxy "xy-1" \
	    	--gxy "y" \
    		--x_min 40 --x_max 400 --y_min -1 --y_max 1 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# Jun 2013, Chee:
# Real and Complex parts from Hermite polynomial of degree 20 (frisco/hermite20.pol) 
# 	(these parts have been computed by our Convert Progrom)
eg30: ${TARGETS}
	./mk --fxy \
	"1048576y^20 + (-199229440x^2 + 99614720)y^18 + (5080350720x^4 - 15241052160x^2 \
	+ 3810263040)y^16 + (-40642805760x^6 + 304821043200x^4 - 457231564800x^2 + \
	76205260800)y^14 + (132089118720x^8 - 1849247662080x^6 + 6934678732800x^4 - \
	6934678732800x^2 + 866834841600)y^12 + (-193730707456x^10 + 4358940917760x^8 - \
	30512586424320x^6 + 76281466060800x^4 - 57211099545600x^2 + 5721109954560)y^10 \
	+ (132089118720x^12 - 4358940917760x^10 + 49038085324800x^8 - \
	228844398182400x^6 + 429083246592000x^4 - 257449947955200x^2 + \
	21454162329600)y^8 + (-40642805760x^14 + 1849247662080x^12 - 30512586424320x^10 \
	+ 228844398182400x^8 - 800955393638400x^6 + 1201433090457600x^4 - \
	600716545228800x^2 + 42908324659200)y^6 + (5080350720x^16 - 304821043200x^14 + \
	6934678732800x^12 - 76281466060800x^10 + 429083246592000x^8 - \
	1201433090457600x^6 + 1501791363072000x^4 - 643624869888000x^2 + \
	40226554368000)y^4 + (-199229440x^18 + 15241052160x^16 - 457231564800x^14 + \
	6934678732800x^12 - 57211099545600x^10 + 257449947955200x^8 - \
	600716545228800x^6 + 643624869888000x^4 - 241359326208000x^2 + \
	13408851456000)y^2 + (1048576x^20 - 99614720x^18 + 3810263040x^16 - \
	76205260800x^14 + 866834841600x^12 - 5721109954560x^10 + 21454162329600x^8 - \
	42908324659200x^6 + 40226554368000x^4 - 13408851456000x^2 + 670442572800)" \
	    	--gxy \
	"y^19(-20971520x) + (1195376640x^3 - 1793064960x)y^17 + (-16257122304x^5 + \
	81285611520x^3 - 60964208640x)y^15 + (81285611520x^7 - 853498920960x^5 \
	+ 2133747302400x^3 - 1066873651200x)y^13 + (-176118824960x^9 + \
	3170138849280x^7 - 16643228958720x^5 + 27738714931200x^3 - \
	10402018099200x)y^11 + (176118824960x^11 - 4843267686400x^9 + \
	43589409177600x^7 - 152562932121600x^5 + 190703665152000x^3 - \
	57211099545600x)y^9 + (-81285611520x^13 + 3170138849280x^11 - \
	43589409177600x^9 + 261536455065600x^7 - 686533194547200x^5 + \
	686533194547200x^3 - 171633298636800x)y^7 + (16257122304x^15 - \
	853498920960x^13 + 16643228958720x^11 - 152562932121600x^9 + \
	686533194547200x^7 - 1441719708549120x^5 + 1201433090457600x^3 - \
	257449947955200x)y^5 + (-1195376640x^17 + 81285611520x^15 - \
	2133747302400x^13 + 27738714931200x^11 - 190703665152000x^9 + \
	686533194547200x^7 - 1201433090457600x^5 + 858166493184000x^3 - \
	160906217472000x)y^3 + (20971520x^19 - 1793064960x^17 + 60964208640x^15 \
	- 1066873651200x^13 + 10402018099200x^11 - 57211099545600x^9 + \
	171633298636800x^7 - 257449947955200x^5 + 160906217472000x^3 - \
	26817702912000x)y" \
    		--x_min 40 --x_max 400 --y_min -1 --y_max 1 \
		--minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

#=================================================
# Housekeeping
#=================================================
m:
	gvim Makefile

#=================================================
# Rules
# 	First clean out all suffixes:
.SUFFIXES:
#	Then establish the suffixes we want for this file:
.SUFFIXES: .cpp .o
#=================================================

%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

# The following line is causing trouble with levels...
include Dependencies


#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o $(TARGETS)

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# END Makefile
#=================================================
