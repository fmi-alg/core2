#######################################################
# graphics subdirectory
#   whose source is exactly one .cpp file.
#
# To compile the main program, you can simply type "make".
# 	Or, you can compile a program called "zzz" using:
#
#        	> make p=zzz
#
# 	where
#       zzz.cc (or zzz.cpp) is the name of the program.
# 
# To some other targets
#
# 		> make help
# 		> make eg
#
# Author: Chee Yap
# Since Corelib 1.2.
#######################################################

#######################################################
# variables/options
#	--use an editor to make sure that your preferred options
#	  is the last one in its list.
#	--You should at least choose the platform variable ("pf").
#######################################################


CORE_PATH=../..
include ${CORE_PATH}/Make.config

AMIYA_HOME=/home/yap/proj/visualhome/proj/bigImages/FromAmiya
HOME=/home/visual/tools/drawpoly
HOME=/home/exact/corelib/ext/graphics/

# PLATFORM is either "unix" or "gnu" or "cyg" or "mac".
# The PLATFORM variable should not be hardcoded
# 	but read from the Make.config file
pf = $(PLATFORM)

CC = g++
CC = g++-4.9

# file extension
ext= 
ext=.c
ext=.cc
ext=.cpp

# executable extension
   ifeq ($(pf),cyg)
	exe=.exe
   else
	exe=
   endif

#######################################################
# Includes and Runtime libraries (alternatives are kept around!)
#######################################################

	SRCS = drawCurve.cpp
	OBJS = $(SRCS:.c=.o)

   ifeq ($(pf),cyg)
	COMPILERFLAGS = -Wall -mno-cygwin -DCYGWIN
	LIBS=  -lm -lglut -lglu -lGL 
	LIBS= -L\bin -LC:\WINNT\system32 -lGL -lglu -lglut 
	COMPILERFLAGS = -Wall -DCYGWIN
	SRCS = drawCurve.cpp
	OBJS = $(SRCS:.c=.o)
   else
        ifeq ($(pf),mac)
   MESA_VERSION=7.8.1_1+hw_render
   MESA_VERSION=7.4.3_0+hw_render
   MESA_PATH=/opt/local/var/macports/software/mesa/$(MESA_VERSION)
   MESA_INC_PATH=$(MESA_PATH)/opt/local/include
   MESA_INC_PATH=/opt/local/include
   MESA_LIB_PATH=$(MESA_PATH)/opt/local/lib
   MESA_LIB_PATH=/opt/local/lib
CORE_LIB=-L$(CORE_PATH)/lib -L$(CORE_PATH)/mpfr/lib -L$(CORE_PATH)/gmp/lib \
	 -L$(MESA_LIB_PATH) -lGL -lGLU -lglut \
	 -lcore++$(VAR) -lmpfr -lgmp
CORE_LIB=-L$(CORE_PATH)/lib -L$(CORE_PATH)/mpfr/lib -L$(CORE_PATH)/gmp/lib \
	 -lcore++$(VAR) -lmpfr -lgmp \
 	-framework GLUT -framework OpenGL -framework Cocoa

CORE_INC=-I. -I$(CORE_PATH)/inc -I$(CORE_PATH)/mpfr/include \
	 -I$(CORE_PATH)/gmp/include -I/usr/include/ \
	 -I$(MESA_INC_PATH) 


	COMPILERFLAGS = -Wall 
	LIBS = -L/opt/local/var/macports/software/mesa/7.4.3_0+hw_render/opt/local/lib -lglut -lGLU -lGL -lm -L/usr/X11R6/lib -lXi -lXmu
	LIBS=  -framework GLUT -framework OpenGL -framework Cocoa
        else
	COMPILERFLAGS = -Wall 
	LIBS = -L/usr/X11R6/lib -L/usr/unsupported/installers/chenli/lib \
		-lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lsocket -lnsl
	LIBS = -L/usr/X11R6/lib \
		-L/usr/unsupported/packages/opengl/Mesa2.4/lib \
		-lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lsocket -lnsl
	LIBS = -L/usr/X11R6/lib \
		-L/usr/unsupported/installers/exact/glut-3.7/lib/glut \
		-lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lsocket -lnsl
	LIBS= -lglut -lGLU -lGL -lm -L/usr/X11R6/lib -lXi -lXmu
	LIBS= -lglut -lGLU -lGL -lm -L/usr/X11R6/lib
        endif
   endif

#######################################################
# Include headers files
#######################################################
   ifeq ($(pf),cyg)
	INC= -I\usr\include
   else
      ifeq ($(pf),mac)
	INC = -I/opt/local/var/macports/software/mesa/7.4.3_0+hw_render/opt/local/include
      else
	INC = -I/
	INC = -I/usr/unsupported/installers/chenli/include
	INC = -I/usr/unsupported/packages/opengl/Mesa2.4/include
	INC = -I/usr/unsupported/installers/exact/glut-3.7/include
      endif
   endif

# Load Flags
   ifeq ($(pf),cyg)
   	LDFLAGS =
   else
   	LDFLAGS =
   endif

#######################################################
# program base is "p"
#######################################################
p=drawCurve
p=gluiExample

# program version is "v"
v=1
v=
# program is "pv"
pv=$(p)$(v)

# data
DATA=/home/visual/tools/drawpoly/data
DATA=
DATA=data

ifile=input
ifile=cissoid
ifile=butterfly

# tar files
fn=DRAW

#######################################################
# targets $(COMPILERFLAGS)
#######################################################
p executable default:
	$(CC) $(COMPILERFLAGS) -g $(INC) $(pv)$(ext) \
	    $(LDFLAGS) $(LIBS) -o $(pv)

glui:  gluiExample.o
	${CXX} $(LDFLAGS) $(LOCALLDFLAGS) $< $(CORE_LIB) -o $@

newdraw: newdraw$(ext)
	$(CC) -g $(INC) newdraw$(ext) \
	    $(LDFLAGS) $(LIBS) -o newdraw

draw: draw.cc
	$(CC) -g $(INC) draw.cc \
	    $(LDFLAGS) $(LIBS) -o draw

poly drawpoly: drawpoly.cc
	$(CC) -g $(INC) drawpoly.cc \
	    $(LDFLAGS) $(LIBS) -o drawpoly

drawCurve1: drawCurve.cpp
	$(CC) $(COMPILERFLAGS) -o $@ $(OBJS) $(LIBS)

drawCurve: drawCurve.o

curve: drawCurve

draw.orig: draw.orig.cc
	$(CC) -g $(INC) draw.orig.cc \
	    $(LDFLAGS) $(LIBS) -o draw.orig
help:
	@echo "USAGE: Type the following"
	@echo "  > make eg                 -- to see an example (or: eg0, eg1, eg2, eg3, eg4)"
	@echo "  > make glui                 -- to see a glui example 
	@echo "  > make params             -- to see default parameters of eg"
	@echo "  > make eg ifile=XXX       -- to display file XXX in local directory ${DATA}"

opt params:
	@echo "================ Parameters =================="
	@echo "    DATA = " ${DATA}
	@echo "    ifile = " ${ifile}
	@echo "=============================================="

#######################################################
# Targets
#######################################################

eg test testcurve:
	./drawCurve < ${DATA}/${ifile}

eg0 test0 testcurve0:
	./drawCurve < ${DATA}/test0

eg1 test1:
	./drawCurve < ${DATA}/test1

eg2 test2 butterfly:
	./drawCurve < ${DATA}/butterfly

eg3 test3 cissoid:
	./drawCurve < ${DATA}/cissoid

eg4 test4 ny:
	./${pv} < ${DATA}/nylines

t:
	./drawCurve < ${DATA}/${ifile}

t1:
	./drawCurve 1 < ${DATA}/plot

testpoly:
	./drawpoly < ${DATA}/${ifile}

testdraw:
	./draw < ${DATA}/${ifile}

testnew:
	./newdraw < ${DATA}/${ifile}

teapot: teapot.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot

teapot2: teapot2.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot2.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot2

teapot3: teapot3.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot3.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot3

teapot4: teapot4.cpp
	$(CC) $(COMPILERFLAGS) -g $(INC) teapot4.cpp \
	    $(LDFLAGS) $(LIBS) -o teapot4


#=================================================
# Rules
#=================================================
%: %.o $(DEPEND)
	${CXX} $(LDFLAGS) $(LOCALLDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) $< -o $@



#######################################################
# housekeeping
#######################################################
pf platform:
	-@echo Platform is $(PLATFORM)

m:
	gvim Makefile

save:
	ci -l draw.cc drawpoly.cc newdraw.cc Makefile README 

tar:
	tar -cvf DRAW.tar draw*.c* Makefile README input* 

# to use this, you must specify the file name: e.g., make tar fn=1-april-1999
tar1:
	tar -cvf $(fn).tar Makefile README drawpoly.cc data/input*

distrib:
	tar -cvf $(fn).tar Makefile README drawCurve.cpp data/test* data/butterfly

clean:
	-@rm -f *.o *~ 

vclean veryclean: clean
	-@rm -f x* drawCurve


#######################################################
# end of Makefile
#######################################################

