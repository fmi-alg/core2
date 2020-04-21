# Makefile for voronoi 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:54:37 exact Exp $

CORE_PATH=../../..
include ../../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= voronoi
MORETARGETS= 
EXTRATARGETS= voronoi_lattice

OBJS=edgelist.o geometry.o heap.o memory.o output.o voronoi.o PQlist.o

# The getopt.{h,c} files have been removed from our distribution
#	because of the licensing problems.  You can download
#	these files from the Core Library Website (under downloads)
#	if you need them:
#
# ifeq ($(PLATFORM), mingw)
#   OBJS+=getopt.o
# endif
#
LEV = 3
LEV = 2
LEV = 1

default: $(TARGETS)

test: $(TARGETS)
	./voronoi -c < inputs/lattice9 > outputs/lattice9.ps
	./voronoi -c < inputs/points100 > outputs/points100.ps
	./voronoi -ct < inputs/points100 > outputs/points100-tri.ps

moretest:
	
extratest: $(EXTRATARGETS)

all: $(TARGETS) $(MORETARGETS)

voronoi: main.o $(OBJS) 

voronoi_lattice: main_lattice.o $(OBJS) 

tri triangulate:
	./voronoi -d < inputs/lattice9a > outputs/lattice9a
	./voronoi -t < inputs/lattice9a > outputs/lattice9a-tri
	./voronoi -c < inputs/lattice9a > outputs/lattice9a.ps

	
#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=$(LEV) $(CORE_INC) $< -o $@

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

