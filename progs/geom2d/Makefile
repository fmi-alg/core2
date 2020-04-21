# Makefile for testPoint 
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:24 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
p=tReadPoints
p=t
ifile=i
arg= < inputs/${ifile}
arg=

LEV= 3
TARGETS= testLine pointOnCircle

MORETARGETS= testPoint pointInput convexHull 

default: $(TARGETS)

more: $(MORETARGETS)

test: $(TARGETS)
	./testLine
	./pointOnCircle

moretest: $(MORETARGETS)
	./testPoint
	./pointInput < inputs/p2d_10
	./pointInput < inputs/p2d_4
	./convexHull

p: ${p}
	./${p} ${arg}

${p}: ${p}.o

testPoint: testPoint.o

pointInput: pointInput.o

tPointFormat: tPointFormat.o

testLine: testLine.o

pointOnCircle: pointOnCircle.o

pointInput: pointInput.o

convexHull: convexHull.o

tReadPoints: tReadPoints.o

t: t.o

tt: tt.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=${LEV} $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	echo "EXEPROGS=" $(EXEPROGS)
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

