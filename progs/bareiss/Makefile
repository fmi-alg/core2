# Makefile for bareiss 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.2 2010/06/08 11:43:41 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= bareiss inverse

all: $(TARGETS)

test: $(TARGETS)
	./bareiss
	./inverse

moretest: $(TARGETS)
# nothing to do

bareiss: bareiss.o

inverse: inverse.o

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

