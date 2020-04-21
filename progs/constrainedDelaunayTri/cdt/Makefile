# Makefile 
#	for Constrained Delaunay Triangulation program
#	and related software 
#
# If using tcsh:
#   setenv LD_LIBRARY_PATH \
#	/usr/unsupported/packages/opengl/Mesa4.0/lib:$LD_LIBRARY_PATH
#   export LD_LIBRARY_PATH=/usr/unsupported/packages/opengl/Mesa4.0/lib:$LD_LIBRARY_PATH

############################################################
# Variables
############################################################

n=1

#CXX = g++ 
#CXXFLAGS = -g -Wall -pedantic 
#INCS= -I/usr/unsupported/packages/opengl/Mesa4.0/include
#LIBS= -L/usr/unsupported/packages/opengl/Mesa4.0/lib -lglut -lGL -lGLU

CORE_PATH=../../../
include ../Make.options

############################################################
# Targets
############################################################

cdt:	cdt.o

viewer:  viewer.o

points:  points.o

%: %.o	
	$(CXX) $(CORE_LIB) $< -o $@

#	ORIG: $(CXX) $(LIBS) $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $(CORE_INC) $< -o $@

#	ORIG:	$(CXX) $(CXXFLAGS) -c $(INCS) $< -o $@

test:
	./cdt input/src$(n).noff > output/src$(n).noff

testran:
	./cdt input/ran$(n).noff > output/ran$(n).noff

############################################################
# Housekeeping
############################################################

save:	
	-@ci -m"make save" -l \
		*.cpp *.h input/*.noff doc/* paper/* \
		Makefile README.txt

tar:	*.cpp *.h input/*.noff output/*.noff Makefile README.txt
	tar -cvf cdt.tar $^

tarall: *.cpp *.h input/*.noff output/*.noff \
		doc/format.txt doc/cdt.ps Makefile README.txt paper/p.tar 
	tar -cvf cdtall.tar $^

.PHONY	:clean

clean:
	rm *.exe *.o *.out

############################################################
