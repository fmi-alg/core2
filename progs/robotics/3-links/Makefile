# Makefile for progs/robotics/2-links
#
# NOTE: this is the version for "thick 2-link robot" (Dec 2013)
# 	The new parameter is "thickness".
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "2-link"
# 	       > make help	-- help message
# 	       > make test	-- compiles and runs "2-link" non-interactively
# 	       			   (this is Core Library's automatic self-test)
# 	       > make [target]	-- where [target] has the form egX
# 	                     	   and X=1,2,3,4,5,6,100,150,200, etc.
#
# (B)	But you may try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "2-link" interactively on
# 	       				some default example
# 	       > make egx [...]
# 	       			-- compiles and runs "2-link" interactively,
# 	       				but where you can change any argument
# 	       				(see next)
#
#	The target "egx" is equivalent to typing:
#
#	       > 2-links $(interactive) $(startX) $(startY)  $(startTheta) \
#	       		$(goalX) $(goalY) $(goalTheta) \
#			$(epsilon) $(len1) $(len2) $(thickness) \
#			$(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(Qtype) $(seed)  $(inputDir) \
#			$(xtrans) $(ytrans) $(scale) \
#			$(verbose) $(title) \
#
#	where the arguments to 2-link correspond to the values $(var) of
#	various Makefile variables.   
#
# 	The best search-strategy is usually "Greedy" (Qtype=2). 
# 	We encourage you to just try other strategies to see...
#
# (C)	You can override any of these values at the command line.
#	For instance, to change the length of the first link
#	to 55, and the start position of the robot to (20,200), you can type:
#
#		> make egx len1=55 startY=200 startX=20
#
# (D)	Instead of the target "eg" you can use any of these targets:
#
#		ega, egb,
#		eg1, eg1a, eg1b,
#		eg2, eg2a, eg2b,
#		eg3
#		eg4 (= egbug, bug), eg4a (= egbuga, buga),
#		eg5 (= egbug2, bug2), eg5a (= egbug2a, bug2a),
#		eg6 (= kavraki), eg6a (= kavrakia),
#		eg7 (= tunnel), eg7a (= tunnela), eg7b (= tunnelb)
#		eg8 (= hsu), eg8a (= hsua), eg8b (= hsub)
#		eg100, eg100a
#		eg150
#		eg200, eg200a, eg200b
#		eg300
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
# (E) (Chee: August 2013) Started to use CMake to build executables.
# 	The main "2-links" target is now renamed "2-links-orig" and deprecated.
#
# April 13, 2012 - Sep 2013
# Author: Zhongdi Luo and Chee Yap
#
# Since Core Library 2.1
# $Id:$

CORE_PATH=../../..
include ../Make.options

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

thickness = 10   	# thickness of robot
bandwidth = 10   	# the minimum angle separation between the 2 links 

inputDir = inputs	# where to find input files
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

Qtype = 0		# type of priority queue
			#  (0=random, 1=BFS, 2=Greedy Best First, 3=Dist+size, 4=Vor?) 
seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
xtrans = 0		# x-translation of input file
ytrans = 0		# y-translation of input file
scale = 1		# scaling of input file

verbose = 0		# if true, display statistics
title = Demos		# Title of control panel

smarterStrategy = 1	# Two-Strategy Option    0: original 1: smarter
smarterStrategy = 0 	# Two-Strategy Option    0: original 1: smarter

threshold = 4		# threshold for angular split: if |phi(B)| <= threshold,
crossing = 1        	# crossing option (0: original self-crossing, 1: non-crossing)
			#	then we may do angular split
			# threshold = 0,1,2,...
#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = 2-links

OBJ_FILES = Box.o QuadTree.o Corner.o Wall.o fileProcessor.o Timer.o Polygon.o

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "2-links" is "1").
#
test: $(TARGETS)

testa: $(TARGETS)
	./2-links 1

testb: $(TARGETS)
	./2-links 1 \
    $(startX) $(startY)  $(startTheta1) $(startTheta2) \
    $(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \
    $(epsilon)  $(len1) $(len2) $(thickness) input2.txt  $(boxWidth) $(boxHt) \
    $(windowPosX) $(windowPosY) \
    $(Qtype) $(seed) $(inputDir) \
    $(xtrans) $(ytrans) $(scale) \
    $(verbose) $(title) \
    $(smarterStrategy) \
	$(threshold) 
	$(crossing)\
	$(bandwidth)\
	&

help:
	@echo "USAGE:  The main program is called 2-links.  Demos can be invoked thus:"
	@echo "         > make                       -- to compile 2-links"
	@echo "         > make eg                    -- to run default example"
	@echo "         > make egx  [Qtype=3 ...]    -- to run example with parameters "
	@echo "     where parameters include fileName, radius, startX, startY,"
	@echo "     goalX, goalY, etc.  See the Makefile for examples."
	@echo "     You can also call demos where these these parameters are prechosen:"
	@echo "              > make egX"
	@echo "     where X is replaced by 1, 2, 3, 4, 5, 6, 100, 150, 200, 300, etc."
	@echo "     Some demos have variants (such as egXa, egXb or egXc)"
	@echo "     or alternative names (such as bug, bug2, tunnal, etc): "
	@echo "        eg [some selected choice]"
	@echo "        egx [all the Makefile defaults, which you can override]"
	@echo "        ega [compiled defaults, no arguments taken]"
	@echo "        egb [simple example from input2.txt]"
	@echo "        eg1, eg1a, eg1b, [environment with various box barriers]"
	@echo "        eg2, eg2a, eg2b, [8-way corridor with 3 obstacles]"
	@echo "        eg3  [simple environment with door]"
	@echo "        eg4 (= egbug, bug), eg4a (= egbuga, buga),"
	@echo "        eg5 (= egbug2, bug2), eg5a (= bug2a), eg5b (= bug2b)"
	@echo "        eg6 (= kavraki), eg6a (= kavrakia),"
	@echo "        eg7 (= salzman, tunnel), eg7a (= salzmana)"
	@echo "        eg8 (= hsu), eg8a (= hsua)"
	@echo "        eg9 (= narrow), eg9a, eg9b"
	@echo "        eg10 (= 2chambers), eg10a, eg10b, etc"
	@echo "        eg11 (= nestedRectangle), eg11a (=nestedRectanglea)"
	@echo "        eg12 (= maze), eg12a"
	@echo "        eg13 (= troom), eg13a"
	@echo "        eg100, eg100a"
	@echo "        eg150"
	@echo "        eg200, eg200a, eg200b"
	@echo "        eg300, eg300a"

cmake build/2-links/2-links:
	cd build; \
	    cmake ../.. ;\
	    make; echo "\n\n AFTER MAKE \n\n"  

moretest: $(TARGETS)
# nothing to do

# Deprecated (use "2-links" instead):
# 2-links-orig: $(OBJ_FILES) 2-links.o
#	${CXX} $(OBJ_FILES) $(LDFLAGS) 2-links.cpp $(CORE_LIB) -o 2-links
2-links: $(OBJ_FILES) 2-links.o
	${CXX} $(OBJ_FILES) $(LDFLAGS) 2-links.cpp $(CORE_LIB) -o 2-links

# Cmake version:
# 2-links: build/2-links/2-links
#	ln -s build/2-links/2-links 2-links


#=================================================
# Examples of running 2-links program
#=================================================
# choose an interesting example as default!
eg: eg10

# this is default, but every option may be changed at command line:
egx: 
	./2-links $(interactive) $(startX) $(startY)  $(startTheta1) $(startTheta2) \
	    	$(goalX) $(goalY) $(goalTheta1) $(goalTheta2) \
		$(epsilon) $(len1) $(len2) $(thickness) \
		$(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# run with default compiled options:
ega:	$(TARGETS)
	@echo "Target 'ega':  runs 2-links with default compiled options"
	./2-links &

# run with the file input2f.txt
# 	NOTE: input2f.txt is the variant of input2.txt in which we
# 		split the concave polygon into two convex one!!!
egb: $(TARGETS)
	./2-links $(interactive) \
	    60 350 330 140 \
	    300 60 90 30 \
	    5 45 65 $(thickness) input2f.txt \
	    $(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
	    0 222 $(inputDir) \
	    $(xtrans) $(ytrans) $(scale) \
	    $(verbose) $(title) \
	    $(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# Simple NO PATH example:
nopath eg0:	./2-links
	@echo "Target 'eg0':  runs 2-links with default options for file input0.txt"
	@echo "--  this example has no path..."
	./2-links $(interactive) \
	    	60 350 330 140 \
	    	200 160 90 0 \
	    	5 45 65 $(thickness) input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

eg1: ./2-links
	@echo "Target eg1: Barrier environment" 
	./2-links $(interactive) 195 340 0 150 270 80  55 146\
		4 46 60 $(thickness) basic3.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 1 \
		$(verbose) "Eg1: Barrier environment" \
		$(smarterStrategy) \
		4 \
		$(crossing) \
		90 \
		&

eg2: ./2-links
	@echo "Target eg2: 8-way junction" 
	./2-links $(interactive) 216 297 115 155 210 220 260 200\
		4 85 80 8 zigzagrotation.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 1 \
		$(verbose) "Eg2: 8-ways with 3 more obstacles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		30 \
		&
		
eg2a: ./2-links
	@echo "Target eg2: 8-way junction non-crossing test" 
	./2-links $(interactive) 216 297 155 115 210 220 260 200\
		4 85 80 8 zigzagrotation.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 1 \
		$(verbose) "Eg2: 8-ways with 3 more obstacles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg2b has smaller epsilon and longer links. It generates a more interesting path

eg2b: ./2-links
	@echo "Target eg2: 8-way junction" 
	./2-links $(interactive) 216 297 95 175 210 220 270 190\
		2 88 98 8 zigzagrotation.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		0 0 1 \
		$(verbose) "Eg2: 8-ways with 3 more obstacles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		79 \
		&
# eg3: input3.txt is a junction with 8 convergent passages
eg3 8way: ./2-links
	@echo "Target eg3: Hall way with 8 convergent hallways"
	./2-links $(interactive) 262 250 180 270 271 256 90 0\
		4 85 74 6 8way.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg3: 8 ways (L2=74->75, path->NOPATH" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		30 \
		&

eg3a 8waya: ./2-links
	@echo "Target eg3b: Hall way with 8 convergent hallways"
	./2-links $(interactive) 262 250 180 270 271 256 90 0\
		4 85 75 $(thickness) 8way.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg3a: 8 ways (L2=75->74, NOPATH->path" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		30 \
		&

eg3b 8wayb: ./2-links
	@echo "Target eg3: Hall way with 8 convergent hallways"
	./2-links $(interactive) 262 160 90 270 271 346 270 90\
		4 90 70 $(thickness) 8way.txt \
		512 512 $(windowPosX) $(windowPosY) \
		4 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg3b: 8 ways" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# BUG TRAPS:
# 
# Bugtrap example -- Non-Existence of path, but barely!
eg4 egbug bug: ./2-links
	@echo "Target 'bug':  runs 2-links for bugtrap input"
	@echo "--  this example shows there is NO PATH, but barely!"
	@echo "--  If you decrease the epsilon from 5 to 4, there is a path."
	@echo "--  If you decrease the longer link from 79 to 78, there is a path."
	./2-links $(interactive) 200 320 30 60 30 30 90 120 \
		4 45 66 0 bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg4: Bugtrap" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# Bugtrap example -- Existence of path, but barely!
eg4a egbuga buga: ./2-links
	@echo "Target 'bug':  runs 2-links for bugtrap input"
	@echo "--  this example shows there is NO PATH, but barely!"
	@echo "--  If you decrease the longer link from 79 to 78, there is a path"
	@echo "--  Or, you decrease the epsilon from 5 to 4, there is a path"
	./2-links $(interactive) 200 320 30 60 30 30 90 120 \
		4 45 79 $(thickness) bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg4: Bugtrap" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&


# Bugtrap2 example -- Existance of path.
eg5 egbug2 bug2: ./2-links
	@echo "Target 'eg5 or bug2':  Double bugtrap -- path!"
	@echo "--  this examples shows there is PATH, but barely so!"
	./2-links $(interactive) 190 210 180 300 30 30 90 340\
		4 55 50 $(thickness) bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg5: Double Bugtrap (bandwidth=95/100 => path/NOPATH)"\
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		95 \
		&

# Bugtrap2 example -- Non-existance of path.
eg5a egbug2a bug2a: ./2-links
	@echo "Target 'eg5a or bug2a':  Double bugtrap -- NOPATH"
	@echo "--  this examples shows there is PATH, but barely so!"
	./2-links $(interactive) 190 210 180 300 30 30 90 340\
		4 55 50 $(thickness) bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg5a: Double Bugtrap (bandwidth=95/100 => path/NOPATH)"\
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		100 \
		&

# Bugtrap2 example -- Non-existance of path (but compare with eg5a)
eg5b egbug2b bug2b: ./2-links
	@echo "Target 'eg5b or bug2b':  Double bugtrap -- NOPATH, but compare to eg5a"
	@echo "--  this examples shows NOPATH, but barely so!"
	./2-links $(interactive) \
		30 30 90 340 \
		190 210 180 300 \
		4 55 50 $(thickness) bugtrap2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg5b: Double Bugtrap (NOPATH, but compare to eg5a)"\
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		100 \
		&

# eg6: This environment is taken from Kavraki's groups Demo for OOPSMP:
#  NOTE: we must do scaling of the input
#  	(because dimensions came from xfig)
eg6: ./2-links
	@echo "Target DemoSE2_PRM (from Kavraki's group)"
	./2-links $(interactive) 190 420 180 270 300 65 0 70\
		4 125 160 $(thickness) DemoSE2_PRM.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		250 250 0.04 \
		$(verbose) "Eg6: Target DemoSE2_PRM (from Kavraki's group)"\
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg7, egtunnel:  tunnel example from Salzman et al.(2013)
# 	NOTE that the current search strategy seems to miss the direct path!!
# 	TODO: develop a better search (e.g. Best First Search)
eg7 salzman tunnel: ./2-links
	@echo "Eg7: Target tunnel.poly (from Salzman et al.(2013))"
	./2-links $(interactive) 210 330 110 0 450 220 0 170\
		4 50 75 $(thickness) tunnel.poly \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed) $(inputDir) \
		100 200 0.05 \
		$(verbose) "Eg7: Target tunnel.poly (paper of Salzman et al.(2013))"\
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&
		
# eg8: from Paper of Hsu-Lathombe-Kurniawati
eg8 hsu: ./2-links
	@echo "Target eg8: example from paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when bandwidth=7, epsilon=2"
	@echo " 	There is NO PATH... when bandwidth=8, epsilon=2"
	./2-links $(interactive) 20 390 0 270 275 180 270 0\
		2 30 25 $(thickness) hsu-latombe-kurniawati3.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-70 -70 0.065 \
		$(verbose) "Eg8: From Hsu, Latombe, Kurniawatti (2006), path" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		7 \
		&

# eg8a: from Paper of Hsu-Lathombe-Kurniawati
eg8a hsua: ./2-links
	@echo "Target eg8a: example from paper of Hsu, Latombe, Kurniawati (2006)"
	@echo "	There is a path passage... when bandwidth=7, epsilon=2"
	@echo " 	There is NO PATH... when bandwidth=8, epsilon=2"
	./2-links $(interactive) 20 390 0 270 275 180 270 0\
		2 30 25 $(thickness) hsu-latombe-kurniawati3.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-70 -70 0.065 \
		$(verbose) "Eg8a: From Hsu, Latombe, Kurniawatti (2006), NO-PATH!" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		8 \
		&

# eg9: narrow corridor (path, but see eg9a)
eg9 narrow: ./2-links
	@echo "Target eg9: Shows the differences with bandwidth"
	./2-links $(interactive) 185 370 0 270 175 160 270 350 \
		4 70 90 $(thickness) narrowCorridor.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 0.065 \
		$(verbose) "Eg 9/9a:  Corridor (bandwidth=53/54 => path/NOPATH)" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		53 \
		&
# eg9a: narrow corridor (NOPATH, but see eg9)
eg9a narrowa: ./2-links
	@echo "Target eg9a: Shows the differences between with bandwidth"
	./2-links $(interactive) 185 370 0 270 175 160 270 350 \
		4 70 90 $(thickness) narrowCorridor.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 0.065 \
		$(verbose) "Eg 9/9a:  Corridor (bandwidth=53/54 => path/NOPATH)" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		53 \
		&

# eg9b: narrow corridor (NOPATH, but see eg9)
eg9b narrowb: ./2-links
	@echo "Target eg9b: test the differences between crossing and non-crossing"
	./2-links $(interactive) 185 370 0 270 175 160 270 0\
		4 110 90 $(thickness) narrowCorridor.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 0.065 \
		$(verbose) "Eg 9b:  Narrow Corridor non-crossing test" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		$(bandwidth) \
		&
# eg9c: narrow corridor (NOPATH, but see eg9)
eg9c narrowc: ./2-links
	@echo "Target eg9c: test the differences between crossing and non-crossing"
	./2-links $(interactive) 185 370 0 270 175 160 270 0\
		4 110 90 $(thickness) narrowCorridor.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		0 0 0.065 \
		$(verbose) "Eg 9b:  Narrow Corridor non-crossing test" \
		$(smarterStrategy) \
		$(threshold) \
		0 \
		$(bandwidth) \
		&
				
eg10 2chambers: ./2-links
	@echo "Target eg10: two Chamber environment using Greedy"
	./2-links $(interactive) 425 410 180 270 105 90 90 0\
		2 75 75 0 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg 10: 2 Chambers" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		80 \
		&

eg10a 2chambersa: ./2-links
	@echo "Target eg10: two Chamber environment using Random Search"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 415 400 180 270 105 90 90 0\
		4 8 10 $(thickness) 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		0 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg10a: 2 Chambers with shorter links - Random" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&
		
eg10b 2chambersb: ./2-links
	@echo "Target eg10: two Chamber environment using Greedy and shorter links"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 415 400 180 270 105 90 90 0\
		4 15 20 $(thickness) 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg 10b: 2 Chambers with shorter links - Greedy" \
		1 \
		6 \
		&
		
eg10c 2chambersc: ./2-links
	@echo "Target eg10c: test the differences between crossing and non-crossing"
	./2-links $(interactive) 425 410 180 270 105 90 0 90\
		4 87 87 $(thickness) 2chambers.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg10c: 2 Chambers non-crossing test" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

eg11 nestedRectangle: ./2-links
	@echo "Target eg11: 3nd test example from Chee and Zhongdi"
	./2-links $(interactive) 425 390 180 270 95 100 0 90\
		2 50 40 6 nestedRectangle.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg 11: Nested Rectangle" \
		0 \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&
		
eg11a nestedRectanglea: ./2-links
	@echo "Target eg11: 3nd test example from Chee and Zhongdi"
#	@echo "	There is a path passage... when radius=6, epsilon=1"
#	@echo " 	There is No Path... when radius=7, epsilon=1"
	./2-links $(interactive) 425 390 180 270 95 100 0 90\
		2 10 10 $(thickness) nestedRectangle.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(Qtype) $(seed)  $(inputDir) \
		-220 0 0.065 \
		$(verbose) "Eg 11a: Nested Rectangle with shorter links" \
		1 \
		4 \
		&

		
# maze example!
eg12 maze: ./2-links
	./2-links $(interactive) 375 400 180 0 105 60 0 180\
		4 30 33 0 input4.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 12: Maze " \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		146 \
		&
		
eg12a mazea: ./2-links
	./2-links $(interactive) 405 400 180 270 105 60 0 180\
		4 15 15 $(thickness) input4.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed) $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 12a: Maze " \
		1 \
		1 \
		1 \
		$(bandwidth) \
		&
# eg13:  T-Room example -- to show clearly the difference between
# 		self-crossing and non-crossing  
eg13 troom: ./2-links
	./2-links $(interactive) 257 230 251 294 252 420 294 251\
		4 94 85 \
		0 T-room2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		25 -120 0.08 \
		$(verbose) "Eg13: T-room with path (bandwidth=10/11=>path/NOPATH)" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		10 \
		&

# eg23: L-Room example -- this is a variation of the T-Room example (with no path)
eg23 lroom: ./2-links
	./2-links $(interactive) 257 230 251 294 252 420 294 251\
		4 94 85 \
		0 L-room2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		25 -120 0.08 \
		$(verbose) "Eg23: L-room with NOPATH" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		10 \
		&
# eg13a:  T-Room example -- to show clearly the difference between
# 		self-crossing and non-crossing
eg13a trooma: ./2-links
	./2-links $(interactive) 257 230 251 294 252 420 294 251\
		4 94 85 \
		0 T-room2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		25 -120 0.08 \
		$(verbose) "Eg13a: T-room with NOPATH (bandwidth=10/11=>path/NOPATH)" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		11 \
		&

# eg13b:  T-Room example -- bigger magnification (for images in paper)
# 		Difference if allow self-crossing or not
eg13b troomb: ./2-links
	./2-links $(interactive) 260 280 256 286 260 490 278 245\
		4 130 80 \
		0 T-room.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		20 -100 0.08 \
		$(verbose) "Eg13b: T-room with path (no self-crossing)" \
		$(smarterStrategy) \
		$(threshold) \
		1 \
		29 \
		&
		
# eg13c:  T-Room example -- bigger magnification (for images in paper)
# 		Difference if allow self-crossing or not
eg13c troomc: ./2-links
	./2-links $(interactive) 260 280 256 286 260 490 278 245\
		4 130 80 \
		0 T-room.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		20 -100 0.08 \
		$(verbose) "Eg13c: T-room with path (allow self-crossing)" \
		$(smarterStrategy) \
		$(threshold) \
		0 \
		29 \
		&
		

# RANDOMLY GENERATED ENVIRONMENT:
#	This one, generated using our Python Generator, has 200 2-links.
#	Note that these 2-links are not disjoint, but our theory remains
#	valid for such inputs.  This is the power of the SSS approach (over
#	the exact algorithms.

# eg100 100: 100 random  triangles
eg100 100: ./2-links
	@echo "Target eg100:  runs 2-links amidst 100 randomly generated triangles"
	./2-links $(interactive) 10 490 0 270 495 180 270 0\
		8 40 60 $(thickness) input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 100: 100 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg100a: 100 random triangles
eg100a 100a: ./2-links
	@echo "Target eg100a:  runs 2-links amidst 100 randomly generated triangles"
	@echo "This one is a bit harder than eg100 (with radius 8 instead of 5)"
	./2-links $(interactive) 200 270 0 20 20 0\
		10 8 $(thickness) input100.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 100a: 100 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg150: 150 random triangles
eg150 150: ./2-links
	@echo "Target eg150:  runs 2-links amidst 150 randomly generated triangles"
	./2-links $(interactive) 5 300 0 270 380 380 270 45\
		4 40 60 $(thickness) input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		4 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 150: 150 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg150a: 150 random triangles
eg150a 150a: ./2-links
	@echo "Target eg150a:  runs 2-links amidst 150 randomly generated triangles"
	./2-links $(interactive) 5 300 0 270 380 240 270 0\
		4 40 60 $(thickness) input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 150a: 150 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg150c: 150 random triangles
eg150c 150c: ./2-links
	@echo "Target eg150c:  runs 2-links amidst 150 randomly generated triangles"
	./2-links $(interactive) 5 500 0 270 380 240 270 0\
		8 40 60 $(thickness) input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		3 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 150c: 150 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg200: 200 random triangles
eg200 200: ./2-links
	@echo "Target eg200:  runs 2-links on randomly generated triangles"
	@echo "--  This may take a noticeable amount bit of time!!!"
	./2-links $(interactive) \
	    	10 490 0 270 \
		320 280 270 0 \
		4 40 60 $(thickness) input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 $(seed)  $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 200: 200 randomly generated triangles (has path)" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg200a: there is an obvious path but probabilistic methods might waste
# 	lots of time checking elsewhere!
#
eg200a 200a: ./2-links
	@echo "Target eg200a:  runs 2-links on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./2-links $(interactive) \
	    	10 490 0 270 \
		495 180 270 0 \
		5 40 60 $(thickness) input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 200a: 200 randomly generated triangles (NO-PATH)" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg200b: there is no path because eps=5, but path if eps=4
#
eg200b: ./2-links
	@echo "Target eg200b:  runs 2-links on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./2-links $(interactive) \
	    	50 190 0 270 \
		335 300 270 0 \
		4 40 60 $(thickness) input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 200b: 200 triangles (eps: 4->5, path->NOPATH)" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg200c: there is no path because eps=5, but path if eps=4
#
eg200c: ./2-links
	@echo "Target eg200c:  runs 2-links on randomly generated obstacles"
	@echo "--  This may take a noticeable amount bit of time, "
	@echo "    depending on the search strategy"
	./2-links $(interactive) \
	    	50 190 0 270 \
		335 300 270 0 \
		5 40 60 $(thickness) input200.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 200c: 200 triangles (eps: 4->5, path->NOPATH)" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# eg300: 300 random 2-links
#
eg300: ./2-links
	@echo "Target eg300:  runs 2-links amidst 300 randomly generated 2-links"
	@echo "--  This may take a noticeable amount bit of time "
	@echo "    depending on the search strategy"
	./2-links $(interactive) \
	    	10 400 90 270 \
		270 190 270 90 \
		4 40 35 $(thickness) input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 300: 300 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		127 \
		&

eg300a: ./2-links
	@echo "Target eg300a:  runs 2-links amidst 300 randomly generated 2-links"
	@echo "--  Slightly more challenging than eg300. "
	./2-links $(interactive) \
	    	10 400 90 270 \
		250 200 290 30 \
		4 40 30 $(thickness) input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		2 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) "Eg 300a: 300 randomly generated triangles" \
		$(smarterStrategy) \
		$(threshold) \
		$(crossing) \
		$(bandwidth) \
		&

# under development...
# (1) Run all series of examples using Qtypes=0,1,2 and compare timing


#=================================================
# Experiments for our paper 
#=================================================
#bugtrap: radius 40, other setting same as buga
#200: alpha 130, 460, beta 20, 20, other setting same as eg200
#300: alpha 230, 210, beta 480, 10, other setting unchanged
#150: radius 7, epsilon 1, other setting unchanged

ex-all: ex-bug ex-150 ex-200 ex-300

# Bugtrap example 
ex-bug experiment-bug: ./2-links
	@echo "Target 'ex-bug':  runs 2-links for bugtrap input, bugtrap.txt"
	./2-links 1 200 350 0 30 30 0 \
		17 40 $(thickness) bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# Bugtrap example 
ex-buga: ./2-links
	@echo "Target 'ex-bug':  runs 2-links for bugtrap input, bugtrap.txt"
	./2-links 1 200 350 0 30 30 0 \
		1 15 $(thickness) bugtrap.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 11 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# ex-150: 150 random 2-links (use radius=1)
ex-150: ./2-links
	@echo "Target ex-150:  runs 2-links amidst 150 randomly generated 2-links"
	./2-links 1 200 270 0 20 20 0\
		1 7 $(thickness) input150.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&

# ex-200: 200 random 2-links
ex-200: ./2-links
	@echo "Target 2x-200:  runs 2-links on randomly generated obstacles"
	./2-links 0 130 460 0 20 20 0 \
		5 2 $(thickness) input1.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&
	#
# ex-300: 300 random 2-links
#
ex-300: ./2-links
	@echo "Target ex-300:  runs 2-links amidst 300 randomly generated 2-links"
	./2-links 0 230 210 0 480 10 0\
		3 3 $(thickness) input300.txt \
		512 512 $(windowPosX) $(windowPosY) \
		1 1111 $(inputDir) \
		$(xtrans) $(ytrans) $(scale) \
		$(verbose) $(title) \
		&



#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim 2-links.cpp

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

