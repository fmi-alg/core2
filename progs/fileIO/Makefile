# Makefile for fileIO 
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:51:23 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= fileIO
MORETARGETS= IOspeed stringIO

default: $(TARGETS)

test: $(TARGETS)
	./fileIO

moretest: $(MORETARGETS)
	./IOspeed
	./stringIO

fileIO: fileIO.o

IOspeed: IOspeed.o

stringIO: stringIO.o

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
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

