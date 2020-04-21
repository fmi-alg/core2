# Root CMake for robotics project

# CMake Version
cmake_minimum_required(VERSION 2.8.8)

# Project name
project(robotics)

# CORE:
set(CORE_PATH "../..")

# WHERE TO LOOK FOR Modules needed by CORE:
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../../cmake/Modules/")

message("\n\n my CMAKE_MODULE_PATH is " ${CMAKE_MODULE_PATH} "\n\n")

#There are lots of scripts with cmake
#for finding external libraries. 
#see /usr/local/share/cmake-2.6/Modules/Find*.cmake for more examples
find_package(gmp)		# found -- case insensitive: "gmp", "Gmp" or "GMP" OK
find_package(mpfr)		# found
find_package(GLEW REQUIRED)	# found
find_package(OpenGL REQUIRED)	# found
find_package(GLUT REQUIRED)	# found
find_package(GLUI REQUIRED)	# found

# COMPILE FLAGS
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CORE_CXXFLAGS "-Wfatal-errors")
 
# DIRECTORIES
#add_subdirectory(2-links)
add_subdirectory(triangle)
