# Makefile for wbigfloat ("Weak BigFloat")
#
# Core Library, $Id: Makefile,v 1.1 2010/05/24 13:25:47 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= wbigfloat
MORETARGETS= $(TARGETS)

default: $(TARGETS)

test: $(TARGETS)
	./wbigfloat

moretest: $(MORETARGETS)
	./wbigfloat

wbigfloat: wbigfloat.o
	${CXX} $(LDFLAGS) $? $(CORE_LIB) -o $@

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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

