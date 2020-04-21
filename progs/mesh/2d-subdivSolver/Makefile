# Makefile for sub-division solver

#CORE_PATH=../../..
#include ../Make.options

#VAR=Opt
#CORE_INC=-I. -I$(CORE_PATH)/inc -I../arrangements-gui-copy
#CORE_LIB=-L$(CORE_PATH)/lib \
	 -lcore++$(VAR) 
#CORE_LIB=

#COREX_LIB_LEVEL=-lcoreex++$(VAR)_level
#COREX_LIB_LEVEL=


#=================================================
# Define target files (put all your files here)
#=================================================

p = main
default: ${p}

main = main

DEPENDENCES =  main.cpp src.h mk-inl.h gs-inl.h

#=================================================
# variables
#=================================================
f1 = "x^2+y^2-1"
f2 = "x-y^2"
xl = "-1"
xu = "1"
yl = "-1"
yu = "1"
eps = "0.0001"

#=================================================
# Rules
#=================================================

compile c main:	${DEPENDENCES}
	g++ -O3 -o main main.cpp  \
		 -I/usr/include -I/usr/local/include \
		-L/usr/lib/ -L/usr/local/lib \
	    -lgmp -lmpfr

compilegs cgs maings:	${DEPENDENCES}
	g++ -O3 -o maings main.cpp  \
		 -I/usr/include -I/usr/local/include \
		-L/usr/lib/ -L/usr/local/lib \
	    -lgmp -lmpfr -DGS

pqtry:	 priorityqTry.cpp
	g++ -O3 -o pqtry priorityqTry.cpp  \
		 -I/usr/include -I/usr/local/include \
		-L/usr/lib/ -L/usr/local/lib \
	    -lgmp -lmpfr

test t: ${main}
	./${main} ${f1} ${f2} ${xl} ${xu} ${yl} ${yu} ${eps}

# observe that if the initial region is [-100, 100] x [-100, 100] 
# it takes 15.287 second, while if the region is [-10, 10] x [-10, 10], 
# it takes 0.141 seconds to find the same roots
# This should be investigated
test1 t1: 
	make t \
		xl=-10 xu=10 \
		yl=-10 yu=10	
test1b t1b: 
	make t \
		xl=-100 xu=100 \
		yl=-100 yu=100	
test1e t1e: 
	make t \
		xl=0.3125 xu=0.625 \
		yl=0.625 yu=0.9375

test2 t2: 
	make t \
		f1="x^3y-xy+1" f2="xy^4+x^2y-105" \
		xl=-10 xu=10 \
		yl=-10 yu=10	
test2b t2b: 
	make t \
		f1="x^3y-xy+1" f2="xy^4+x^2y-105" \
		xl=-100 xu=100 \
		yl=-100 yu=100	
test2d t2d: 
	make t \
		f1="x^3y-xy+1" f2="xy^4+x^2y-105" \
		xl=-1000 xu=1000 \
		yl=-1000 yu=1000	

############################################################
# comparison between the 2 versions 
############################################################
tc:
	make t > t-MK.txt
	make t main=maings > t-GS.txt
t1c:
	make t1 > t1-MK.txt
	make t1 main=maings > t1-GS.txt
t1bc:
	make t1b > t1b-MK.txt
	make t1b main=maings > t1b-GS.txt

t2c:
	make t2 > t2-MK.txt
	make t2 main=maings > t2-GS.txt
t2bc:
	make t2b > t2b-MK.txt
	make t2b main=maings > t2b-GS.txt
t2dc:
	make t2d > t2d-MK.txt
	make t2d main=maings > t2d-GS.txt

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# End
#=================================================
