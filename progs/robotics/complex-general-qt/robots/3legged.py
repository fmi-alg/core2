## 3-legged robot
#
##  This file describe a robot.  
##  The file formats like following:
##  N, number of vertices, followed by N lines of coordinates.
##  M, number of triangles, followed by M lines of triangles,
##  defined by indices for its vertices
##
##  The triangles that are not nice will be decomposed. Their vertices must ordered CCW.
##
##   9:(0,2a+2b) ------------------------- 8:(3a, 2a+2b)
##               |                       |
##               |                       |
##   10:(0,a+2b) -----------------       |
##                  11:(2a,a+2b) |       |
##                               |       |
##    13:(0,a+b) ----------------- ----------------- 6:(5a, a+b)
##               |      12:(2a,a+b)     7:(3a,a+b)       |
##               |              (2a,b)                   |
##               |       --------X----------------       |
##               |       |2:(a,b)        3:(4a,b)|       |
##               |       |                       |       |
##               |       |                       |       |
##               |       |                       |       |
##               ---------                       ---------
##         0:(0,0).      1:(a,0)             4:(4a,0)     5:(5a,0)
#
#
# # parameter
# a = ?
# b = ?
#
# # CENTER of the robot is (2a,b), not (0,0)
#
#
# # N=number of vertices:
# 14
#
# # list of N vertices
# 0:  0 0
# 1:  a 0
# 2:  a b
# 3:  4a b
# 4:  4a 0
# 5:  5a 0
# 6:  5a a+b
# 7:  3a a+b
# 8:  3a 2a+2b
# 9:  0  2a+2b
# 10: 0  a+2b
# 11: 2a a+2b
# 12: 2a a+b
# 13: 0  a+b
#
# # M=number of triangles:
# 12
#
# # list of M triangles
# 0 1 2
# 0 2 13
# 2 12 13
# 2 3 12
# 3 7 12
# 3 6 7
# 3 4 5
# 3 5 6
# 12 7 8
# 11 12 8
# 9 10 11
# 8 9 11


import math
f = open('3legged.rob','w')
a = 2
b = 3

offset_x, offset_y = -2*a, -b


print >> f, 14  # N = 14
# Following are N vertices (one per line)
###########################
print >> f, offset_x + 0,   offset_y + 0
print >> f, offset_x + a,   offset_y + 0
print >> f, offset_x + a,   offset_y + b
print >> f, offset_x + 4*a, offset_y + b
print >> f, offset_x + 4*a, offset_y + 0
print >> f, offset_x + 5*a, offset_y + 0
print >> f, offset_x + 5*a, offset_y + a+b
print >> f, offset_x + 3*a, offset_y + a+b
print >> f, offset_x + 3*a, offset_y + 2*a+2*b
print >> f, offset_x + 0,   offset_y + 2*a+2*b
print >> f, offset_x + 0,   offset_y + a+2*b
print >> f, offset_x + 2*a, offset_y + a+2*b
print >> f, offset_x + 2*a, offset_y + a+b
print >> f, offset_x + 0,   offset_y + a+b
###########################

print >> f, 12  # M = 12
# Following are M triangles (one per line)
print >> f, 0,  1,  2
print >> f, 0,  2,  13
print >> f, 2,  12, 13
print >> f, 2,  3,  12
print >> f, 3,  7,  12
print >> f, 3,  6,  7
print >> f, 3,  4,  5
print >> f, 3,  5,  6
print >> f, 12, 7,  8
print >> f, 11, 12, 8
print >> f, 9,  10, 11
print >> f, 8,  9,  11
#########################################
# END of file
#########################################
