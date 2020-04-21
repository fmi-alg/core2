# Makefile for sample 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:25 exact Exp $

#=================================================
# If you set VAR below, it would override the default values
#	for VAR in ../Make.options or ../../Make.config.
# This is useful for deciding which "variant" of the Core Library
#	you want to use each time.
#=================================================

VAR=Debug
VAR=

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= pivot
p=pivot

p:	${p}

all: $(TARGETS)

test: $(TARGETS)
	./pivot

pivot: pivot.o


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
