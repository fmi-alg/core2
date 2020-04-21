# Makefile for chull
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:23 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS= chull_level3 chull_level1 \
	chull4_level3 chull4_level1

all: $(TARGETS)

default:
	./chull_level3 < inputs/i4 > outputs/i4.ps
	./chull_level3 -l < inputs/i4 > outputs/i4.leda

test: $(TARGETS)
	./chull_level3 < inputs/points > outputs/points.3.ps
	./chull_level1 < inputs/points > outputs/points.1.ps

moretest: $(TARGETS)
	./chull_level3 < inputs/cube > outputs/cube.ps
	./chull_level1 -l < inputs/cube > outputs/cube.leda
	./chull_level3 < inputs/points1 > outputs/points1.ps
	./chull4_level3 < inputs/points1 > outputs/points1-4.ps

bigtest: $(TARGETS)
	./chull_level1 < inputs/para1000 > outputs/para1000.ps
	./chull4_level3 -l < inputs/line1000 > outputs/line1000.leda
	./chull4_level1 < inputs/cube1000 > outputs/cube1000.ps
	./chull4_level3 < inputs/ball1000 > outputs/ball1000.ps

orig: chull-orig.o
	$(CXX) -o chull-orig chull-orig.o

chull2: chull2.o
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@

%_level1: %_level1.o 
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

%_level3: %_level3.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

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
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

