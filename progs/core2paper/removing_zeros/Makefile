# Makefile for removing_zeros 
#
# Since Core Library 2.0
# 		$Id: Makefile,v 1.3 2010/03/29 16:44:26 exact Exp $

CORE_PATH=../../..
include ../../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= removing_zeros

default: $(TARGETS)

all: $(TARGETS)

test: $(TARGETS)
	./removing_zeros 100 1
	./removing_zeros 1000 1
	./removing_zeros 10000 1
	./removing_zeros 100 0
	./removing_zeros 1000 0
	./removing_zeros 10000 0

moretest: $(TARGETS)
	./removing_zeros 100000 0
	./removing_zeros 1000000 0
	./removing_zeros 100000 1
	./removing_zeros 1000000 1

# Computing Factorial w/o removing zeros
example1: $(TARGETS)
	./removing_zeros 100 0
	./removing_zeros 1000 0
	./removing_zeros 10000 0
	./removing_zeros 100000 0
	./removing_zeros 1000000 0

# Computing Factorial w removing zeros
example2: $(TARGETS)
	./removing_zeros 100 1
	./removing_zeros 1000 1
	./removing_zeros 10000 1
	./removing_zeros 100000 1
	./removing_zeros 1000000 1

# Computing Summation w/o removing zeros
example3: $(TARGETS)
	./removing_zeros 100 0 1
	./removing_zeros 1000 0 1
	./removing_zeros 10000 0 1
	./removing_zeros 100000 0 1
	./removing_zeros 1000000 0 1

# Computing Summation w removing zeros
example4: $(TARGETS)
	./removing_zeros 100 1 1
	./removing_zeros 1000 1 1
	./removing_zeros 10000 1 1
	./removing_zeros 100000 1 1
	./removing_zeros 1000000 1 1


removing_zeros: removing_zeros.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

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

