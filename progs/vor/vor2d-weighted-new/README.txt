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

--------------------------------------------------------------------------------

DESCRIPTION:

SubVor is a prototype of a program for computing the Voronoi diagram of a set of polygonal inputs, each equipped with a (possibly different) anisotropic norm. SubVor implements the algorithms described in the companion paper, "Planar Minimization Diagrams via Subdivision with Applications to Anisotropic Voronoi Diagrams" by Huck Bennett, Evanthia Papadopoulou, and Chee Yap, to appear at the Eurographics Symposium on Geometry Processing (SGP) 2016.

Huck Bennett wrote SubVor, in part by using infrastructure by and extending a previous program written by Jyh-Ming Lien. Both programs are (will be) distributed as part of the Core Library (http://cs.nyu.edu/exact/core_pages/intro.html).

--------------------------------------------------------------------------------

REQUIREMENTS:

- Ubuntu 16.04 (earlier versions of Ubuntu and other Linux environments will likely also work).
- g++-5                  (required; compiler).
- Boost                  (required; C++ library, http://www.boost.org/).
- OpenGL, GLUT, and GLUI (required; for visualization).
- PNG library            (required; for outputting figures).
- Python and NumPy       (optional; for generating examples).

All necessary packages can be obtained through Ubuntu's package system via apt-get:
sudo apt-get install g++-5
sudo apt-get install libboost-dev
sudo apt-get install libglui2c2     # May not be available in some Ubuntu versions.
sudo apt-get install freeglut3-dev
sudo apt-get install libpng12-dev
sudo apt-get install python-numpy

--------------------------------------------------------------------------------

USAGE:

Run format:

./subvor <options> <file name>

Options:

--help                 Print help message.
  --geps arg (=1)        Geometric epsilon.
  --aeps arg (=0.015625) Absolute epsilon.
  --save arg (=0)        Save an image of the construction.
  --display arg (=1)     Display the consturcted Voronoi diagram.
  --grid arg (=1)        Display the quadtree grid.

Ex.
./subvor --geps=0.01 --save=true shapes-1-1-1

Interaction:

- Clicking anywhere toggles the underlying quadtree grid.
- Scrolling forward zooms in on the point under the cursor. Zooming backwards zooms out.

Input:

The input file specifies n input points in the plane, (x_0, y_0), ..., (x_{n-1}, y_{n-1}) where x_i, y_i are integers in [0, 1024). Each of k input sites is a simple polygon specified as a sequence of input points in clockwise order. This includes the case where the input sites be points or line segments, i.e. degenerate polygons.

Sites may each be equipped with a separate multiplicative weight or anisotropic norm. A multiplicative weight is specified as "w <c>", where c is the multiplicative weight points. The distance from a point u in the plane to a site V equipped with the multiplicative weight is then min_{v \in V} c * d(u, v), where d(. , .) denotes Euclidean distance. An anisotropic norm is specified as "m <a> <b> <c>", where the metric matrix M = [[a, b], [b, c]]. The distance from a point u in the plane to a site V equipped with the anisotropic norm is then min_{v \in V} sqrt((u - v)^T M (u - v)).

Input file format:

<Comment lines beginning with '#'>
<total number n of vertices used in input polygons>

x_0 y_0
x_1 y_1
...
x_{n-1} y_{n-1}

<total number k of input sites>
<optional multiplicative weight/anisotropic norm information>
<sequence 1 of input point indices given in clockwise order>
...
<optional multiplicative weight/anisotropic norm information>
<sequence k of input point indices given in clockwise order>

Input file example (3 unweighted points):

3
300 300
900 300
600 900

3
0
1
2

See the input file "shapes-4-1-1" for a more elaborate example.

Output:

By default, the display will show:
- Obstacles (shown in black).
- The computed Voronoi diagram (shown in red).
- The subdivision grid (shown in gray).
- Boxes split down to radius absolute epsilon (aeps) in which we cannot decide the topology (filled with blue).

Setting aeps smaller will result in fewer blue boxes (in theory, no blue boxes when aeps is sufficiently small and the input is in general position). Setting geps smaller will result in a smoother Voronoi bisector (with Hausdorff distance arbitrarily close to the actual underlying Voronoi bisector for small enough geps).

--------------------------------------------------------------------------------

BENCHMARKS:

There are a number of examples in the "benchmarks" folder which may be tested using the Makefile. 
- "make paper_figures" runs SubVor on 8 tests corresponding to those in the paper.
- "make shapes" runs SubVor on several benchmarks with polygonal input. The sites are the same in each example, but are equipped with different norms in the different benchmarks.
- "make random_examples" runs various random tests.
- "make conics"/"make conics_highres" produce diagrams for an ellipse, parabola, and hyperbola. The second command computes the diagrams to high geometric precision.
- "make rand_w_points_10/rand_w_points_10_highres" produces a random input of 10 points, each with a randomly generated multiplicative weight. The second command computes the diagrams to high geometric precision.
- "make rand_w_segs_10/rand_w_segs_10_highres" produces a random input of 10 line segments, each with a randomly generated multiplicative weight. The second command computes the diagrams to high geometric precision.

There are four Python scripts for generating random inputs with various properties:
- gen_points.py - Generates random (anisotropically weighted) points.
- gen_circ.py   - Generates "roots of unity" points.
- gen_segs.py   - Generates random (anisotropically weighted) non-intersecting line segments where no three segment endpoints non-collinear.
- gen_mixed.py  - Generates random points and random line segments.
See the individual files for documentation.

--------------------------------------------------------------------------------

NOTES:

This program is a prototype. There are several major and minor issues with the program that should be noted. The most important ones are:

Program issues:
1. Use of machine precision (double precision) in computations.
2. Rootbox construction. Currently the program follows the subdivision phase described in the paper fairly closely, but we are shortcutting the detailed rootbox construction.
3. Use of non-naive (e.g. centered-form) polynomial evaluation.

Clean-up issues:
1. Fix rampant memory leaking.
2. Use standard (e.g. Boost) interval and bivariate polynomial classes.
3. General clean-up, renaming, reorganization, and documentation.
