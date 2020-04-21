
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

OPTS = -MMD -Wall -Wno-deprecated -std=c++11
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

startX = 60		# start configuration
startY = 350
startTheta1 = 330	# in degrees
startTheta2 = 140	# in degrees
goalX = 300		# goal configuration
goalY = 60
goalTheta1 = 90		# in degrees
goalTheta2 = 30		# in degrees

epsilon = 2		# resolution parameter
epsilon = 5		# resolution parameter

len1 = 65		# link1 length
len2 = 85		# link2 length

inputDir = inputs # where to find input files
fileName = input2d.txt	# input environment file
fileName = input2.txt	# input environment file
fileName = input0.txt	# input environment file
fileName = bugtrap.txt	# input environment file
fileName = input.txt	# input environment file
fileName = input2f.txt	# input environment file

boxWidth = 512		# initial configuration box size
boxHt = 512

windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 320	# initial Window position
windowPosY = 20	

seed = 11		# seed for random
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
scale = 1		# scaling of input file

method = rrt              # rrt, prm or gauss (gaussian PRM)
sample_size = 10000       # max number of iterations 

#rrt paramters
rrt_step_size = 0.001     # how much the tree will grow in each iteration
rrt_goal_bias = 0.1       # how much the tree growth will bias to the goal (0~1), 0 mean no bias
rrt_close_to_goal = 0.001 # when the tree is within this distance to the goal, the path is found

#prm paramters
prm_closest_k = 15        # defines k in the k-closest neighbors for each node in the graph

#gaussian prm paramters
gauss_mean_d = 0.01       # defines the mean distance between the free and in-collision samples
gauss_std    = 0.01       # std of the gaussian distribution 


.PHONY: all
all :  release

debug: CXXFLAGS +=  -O1 -g
debug: $(TARGET)

dev: CXXFLAGS += -O2 
dev: $(TARGET)

release: CXXFLAGS += -O3
release: $(TARGET)


eg-rrt: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) input/$(fileName) $(startX) $(startY) $(startTheta1) $(startTheta2)\
	  $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \
	  $(len1) $(len2) $(scale)\
	  $(xtrans) $(ytrans) rrt $(seed) \
	  $(sample_size) $(rrt_step_size) $(rrt_goal_bias) $(rrt_close_to_goal)


eg-prm: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) input/$(fileName) $(startX) $(startY) $(startTheta1) $(startTheta2)\
	  $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \
	  $(len1) $(len2) $(scale)\
	  $(xtrans) $(ytrans)   prm $(seed) \
	  $(sample_size) $(prm_closest_k)

# for same reason this one crashes, debugging....
eg-gauss: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) input/$(fileName) $(startX) $(startY) $(startTheta1) $(startTheta2)\
	  $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \
	  $(len1) $(len2) $(scale)\
	  $(xtrans) $(ytrans)   gauss $(seed)\
	  $(sample_size)  $(prm_closest_k) $(gauss_mean_d) $(gauss_std)


eg0-rrt: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic1.txt 195 340 0 150 \
	    260 80 70 120 \
	    50 80  1 \
	    0 0  rrt $(seed) \
	    100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg0-prm: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic1.txt 195 340 0 150 \
	    260 80 70 120 \
	    50 80  1 \
	    0 0  prm $(seed) \
	    1000 $(prm_closest_k)

eg0-gauss: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic1.txt 195 340 0 150 \
	    260 80 70 120 \
	    50 80  1 \
	    0 0  gauss $(seed) \
	    100000 $(prm_closest_k) 0.01 0.001

eg1-rrt: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic3.txt 195 340 0 150 \
	    260 80 70 120 \
	    50 80   1 \
	    0 0 rrt $(seed) \
	    100000 $(rrt_step_size) $(rrt_goal_bias) $(rrt_close_to_goal)

eg1-prm: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic3.txt 195 340 0 150 \
	    260 80 70 120 \
	    50 80   1 \
	    0 0 prm $(seed) \
	    10000 $(prm_closest_k)
	
eg1-gauss: $(TARGET)
	@echo "Target eg: Default" 
	./$(TARGET) inputs/basic3.txt 195 340 0 150 \
	    260 80 70 120 \
	    50 80   1 \
	    0 0 gauss $(seed) \
	    1000000 $(prm_closest_k) 0.005 0.01
	
eg2-rrt: $(TARGET)
	@echo "Target eg2: 8-way junction"  
	./$(TARGET) inputs/zigzagrotation.txt 216 297 115 155  210 220 260 200  85 85 1 0 0 \
	 rrt $(seed) 1000000 0.05 0.5 $(rrt_close_to_goal)
	
eg2-gauss: $(TARGET)
	@echo "Target eg2: 8-way junction"  
	./$(TARGET) inputs/zigzagrotation.txt 216 297 115 155  210 220 260 200  85 85 1 0 0 \
	 gauss $(seed) 5000000 $(prm_closest_k) 0.01 0.01

eg3-rrt: $(TARGET)
	@echo "Target eg3: Hall way with 8 convergent hallways"
	./$(TARGET) inputs/8way.txt 262 250 180 270   271 256 90 0   90 80   0.065 -220 0 \
	 rrt $(seed) 100000 0.001 $(rrt_goal_bias) $(rrt_close_to_goal)

eg3-gauss: $(TARGET)
	@echo "Target eg3: Hall way with 8 convergent hallways"
	./$(TARGET) inputs/8way.txt 262 250 180 270   271 256 90 0   90 80   0.065 -220 0 \
	 gauss $(seed) 8000000 20 0.01 0.01


eg4a: $(TARGET)
	./$(TARGET) inputs/bugtrap.txt 200 320 30 60  40 30 90 120  45 79 1 0 0 \
	 rrt $(seed) 200000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg4a-gauss: $(TARGET)
	./$(TARGET) inputs/bugtrap.txt 200 320 30 60  40 30 90 120  45 79 1 0 0 \
	 	 gauss 4 200000 20 0.01 0.01

eg5-rrt: $(TARGET)
	./$(TARGET) inputs/bugtrap2.txt 190 210 180 300  30 30 90 0  60 50 1 0 0 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg5-gauss: $(TARGET)
	./$(TARGET) inputs/bugtrap2.txt 190 210 180 300  30 30 90 0  60 50 1 0 0 \
	 gauss $(seed) 1000000 20 0.01 0.01


eg6: $(TARGET)
	./$(TARGET) inputs/DemoSE2_PRM.txt 75 350 90 270  320 100 0 90  60 40  0.04 250 250 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg7-rrt: $(TARGET)
	./$(TARGET) inputs/tunnel.poly 200 280 90 0  450 300 0 90  10 15 0.05 100 200 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg7-gauss: $(TARGET)
	./$(TARGET) inputs/tunnel.poly 200 280 90 0  450 300 0 90  10 15 0.05 100 200 \
	 gauss $(seed) 80000 20 0.01 0.01

eg8-rrt: $(TARGET)
	./$(TARGET) inputs/input200.txt 10 490 0 270 495 180 270 0 40 60 1 0 0 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg8-gauss: $(TARGET)
	./$(TARGET) inputs/input200.txt 10 490 0 270  495 180 270 0   40 60   1 0 0 \
	  gauss $(seed) 400000 20 0.01 0.01

eg9: $(TARGET)
	./$(TARGET) inputs/narrowCorridor.txt 215 370 0 125 195 160 270 0 95 70  0.065  \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)
	
eg10-rrt: $(TARGET)
	./$(TARGET) inputs/2chambers.txt 415 400 180 270   105 90 90 0   65 80   0.065 -220 0 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal) 
	
	
eg10-gauss: $(TARGET)
	./$(TARGET) inputs/2chambers.txt 415 400 180 270   105 90 90 0   65 80   0.065 -220 0 \
	gauss $(seed) 1000000 20 0.01 0.01

		
eg11-rrt nestedRectangle-rrt: $(TARGET)
	./$(TARGET) inputs/nestedRectangle.txt 425 390 180 270 95 100 0 90    60 40    0.065   -220 0 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg11-gauss nestedRectangle-gauss: $(TARGET)
	./$(TARGET) inputs/nestedRectangle.txt 425 390 180 270 95 100 0 90    60 40    0.065   -220 0 \
	 gauss $(seed) 2000000 $(prm_closest_k)  0.01 0.01
	
eg100-rrt 100-rrt: $(TARGET)
	./$(TARGET) inputs/input100.txt 10 490 0 270 465 180 270 0    60 40  1 0 0 \
	 rrt $(seed) 100000 0.05 0.5 $(rrt_close_to_goal)

eg100-gauss 100-gauss: $(TARGET)
	./$(TARGET) inputs/input100.txt 10 490 0 270 465 180 270 0    60 40  1 0 0 \
	 gauss $(seed) 400000 $(prm_closest_k)  0.01 0.01
	
eg100a: $(TARGET)
	./$(TARGET) inputs/input100.txt 10 490 0 270 470 180 270 0    38 35  1 0 0 \
	 rrt $(seed) 150000 0.05 0.5 $(rrt_close_to_goal)
	
eg150-rrt 150-rrt: $(TARGET)
	./$(TARGET) inputs/input150.txt 5 300 0 270 380 380 270 45    40 60  1 0 0 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)
	
eg150-gauss 150-gauss: $(TARGET)
	./$(TARGET) inputs/input150.txt 5 300 0 270 380 380 270 45    40 60  1 0 0 \
	 gauss $(seed) 600000 $(prm_closest_k)  0.01 0.01
	
		
eg200 200: $(TARGET)
	./$(TARGET) inputs/input200.txt 10 490 0 270 320 280 270 0   40 60   1 0 0 \
	 rrt $(seed) 100000 0.05 0.8 $(rrt_close_to_goal)

eg300-rrt 300-rrt: $(TARGET)
	./$(TARGET) inputs/input300.txt 0 400 90 270 330 40 270 0   40 30  1 0 0 \
	 rrt $(seed) 100000 0.05 $(rrt_goal_bias) $(rrt_close_to_goal)

eg300-gauss 300-gauss: $(TARGET)
	./$(TARGET) inputs/input300.txt 0 400 90 270 330 40 270 0   40 30  1 0 0 \
	 gauss $(seed) 600000 20 0.01 0.01

eg12-rrt: $(TARGET)
	./$(TARGET) inputs/input4.txt 405 400 180 270 105 60 0 180   15 15   1 0 0 \
	 rrt $(seed) 200000 0.05 0.5 $(rrt_close_to_goal)

eg12-gauss: $(TARGET)
	./$(TARGET) inputs/input4.txt 405 400 180 270 105 60 0 180   15 15   1 0 0 \
	 gauss $(seed) 40000 20 0.01 0.01
						
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
