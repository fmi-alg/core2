# Makefile for sample 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.3 2006/10/31 16:29:21 exact Exp $

#=================================================
# If you set VAR below, it would override the default values
#	for VAR in ../Make.options or ../../Make.config.
# This is useful for deciding which "variant" of the Core Library
#	you want to use each time.
#=================================================

VAR=Debug
VAR=

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= sample

all: $(TARGETS)

test: $(TARGETS)
	./sample

moretest:
# nothing to do

testgmp:
	-@test -z "gmptest.o" || rm -f gmptest.o
	-@test -z "gmptest${EXETYPE}" || rm -f gmptest${EXETYPE}
	$(CXX) $(CORE_INC) gmptest.cpp -L../../gmp/lib -lgmp -o gmptest
	./gmptest

testmpfr:
	-@test -z "mpfrtest.o" || rm -f mpfrtest.o
	-@test -z "mpfrtest${EXETYPE}" || rm -f mpfrtest${EXETYPE}
	$(CXX) $(CORE_INC) mpfrtest.cpp -L../../mpfr/lib -lmpfr \
		-L../../gmp/lib -lgmp -o mpfrtest
	./mpfrtest

sample: sample.o

t_mpfr: t_mpfr.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@test -z "gmptest${EXETYPE}" || rm -f gmptest${EXETYPE}
