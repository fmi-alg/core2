# Makefile for progs/robotics/2d-disc
#
# April 13, 2012
#
# Since Core2
# $Id:$

CORE_PATH=../../..
include ../Make.options

#=================================================
# Original file
#=================================================
# Box.cpp
# Corner.cpp
# QuadTree.cpp
# Wall.cpp
# robotics2D.cpp
# 
# CC = g++ -I/usr/include/opengl -g -Wall -ansi
# 
# all:
# 	${CC} *.cpp  -lglui -lglut32 -lglu32 -lopengl32  -o output.exe
# 
# clean:
# 	rm *.exe
#=================================================
# User definition (put parameters manually here)
#=================================================
xmin = -10
xmax =  10
ymin = -10
ymax =  10

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = disc

OBJ_FILES = Box.o Corner.o QuadTree.o Wall.o

all: $(TARGETS)

test: $(TARGETS)

eg: ./disc
	./disc  < input2.txt

moretest: $(TARGETS)
# nothing to do


disc: $(OBJ_FILES) disc.o

#=================================================
# Examples 
#=================================================

# Intersecting a parabola and the unit circle:
eg1: ${TARGETS}
	./mk --fxy "y-x^2" --gxy "x^2+y^2-1" \
    --x_min ${xmin} --x_max ${xmax} --y_min ${ymin} --y_max ${ymax} --minsize ${minsize} --maxsize ${maxsize} --maxgen ${maxgen}

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(OBJ_FILES) $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

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

#=================================================
# END Makefile 
#=================================================

