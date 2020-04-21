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

gen_points.py generates random points in the plane.

Usage:
gen_points.py <number of points> <options>

Options:
-w          -- Use weights.
-m          -- Use anisotropic weights.
-l <weight> -- Sets the maximum weight strength. This affects both weighted and anisotropically weighted diagrams.

Example usage:
python gen_points.py 10 -w -l 10
'''

import numpy as np
from sys import argv
from gen_infra import *

weight = M_WEIGHT
use_weights = False
use_matrix  = False
pts = []

def output_file(n):
    global pts
    sn = str(n)

    # Generate random points.
    w_str = "w_" if use_weights else ""
    m_str = "m_" if use_matrix else ""
    l_str = "l_" + str(weight) + "_" if (use_weights or use_matrix) else ""

    f = open("test_" + w_str + m_str + l_str + str(n), 'w')
    f.write(sn + "\n")
    for i in range(n):
        pt = (rcoor(), rcoor())
        if not pt in pts:
            pts += [pt]
            f.write(str(pt[0]) + " " + str(pt[1]) + "\n")

    # Write points.
    f.write("\n" + sn + "\n")
    for i in range(n):
        if use_weights:
            f.write("w " + str(iur(weight)) + "\n")
        elif use_matrix:
            m = raniso(weight)
            a, b, c = m[0][0], m[0][1], m[1][1]
            f.write("m " + str(a) + " " + str(b) + " " + str(c) + "\n")
        f.write(str(i) + "\n")

    f.close()

def parse_args(argv):
    global use_weights, use_matrix, weight
    i = 2                # Omitting 'python' and file name.
    while i < len(argv):
        arg = argv[i]
        if arg == '-w':
            use_weights = True
        if arg == '-m':  # TODO
            use_weights = False
            use_matrix  = True
        if arg == '-l':
            weight = int(argv[i + 1])
            i += 1
        i += 1
            
parse_args(argv)
output_file(int(argv[1]))
