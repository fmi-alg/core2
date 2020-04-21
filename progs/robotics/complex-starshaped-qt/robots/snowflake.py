#  snowflake.py
#
#########################################
#	The snowflake robot is decomposed into a central
#		regular hexagon with side length 2,
#	with 6 rectangles of dimensions 2xL  (the L's may be different
#		for some rectangles). 
#
#	Assume L>2 so that the ``short sides'' of the rectangles
#	have length 2.  The ``inner'' short side of each rectangle 
#	abuts a side of the hexagon.  This suggests that the snowflake
#	has 6 + (2x6) = 18 vertices.   However, we will need to 
#	to introduce a vertex
#	in the middle of the ``outer'' short side of each rectangle because
#	of the requirement that T_i is nice.
#	Thus, the snowflake is actually has 24 vertices.
#		
#	We design the snowflake so that only one irrational number,
#			a = sqrt(3)=1.732
#	is needed to describe the coordinates:
#
#
#########################################
#  GENERAL FORMAT FOR STAR SHAPE ROBOTS
#########################################
#  	This file uses a special format to describe star-shaped robots.
#  	There is a python script to convert this snowflake.py file to a file
#           "snowflake.rob"  which can be read by our C++ parser.
#
#	The general file format for a star-shaped is as follows:
#
#	The first number is
#
#		N = number of vertices of the star-shaped robot
#
#	followed by the list of the vertices (one vertex per line),
#
#		v_1, v_2, ..., v_N
#	
#	We assume 
#		(1) v_0 = (0,0) is NOT a vertex of the robot
#		(2) the vertices are given in CCW order
#		(3) each of the triangles    T_i = (v_0, v_i, v_{i+1})
#				for i=1,2,...,N  is nice.
#			where T_N = (v_0, v_N, v_1)
#		
#	The snowflake robot is decomposed into a central
#		regular hexagon with side length 2,
#	with 6 rectangles of dimensions 2xL  (the L's may be different
#		for some rectangles). 
#
#	Assume L>2 so that the ``short sides'' of the rectangles
#	have length 2.  The ``inner'' short side of each rectangle 
#	abuts a side of the hexagon.  This suggests that the snowflake
#	has 6 + (2x6) = 18 vertices.   However, we will need to 
#	to introduce a vertex
#	in the middle of the ``outer'' short side of each rectangle because
#	of the requirement that T_i is nice.
#	Thus, the snowflake is actually has 24 vertices.
#		
#	We design the snowflake so that only one irrational number,
#			a = sqrt(3)=1.732
#	is needed to describe the coordinates:
#########################################

import math
f = open('snowflake.rob','w')
a = math.sqrt(3)


print >> f, 24  # N = 24
# Following are N vertices (one per line)
###########################
print >> f, 10, -1
print >> f, 10, 0
print >> f, 10, 1
###########################
print >> f, a, 1
###########################
print >> f, 2+a, 1+2*a
print >> f, 2+a/2, 3.0/2+2*a
print >> f, 2, 2+2*a
###########################
print >> f, 0, 2
###########################
print >> f, -2, 2+2*a
print >> f, -2-a/2, 3.0/2+2*a
print >> f, -2-a, 1+2*a
###########################
print >> f, -a, 1
###########################
print >> f, -10, 1
print >> f, -10, 0
print >> f, -10, -1
###########################
print >> f, -a, -1
###########################
print >> f, -2-a, -1-2*a
print >> f, -2-a/2, -3.0/2-2*a
print >> f, -2, -2-2*a
###########################
print >> f, 0, -2
###########################
print >> f, 2, -2-2*a
print >> f, 2+a/2, -3.0/2-2*a
print >> f, 2+a, -1-2*a
###########################
print >> f, a, -1
###########################

#########################################
# END of file
#########################################
