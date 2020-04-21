# Makefile for hyperGeom 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.3 2010/07/14 13:20:49 exact Exp $

include ../Make.options

PREC 	= 80
f	= sin
f	= cos
f	= erf
f	= log

#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS= demo 

MORE_TARGETS= demo demoPi

default: $(TARGETS)

# COMMENT: for Core2, as noted in README file, there are some
#	bugs in hypergeom, and it is strangely slow for arccos/arcsin.
# 	Hence we cut down the precision to make the test run
#	in reasonable time.
test: $(TARGETS)
	./demo sin 0.01 1.57 0.4 $(PREC) > output/sin.out
	./demo cos 0.01 1.57 0.4 $(PREC) > output/cos.out
	./demo tan 0.01 1.57 0.4 $(PREC) > output/tan.out
	./demo cot 0.01 1.57 0.4 $(PREC) > output/cot.out
	./demo arcsin 0.02 0.40 0.2 $(PREC) > output/arcsin.out
	./demo arccos 0.1 0.20 0.2 $(PREC) > output/arccos.out
	./demo arctan 0.01 0.40 0.2 $(PREC) > output/arctan.out
	#./demo exp 0.01 2.00 0.7 $(PREC) > output/exp.out
	./demo erf 0.01 2.00 0.7 $(PREC) > output/erf.out
	./demo log 1.00 2.00 0.7 $(PREC) > output/log.out

smalltest: $(TARGETS)
	./demo $(f) 1.00 2.00 0.1 $(PREC) > output/log.out

moretest: $(MORE_TARGETS)
	#./demoPi 0.1 100 > output/pi_100.out
	#./demoPi -0.7 200 > output/pi_200.out
	./demoPi -0.7 80 > output/pi_80.out
	./demo sin 0.01 3.14 0.07 $(PREC) > output/sin.out2
	./demo cos 0.01 3.14 0.07 $(PREC) > output/cos.out2
	./demo tan 0.01 3.14 0.07 $(PREC) > output/tan.out2
	#./demo cot 0.01 3.14 0.07 $(PREC) > output/cot.out2
	./demo arcsin 0.01 1.00 0.07 $(PREC) > output/arcsin.out2
	# arccos(0.71000) takes forever!!!  WHY?  
	#./demo arccos 0.01 1.00 0.07 $(PREC) > output/arccos.out2
	./demo arccos 0.01 0.65 0.07 $(PREC) > output/arccos.out2
	./demo arctan 0.01 1.00 0.07 $(PREC) > output/arctan.out2
	#./demo exp 0.01 3.14 0.07 $(PREC) > output/exp.out2
	./demo erf 0.01 3.14 0.07 $(PREC) > output/erf.out2
	#./demo log 1.01 3.14 0.07 $(PREC) > output/log.out:

# remark: should test at negative arguments!

compare: $(MAPLE_TARGETS) test
	diff output/sin.out output/maple-sin.out
	diff output/cos.out output/maple-cos.out
	diff output/tan.out output/maple-tan.out
	diff output/cot.out output/maple-cot.out
	diff output/arcsin.out output/maple-arcsin.out
	diff output/arccos.out output/maple-arccos.out
	diff output/arctan.out output/maple-arctan.out
	diff output/exp.out output/maple-exp.out
	diff output/erf.out output/maple-erf.out
	diff output/log.out output/maple-log.out

demo: demo.o

demoPi: demoPi.o 

verify: verify.o

# testing arccos...
a:
	./demo arccos 0.01 0.72 0.07 $(PREC) > output/arccos.out2
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
	-${MAKE} -C const clean
	-${MAKE} -C timing clean

EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-${MAKE} -C const veryclean
	-${MAKE} -C timing veryclean

rcs:
	-@ci -l -m"make rcs" *.cpp *.h Makefile README

tar:
	-@tar cvf T.tar *.cpp Makefile TODO *.h timing README const other
