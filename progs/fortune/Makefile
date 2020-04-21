# Makefile for fortune 
#
# Core Library 
# 	$Id: Makefile,v 1.1 2006/03/07 04:51:24 exact Exp $

include ../Make.options

default voronoi:
	-${MAKE} -C vor default

test: 
	-${MAKE} -C vor test

predicates:
	-${MAKE} -C pred default

pythagorean:
	-${MAKE} -C pythagoren default

moretest:
	-${MAKE} -C vor moretest
	-${MAKE} -C pred test
	-${MAKE} -C pred moretest

extratest:
	-${MAKE} -C pythagorean test

clean:
	-${MAKE} -C vor clean
	-${MAKE} -C pred clean

veryclean:
	-${MAKE} -C vor veryclean
	${MAKE} -C pred veryclean
