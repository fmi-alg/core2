# Makefile for pentagon
#
# Core Library, $Id: Makefile,v 1.1 2006/04/03 18:55:31 exact Exp $

CORE_PATH=../../..
include $(CORE_PATH)/Make.config

CORE_INC=-I$(CORE_PATH)/inc -I$(CORE_PATH)/gmp/include
CORE_LIB=-L$(CORE_PATH)/lib -L$(CORE_PATH)/gmp/lib -lcore$(VAR) -lgmp -lm

#=================================================
# Define target files (put all your files here)
#=================================================
LEV=1
LEV=3
TARGETS= pentagon_level3 pentagon_level1
p=tGeom2d
ifile = i5
ifile = i5a
ifile = i5b

default: ${p}
	./${p}

all: $(TARGETS)

test: $(TARGETS)
	./pentagon_level1 < inputs/${ifile}
	./pentagon_level3 < inputs/${ifile}

tGeom2d: tGeom2d.o
	${CXX} $(CXXFLAGS) $? -lcorex${VAR}_level${LEV} $(CORE_LIB) -o $@

%_level1: %_level1.o 
	${CXX} $(CXXFLAGS) $? -lcorex${VAR}_level1 $(CORE_LIB) -o $@

%_level3: %_level3.o 
	${CXX} $(CXXFLAGS) $? -lcorex${VAR}_level3 $(CORE_LIB) -o $@

#=================================================
# Rules for Level 1 and Level 3
#=================================================

%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

.cpp.o:
	${CXX} -c -DCORE_LEVEL=${LEV} $(CXXFLAGS) $(CORE_INC) $< -o $@

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

