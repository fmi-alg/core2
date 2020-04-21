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

gen_segs.py generates random non-intersecting line segments in the plane such that no three segment endpoints are collinear.

Usage:
gen_segs.py <number of line segments> <options>

Options:
-w          -- Use weights.
-m          -- Use anisotropic weights.
-l <weight> -- Sets the maximum weight strength. This affects both weighted and anisotropically weighted diagrams.
-d <length> -- Sets an upper bound on the length of line segments.

Example usage:
python gen_segs.py 10 -m -l 10 -d 300
'''

import numpy as np
from sys import argv
from math import sqrt
from gen_infra import *

weight = M_WEIGHT
use_weights = False
use_matrix  = False
segs = []
distance_ub = float("inf")

def intersects_any(r, s):
    for seg in segs:
        if intersects_or_collinear(seg[0], seg[1], r, s):
            return True
    return False

def output_file(n):
    global segs
    
    # Generate random points.
    w_str = "w_" if use_weights else ""
    m_str = "m_" if use_matrix else ""
    d_str = "d_" + str(distance_ub) + "_" if distance_ub < float("inf") else ""
    l_str = "l_" + str(weight) + "_" if (use_weights or use_matrix) else ""

    f = open("test_segs_" + w_str + m_str + d_str + l_str + str(n), 'w')
    f.write(str(2 * n) + "\n")
    while len(segs) < n:
        x1, y1 = rcoor(), rcoor()
        x2, y2 = rcoor(), rcoor()
        if not intersects_any((x1, y1), (x2, y2)) and \
           dist((x1, y1), (x2, y2)) <= distance_ub:
            f.write(str(x1) + " " + str(y1) + "\n")
            f.write(str(x2) + " " + str(y2) + "\n")
            segs += [((x1, y1), (x2, y2))]
    
    # Write segments.
    f.write("\n" + str(n) + "\n")
    for i in range(0, 2 * n, 2):
        if use_weights:
            f.write("w " + str(iur(weight)) + "\n")
        elif use_matrix:
            m = raniso(weight)
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + " " + str(i + 1) + "\n")

    f.close()

def parse_args(argv):
    global use_weights, use_matrix, distance_ub, weight
    i = 2                # Omitting 'python' and file name.
    while i < len(argv):
        arg = argv[i]
        if arg == '-w':
            use_weights = True
        if arg == '-m':  # TODO
            use_weights = False
            use_matrix  = True
        if arg == '-d':
            distance_ub = int(argv[i + 1])
            i += 1
        if arg == '-l':
            weight = int(argv[i + 1])
            i += 1
        i += 1

parse_args(argv)
output_file(int(argv[1]))
