# Makefile for demos
#
# DEMO LIST:
# 	scheinerman
# 	simple
# 	golden
# 	pointOnPlane
# 	pointOnCircle
#
# You can call this makefile with LEV=1, 2 or 3
# in compiling these demos:  E.g.
#
# 	> make LEV=2 pointOnPlane
# 	> pointOnPlane 
# 	   ... runs the program in level 2 ...
#
# To redo the test in Level 1:
#
# 	> touch pointOnPlane.cpp  
# 	> make LEV=1 pointOnPlane
# 	> pointOnPlane
# 	   ... runs the program in Level 1.
#
#
# Since Core Library 1.6.
# Core Library, $Id: Makefile,v 1.7 2010/05/27 15:16:09 exact Exp $

include ../Make.options

#=================================================
# Define variables 
#=================================================

p=simple
p=golden
p=pointOnLine
p=scheinermanx
p=scheinerman
p=pointOnPlane

ifile=i
arg= < inputs/${ifile}
arg=

LEV=1
LEV=2
LEV=3
TARGETS= scheinerman${LEV} pointOnPlane${LEV}
MORETARGETS= simple${LEV} golden${LEV}
EXTRA=scheinermanx${LEV}
ALLTARGETS= ${TARGETS} ${MORETARGETS}

#=================================================
# Define targets
#=================================================

default: $(TARGETS)
	
# demo for Dagstuhl (2017)
dag dagstuhl: 
	./pointOnPlane1
	./pointOnPlane3

test2: $(TARGETS)
	./scheinerman${LEV}

moretest: $(MORETARGETS)
	./simple${LEV}
	./golden${LEV}

all: scheinerman${LEV} pointOnPlane${LEV} simple${LEV} golden${LEV}
	./scheinerman${LEV}
	./pointOnPlane${LEV}
	./simple${LEV}
	./golden${LEV}

p: ${p}
	./${p} ${arg}

${p}: ${p}.o

t1 pointOnCircle${LEV}: pointOnCircle.o

t2 pointOnPlane${LEV}: pointOnPlane.o

t3 pointOnLine${LEV}: pointOnLine.o

t4 scheinerman${LEV}: scheinerman.o

t4x scheinermanx${LEV}: scheinermanx.o
	${CXX} $(LDFLAGS) $< -lcurses -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

golden${LEV}: golden.o

simple${LEV}: simple.o

t${LEV}: t.o

#=================================================
# Rules
#=================================================
%${LEV}: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

%: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=${LEV} $(CORE_INC) \
	    -DCORE_LEVEL_1_USE_WRAPPERS $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

m:
	gvim Makefile
#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

vclean veryclean: clean
	echo "EXEPROGS=" $(EXEPROGS)
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)
	-@rm -f simple1$(EXETYPE) simple3$(EXETYPE) simple$(EXETYPE)
	-@rm -f golden1$(EXETYPE) golden3$(EXETYPE) golden$(EXETYPE)
	-@rm -f scheinerman1$(EXETYPE) scheinerman3$(EXETYPE) scheinerman$(EXETYPE)
	-@rm -f pointOnPlane1$(EXETYPE) pointOnPlane3$(EXETYPE) pointOnPlane$(EXETYPE)

save: 
	ci -l -m"Make save" Makefile *.cpp README
