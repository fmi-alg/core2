# Makefile for ball robot
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "ball"
# 	       > make help	-- usage help
# 	       > make test	-- compiles and runs "ball" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "ball" interactively
#
# 	The default search-mode is "random".  We encourage you to just
# 	hit the "Run me" button to see different runs of the program.
# 	NOTE: if you try enough times, ALL 3 homotopy paths will be discovered!
#
#	This is equivalent to typing:
#
#	       > ball $(interactive) $(startX) $(startY) $(startZ)  $(goalX) $(goalY) $(goalZ) \
#			$(epsilon) $(radius) $(fileName) \
#			$(boxWidth) $(windowPosX) $(windowPosY) \
#			$(Qtype) $(seed)  $(inputDir) \
#			$(xtrans) $(ytrans) $(ztrans) $(scale) \
#			&
#
#	where the arguments to ball correspond to the values $(var) of
#	various Makefile variables.
#
# (C)	You can override any of these values at the command line.
#	For instance, to change the radius of the robot
#	to 55, and to change the start position of the robot to (20,200), you can type:
#
#		> make eg radius=55 startY=200 startX=20
#
# (D)	Instead of the target "eg" you can use any of these targets:
#
#		ega, egb, egc
#		eg1,				-- no path example
#		eg2, eg2a			-- like input2.txt, but with bounding box
#		eg3, eg3a, eg3b			-- 100 random triangles
#		eg4				-- no path example
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".
#
#
# June 1, 2014
# --Surin Ahh, Chee Yap, Ziyao Wei
#
# Since Core Library 2.1
# $Id: Makefile,v 1.1 2012/10/26 03:58:52 cheeyap Exp cheeyap $

CORE_PATH=../../../
include ../Make.options

#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

startX = 75		# start configuration
startY = 360
startZ = 100
goalX = 450		# goal configuration
goalY = 50
goalZ = 400

epsilon = 5		# resolution parameter
epsilon = 10		# resolution parameter
radius = 40		# robot radius
radius = 30		# robot radius

inputDir = inputs	# where to find input files
fileName = input0.txt	# input environment file
fileName = input.txt	# input environment file
fileName = input2.txt
fileName = input1.txt	# input environment file

boxWidth = 512		# initial configuration box size

windowPosX = 200	# initial Window position
windowPosY = 200
windowPosX = 250	# initial Window position
windowPosY = 150

Qtype = 2		# type of priority queue
			#	(0 = random, 1 = BFS, 3 = astar)
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
ztrans = 0
scale = 1		# scaling of input file

eyeX = 0
eyeY = 256
eyeZ = 1280
atX = 0
atY = 256
atZ = 0
upX = 0
upY = 1
upZ = 0

transparency = 70
frameRate = 60

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = ball

OBJ_FILES = Box.o Octree.o Corner.o Edge.o Wall.o fileProcessor.o Timer.o

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "ball" is "1").
#
test: $(TARGETS)
	./ball 1 \
		$(startX) $(startY) $(startZ) $(goalX) $(goalY) $(goalZ) \

	make $(epsilon)  $(radius)  input.txt  $(boxWidth) \
	        $(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) $(scale) \
		&


testall: $(TARGETS)
	make ega interactive=1
	make egb interactive=1

testa: $(TARGETS)
	./ball 1

testb: $(TARGETS)
	./ball 1 \
		$(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon)  $(radius)  input2.txt  $(boxWidth) \
                $(windowPosX) $(windowPosY) $(Qtype) $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

help:
	@echo "USAGE:  The main program is called ball.  Demos can be invoked thus:"
	@echo "         > make eg"
	@echo "         > make egX"
	@echo "     where X is replaced by 0,1,2,3,4,5,6,100,200,300, etc."
	@echo "     Some demos have variants, such as egXa, egXb or egXc:"
	@echo "     LIST OF TARGETS:"
	@echo "	ega, egb, egc"
	@echo "	eg0,				-- no path example"
	@echo "	eg1, eg1a, (bug, buga)		-- bugtrap"
	@echo "	eg2, eg2a			-- like input2.txt, but with bounding box"
	@echo "	eg3, eg3a, eg3b			-- 100 random triangles"
	@echo "	eg4				-- maze"
	@echo "	eg5, eg5a (bug2, bug2a)		-- double bugtrap"
	@echo "	eg6, eg6a 			-- example from Kavraki's OOPSMP"
	@echo "	eg100, eg100a, eg100b		-- 100 random triangles"
	@echo "	eg200, eg200a			-- 200 random triangles"
	@echo "	eg300				-- 300 random triangles"


#=================================================
# Examples of running ball program
#=================================================
#
# this is default, but every option may be changed at command line:
#
eg:	ball
	@echo "Target 'eg':  runs ball with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=30 goalX=111 goalY=450 startY=200 startX=20"
	./ball $(interactive) \
	        $(startX) $(startY) $(startZ)  $(goalX) $(goalY) $(goalZ) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) $(scale) \
                $(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

# run with default compiled options:
#	The input file is input1.txt.
#	There are 3 combinatorially different paths.
#	Randomized strategy will find all three of them (keep hitting "run").
#
ega:	ball
	@echo "Target 'ega':  runs ball with default compiled options"
	./ball $(interactive) \
	    &

# run with default Makefile options:
#
egb:	ball
	@echo "Target 'egb':  runs ball with default Makefile options for file input2.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) input2.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) $(scale) \
                $(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

# Using input2.txt: using the "random strategy", if you "hit run" enough times
# 	the random strategy will eventually find all three distinct homotopy paths!!
#
egc:	ball
	@echo "Target 'egc':  runs ball with default Makefile options for file input3.txt"
	./ball $(interactive) 320 300 230 320 200 475 \
		$(epsilon) 20 input3.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) 1 $(scale) \
		$(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

# Simple NO PATH example:
eg0 nopath:	ball
	@echo "Target 'eg0' 'nopath':  environment with a wall separating start from goal"
	./ball  $(interactive) $(startX) $(startY) $(startZ)  $(goalX) $(goalY) $(goalZ) \
		$(epsilon) $(radius) wall.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) $(scale) \
		&

eg0a nopatha:	ball
	@echo "Target 'eg0a':  runs ball with astar strategy for file input0.txt"
	@echo "--  this example has no path..."
	./ball $(interactive) 400 400  200 200 \
		10 20 input0.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# EXAMPLES
###################################################
#
eg1:	ball
	@echo "Target 'eg1':  runs ball with default Makefile options for file input2.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) input2.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) $(scale) \
                $(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

eg2:	ball
	@echo "Target 'eg2':  runs ball with default Makefile options for file input3.txt"
	./ball $(interactive) 320 300 230 320 200 475 \
		$(epsilon) 20 input3.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) 1 $(scale) \
		$(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

# testing quadrilaterl input files
eg3:	ball
	@echo "Target 'eg3':  runs ball with default Makefile options for file input10.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) input10.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) 2 \
                $(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&
eg3b:	ball
	@echo "Target 'eg3b':  runs ball with default Makefile options for file input11.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) input11.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) 2 \
                $(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

eg3c:	ball
	@echo "Target 'eg3c':  runs ball with default Makefile options for file input12.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) input12.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) 2 \
                $(eyeX) $(eyeY) $(eyeZ) $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
		&

eg4:	ball
	@echo "Target 'eg4':  runs ball with default Makefile options for file input21.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) input21.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) 4 \
                74 200 1580 $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
                0.275 0.47 0.068 \
		&

egPoints:	ball
	@echo "Target 'egPoints':  runs ball with default Makefile options for file points.txt"
	./ball $(interactive) 200 360 50 275 200 450 \
		$(epsilon) $(radius) points.txt \
		$(boxWidth) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(ztrans) 1 \
                74 200 1580 $(atX) $(atY) $(atZ) $(upX) $(upY) $(upZ) \
                $(transparency) $(frameRate) \
                -0.135 0.333 -0.075 \
		&

moretest: $(TARGETS)
# nothing to do

ball: $(OBJ_FILES) ball.o

#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim ball.cpp

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
