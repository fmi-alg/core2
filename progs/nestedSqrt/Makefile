# Makefile for nestedSqrt 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:25 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
p = nestedSqrtX
TARGETS= nestedSqrt 
MORE_TARGETS= nestedSqrtX

default: $(TARGETS)

more: $(MORE_TARGETS)

test: $(TARGETS)
	./nestedSqrt 

moretest: $(MORE_TARGETS)
	./nestedSqrtX

p: $(p)

nestedSqrt: nestedSqrt.o

X nestedSqrtX: nestedSqrtX.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORE_TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

