# Makefile for geom3d 
#
# Core Library, $Id: Makefile,v 1.3 2010/10/23 01:53:35 exact Exp $

include ../Make.options

#=================================================
# VARIABLES
#=================================================

LEV:=1

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= lines3d_level3 lines3d_level2 lines3d_level1

MORE_TARGETS= lines2d_level1 lines2d_level2 lines2d_level3

default: $(TARGETS)

more: $(MORE_TARGETS)

test: $(TARGETS)
	time ./lines3d_level1
	time ./lines3d_level2 
	time ./lines3d_level3 

moretest: $(MORE_TARGETS)
	time ./lines2d_level1 
	time ./lines2d_level2 
	time ./lines2d_level3 

testall: test moretest


23: lines2d_level3
	./lines2d_level3 
21: lines2d_level1
	./lines2d_level1 
33: lines3d_level3
	./lines3d_level3 
31: lines3d_level1
	./lines3d_level1 
22: lines2d_level2
	./lines2d_level2 
32: lines3d_level2
	./lines3d_level2 

#=================================================
# Rules 
# 	NOTE: Since LEV is used as target name, it must be
# 	a "standard variable" (as declared by saying "LEV:-1"
# 	above).  It cannot be a "recursive variable" (as when
# 	we say "LEV=1").
#=================================================
%_level${LEV}.o: %.cpp
	${CXX} -c -DCORE_LEVEL=${LEV} $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level${LEV}: %_level${LEV}.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level${LEV} $(CORE_LIB) -o $@

%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level1: %_level1.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level1 $(CORE_LIB) -o $@

%_level2.o: %.cpp
	${CXX} -c -DCORE_LEVEL=2 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level2: %_level2.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level2 $(CORE_LIB) -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3: %_level3.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level3 $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

