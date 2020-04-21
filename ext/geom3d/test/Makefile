# Makefile for test
#
# Core Library, $Id: Makefile,v 1.1 2006/04/03 18:55:31 exact Exp $

CORE_PATH=../../..
include $(CORE_PATH)/Make.config

CORE_INC=-I$(CORE_PATH)/inc -I$(CORE_PATH)/gmp
CORE_LIB=-L$(CORE_PATH)/lib -L$(CORE_PATH)/gmp/.libs -lcore$(VAR) -lgmp -lm

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= test_level3 test2_level3

all: $(TARGETS)

test: $(TARGETS)
	./test_level3
	./test2_level3

%_level1: %_level1.o 
	${CXX} $(CXXFLAGS) $? -lcorex_level1 $(CORE_LIB) -o $@

%_level3: %_level3.o 
	${CXX} $(CXXFLAGS) $? -lcorex_level3 $(CORE_LIB) -o $@

#=================================================
# Rules for Level 1 and Level 3
#=================================================
%_level1.o: %.cpp
	${CXX} -c -DCORE_LEVEL=1 $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level3.o: %.cpp
	${CXX} -c -DCORE_LEVEL=3 $(CXXFLAGS) $(CORE_INC) $< -o $@

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
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

