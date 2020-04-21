# Makefile for progs/gwb/polyhedron/
#
#  HOW TO USE THIS Makefile:
#
# (A)	To compile run this demo, you can use this Makefile and just have to type:
#
# 	       > make 		-- compiles the main program into "gwb"
# 	       > make test	-- compiles and runs "gwb" non-interactively
# 	       			   (this is Core Library's automatic self-test)
#
# (B)	See the default demo:
#
# 	       > make eg		-- compiles and runs "gwb" interactively
# 	       > make egx rad1=30	-- like "eg" but you can change parameters
#
#	To see what these default parameters are, you can type
#
#		> make params
#
# (C)	You can override any of these default parameters at the command line.
#	For instance, to change the input file to "input2.txt", you may type:
#
#		> make eg fineName=input2.txt
#
# (D)	Instead of the target "eg" you can try these targets (alternative
# 	names are in parenthesis):
#
#		eg0 (annulus)
#		eg1 (simple)
#		eg2 (simple2)
#		eg3 (dog)
#
# 	These targets uses different input files, all taken from
# 	the subdirectory "inputs".   
#
################################################################################
# 	Kai Cao and Chee Yap
# 	May 20, 2012
# 	Since Core Library 2.1
# 	$Id:$
################################################################################

#=================================================
#	Core Library Options
#=================================================
VAR=Opt
VAR=Debug
CORE_PATH=../../..
include ../Make.options


#=================================================
# User variables (you can change them in the command line)
#=================================================
interactive = 0		# 0=interactive, >0 is non-interactive

inputDir = inputs	# where to find input files
fileName = cube.gwb	# cube
fileName = trefoil.gwb	# trefoil
fileName = tetrahedron.gwb	# tetrahedron

windowWidth = 512
windowHeight = 512 
windowPosX = 200	# initial Window position
windowPosY = 200	
windowPosX = 450	# initial Window position
windowPosY = 20	

mode = wire		# show wire frame of surfaces
mode = face		# show faces of surfaces


model = cube		# name of model to be displayed
model = bullet
model = trefoil

rad = 40		# radius
ht = 10			# height
hor = 30		# horizontal dim
ver = 20		# vertical dim

ht2=40  		# height 2: second part of the model

#alias of the variables
radius=$(rad)
height=$(ht)
ht1=$(ht)
nsegments=$(hor)
nsections=$(ver)
height2=$(ht2)
thinkness=$(ht)

nsegments2=5		# nsegment2: how many segments in the second part
nsections2=1		# nsections2: how many sections in the second part
nparts=10		
nparts2=20

position2=80		# the position of the second part of the polyhedron: Like the distance from the leg to the center
position3=90		# the potition of the third part of the polyhedron: Like how many
thickness3=10
height3=50


rotUnitX=5		# How much it rotates when you press 'a', 'd', 's', 'w'
rotUnitY=5
rotUnitZ=5

initRotDeg=45		# Initialize the rotation of the polyhedron
initRotAxisX=1
initRotAxisY=1
initRotAxisZ=1


lightRate=0.55
# trefoil
rad1 = 40		# thickness of cross-section of knot
rad2 = 10		# diameter of overall trefoil knotf
nsections = 50		# number of cross-sections in trefoil 
nsegments = 8		# number of segments in each cross-section


## unused:
xtrans = 5		# x-translation of the input data 
ytrans = 5		# y-translation of the input data
scale  = 1		# scaling of input data 

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS = gwb
SUBDIRS = . 
SRCS=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJS=${SRCS:.cpp=.o}
OBJ_FILES = $(OBJS)
CXXFLAGS += -MMD -lGL -lGLU -lglui -lglut

all: $(TARGETS)

# note: this target is the standard target that Core Library uses
#       to test its subdirectories.   So the program must run in a
#       non-interactive mode !!!
#
test: $(TARGETS)
	./gwb 0

testall: $(TARGETS)

#=================================================
# Examples of running gwb program
#=================================================
#
# This is default, but every option may be changed at command line:

##################################################
# SIMPLE MAKEFILE:
##################################################
#compiling...
#gwb:	*.h *.cpp
#	g++ *.h *.cpp -o gwb -LGL -lglut -lglui

#eg default (does not work)
eg:	trefoil	

#eg default (does not work)
egx:	gwb	
	@echo "Target 'eg':  runs gwb with the default options of Makefile"
	@echo "--  You can all of them in you 'make' command line, e.g.,"
	@echo "--      > make eg fileName=cube.txt"
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) $(model) \
	    	$(rad1) $(rad2) $(nsegments) $(nsections) \

#eg0 
#		rad: radius of the hemisphere
#		nsegments: # of segments of the horizontal polygon ( The biggest circle )
#		nsections: # of sections of the hemisphere
# 		lightRate: to control the lightness of the face,
#		We use the inverse of the lightRate as the lightness
eg0 facehemi:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	face hemisphere 200 $(height) 20 20 $(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) $(position3) $(thickness3) $(height3) 0.55 45 1 1 1
eg0x facehemix:	gwb
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) hemisphere $(rad1) $(ht) $(nsegments) $(nsections) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg1 sphere 
#		rad: radius of the sphere
#		nsegments: # of segments of the horizontal polygon ( The biggest circle )
#		nsections: # of sections of the sphere
eg1 sphere:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) sphere 200 $(height) 20 20
eg1x spherex:	gwb
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) sphere $(rad) $(ht) $(nsegments) $(nsections)\
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg2 torus
#		rad1: radius of main circle
#		rad2: radius of the subordinate circle which goes around the main circle
#		nsegments: # of segments of the horizontal polygon ( The main circle )
#		nsections: # of sections of in a subordinate circle
eg2 donut torus :	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) torus 200 $(height) 20 20 $(height2) 50 \
		

eg2x donutx torusx :
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
	    	$(mode) torus $(rad1) $(ht) $(hor) $(ver) $(height2) $(rad2)
		$(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg3 cylinder
#		rad: radius of the circle
#		ht: height of the cylinder
#		hor: # of segments of the top/down circle
#		ver: # of layers/sections of the cylinder 
eg3 cylinder:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cylinder 200 500 20 20

eg3x cylinderx:
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cylinder $(rad) $(ht) $(hor) $(ver) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg4 disk 
#		rad: radius of the main circle
#		ht: height of the disk 
#		which is also the diameter of the hemisphere around the side of the main circle
#		hor: # of segments of the top/down circle
#		ver: # of layers/sections of the disk
eg4 disk:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) disk 200 80 20 20 
eg4x diskx:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) disk $(rad) $(ht) $(hor) $(ver) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)
#eg5 bullet 
#		rad: radius of the main circle ( or the bottom circle of the bullet )
#		ht: height of the disk cylinder,as part of the bullet 
#		hor: # of segments of the horizontal circles
#		ver: # of layers/sections of the upper hemisphere
eg5 bullet:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) bullet 100 500 30 20 \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		40 1 1 1

eg5x bulletx:
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) bullet $(rad) $(ht) $(hor) $(ver) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg6 cone 
#		rad: radius of the bottom circle
#		ht: height of the cone
#		hor: # of segments of the horizontal circles
#		ver: # of layers/sections of the cone
eg6 cone:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cone 200 500 30 20

eg6x conex:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) cone $(rad) $(ht) $(hor) $(ver) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg7 chain 
#		rad1: radius of main circle
#		rad2: radius of the subordinate circle which goes around the main circle
#		nsegments: # of segments of the horizontal polygon ( The main circle )
#		nsections: # of sections in a subordinate circle
#		nparts: # of rings in the chain
eg7 chain:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) chain 100 $(ht) 20 20 $(ht2) 30 \
		$(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) 5 $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		0 $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

eg7x chainx:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) chain $(rad1) $(ht) $(hor) $(ver) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg8 table 
#		rad1: radius of the desktop
#		ht(thickness): thickness of the desktop	
#		nsegments: # of segments of the horizontal polygon of the desktop
#		ht2: the height of the legs
#		rad2: radius of the top/bottom face of legs
#		nsegments2: # of segments of the horizontal polygon of the legs
#		nparts2: # of legs
#		nposition2: position of the legs, horizontal distance 
#		between the center of the leg and the center of the desktop

eg8 table:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) table 200 10 100 $(nsections) 200 10 \
		$(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		8 $(nsections2) $(nparts) 4 180 \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		-45 $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

eg8x tablex:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) table $(rad1) $(ht) $(nsegments) $(nsections) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

#eg9 trefoil (40 10 80 12)
#	rad1 is the diameter of the knot
#	rad2 is the thickness of the knot
#		ASSUMPTION: rad1>rad2
#	nsegments: # of segments of around the main frame of trefoil
#	nsections: # of sections in the circle goes around the main frame of the trefoil
#	interp is the number of interpolation points
#		(more points gives the smoother curves)
eg9 trefoil:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) trefoil 80 $(ht) 80 12 $(ht2) 15 $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		0 $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

eg9x trefoilx:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) trefoil $(rad1) $(ht) $(nsegments) $(nsections) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

# eg10 chair ( A chair is a table with a back )
#		rad1: radius of the desktop
#		ht(thickness): thickness of the desktop	
#		nsegments: # of segments of the horizontal polygon of the desktop
#		ht2: the height of the legs
#		rad2: radius of the top/bottom face of legs
#		nsegments2: # of segments of the horizontal polygon of the legs
#		nparts2: # of legs
#		nposition2: position of the legs, horizontal distance 
#		between the center of the leg and the center of the desktop
#		nposition3: position of the back, horizontal distance
#		between the further corner of the back and the center of the desktop
#		thickness3: the thickness of the back
#		hight3: the height of the back		 

eg10 chair:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) chair 200 10 4 $(nsections) 200 10 $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		4 $(nsections2) $(nparts) 4 180 170 10 300 $(lightRate)\
		-45 $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

eg10x chairx:	gwb	
	./gwb $(interactive) $(inputDir) $(fileName) \
		$(windowWidth) $(windowHeight) $(windowPosX) $(windowPosY) \
		$(mode) chair $(rad) $(ht) $(nsegments) $(nsections) \
		$(ht2) $(rad2) $(rotUnitX) $(rotUnitY) $(rotUnitZ) \
		$(nsegments2) $(nsections2) $(nparts) $(nparts2) $(position2) \
		$(position3) $(thickness3) $(height3) $(lightRate)\
		$(initRotDeg) $(initRotAxisX) $(initRotAxisY) $(initRotAxisZ)

# 		Read the file: cube.gwb and draw a cube
eg11 filecube:	gwb
	./gwb 2 $(inputDir) cube.gwb

# 		Read the file: cube.gwb and draw an icosahedron
eg12 fileicosa:	gwb
	./gwb 2 $(inputDir) icosahedron.gwb

# 		Read the file: cube.gwb and draw a dodecahedron
eg13 filedodeca: 	gwb
	./gwb 2 $(inputDir) dodecahedron.gwb 90

# 		Read the file: cube.gwb and draw a tetrahedron
eg14 filetetra:	gwb
	./gwb 2 $(inputDir) tetrahedron.gwb

# 		Read the file: cube.gwb and draw a icosahedron
eg15 fileocta:	gwb
	./gwb 2 $(inputDir) octahedron.gwb

eg16 filehole:	gwb
	./gwb 2 $(inputDir) cube-with-hole.gwb
##################################################
		
moretest: $(TARGETS)
# nothing to do

$(TARGETS): $(OBJ_FILES)

#=================================================
# Temp
#=================================================
m:
	gvim -f Makefile
vi:
	gvim -f gwb.cpp

#=================================================
# Rules
#=================================================

.SUFFIXES: .cpp

%: %.o
	${CXX} $(OBJ_FILES) $(LDFLAGS) $(CORE_LIB) -o $@ $(CXXFLAGS)

.cpp.o:
	${CXX} $(CXXFLAGS) $(CORE_INC) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d 

Dependencies:
	touch Dependencies

include Dependencies

#=================================================
# Help
#=================================================
help:
	@echo "USAGE: Type the following"
	@echo "  > make eg                 -- to see an example : trefoil"
	@echo "  > make params             -- to see default parameters of eg"
	@echo "  > make egN                -- where N = 0, 1, 2, ..., 14, 15, 16"
	@echo "                               to see various examples "
	@echo " LIST of TARGETS:"
	@echo "		eg0 (facehemi)  -- a hemisphere"
	@echo "		eg1 (sphere)	"
	@echo "		eg2 (torus,donut)"
	@echo "		eg3 (cylinder)"
	@echo "		eg4 (disk)"
	@echo "		eg5 (bullet)"
	@echo "		eg6 (cone)"
	@echo "		eg7 (chain)	-- a model with several solids"
	@echo "		eg8 (table)"
	@echo "		eg9 (trefoil)	-- an interesting exmaple"
	@echo "		eg10 (chair)	-- a table with a back"
	@echo "		eg11 (cube) 	-- a cube, which is also a trivial calendar"
	@echo "		eg12 (fileicosa)-- icosahedron read from a file "
	@echo "		eg13 (filedodeca)- dodecahedron reaf from a file"
	@echo "		eg14 (filetetra)-- tetrahedron read from a file"	
	@echo "		eg15 (fileocta) -- octahedron read from a file"
	@echo "		eg16 (filehole) -- a cube wiht a hole read from a file"

#=================================================
# Misc
#=================================================

par params:
	@echo "================ Parameters =================="
	@echo "    interactive = " ${interactive}
	@echo "    inputDir = " ${inputDir}
	@echo "    fileName = " ${fileName}
	@echo "    windowWidth = " ${windowWidth}
	@echo "    windowHeight = " ${windowHeight}
	@echo "    windowPosX = " ${windowPosX}
	@echo "    windowPosY = " ${windowPosY}	
	@echo "    mode = " ${mode}
	@echo "    model = " ${model}
	@echo "    rad = " ${rad}
	@echo "    radius = " ${radius}
	@echo "    ht = " ${ht}
	@echo "    height = " ${height}
	@echo "    hor = " ${hor}
	@echo "    nsegments = " ${nsegments}
	@echo "    ver = " ${ver}
	@echo "    nsections = " ${nsections}	
	@echo "    height2 = " ${height2} 
	@echo "    rad2 = " ${rad2}
	@echo "    rotUnitX = " ${rotUnitX}
	@echo "    rotUnitY = " ${rotUnitY}
	@echo "    rotUnitZ = " ${rotUnitZ}
	@echo "    nsegments2 = " ${nsegments2}
	@echo "    nsections2 = " ${nsections2}
	@echo "    nparts = " ${nparts}
	@echo "    nparts2 = " ${nparts2}
	@echo "    position2 = " ${position2}
	@echo "    position3 = " ${position3}
	@echo "    thickness3 = " ${thickness3}
	@echo "    height3 = " ${height3}
	@echo "    lightRate = " ${lightRate}
	@echo "    initRotDeg = " ${initRotDeg}
	@echo "    initRotAxisX = " ${initRotAxisX}
	@echo "    initRotAxisY = " ${initRotAxisY}
	@echo "    initRotAxisZ = " ${initRotAxisZ}

	@echo "=============================================="

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o Dependencies $(TARGETS)

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

vclean veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS) Dependencies

#=================================================
# END Makefile 
#=================================================

