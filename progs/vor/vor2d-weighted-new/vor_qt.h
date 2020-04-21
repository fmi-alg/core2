/*
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
*/

/**
 *  An adaptation of the smooth quadtree data structure for 2 dimensional Voronoi diagrams.
 *   
 *  Based on:
 *  Bennett, Huck and Chee Yap. "Amortized Analysis of Smooth Quadtrees in All Dimensions."
 *  Scandinavian Symposium and Workshops on Algorithm Theory (SWAT) 2014.
 *
 *  The ith bit of the child index indicates whether the box is above or below
 *  the center of the box in the ith dimension.
 *  
 *  For example, in 2 dimensions the children are indexed as follows:
 *   ______ ______
 *  |      |      |
 *  |  10  |  11  |
 *  |______|______|
 *  |      |      |
 *  |  00  |  01  |
 *  |______|______|
 * 
 *  Written by Huck Bennett, 9/21/2014
 *  TODO(Huck):
 *  - Cleanup direction/child indicator bit logic.
 *  - Replace raw pointers with smart pointers.
 *  - Simplify and document the public API.
 *  - Use only one of #pragma/header guard?
 */

#pragma once

#include "vor_box.h"

#include <memory>
#include <vector>

namespace vor2d {

using std::shared_ptr;
using std::vector;

class vor_qt {
 public:
  vor_qt(int dimension, double width);
  ~vor_qt();

  vor_box* root();
  vor_box* get_box(const vector<double>& point);
  const int dimension() const;
  int splits();
  int smooth_splits();
  double width();

 protected:
  void inc_splits();
  void inc_smooth_splits();

  const int dimension_;
  vor_box* root_;

  // Box specification.
  double width_;

  // Statistics
  int num_splits_ = 0;
  int num_smooth_splits_ = 0;

  friend class vor_box;
};

} // namespace vor2d
