# Makefile for progs/robotics/complex-qt
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "disc"
# 	       > make help	-- usage help
# 	       > make test	-- compiles and runs "disc" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "disc" interactively
#
# 	The default search-mode is "random".  We encourage you to just
# 	hit the "Run me" button to see different runs of the program.
# 	NOTE: if you try enough times, ALL 3 homotopy paths will be discovered!
#
#	This is equivalent to typing:
#
#	       > disc $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
#			$(epsilon) $(radius) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(Qtype) $(seed)  $(inputDir) \
#			$(xtrans) $(ytrans) $(scale) \
#			&
#
#	where the arguments to disc correspond to the values $(var) of
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
#		eg0,				-- no path example
#		eg1, eg1a, (bug, buga)		-- bugtrap
#		eg2, eg2a			-- like input2.txt, but with bounding box
#		eg3, eg3a, eg3b			-- 100 random triangles
#		eg4				-- maze
#		eg5, eg5a (bug2, bug2a)		-- double bugtrap
#		eg6, eg6a 			-- example from Kavraki's OOPSMP
#		eg100, eg100a, eg100b		-- 100 random triangles
#		eg200, eg200a			-- 200 random triangles
#		eg300				-- 300 random triangles
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
#
# April 13, 2012
#
# Since Core Library 2.1
# $Id: Makefile,v 1.1 2012/10/26 03:58:52 cheeyap Exp cheeyap $

CORE_PATH=../../..
include ../Make.options

#=================================================
# User variables (you can change them in the command line)
#	NOTE: if any variable value begins with '@' then it is
#	not used (that means the default value from the cfg file is used)
#=================================================
interactive = 1			# 0=interactive, >0 is non-interactive
cfgName = L_rand100.cfg		# config file
fileName = map_parking.txt	# enviroment file
inputDir = inputs		# Path for input files
robotDir = robots		# Path for robot files
robotName = L.rob		# robot 
alpha0 = 400			#  startx	
alpha1 = 320			#  starty
alpha2 = 0			#  startTheta
beta0 = 100			#  goalx
beta1 = 70			#  goaly
beta2 = 0			#  goalTheta
epsilon = 4 			#  resolution parameter
R0 	= 85			#  robot radius
boxWidth = 512			#  boxWidth
boxHeight = 512			#  boxHeight
windowPosX = 320		#  window X pos
windowPosY = 20			#  window Y pos
QType   = 2			#  PriorityQ Type(random or no)
seed   = 111			#  for random number generator
deltaX  = 0			#  x-translation of input file
deltaY  = 0			#  y-translation of input file
scale  = 1			#  scaling of input file
verbosity  = 1			#  0 is least verbose, 1 is slightly more

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = complex

OBJ_FILES = Box.o Corner.o QuadTree.o Polygon.o Wall.o \
	    Timer.o fileProcessor.o \

all: $(TARGETS)

complex: $(OBJ_FILES) complex.o

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "disc" is "1").
#
test: $(TARGETS)
	./complex 1 \
		$(cfgName)  $(fileName)  $(inputDir) \
		$(robotDir)  $(robotName) \
		$(alpha0) $(alpha1)  $(alpha2) \
		$(beta0) $(beta1) $(beta2) \
		$(epsilon) $(R0) \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \
		&

testall: $(TARGETS)
	make ega interactive=1
	make egb interactive=1

testa: $(TARGETS)
	./complex 1

testb: $(TARGETS)
	./complex 1 \
		L_parking.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \
		&

help:
	@echo "USAGE:  The main program is called complex.  Demos can be invoked thus:"
	@echo "         > make eg"
	@echo "         > make egX"
	@echo "     where X is replaced by 0,1,2,3,4,5,6,100,200,300, etc."
	@echo "     Some demos have variants, such as egXa, egXb or egXc:"
	@echo "     LIST OF TARGETS:"
	@echo "	ega, egb, egc"
	@echo "	eg0,				-- no path example"
	@echo "	eg1, eg1a, (bug, buga)		-- bugtrap"
	@echo "	eg2, eg2a	-- like input2.txt, but with bounding box"
	@echo "	eg3, eg3a, eg3b			-- 100 random triangles"
	@echo "	eg4 (maze)			-- maze"
	@echo "	eg5, eg5a (bug2, bug2a)		-- double bugtrap"
	@echo "	eg6, eg6a 		-- example from Kavraki's OOPSMP"
	@echo "	eg100, eg100a, eg100b		-- 100 random triangles"
	@echo "	eg200, eg200a			-- 200 random triangles"
	@echo "	eg300				-- 300 random triangles"
	@echo "========"
	@echo "	You may also try demoX (X=1,2,3) for a quick walk through" 


#=================================================
# Examples of running complex program
#=================================================
#
# this is default, but every option may be changed at command line:
# NOTE: in the random mode, after 11 runs, you will find the middle path.
# 	After an additional 21 runs, you will find the third path.
#
eg:	complex
	@echo "Target 'eg':  runs complex with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 
	./disc $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# run with default compiled options:
#	The input file is input2.txt.
#	There are 3 combinatorially different paths.
#	Randomized strategy will find all three of them (keep hitting "run").
#
ega:	disc
	@echo "Target 'ega':  runs disc with default compiled options"
	./disc $(interactive) \
	    &

# run with default Makefile options:
#
egb:	disc
	@echo "Target 'egb':  runs disc with default Makefile options for file input.txt"
	./disc $(interactive) 10 350 300 20 \
		10 30 input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Using input2.txt: using the "random strategy", if you "hit run" enough times
# 	the random strategy will eventually find all three distinct homotopy paths!!
#
egc:	disc
	@echo "Target 'egb':  runs disc with default Makefile options for file input.txt"
	./disc $(interactive) 10 350 300 20 \
		10 30 input2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Simple NO PATH example:
eg0 nopath:	disc
	@echo "Target 'eg0':  runs disc with random strategy for file input0.txt"
	@echo "--  this example has no path..."
	./disc $(interactive) 400 400  200 200 \
		10 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		0 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

eg0a nopatha:	disc
	@echo "Target 'eg0a':  runs disc with astar strategy for file input0.txt"
	@echo "--  this example has no path..."
	./disc $(interactive) 400 400  200 200 \
		10 20 input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# BUG TRAP:
###################################################
# Bugtrap example -- Non-existence of path, but barely!
eg1 egbug bug: disc
	@echo "Target 'egbug':  runs disc for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./disc $(interactive) 200 350 30 30 \
		5 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Bugtrap example -- Existence of path, but barely!
eg1a egbuga buga: disc
	@echo "Target 'egbuga':  runs disc for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is a PATH, but barely so!"
	@echo "--  the only change from target 'egbug' is that epsilon is now 4, not 5"
	@echo "--  that are NOT adjacent but shares a corner."
	./disc $(interactive) 200 350 30 30 \
		4 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# BUG2 TRAP: (one trap inside another)
###################################################
# Bugtrap2 example -- Existence of path, but barely! (eps=1)
eg5 egbug2 bug2: disc
	@echo "Target 'egbug2':  runs disc for bugtrap input, bugtrap2.txt"
	@echo "--  this example shows a PATH, but barely so!"
	./disc $(interactive) 180 220 20 20 \
		1 8 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Bugtrap2 example -- Non-existence of path, but barely! (eps=2)
eg5a egbug2a bug2a: disc
	@echo "Target 'egbug2':  runs disc for bugtrap input, bugtrap2.txt"
	@echo "--  this example shows NO PATH, but barely so!"
	./disc $(interactive) 180 220 20 20 \
		2 8 bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&


# eg2:	the input environment is "input2.txt" but has a bounding box 
#
eg2: disc
	@echo "Target eg2:  runs disc on input2e.txt which has a bounding box"
	./disc $(interactive) 150 260 20 20 \
		4 15 input2e.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

eg2a: disc
	@echo "Target eg2a: runs disc on input2e.txt which has a bounding box"
	./disc $(interactive) 10 350  500 20 \
		10 20 input2e.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

eg2b: disc
	@echo "Target eg2a: runs disc on input2f.txt which splits the concave polygon"
	@echo "   into two convex ones."
	./disc $(interactive) 10 350  500 20 \
		10 20 input2f.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# This environment has 100 randomly generated triangles (this has a path)
eg3 eg100: disc
	@echo "Target eg3:  100 randomly generated triangles (path exists)"
	./disc $(interactive) 10 350  280 220 \
		1 10 input3.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# This environment has 100 randomly generated triangles (this has NO PATH)
eg3a eg100a: disc
	@echo "Target eg3a:  100 randomly generated triangles (NO PATH)"
	./disc $(interactive) 10 350  280 220 \
		1 15 input3.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# This environment has 100 randomly generated triangles (this has PATH)
eg3b eg100b: disc
	@echo "Target eg100b:  runs disc amidst 100 randomly generated triangles"
	./disc $(interactive) 200 270 20 20 \
		2 5 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&
	
# This environment has 100 randomly generated triangles (this has PATH)
eg3c eg100c: disc
	@echo "Target eg100c:  runs disc amidst 100 randomly generated triangles"
	./disc $(interactive) 350 370 20 20 \
		2 5 input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&
	
# This environment is a maze
eg4 maze: disc
	@echo "Target eg4:  runs disc in a randomly generated maze "
	./disc $(interactive) 10 350  280 205 \
		5 5 input4.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&
	
# Bugtrap2 example (see above)
# eg5 egbug2 bug2: 
# eg5a egbug2a bug2a: 

# eg6: This environment is taken from Kavraki's groups Demo for OOPSMP:
#  NOTE: we must do scaling of the input
#  	(because dimensions came from xfig)
eg6: disc
	@echo "Target DemoSE2_PRM (from Kavraki's group)"
	./disc $(interactive) 75 350  380 205 \
		5 20 DemoSE2_PRM.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		250 250 0.04 \
		&

# eg6a:  Variant of eg6 where their "robot" is shown.
eg6a: disc
	@echo "Target eg6a, (DemoSE2_PRM from Kavraki's group) -- their robot is shown"
	@echo "	There is a narrow path... when radius=15, epsilon=2"
	@echo " 	There is No Path... when radius=15, epsilon=3"
	./disc $(interactive) 75 350  380 205 \
		2 15 DemoSE2_PRMa.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		250 250 0.04 \
		&

# eg7: this should be the tunnel example from triangle...

# eg8: from Paper of Hsu-Lathombe-Kurniawati
# 	THIS IS SLOW under random strategy!!!
#	          --------------------->>  PATH FOUND !
#	Expanded 219159 times
#	Time used: 94334.1 ms
#	total Free boxes: 789023
#	total Stuck boxes: 24404
#	total Mixed boxes smaller than epsilon: 662771
#	total Mixed boxes bigger than epsilon: 57932
#
eg8: disc
	@echo "Target eg8: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./disc $(interactive) 75 350  380 205 \
		1 5 hsu-latombe-kurniawati.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		&

eg8a: disc
	@echo "Target eg8: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when radius=6, epsilon=1"
	@echo " 	There is No Path... when radius=7, epsilon=1"
	./disc $(interactive) 75 350  380 205 \
		1 7 hsu-latombe-kurniawati.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		&

eg8b: disc
	@echo "Target eg8a: example from the paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	The environment here is slightly irregular: the polygons have overlapping"
	@echo "      edges.  This caused our soft predicate to be confused -- you see"
	@echo "      some green cells that ought to be red."
	./disc $(interactive) 75 350  380 205 \
		1 5 hsu-latombe-kurniawati2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-70 -70 0.065 \
		&

# RANDOMLY GENERATED ENVIRONMENT:
# eg100 (similar to eg3 above)
	
# eg200:
#	This one, generated using our Python Generator, has 200 triangles.
#	Note that these triangles are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms. RESULTS:
#	    			input file name = inputs/input200.txt
#	    			nPt=600
#	    			nPolygons=200
#	    			Time used: 1726.62 ms.
#	    			Path found !
#	    			Expanded 13720 times
#	    			total Free boxes: 9370
#	    			total Stuck boxes: 16404
#	    			total Mixed boxes smaller than epsilon: 13599
#	    			total Mixed boxes bigger than epsilon: 1788
#
#
eg200: disc
	@echo "Target eg200:  runs disc amidst 200 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 150 260 20 20 \
		2 5 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg200a: disc
	@echo "Target eg200a:  runs disc amidst 200 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 10 360 20 20 \
		2 10 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg200b: there is no path because eps=10
#
# 	The display seems to show some current bugs in the
# 	code.   THIS IS A GOOD PLACE TO RUN IN LEVEL 2 to CHECK!!!
#
eg200b: disc
	@echo "Target eg200b:  runs disc amidst 200 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 150 260 20 20 \
		10 5 input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# eg300: 
# 	RESULTS:
#				input file name = inputs/input300.txt
#				nPt=900
#				nPolygons=300
#				Time used: 28243.1 ms.
#				Path found !
#				Expanded 64220 times
#				total Free boxes: 37999
#				total Stuck boxes: 87069
#				total Mixed boxes smaller than epsilon: 67198
#				total Mixed boxes bigger than epsilon: 395
#
eg300: disc
	@echo "Target eg300:  runs disc amidst 300 randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./disc $(interactive) 10 400 350 20 \
		3 11 input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

#=================================================
# Timing Table (from ESA 2017 for Complex Robot)
#=================================================
#=================================================
# esaL  -- 6 experiments for L-robot
#=================================================
esaL1 Lparking: complex
		./complex 1 \
		L_parking.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaL2 Lgateway: complex
		./complex 1 \
		L_gateway.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaL3 Lrand100: complex
		./complex 1 \
		L_rand100.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaL4 Lrand200: complex
		./complex 1 \
		L_rand200.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaL5 Lbugtrap2: complex
		./complex 1 \
		L_bugtrap2.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaL6 Lbugtrap2no: complex
		./complex 1 \
		L_bugtrap2_nopath.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaL Lexperiments: 
	echo  "ESA EXPERIMENTS 1 to 6 ==========\n" > outputs/esa-L
	echo  "ESA 1 ===========================\n" >> outputs/esa-L
	make esaL1 >> outputs/esa-L
	echo "ESA 2 ===========================\n" >> outputs/esa-L
	make esaL2 >> outputs/esa-L
	echo "ESA 3 ===========================\n" >> outputs/esa-L
	make esaL3 >> outputs/esa-L
	echo "ESA 4 ===========================\n" >> outputs/esa-L
	make esaL4 >> outputs/esa-L
	echo "ESA 5 ===========================\n" >> outputs/esa-L
	make esaL5 >> outputs/esa-L
	echo "ESA 6 ===========================\n" >> outputs/esa-L
	make esaL6 >> outputs/esa-L
	echo "ESA end =========================\n" >> outputs/esa-L

#=================================================
# esaS  -- 6 experiments for SnowFlake robot
#=================================================

esaS1 Sparking: complex
		./complex 1 \
		S_parking.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaS2 Sgateway: complex
		./complex 1 \
		S_gateway.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaS3 Srand100: complex
		./complex 1 \
		S_rand100.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaS4 Srand200: complex
		./complex 1 \
		S_rand200.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaS5 Srand200no: complex
		./complex 1 \
		S_rand200_nopath.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaS6 Sbugtrap2: complex
		./complex 1 \
		S_bugtrap2.cfg  @filename  @inputDir \
		@robotDir  @robotName \
		@alpha0 @alpha1  @alpha2 \
		@beta0 @beta1 @beta2 \
		@epsilon @R0 \
		$(boxWidth) $(boxHeight) \
		$(windowPosX) $(windowPosY) \
		$(QType) $(seed) \
		$(deltaX) $(deltaY) $(scale) \
		$(verbosity) \

esaS-: 
	echo  "ESA EXPERIMENTS 1 to 6 ==========\n" > outputs/esa-S
	echo  "ESA 1 ===========================\n" >> outputs/esa-S
	make esaS1 >> outputs/esa-S
	echo "ESA 2 ===========================\n" >> outputs/esa-S

esaS Sexperiments: 
	echo  "ESA EXPERIMENTS 1 to 6 ==========\n" > outputs/esa-S
	echo  "ESA 1 ===========================\n" >> outputs/esa-S
	make esaS1 >> outputs/esa-S
	echo "ESA 2 ===========================\n" >> outputs/esa-S
	make esaS2 >> outputs/esa-S
	echo "ESA 3 ===========================\n" >> outputs/esa-S
	make esaS3 >> outputs/esa-S
	echo "ESA 4 ===========================\n" >> outputs/esa-S
	make esaS4 >> outputs/esa-S
	echo "ESA 5 ===========================\n" >> outputs/esa-S
	make esaS5 >> outputs/esa-S
	echo "ESA 6 ===========================\n" >> outputs/esa-S
	make esaS6 >> outputs/esa-S
	echo "ESA end =========================\n" >> outputs/esa-S




#=================================================
# Demos (from Talk at RCV 2013 in Berlin RSS Conference)
#=================================================
# Demo1 is same as "eg" (to illustrate the classification, and different strategies)
demo1:	disc
	@echo "Target 'eg':  runs disc with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg radius=60 goalX=111 goalY=450 startY=200 startX=20"
	./disc $(interactive) $(startX) $(startY)  $(goalX) $(goalY) \
		$(epsilon) $(radius) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Demo2 is same as "eg1" (to illustrate NO-PATH in BugTrap).  If eps=4, PATH is found!
demo2: disc
	@echo "Target 'egbug':  runs disc for bugtrap input, bugtrap.txt"
	@echo "--  this examples shows there is NO PATH, but barely so!"
	@echo "--  you could see a green path that must pass between two green boxes"
	@echo "--  that are NOT adjacent but shares a corner."
	./disc $(interactive) 200 350 30 30 \
		5 10 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&

# Demo3 is same as eg100 (this has a path)  -- use Qtype=3 (Astar) for speed
demo3: disc
	@echo "Target eg3:  100 randomly generated triangles (path exists)"
	./disc $(interactive) 10 350  280 220 \
		1 10 input3.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		&
#=================================================
# End of Demos (from Talk at RCV 2013 in Berlin RSS Conference)
#=================================================

moretest: $(TARGETS)
# nothing to do

disc: $(OBJ_FILES) disc.o

#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim disc.cpp

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} -std=gnu++11 $(OBJ_FILES) $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -std=gnu++11 -c -O3 $(CXXFLAGS) $(CORE_INC) $< -o $@

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

