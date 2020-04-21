'''
SubVor, a program for computing Voronoi diagrams via subdivision.
Copyright (C) 2016 Huck Bennett, Evanthia Papadopoulou, and Chee Yap.
For comments or questions, please contact Huck Bennett at hbennett@cs.nyu.edu.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

gen_infra.py provides infrastructure for the other test generation files, and is not meant to be used on its own.

Reference: Section 1.5 in "Computational Geometry in C" by Joesph O'Rourke.
'''

import random
import numpy as np
from math import pi
from math import sin
from math import cos
from math import sqrt

M_WEIGHT = 6
WIDTH = 1024

def iur(bnd):
    u = 0.0
    while u == 0.0:
        u = random.uniform(1.0, bnd)
    return u if random.randint(0, 1) == 0 else 1.0 / u

def rcoor():
    return random.randint(0, WIDTH - 1)

def raniso(weight):
    # Generate a random diagonal matrix.
    m = np.array([[iur(weight), 0.0], [0.0, iur(weight)]])
    
    # Generate a random rotation matrix.
    theta = random.uniform(0, 2 * pi)
    o = np.array([[cos(theta), -sin(theta)], [sin(theta), cos(theta)]])
    
    return np.dot(np.dot(o, m), np.transpose(o))

# All coordinates are integral so this is exact.
def area(p, q, r):
    return np.linalg.det(np.array([[p[0], q[0], r[0]],
                                   [p[1], q[1], r[1]],
                                   [1, 1, 1]]))

def collinear(p, q, r):
    return area(p, q, r) == 0

def left(p, q, r):
    return area(p, q, r) > 0

def dist(p, q):
    return sqrt((p[0] - q[0]) ** 2 + (p[1] - q[1]) ** 2)

def intersects_or_collinear(p, q, r, s):
    if (collinear(r, s, p) or \
        collinear(r, s, q) or \
        collinear(p, q, r) or \
        collinear(p, q, s)):
        return True
    else:
        return (left(r, s, p) ^ left(r, s, q)) and \
            (left(p, q, r) ^ left(p, q, s))



