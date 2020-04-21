# Makefile for mesh 
#
# Since Core Library 2.0
# Authors:  Long Lin, Shuxing Lu (April 2009), Jihun Yu (Jan 2010)
#
# 	$Id: Makefile,v 1.6 2010/10/22 20:33:10 exact Exp $

include ../Make.options

default:
	-@echo "This compile may fail if you do not have GLUT libraries"
	-${MAKE} -C PV default
	-${MAKE} -C cxy default

test: 

localtest: 
	-${MAKE} -C PV test
	-${MAKE} -C cxy test
	-${MAKE} -C ceval test
	-${MAKE} -C krawczyk test

moretest:

clean:
	-${MAKE} -C PV clean
	-${MAKE} -C cxy clean

veryclean:
	-${MAKE} -C PV veryclean
	${MAKE} -C cxy veryclean
