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

gen_circ.py generates equally spaced points on the unit circle (i.e. the nth roots of unity).

Usage:
gen_circ.py <number of points>

Example usage:
gen_circ.py 10
'''

import random
import math
from math import pi
from sys import argv

WIDTH = 1024
SCALING = 384

def output_file(n):
    sn = str(n)

    # Generate random points.
    f = open("test_circ_"  + sn, 'w')
    f.write(sn + "\n")
    for i in range(n):
        f.write(str(SCALING * math.cos(2 * pi * i/n) + WIDTH / 2) 
                + " " + str(SCALING * math.sin(2 * pi * i/n) + WIDTH / 2) + "\n")

    # Write points.
    f.write("\n" + sn + "\n")
    for i in range(n):
        f.write(str(i) + "\n")

    f.close()

output_file(int(argv[1]))
