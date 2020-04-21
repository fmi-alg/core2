# Makefile for sub-division solver

include ../../Make.options
#=================================================
# Define target files (put all your files here)
#=================================================

TARGETS=main

#=================================================
# Rules
#=================================================

$(TARGETS):	
	g++ -o main main.cpp  \
	    -I$(CORE_PATH)/inc \
	    -I$(CORE_PATH)/gmp/include  \
	    -I$(CORE_PATH)/progs/mesh/arrangements-gui \
	    -L$(CORE_PATH)/lib \
	    -L$(CORE_PATH)/gmp/lib \
	    -lgmp -lmpfr -lcore++Opt

test:
	./main

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o
#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE)) $(MORETARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

#=================================================
# End
#=================================================
