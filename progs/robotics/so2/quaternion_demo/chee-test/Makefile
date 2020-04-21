# Makefile for C++ programming
#
# Chee's variation:

#=================================================
include ../../Make.options
#=================================================

#=================================================
# VARIABLES
#=================================================
DEP = Quaternion.h

p=main

#  CORE_LIB=-L$(CORE_PATH)/lib -L$(CORE_PATH)/mpfr/lib -L$(CORE_PATH)/gmp/lib \
#	 -L$(MESA_LIB_PATH) -lglui \
#	 -lcore++$(VAR) -lmpfr -lgmp  -framework OpenGL -framework GLUT 

# COPIED FROM robotics/disc:
#
 CORE_LIB=-L$(CORE_PATH)/lib -L$(CORE_PATH)/mpfr/lib -L$(CORE_PATH)/gmp/lib \
	 -L$(MESA_LIB_PATH) -lglui \
	 -lmpfr -lgmp  
#=================================================
# TARGETS
#=================================================
q quaternion : $(DEP)
	echo "Compiling quaternion program..."		
	g++ -lGL -lGLU -lglut -lglui -lm main.cpp -o main
	./main

qq: $(DEP)
	echo "Compiling quaternion program..."		
	make	
	#g++ $(CORE_LIB) \
	#	-lGL -lGLU -lglut -lglui -lm main.cpp -o main
	./main
#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim $(p).cpp

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(OBJ_FILES) $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

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

#=================================================
# END Makefile 
#=================================================

