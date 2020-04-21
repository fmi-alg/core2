# CMakeLists.txt in 2-links
#
#	USAGE:
#	This file is in $(COREPATH)/progs/robotics/2-links. 
#	To use this file, assume you are currently
#	in the directory $(COREPATH)/progs/robotics/2-links.  Do this:
#
#		> cd build				-- go to build dir
#		> cmake ..				-- create Makefile
#		> make					-- create executables
#		> ln -s 2-links/2-links ../2-links	-- install executable
#
#	This will deposit all the executable files under 2-links/build.
#
# Aug 31, 2013
# Author: Chee Yap
#
# Since Core Library 2.1
# $Id:$
#
# CMake Version
cmake_minimum_required(VERSION 2.8.8)
#
# Project name
project(2-links)

include_directories(${ROBOTICS_SOURCE_DIR}/2-links)
link_directories(${ROBOTICS_BINARY_DIR}/2-links)


# list the object files
set(OBJ_FILES 
	Box.cpp QuadTree.cpp Corner.cpp Wall.cpp fileProcessor.cpp Timer.cpp Polygon.cpp)

# create the object file library
add_library(2-links_objects OBJECT ${OBJ_FILES}) 	

# list of libraries
set(ALL_LIBS ${GMP_LIBRARY}
	${MPFR_LIBRARY} ${GLUT_LIBRARY} ${GLUI_LIBRARY} ${OPENGL_LIBRARY} m)

#these are all compiled the same way (keep for example only!)
	#set(PROGRAMS oglfirst pointers)
	
	foreach(program ${PROGRAMS})
	  add_executable(${program} ${program}.cpp)
	  target_link_libraries(${program} ${ALL_LIBS})
	endforeach(program)

#building just a library. 
# add_library(geometry.cpp)

# Add executable
add_executable(2-links 2-links.cpp ${OBJ_FILES})
# add_executable(2-links 2-links.cpp $TARGET_OBJECTS:2-links_object)
#
#linking against a custom library
target_link_libraries(2-links ${ALL_LIBS})


