# Makefile for core2paper 
#
# Core Library 
# 	$Id: Makefile,v 1.1 2010/05/14 09:10:32 exact Exp $

include ../Make.options

default :
	-${MAKE} -C sumProd default
	-${MAKE} -C transcendental default	

sumProd:
	-${MAKE} -C sumProd default

transcendental :
	-${MAKE} -C transcendental default	
	
test: 
	-${MAKE} -C sumProd test
	-${MAKE} -C transcendental test

moretest: 
	-${MAKE} -C sumProd moretest	
	-${MAKE} -C transcendental moretest	
	
clean:
	-${MAKE} -C sumProd clean
	-${MAKE} -C transcendental clean

veryclean:
	-${MAKE} -C sumProd veryclean
	-${MAKE} -C transcendental veryclean
