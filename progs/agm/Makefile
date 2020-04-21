# Makefile for AGM
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.3 2010/07/07 15:26:39 exact Exp $

include ../Make.options
#CXXFLAGS += -DAGM_DEBUG

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= t

ALLTARGETS= p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15 p16 p17 p18 p19


test: $(TARGETS)
	./t

all: $(ALLTARGETS)

t: t.o agm.o

verify-digits: verify-digits.o agm.o fun.o

discrete: discrete.o agm.o

p1: p1.o agm.o fun.o

p2: p2.o agm.o fun.o

p3: p3.o agm.o fun.o

p4: p4.o agm.o fun.o

p5: p5.o agm.o fun.o

p6: p6.o agm.o fun.o

p7: p7.o agm.o fun.o

p8: p8.o agm.o fun.o

p9: p9.o agm.o fun.o

p10: p10.o agm.o fun.o

p11: p11.o agm.o fun.o

p12: p12.o agm.o fun.o

p13: p13.o agm.o fun.o

p14: p14.o agm.o fun.o

p15: p15.o agm.o fun.o

p16: p16.o agm.o fun.o

p17: p17.o agm.o fun.o

p18: p18.o agm.o fun.o

p19: p19.o agm.o fun.o

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) agm.o fun.o -o $@

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

