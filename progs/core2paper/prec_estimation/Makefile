# Makefile for core2paper/prec_estimation 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2010/05/14 08:19:50 exact Exp $

CORE_PATH=../../..
include ../../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= Opt_Prec_Estimation

all: $(TARGETS)

test: $(TARGETS)
	./Opt_Prec_Estimation 4000 0
	./Opt_Prec_Estimation 4000 1
	./Opt_Prec_Estimation 40000 0
	./Opt_Prec_Estimation 40000 1

moretest: $(TARGETS)
	./Opt_Prec_Estimation 100000 0
	./Opt_Prec_Estimation 100000 1

example1: $(TARGETS)
	./Opt_Prec_Estimation 100 0
	./Opt_Prec_Estimation 1000 0
	./Opt_Prec_Estimation 10000 0
	./Opt_Prec_Estimation 100000 0

example2: $(TARGETS)
	./Opt_Prec_Estimation 100 1
	./Opt_Prec_Estimation 1000 1
	./Opt_Prec_Estimation 10000 1
	./Opt_Prec_Estimation 100000 1

example3: $(TARGETS)
	./Opt_Prec_Estimation 100 0 1
	./Opt_Prec_Estimation 1000 0 1
	./Opt_Prec_Estimation 10000 0 1
	./Opt_Prec_Estimation 100000 0 1

example4: $(TARGETS)
	./Opt_Prec_Estimation 100 1 1
	./Opt_Prec_Estimation 1000 1 1
	./Opt_Prec_Estimation 10000 1 1
	./Opt_Prec_Estimation 100000 1 1

Opt_Prec_Estimation: Opt_Prec_Estimation.o

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
ALL=$(TARGETS) $(MORE_TARGETS) Opt_Prec_Estimation
EXEPROGS=$(ALL:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)


