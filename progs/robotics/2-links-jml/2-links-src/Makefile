
# turn on/off open GL display
# you will need to install freeglut on linux to make

OPENGL_ON := 1


#-----------------------------------------------------------------------------
# On Linux,    this should be 'Linux'
# On Mac OS X, this should be 'Darwin'
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# On Linux,    this should be 'Linux'
# On Mac OS X, this should be 'MAC_OS_X'
#-----------------------------------------------------------------------------

PLATFORM := $(shell uname -s)
MACHINE := $(shell uname -m)

ifeq ($(PLATFORM),Linux)
  ifeq ($(MACHINE),x86_64)
    PLATFORM := LINUX64
  else
    PLATFORM := LINUX
  endif
else
  ifeq ($(PLATFORM),Darwin)
    PLATFORM := MAC_OS_X
  else
    PLATFORM := OTHER
  endif
endif


#-----------------------------------------------------------------------------
# Choose a compiler & its options
#--------------------------------------------------------------------------

CXX      = g++
LINK     = g++

OPTS = -ansi -MMD -Wall -Wno-deprecated  
ifeq ($(PLATFORM),MAC_OS_X)
 OPTS += -DMACOS
endif

#--------------------------------------------------------------------
# Xlib and OpenGL
#--------------------------------------------------------------------

ifeq ($(PLATFORM),MAC_OS_X)
   GL_LIB = -framework OpenGL -framework GLUT -framework Foundation
else
   X_LIB  = -Wl,-rpath,/usr/X11R6/lib -L/usr/X11R6/lib -L/usr/local/lib -lXext -lX11 -lXt 
   GL_LIB = -lglut -lGLU -lGL -lm
endif

#--------------------------------------------------------------------
# Triangulation
#--------------------------------------------------------------------
TRI_DIR = ./FIST
TRI_INCL= -I${TRI_DIR}
ifeq ($(PLATFORM),MAC_OS_X)
  TRI_LIB = -L${TRI_DIR} -lFIST.osx
else 
  ifeq ($(PLATFORM),LINUX64)
    TRI_LIB = -L${TRI_DIR} -lFIST.linux64
  else
    TRI_LIB = -L${TRI_DIR} -lFIST.linux
  endif
endif

#--------------------------------------------------------------------
# Put all together
#--------------------------------------------------------------------
TARGET  = rrtd
SUBDIRS = . ./GL ./mathtool ./utils ./FIST
INCLUDE = $(addprefix -I,$(SUBDIRS)) $(TRI_INCL)
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
LIB = $(X_LIB) $(GL_LIB) $(TRI_LIB)

CFLAGS   = $(OPTS) $(INCLUDE) 
CXXFLAGS = $(CFLAGS) 


#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive
###########
len1 = 65		# link1 length
len2 = 85		# link2 length
#
len1 = 195		# link1 length
len2 = 340		# link2 length
###########
startX = 60		# start configuration
startY = 350
startTheta1 = 330	# in degrees
startTheta2 = 140	# in degrees
#
startX = 0		# start configuration
startY = 150
startTheta1 = 260	# in degrees
startTheta2 = 80	# in degrees
###########
goalX = 300		# goal configuration
goalY = 60
goalTheta1 = 90		# in degrees
goalTheta2 = 30		# in degrees
#
goalX = 70		# goal configuration
goalY = 120
goalTheta1 = 50		# in degrees
goalTheta2 = 80		# in degrees
###########
epsilon = 2		# resolution parameter
epsilon = 5		# resolution parameter

###########
inputDir = inputs	# dir to find input files
#
fileName = input2d.txt	# input environment file
fileName = input2.txt	# input environment file
fileName = input0.txt	# input environment file
fileName = bugtrap.txt	# input environment file
fileName = input.txt	# input environment file
fileName = input2f.txt	# input environment file
fileName = basic1.txt	# input environment file

###########
input = inputs/basic1.txt	# combined dir/file

###########
boxWidth = 512		# initial configuration box size
boxHt = 512

###########
windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 320	# initial Window position
windowPosY = 20	

###########
Qtype = 0		# type of priority queue
			#  (0=random, 1=BFS, 2=Greedy Best First, 3=Dist+size, 4=Vor?) 
###########
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
###########
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
scale = 1		# scaling of input file

###########
verbose = 0		# if true, display statistics
title = Demos		# Title of control panel

###########
smarterStrategy = 0 # Two-Strategy Option    0: original 1: smarter
smarterStrategy = 1 # Two-Strategy Option    0: original 1: smarter

###########
threshold = 4		# threshold for angular split: if |phi(B)| <= threshold,
			#	then we may do angular split
			# threshold = 0,1,2,...


.PHONY: all
all :  release

debug: CXXFLAGS += -g
debug: $(TARGET)

dev: CXXFLAGS += -O2 -g
dev: $(TARGET)

release: CXXFLAGS += -O3
release: $(TARGET)

eg eg0: $(TARGET)
	@echo "Target eg0: Default" 
	./$(TARGET) inputs/basic1.txt \
	    195 340 0 150 260 80 70 120 50 80

egx: $(TARGET)
	@echo "Target egX: Default" 
	./$(TARGET) $(input) $(len1) $(len2) $(startX) $(startY) $(startTheta1) $(startTheta2) $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) 

egxx: $(TARGET)
	@echo "Target egX: Default" 
	./$(TARGET) $(inputDir)/$(fileName) \
	    $(len1) $(len2) \
	    $(startX) $(startY) $(startTheta1) $(startTheta2) \
	    $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \

eg1: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic3.txt 195 340 0 150 260 80 70 120 50 80
	
eg2: $(TARGET)
	@echo "Target eg2: 8-way junction"  
	./$(TARGET) inputs/zigzagrotation.txt 216 297 115 155 210 220 260 200 85 85

eg3: $(TARGET)
	@echo "Target eg3: Hall way with 8 convergent hallways"
	./$(TARGET) inputs/8way.txt 262 250 180 270 271 256 90 0 90 80 0.065 -220 0

eg4a: $(TARGET)
	./$(TARGET) inputs/bugtrap.txt 200 320 30 60 40 30 90 120 45 79

#--------------------------------------------------------------------

#--------------------------------------------------------------------
$(TARGET): $(OBJS)
	${CXX} ${CXXFLAGS} -o $@ $(OBJS) $(LIB)

clean:
	-rm -f $(OBJS) $(TARGET) Dependencies

#--------------------------------------------------------------------
.SUFFIXES: .cpp

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

include Dependencies
