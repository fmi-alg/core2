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
# include ../Make.options
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
# Help
#=================================================
help:
	@echo "USAGE:  The main program is called mk.  Demos can be invoked thus:"
	@echo "         > make eg"
	@echo "         > make egX"
	@echo "     where X is replaced by 1,2,...,14."
	@echo "     Some demos have variants, such as eg6a, eg6b or eg6c."
	@echo "     LIST OF TARGETS:"
	@echo "	eg, eg1		-- intersecting a parabola and circle"
	@echo "	eg2		-- intersecting two cubic curves"
	@echo "	eg3      	-- circle and an interesting quartic curve"
	@echo "	eg4      	-- circle and an degree 9 curve"
	@echo "	eg5      	-- 5th roots of unity: real and complex part of z^5-1"
	@echo "	eg6      	-- Laguerre polynomial of degree 6 (6 real roots in [0,20]"
	@echo " eg7		-- some polynomial of degree 6"
	@echo " eg8		-- 10th roots of unity"
	@echo " eg9		-- 20th roots of unity"
	@echo " eg10		-- Hermite polynomial of degree 20"
	@echo " eg11		-- Chebyshev polynomial of degree 20"
	@echo " eg12		-- Wilkinson Polynomial of degree 20"
	@echo " eg13		-- Laguerre Polynomial of degree 4"
	@echo " eg14		-- Laguerre Polynomial of degree 5"
	@echo " ======================================================"

#=================================================
# Sample examples for testing only (cited from 
# krawczyk examples)
#=================================================

# Intersecting a parabola and the unit circle:
eg1: ${TARGETS}
	./mk --fxy "y-x^2" --gxy "x^2+y^2-1" \
    --x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

# Intersecting two cubic curves:
# 	f: x^3 -x = x(x-1)(x+1),    and g: x^2y -7.
# 	So solution is (-1,7), (1,7).
eg2: $(TARGETS)
	./mk --fxy "x^3-x" --gxy "x^2y-7" \
    --x_min -10 --x_max 10 --y_min -2 --y_max 15 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg3: $(TARGETS)
	./mk --fxy "((x^2 + y^2 + 12x + 9)^2 - 4(2x + 3)^3)" --gxy "(x^2 + y^2 - 2)" \
    --x_min -3 --x_max 3 --y_min -3 --y_max 3 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

eg4: ${TARGETS}
	./mk --fxy "(x^3 + y^3 - 3xy)(x^2 + y^2 - 0.1)((x^2 + y^2)(y^2 + x(x+1)) - 4xy^2)" --gxy "x^2 + y^2 - 2" \
    --x_min -3 --x_max 3 --y_min -3 --y_max 3 --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

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
