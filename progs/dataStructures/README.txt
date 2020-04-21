SmoothQuadtree, an implementation of the smooth quadtree data structure.
Copyright (C) 2016 Huck Bennett and Chee Yap.
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

-------------------------------------------------------------------------------
Description:
-------------------------------------------------------------------------------

A templated, d-dimensional implementation of the smooth quadtree data structure.
A quadtree is called "smooth" if every pair of adjacent boxes differ in
depth by at most 1. The smooth quadtree data structure maintains smoothness
as an invariant.

This program is based on the paper "Amortized Analysis of Smooth Quadtrees in
All Dimensions" by Huck Bennett and Chee Yap, Scandinavian Symposium and
Workshops on Algorithm Theory (SWAT) 2014, and Computational Geometry: Theory
and Applications (to appear). This data structure also appears in the Core
Library: http://cs.nyu.edu/exact/core_pages/intro.html. For an example of this
data structure used in an application, see https://github.com/hbennett/SubVor.

----------------------
Description of files:
----------------------

smooth_quadtree.h - The base smooth quadtree data structure.
smooth_quadtree_box.h - A box in the smooth quadtree data structure.
smooth_quadtree_test.cpp - A test class for the smooth quadtree data structure.

----------------------
Indexing:
----------------------

The ith bit (in BIG ENDIAN order) of the child index indicates whether the box is above or below the center of the box in the ith dimension.
 
For example, in 2 dimensions the children are indexed as follows:
   ______ ______
  |      |      |
  |  10  |  11  |
  |______|______|
  |      |      |
  |  00  |  01  |
  |______|______|

-------------------------------------------------------------------------------
Basic API:
-------------------------------------------------------------------------------

The interesting functions work as described below. There are also a number of getter and setter functions.

----------------------
smooth_quadtree.h:
----------------------

- SmoothQuadtree<T>(int dimension, double width)

The smooth quadtree constructor takes a dimension 'dim' (with 1 <= dim <= 32), and a width 'width' for the subdivision bounding box. The output quadtree then corresponds to a subdivision of a 'dim'-dimensional, 0-centered box of width 'width'.

- SmoothQuadtreeBox<T>* get_box(const vector<double>& point)

get_box takes a point (x_1, x_2, ..., x_n) of dimension dim as input, and outputs the subdivision box containing the point. It outputs nullptr if the point lies outside the subdivision bounding box.

Note that the values x_i appear in LITTLE ENDIAN order, as opposed to the child indicators which appear in BIG ENDIAN order.

----------------------
smooth_quadtree_box.h:
----------------------

- void smooth_split()

Splits the box, and then smooths the quadtree.

- SmoothQuadtreeBox<T>* principal_neighbor_dir(int dir) const

Returns the unique smallest neighbor in direction dir of equal or greater size, or NULL if no such neighbor exists.

- shared_ptr<vector<SmoothQuadtreeBox<T>*>> leaf_neighbors_dir(int dir) const

Returns all of the leaf neighbors in direction dir.

-------------------------------------------------------------------------------
TODO/Bugs:
-------------------------------------------------------------------------------

TODO:
- Clean up direction/child indicator bit logic.
- Use smart pointers throughout.
- Add more extensive tests.
- Use a standard testing framework.

Bugs:
None known. Please report any bugs to hbennett@cs.nyu.edu.
