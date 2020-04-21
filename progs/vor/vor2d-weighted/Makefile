# Makefile for progs/robotics/vor/vor2d
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "vor2d"
# 	       			 	and run a default demo interactively
# 	       > make all	-- compiles vor2d into 3 CORE Levels
#				   	(vor2d_lev1,  vor2d_lev2, vor2d_lev3)
# 	       > make test	-- compiles and runs "vor2d" non-interactively
# 	       			 	(this is Core Library's automatic self-test)
#
#
# (B)	But you probably want to try the various interactive demos of this program:
#
# 	       > make eg	-- compiles and runs "vor2d" interactively
#
#	This is equivalent to typing:
#
#		> ./vor2d $(interactive) \
#			$(pseudo) $(interior) $(fileName) \
#			$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
#			$(inputDir) \
#			$(xtrans) $(ytrans) $(scale) \
#			$(closePoly) $(c1) $(c2) \
#			$(maxEps)
#
#	where the meaning of these arguments are described below when we
#	define these parameters.
#
#	To see what these default parameters are, you can type
#
#		> make params
#
# (C)	You can override any of these parameters at the command line.
#	For instance, to change the input file to "input100.txt", you may type:
#
#		> make eg fileName=input100.txt
#
#	Various input environments are found in the subdirectory called "inputs".
#
# 	REMARK:
# 		By default, we call the executable program "vor2d".
# 	But "vor2d" is really the last compiled version of the program,
# 	and it could represent Levels 1, 2, or 3.   Hence, to choose the
# 	CORE Level explicitly, you should call
#
#		> make eg fileName=input100.txt LEV=X  
#			(where X=1,2,3)
#
#	Then the program "vor2d_levX" will be called.
# 	       	
# (D)	Instead of the target "eg" you can try these targets (alternative
# 	names are in parenthesis):
#
#		eg0 (annulus)
#		eg1 (simple)	-- variant: eg1a (for medial axis)
#		eg2 (simple2)
#		eg3 (dog)
#		eg4 (line)	-- variant: eg4a, eg4b (linea, lineb) 
#		eg5 (boxes)	-- variant: eg5a (boxesa) 
#		eg6 (boundedstar)
#		eg7 (bug)	-- a bug trap
#		eg8 (overlap)
# 		eg9 (2color)	-- variant: eg9a, eg9b
# 		eg10 (2tri)
# 		eg11 (star) 	-- variant: eg11a (stara)
# 		eg12 (bug2) 	-- a double bug trap
# 		eg13 (monkey)
# 		eg14 (mammoth)	
# 		eg15 	 	-- Vikram's degenerate example
# 		eg150		-- 150 random overlapping triangles 
# 		eg200		-- 200 random overlapping triangles 
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
################################################################################
# 	Chee Yap, Jyh-Ming Lien, Vikram Sharma
# 	May 20, 2012
# 	Since Core Library 2.1
# 	$Id:$
################################################################################

#=================================================
#	Core Library Options
#=================================================
VAR=Debug
VAR=Opt



CORE_PATH=../../..
include ../Make.options

# CHOOSE YOUR CORE_LEVEL
# 	(LEV=0 is really the same as LEV=1 but it affects this Makefile)
LEV=2
LEV=3
LEV=1
LEV=0

#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

epsilon = 2		# resolution parameter
epsilon = 10		# resolution parameter

pseudo = 0		# boolean: 0 or true => show pseudo Vor objects
pseudo = 1		# boolean: 1 or false => do not show pseudo Vor objects
interior = 0		# boolean: 0 or true => show interior Vor objects
interior = 1		# boolean: 1 or false => do not show interior Vor objects

inputDir = inputs	# where to find input files
fileName = input2d.txt	# input environment file
fileName = input0.txt	# input environment file
fileName = input.txt	# input environment file
fileName = input100.txt	# input environment file
fileName = input150.txt	# input environment file
fileName = input200.txt	# input environment file
fileName = input300.txt	# input environment file
fileName = bugtrap.txt	# input environment file

boxWidth = 512		# initial configuration box size
			# See eg20 for configuration size 768 (1.5 times
			# larger)
boxHt = 512

windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 450	# initial Window position
windowPosY = 20	

seed = 11		# seed for random
step = 0		# number of steps to run
			#	(step=0 means run to completion)
		
xtrans = 5		# x-translation of the input data 
ytrans = 5		# y-translation of the input data
scale  = 1		# scaling of input data 
closePoly  = 1  # if true (non zero) then do not attempt to close input polygons.
c1  = 0			# if 0 (false) then do not use c1 filter
c2  = 0			# if 0 (false) then do not use c2 filter

maxEps = 200		# maximum size of a box containing an Voronoi point

#=================================================
# These are generated variables -- do not change
#=================================================
TARGETS = vor2d
ALL_TARGETS = vor2d_lev3 vor2d_lev2 vor2d_lev1
SUBDIRS = . 
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
OBJ_FILES = $(OBJS)
CXXFLAGS += -g -MMD

DEPENDS=Box.o fileProcessor.o QuadTree.o Corner.o Wall.o Timer.o

VOR2D = ./vor2d_lev${LEV}
ifeq (${LEV},0)
    VOR2D = ./vor2d
endif


#=================================================
# Targets for CORE LIBRARY TESTING
#=================================================

all: $(VOR2D) $(ALL_TARGETS)

default: eg

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode (i.e., the first argument to "vor2d" is "1").
#
test: $(ALL_TARGETS)
	./vor2d_lev1 1 \
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps)
	./vor2d_lev2 1 \
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps)
	./vor2d_lev3 1 \
		1 $(pseudo) $(interior) input.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps)

testmore: $(ALL_TARGETS)
	make eg0 interactive=1 LEV=1
	make eg1 interactive=1 LEV=1
	make eg3 interactive=1 LEV=1
	make eg0 interactive=1 LEV=2
	make eg1 interactive=1 LEV=2
	make eg3 interactive=1 LEV=2
	make eg0 interactive=1 LEV=3
	make eg1 interactive=1 LEV=3
	make eg3 interactive=1 LEV=3

testa: $(TARGETS)
	$(VOR2D) 1 &

timing1: ./vor2d_lev1 ./vor2d_lev2 ./vor2d_lev3
	@echo "Level 1"
	make eg2 LEV=1
	@echo "Level 2"
	make eg2 LEV=2 
	@echo "Level 3"
	make eg2 LEV=3

lev1 vor2d_lev1:
	make clean
	make LEV=1 compile

lev2 vor2d_lev2:
	make clean
	make LEV=2 compile

lev3 vor2d_lev3:
	make clean
	make LEV=3 compile

compile: ./vor2d
	-cp ./vor2d $(VOR2D)

#vor2d: $(OBJ_FILES)

help:
	@echo "USAGE: Type the following"
	@echo "  > make eg                 -- to see an example"
	@echo "  > make params             -- to see default parameters of eg"
	@echo "  > make eg epsilon=7 [...] -- change the default epsilon [...] of eg"
	@echo "  > make egN                -- where N = 0, 1, 2, ..., 14, 15, 150, 200"
	@echo "                               to see various examples "
	@echo " LIST of TARGETS:"
	@echo "		eg0 (annulus)"
	@echo "		eg1 (simple)	-- variant: eg1a (for medial axis)"
	@echo "		eg2 (simple2)"
	@echo "		eg3 (dog)"
	@echo "		eg4 (line)	-- variant: eg4a, eg4b (linea, lineb) "
	@echo "		eg5 (boxes)	-- variant: eg5a (boxesa) "
	@echo "		eg6 (boundedstar)"
	@echo "		eg7 (bug)	-- a bug trap"
	@echo "		eg8 (overlap)"
	@echo "		eg9 (2color)	-- variant: eg9a, eg9b"
	@echo "		eg10 (2tri)"
	@echo "		eg11 (star) 	-- variant: eg11a (stara)"
	@echo "		eg12 (bug2) 	-- a double bug trap"
	@echo "		eg13 (monkey)"
	@echo "		eg14 (mammoth)"	
	@echo "		eg15 	 	-- Vikram's degenerate example"
	@echo "		eg150		-- 150 random overlapping triangles "
	@echo "		eg200		-- 200 random overlapping triangles "



#=================================================
# TARGETS with demos (examples) of running vor2d program
#=================================================
#
# This is default, but every option may be changed at command line:
#
eg:	$(VOR2D)
	@echo "Target 'eg':  runs vor2d with the default options of Makefile"
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) $(fileName) \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# A first weighted Voronoi diagram example.
egw0:	$(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) weighted-input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&


# Eg0: Simple NO PATH example (an triangular annulus region)
nopath eg0:	$(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) input0.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg1: Simple example with 3 obstacles (2 Voronoi vertices, one concave verteix)
# 	REMARK: input.txt and input2.txt are really the same.
# 		But input2.txt has comments in it.
simple eg1:	$(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) input2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg1a: Modification of Eg1 using input2a.txt,
# 		in which the polygon vertices are in clockwise order!
eg1a:	$(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) input2a.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
	#
# Eg2: Same as simple, but with bounding box
simple2 eg2: $(VOR2D)
	./$(VOR2D) $(interactive)\
		1 $(pseudo) $(interior) input5.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg3: dog example
eg3 dog:	$(VOR2D)
	@echo "Target 'eg3' or 'dog': "
	$(VOR2D) $(interactive) \
		0.1 $(pseudo) $(interior) dog.poly \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		100 256 1.4 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg4: Example of a non-regularized input (set of 4 lines) with closed polygons
eg4 line:	$(VOR2D)
	@echo "Target 'eg4' or 'line': "
	$(VOR2D) $(interactive) \
		0.5 $(pseudo) $(interior) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 1 \
		0 $(c2) $(maxEps) \
		&
	
# Eg4a: Example of a non-regularized input (set of 4 lines) but using C1 filter
eg4a linea:	$(VOR2D)
	@echo "Target 'eg4a' or 'linea': "
	$(VOR2D) $(interactive) \
		0.5 $(pseudo) $(interior) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 1 \
		1 $(c2) $(maxEps) \
		&
	
# Eg4b: Example of a non-regularized input (set of 4 lines) but open polygons 
eg4b lineb:	$(VOR2D)
	@echo "Target 'eg4b' or 'lineb': "
	$(VOR2D) $(interactive) \
		0.5 $(pseudo) $(interior) line.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 0 \
		0 $(c2) $(maxEps) \
		&
	
# Eg5: A degenerate input with 5 boxes
eg5 boxes: $(VOR2D)
	$(VOR2D) $(interactive)\
		1 $(pseudo) $(interior) boxes.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) 100 100 $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg5a: A degenerate input with 5 boxes (but with no translations, our current
# 	code shows some discontinuous curves)
eg5a boxesa: $(VOR2D)
	$(VOR2D) $(interactive)\
		1 $(pseudo) $(interior) boxes.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) 0 0 $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg6: A triangular star within a bounding frame
eg6 boundedstar: $(VOR2D)
	$(VOR2D) $(interactive)\
		1 $(pseudo) $(interior) input6.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg7: bugtrap
eg7 bug: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) bugtrap.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg7: bugtrap
eg7X bugX: vor2d_lev3
	./vor2d_lev3 $(interactive) \
		1 $(pseudo) $(interior) bugtrap.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg8: Overlapping polygons: 2 triangles + 2 concentric squares (with funny orientation)
eg8 overlap: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) overlap.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9x: Box containing Voronoi Vertex whose vertices has only two colors
# 	CORRECT trans/scale params:   xtrans=-50  ytrans=80,   scale = 0.1
eg9x 2colorx: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2colorX.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-50 80 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9: Box containing Voronoi Vertex whose vertices has only two colors
eg9 2color: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2color.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-360 -150 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9a: Box containing Voronoi Vertex whose vertices has only two colors
eg9a 2colora: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2color.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-350 -150 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg9b: Box containing Voronoi Vertex whose vertices has only two colors
eg9b 2colorb: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) vorBoxWith2color.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		-340 -150 0.1 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg10: Two triangles generating a Voronoi curve
eg10 2tri: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) input10.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
# Eg11: Star example
eg11 star:	$(VOR2D)
	$(VOR2D) $(interactive) 1 $(pseudo) $(interior) star.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		100 100 10 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
	#
# Eg11a: with pseudo=1(false) and interior=1(false)
eg11a stara:	$(VOR2D)
	$(VOR2D) $(interactive) 1 1 1 star.poly \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		100 100 4 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg12: bugtrap2  (one bug trap inside another)
eg12 bug2: $(VOR2D)
	$(VOR2D) $(interactive) \
		1 $(pseudo) $(interior) bugtrap2.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg13: monkey example
eg13 monkey:	$(VOR2D)
	@echo "Target 'eg13' or 'monkey': "
	$(VOR2D) $(interactive) \
		0.1 $(pseudo) $(interior) nazca_monkey.poly \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		220 220 2 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg14: mammoth example
eg14 mammoth:	$(VOR2D)
	@echo "Target 'eg14' or 'mammoth': "
	$(VOR2D) $(interactive) \
		0.1 $(pseudo) $(interior) mammoth.poly \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		220 220 20 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
			
# Eg15: Vikram's example -- THIS INPUT IS DEGENERATE !!
#
eg15 vikram:    $(VOR2D)
	 @echo "Target 'eg15' : a degenerate example"
	 @echo "   AT LEVEL 1, You will see an obvious error."
	 @echo "       But that is NOT a programming error."
	 @echo "   AT LEVEL 3, Core Library will detect a DIVIDE-BY-ZERO."
	 $(VOR2D) $(interactive) \
		 0.1 $(pseudo) $(interior) Vikram-1.txt \
		 512 512 $(windowPosX) $(windowPosY) \
		 $(inputDir) \
		 $(xtrans) $(ytrans) $(scale) $(closePoly) \
		 $(c1) $(c2) $(maxEps) \
		 &

# Eg15a: Vikram example (variant)
eg15a :	$(VOR2D)
	@echo "Target 'eg15a' :"
	$(VOR2D) $(interactive) \
		0.1 $(pseudo) $(interior) Vikram-2.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
	
# Eg16: degenerate
eg16 :	$(VOR2D)
	@echo "Target 'eg16 (degenerate)' :"
	$(VOR2D) $(interactive) \
		0.1 $(pseudo) $(interior) input7.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
										

# Eg20: contour examples
eg20 :	$(VOR2D)
	@echo "Target 'eg20 (contour problem)' :"
	$(VOR2D) $(interactive) \
		0.1 $(pseudo) $(interior) contour.txt \
		512 768 350 20 \
		$(inputDir) \
		-50 100 0.06 $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&

# Eg50: standard parabola
eg50 parabola:	$(VOR2D)
	@echo "Target 'eg50' or 'line': "
	$(VOR2D) $(interactive) \
		0.5 $(pseudo) $(interior) parabola.txt \
		512 512 $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) 1 \
		1 $(c2) 10 \
		&
	
# Eg150: Example with 150 randomly generated, overlapping triangles
eg150: $(VOR2D)
	$(VOR2D) $(interactive) \
		4 $(pseudo) $(interior) input150.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
eg200: $(VOR2D)
	$(VOR2D) $(interactive) \
		4 $(pseudo) $(interior) input200.txt \
		$(boxWidth) $(boxHt) $(windowPosX) $(windowPosY) \
		$(inputDir) \
		$(xtrans) $(ytrans) $(scale) $(closePoly) \
		$(c1) $(c2) $(maxEps) \
		&
		
moretest: $(TARGETS)
# nothing to do

$(TARGETS): $(OBJ_FILES)

#=================================================
# Temp
#=================================================
m:
	gvim Makefile
vi:
	gvim vor2d.cpp

#=================================================
# Rules
# 	First clean out all suffixes:
.SUFFIXES:
#	Then establish the suffixes we want for this file:
.SUFFIXES: .cpp .o
#=================================================

%: %.o
#	${CXX} $(OBJ_FILES) $(LDFLAGS) $(CORE_LIB) -o $@
	${CXX} $(OBJ_FILES) $(LDFLAGS) -L/usr/lib -lglui $(CORE_LIB) -o $@

.cpp.o:
	${CXX} $(CXXFLAGS) $(CORE_INC) -DCORE_LEVEL=$(LEV) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

# The following line is causing trouble with levels...
include Dependencies

#=================================================
# Misc
#=================================================

params:
	@echo "================ Parameters =================="
	@echo "    interactive = " ${interactive}
	@echo "    epsilon = " ${epsilon}
	@echo "    pseudo = " ${pseudo}
	@echo "    interior = " ${interior}
	@echo "    inputDir = " ${inputDir}
	@echo "    fileName = " ${fileName}
	@echo "    boxWidth = " ${boxWidth}
	@echo "    boxHt = " ${boxHt}
	@echo "    windowPosX = " ${windowPosX}
	@echo "    windowPosY = " ${windowPosY}
	@echo "    seed = " ${seed}
	@echo "    step = " ${step}
	@echo "    xtrans = " ${xtrans}
	@echo "    ytrans = " ${ytrans}
	@echo "    scale = " ${scale}
	@echo "    closePoly = " ${closePoly}
	@echo "=============================================="

dump screendump:
		open -a /Applications/Preview.app $(screendumpfile)

ln link:
	-rm -f vor2d
	-ln -s vor2d_lev$(LEV) vor2d

#=================================================
# Clean object files
#=================================================
#  Above, we assume that "clean" will remove the executable "vor2d".
#  Hence it is important that "vor2d" is specified in TARGETS.
clean:
	-@test -z "*.o" || rm -f *.o Dependencies $(TARGETS)

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(ALL_TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS) Dependencies

#=================================================
# END Makefile 
#=================================================

