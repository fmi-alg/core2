## C-shaped robot: C.rob
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
##           15:(a, 5a)  ----------------------  14:(4a, 5a)
##                     /                        \
##                   /   9:(2a, 4a)  10:(3a, 4a)  \
##                 /          ------------          \
##               /          /              \          \  13:(5a, 4a)
##     16:(0,4a)|         /                  \         | 
##              |       |8:(a, 3a)  11:(4a, 3a)--------| 12:(5a, 3a)
##              |       |                               
##              |       +                               
##              |       |                               
##              |       |7:(a, 2a)   4:(4a, 2a)--------| 3:(5a, 2a)
##              |         \                  /         |
##      17:(0,a) \          \              /          /  2:(5a, a)
##                 \          ------------          /
##                   \   6:(2a, a)   5:(3a, a)    /
##                     \                        /
##                       ----------------------
##                    0:(a,0)                1:(4a,0)
#
##
#
#
# # parameter
# a = ?
# b = ?
#
# # CENTER of the robot is (a,2.5a), not (0,0)
#
#
# # N=number of vertices:
# 18
#
# # list of N vertices
# 0:  a, 0
# 1:  4a, 0
# 2:  5a, a
# 3:  5a, 2a
# 4:  4a, 2a
# 5:  3a, a
# 6:  2a, a
# 7:  a, 2a
# 8:  a, 3a
# 9:  2a, 4a
# 10: 3a, 4a
# 11: 4a, 3a
# 12: 5a, 3a
# 13: 5a, 4a
# 14: 4a, 5a
# 15: a, 5a
# 16: 0, 4a
# 17: 0, a
#
# # M=number of triangles:
# 16
#
# # list of M triangles
# 0 5 6
# 0 1 5
# 1 2 5
# 5 2 4
# 4 2 3
# 11 12 13
# 11 13 14
# 10 11 14
# 9 10 14
# 9 14 15
# 8 9 15
# 8 15 16
# 7 8 16
# 7 16 17
# 0 7 17
# 0 6 7

import math
f = open('C.rob','w')
a = 2

offset_x, offset_y = -a, -2.5*a


print >> f, 18  # N = 14
# Following are N vertices (one per line)
# 18
###########################
print >> f, offset_x + a,   offset_y + 0
print >> f, offset_x + 4*a, offset_y + 0
print >> f, offset_x + 5*a, offset_y + a
print >> f, offset_x + 5*a, offset_y + 2*a
print >> f, offset_x + 4*a, offset_y + 2*a
print >> f, offset_x + 3*a, offset_y + a
print >> f, offset_x + 2*a, offset_y + a
print >> f, offset_x + a,   offset_y + 2*a
print >> f, offset_x + a,   offset_y + 3*a
print >> f, offset_x + 2*a, offset_y + 4*a
print >> f, offset_x + 3*a, offset_y + 4*a
print >> f, offset_x + 4*a, offset_y + 3*a
print >> f, offset_x + 5*a, offset_y + 3*a
print >> f, offset_x + 5*a, offset_y + 4*a
print >> f, offset_x + 4*a, offset_y + 5*a
print >> f, offset_x + a,   offset_y + 5*a
print >> f, offset_x + 0,   offset_y + 4*a
print >> f, offset_x + 0,   offset_y + a
###########################

print >> f, 16  # M = 12
# Following are M triangles (one per line)
print >> f, 0,  5,  6
print >> f, 0,  1,  5
print >> f, 1,  2,  5
print >> f, 5,  2,  4
print >> f, 4,  2,  3
print >> f, 11, 12, 13
print >> f, 11, 13, 14
print >> f, 10, 11, 14
print >> f, 9,  10, 14
print >> f, 9,  14, 15
print >> f, 8,  9,  15
print >> f, 8,  15, 16
print >> f, 7,  8,  16
print >> f, 7,  16, 17
print >> f, 0,  7,  17
print >> f, 0,  6,  7
#########################################
# END of file
#########################################
