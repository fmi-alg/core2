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

#pragma once

#include "Box.h"
#include "Corner.h"
#include "Edge.h"
#include "../../dataStructures/smooth_quadtree.h"

namespace vor2d {

class VorQuadTree : public SmoothQuadTree<void> {
 public:
  VorQuadTree(int dimension, double width) {
    root_ = new VorBox(0 /* depth */, 0 /* indicator */, center, this);
  }

  ~VorQuadTree() {
    delete root_;
  }
  
  void subdivide() {
    while (!queue_->empty()) {
      expand();
    }
  }
};

} // namespace vor2d
