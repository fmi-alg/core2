# Makefile for bareiss 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2010/06/10 19:52:07 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = interval_t

all: $(TARGETS)

test: $(TARGETS)
	./interval_t

moretest: $(TARGETS)
# nothing to do

interval_t: interval_t.o


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

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

