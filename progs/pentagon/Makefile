# Makefile for pentagon
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:25 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS= pentagon_level3 
MORE_TARGETS= pentagon_level1

default: $(TARGETS)

more: $(MORE_TARGETS)

test: $(TARGETS)
	./pentagon_level3 

moretest: $(MORE_TARGETS)
	./pentagon_level1

%_level1: %_level1.o transform_level1.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level1 $(CORE_LIB) -o $@

%_level3: %_level3.o transform_level3.o
	${CXX} $(LDFLAGS) $? -lcorex++$(VAR)_level3 $(CORE_LIB) -o $@

#=================================================
# Rules for CORE_LEVEL 1 and CORE_LEVEL 3
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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

