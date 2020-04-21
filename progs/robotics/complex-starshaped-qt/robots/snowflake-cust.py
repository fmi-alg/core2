#  snowflake-cust.py
#
#########################################
# snowflake-cust has n sticks, equally spaced around a center.
#
#
#
#########################################

import math
PI = math.acos(-1)
f = open('snowflake-cust.rob','w')
a = 1.5   # inner radius
b = 10  # outter radius
n = 6 # number of sticks


def rotate(px, py, angle):
  # Rotate a point counterclockwise by a given angle (0,0).
  qx =  math.cos(angle) * px - math.sin(angle) * py
  qy =  math.sin(angle) * px + math.cos(angle) * py
  return qx, qy

def write(p):
  x, y = p
  print >> f, x, y

print >> f, n * 3
for i in xrange(n):
  x0, y0  = a * math.cos(PI/n), a * math.sin(PI/n)
  x1 = a + b
  write(rotate(x0, -y0, i * 2 * PI / n ))
  write(rotate(x1, -y0, i * 2 * PI / n ))
  write(rotate(x1, y0, i * 2 * PI / n ))


###########################

#########################################
# END of file
#########################################
