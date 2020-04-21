/* **************************************
   File: smooth_quadtree.h

   Description: 
   	Implementation of Smooth Quadtree data structure

   Author:  Huck Bennett and Chee Yap 
   Date:    June, 2016

   Since Core Library  Version 2.1
   $Id:$
 ************************************** */

/**
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
*/

#ifndef SMOOTH_QUADTREE_H
#define SMOOTH_QUADTREE_H

#include "smooth_quadtree_box.h"

using std::vector;

template <typename T>
class SmoothQuadtree {
 public:
  SmoothQuadtree<T>(int dimension, double width) : dimension_(dimension), width_(width) {
    assert(1 <= dimension && dimension <= 32);

    double* center = new double[dimension];
    for (int i = 0; i < dimension; i++) {
      center[i] = 0.0;  // should generalize (Chee)
    }

    root_ = new SmoothQuadtreeBox<T>(0 /* depth */, 0 /* indicator */, center, this);
    SmoothQuadtreeBox<T>** neighbors = root_->neighbors();
    for (int i = 0; i < 2 * dimension; i++) {
      neighbors[i] = nullptr;
    }
  }

  ~SmoothQuadtree<T>() {
    delete root_;
  }

  /**
   * Returns the leaf box containing a point (x_1, x_2, ... x_n),
   * or nullptr if the point lies outside of the subdivision.
   *
   * Note that the values x_i appear in LITTLE ENDIAN order, as opposed
   * to the child indicators which appear in BIG ENDIAN order.
   */
  SmoothQuadtreeBox<T>* get_box(const vector<double>& point) {
    if (point.size() != dimension_) {
      return nullptr;
    }

    // Verify that the query point is within the initial bounding box.
    for (int i = 0; i < dimension_; i++) {
      if (point[i] < -width_ || point[i] > width_) {
	return nullptr;
      }
    }

    // Recurse through the tree.
    SmoothQuadtreeBox<T>* cur_box = root_;
    while (!cur_box->is_leaf()) {
      int ind = 0;
      for (int i = 0; i < dimension_; i++) {
    	if (point[i] > cur_box->center()[i]) {
	  ind |= (1 << i);
    	}	
      }
      cur_box = cur_box->children()[ind];
    }

    return cur_box;
  }

  SmoothQuadtreeBox<T>* root() {
    return root_;
  }

  const int dimension() const {
    return dimension_;
  }

  int splits() const {
    return num_splits_;
  }

  int smooth_splits() const {
    return num_smooth_splits_;
  }

  double width() const {
    return width_;
  }

 protected:
  void inc_splits() {
    num_splits_++;
  }

  void inc_smooth_splits() {
    num_smooth_splits_++;
  }

  const int dimension_;
  SmoothQuadtreeBox<T>* root_;

  // Box specification.
  double width_;

  // Statistics
  int num_splits_ = 0;
  int num_smooth_splits_ = 0;

  friend class SmoothQuadtreeBox<T>;
};

#endif // SMOOTH_QUADTREE_H
