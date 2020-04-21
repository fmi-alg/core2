## S-shaped robot: S.rob
#
#
##  This file describe a robot.  
##  The file formats like following:
##  N, number of vertices, followed by N lines of coordinates.
##  M, number of triangles, followed by M lines of triangles,
##  defined by indices for its vertices
##
##  The triangles that are not nice will be decomposed. Their vertices must ordered CCW.
##
#                             ----------------------------- 
#                            |                             |
#                            |                             |
#                            |                             |
#                            |       ---------------------- 
#                            |      |               :      
#                            |      |               :      
#                            |<-a ->|<-     b     ->:       
#                            |      |               :       
#                            |       ----------------------
#                            |                      :      |
#                            |                      :      |
#                            |                      :      |
#                             ----------------------       |
#                                       ^           |      |
#                                       c           |      |
#                                                   |      |
#                                       v           |      |
#                             ----------------------       |
#                            |                             |
#                            |                             |
#                            |                             |
#                             -----------------------------
#
# # parameter
# a = ? thickness of stroke
# b = ? controls width
# c = ? controls height
#
# # CENTER of the robot is (a+b/2, 3a/2+c), not (0,0)
#

import math
f = open('S.rob','w')
a = 2
b = 4
c = 3

offset_x, offset_y = -(a+b/2), -(1.5*a+c)


print >> f, 12  # N = 12
# Following are N vertices (one per line)
# 12
###########################
print >> f, offset_x + 0,       offset_y + 0
print >> f, offset_x + 2*a+b,   offset_y + 0
print >> f, offset_x + 2*a+b,   offset_y + 2*a+c
print >> f, offset_x + a,       offset_y + 2*a+c
print >> f, offset_x + a,       offset_y + 2*a+2*c
print >> f, offset_x + 2*a+b,   offset_y + 2*a+2*c
print >> f, offset_x + 2*a+b,   offset_y + 3*a+2*c
print >> f, offset_x + 0,       offset_y + 3*a+2*c
print >> f, offset_x + 0,       offset_y + a+c
print >> f, offset_x + a+b,     offset_y + a+c
print >> f, offset_x + a+b,     offset_y + a
print >> f, offset_x + 0,       offset_y + a

###########################

print >> f, 10  # M = 10
# Following are M triangles (one per line)
print >> f, 10, 11, 0
print >> f, 0,  1,  10
print >> f, 10, 1,  2
print >> f, 9,  10, 2
print >> f, 3,  9,  2
print >> f, 3,  8,  9
print >> f, 3,  7,  8
print >> f, 3,  4,  7
print >> f, 7,  4,  5
print >> f, 7,  5,  6
#########################################
# END of file
#########################################
