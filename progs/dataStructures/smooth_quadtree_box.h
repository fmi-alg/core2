/* **************************************
   File: smooth_quadtree_box.h

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

#ifndef SMOOTH_QUADTREE_BOX_H
#define SMOOTH_QUADTREE_BOX_H

#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <math.h>

using std::shared_ptr;
using std::vector;

template <typename T>
class SmoothQuadtree;

template <typename T>
class SmoothQuadtreeBox {
 public:
  SmoothQuadtreeBox<T>(int depth, int indicator, double center[], SmoothQuadtree<T>* tree)
  : depth_(depth), indicator_(indicator), center_(center), tree_(tree) {
    neighbors_ = new SmoothQuadtreeBox<T>*[2 * dimension()];
    children_ = nullptr;
    data_ = nullptr;
  }

  ~SmoothQuadtreeBox<T>() {
    // If the box is split then recurse to children.
    if (!is_leaf()) {
      for (int i = 0; i < (1 << dimension()); i++) {
	delete children_[i];
      }
      delete[] children_;
    }

    delete[] neighbors_;
    delete[] center_;
  }

  /**
   * Splits this box, and then smooths the quadtree.
   */ 
  void smooth_split() {
    if (!is_leaf()) {
      return;
    }
    smooth_split_aux();
    tree_->inc_smooth_splits();
  }

  /** 
   * Returns the unique smallest neighbor in direction dir of equal or greater
   * size, or NULL if no such neighbor exists.
   */
  SmoothQuadtreeBox<T>* principal_neighbor_dir(int dir) const {
    int axis = fabs(dir);
    int sign = (dir < 0) ? 0 : 1;
    return neighbors_[(axis - 1) + sign * dimension()];
  }

  /**
   * Returns all of the leaf neighbors in direction dir.
   */
  shared_ptr<vector<SmoothQuadtreeBox<T>*>> leaf_neighbors_dir(int dir) const {
    shared_ptr<vector<SmoothQuadtreeBox<T>*>> neighbors(new vector<SmoothQuadtreeBox<T>*>());
    SmoothQuadtreeBox<T>* cur_neighbor = principal_neighbor_dir(dir);

    if (cur_neighbor == nullptr) {
      return neighbors;
    } else if (cur_neighbor->is_leaf()) {
      neighbors->push_back(cur_neighbor);
      return neighbors;
    } else { // cur_neighbor is non-null and split.
      return cur_neighbor->enumerate_halfspace_leaf_descendants(-dir);
    }
  }

  void set_data(T* data) {
    data_ = data;
  }

  T* data() {
    return data_;
  }

  double* width() const {
    pow(2, -depth_) * tree_->width();
  }

  double* center() {
    return center_;
  }

  SmoothQuadtreeBox<T>** children() {
    return children_;
  }

  bool is_leaf() const {
    return children_ == nullptr;
  }

  const int depth() const {
    return depth_;
  }

 protected:
  SmoothQuadtreeBox<T>** neighbors() const {
    return neighbors_;
  }

  shared_ptr<vector<SmoothQuadtreeBox<T>*>> enumerate_halfspace_leaf_descendants(int dir) {
    assert(!is_leaf());

    shared_ptr<vector<SmoothQuadtreeBox<T>*>> neighbors (new vector<SmoothQuadtreeBox<T>*>());
    int axis = fabs(dir);
    int dir_bit = (dir < 0 ? 0 : 1) << (axis - 1);
    int bottom_mask = (1 << (axis - 1)) - 1;
    int top_mask = (1 << dimension()) - 1 - bottom_mask;

    for (int i = 0; i < (1 << (dimension() - 1)); i++) {
      int ind = ((i & top_mask) << 1) | dir_bit | (i & bottom_mask);
      assert(ind >= 0 && ind < 1 << dimension());
      SmoothQuadtreeBox<T>* cur_child = children_[ind];
      if (cur_child->is_leaf()) {
	neighbors->push_back(cur_child);
      } else {
	shared_ptr<vector<SmoothQuadtreeBox<T>*>> descendants = cur_child->enumerate_halfspace_leaf_descendants(dir);
        neighbors->insert(neighbors->begin(), descendants->begin(), descendants->end()); 
      }
    }

    return neighbors;
  }

  const int dimension() const {
    return tree_->dimension();
  }

  void split() {
    int num_children = 1 << dimension();
    double* center;

    // Initialize the children.
    children_ = new SmoothQuadtreeBox<T>*[num_children];
    for (int i = 0; i < num_children; i++) {

      // Compute the center of the new child.
      center = new double[dimension()];
      for (int j = 0; j < dimension(); j++) {
	center[j] = center_[j] + ((i & (1 << j)) == 0 ? -1 : 1) * pow(2, -(depth_ + 1)) * tree_->width();
      }
      children_[i] = new SmoothQuadtreeBox<T>(depth_ + 1, i, center, tree_);
    }

    // Initialize neighbors of new children.
    // Each box has 2 * d principal neighbor pointers, one in each semi-axis direction.
    // Specifically, each box has exactly one sibling neighbor and one non-sibling or null neighbor
    // in the opposite direction along each axis.
    for (int child_ind = 0; child_ind < num_children; child_ind++) {
      SmoothQuadtreeBox<T>* cur_child = children_[child_ind];
      for (int d = 0; d < dimension(); d++) {
	int neighbor_child_ind = child_ind ^ (1 << d);
	int sibling_dir_ind = (neighbor_child_ind & (1 << d)) == 0 ? 0 : 1;

	// Set sibling neighbor.
	int sib_dim_ind = d + dimension() * sibling_dir_ind;
	cur_child->neighbors_[sib_dim_ind] = children_[neighbor_child_ind];

	// Set non-sibling neighbor and update neighbor's pointers.
	int non_sib_dim_ind = d + dimension() * (1 - sibling_dir_ind);
	SmoothQuadtreeBox<T>* cur_neighbor = neighbors_[non_sib_dim_ind];

	if (cur_neighbor == nullptr || cur_neighbor->is_leaf()) {
	  cur_child->neighbors()[non_sib_dim_ind] = cur_neighbor;
	} else {
	  cur_neighbor = cur_neighbor->children()[neighbor_child_ind];
	  cur_child->neighbors()[non_sib_dim_ind] = cur_neighbor;
	  cur_neighbor->neighbors()[sib_dim_ind] = cur_child;
	}
      }
    }
    tree_->inc_splits();
  }

  void smooth_split_aux() {
    assert(is_leaf());
    for (int d = 0; d < dimension(); d++) {
      int non_sib_neighbor_ind = d + (((indicator_ & (1 << d)) == 0) ? 0 : 1) * dimension();
      SmoothQuadtreeBox<T>* cur_neighbor = neighbors_[non_sib_neighbor_ind];
      if (cur_neighbor != nullptr && cur_neighbor->depth() < depth_) {
	cur_neighbor->smooth_split_aux();
      }
    }
    split();
  }

  const int depth_;
  const int indicator_;
  double* center_;
  T* data_;
  SmoothQuadtreeBox<T>** children_;
  SmoothQuadtreeBox<T>** neighbors_;
  SmoothQuadtree<T>* tree_;

  friend class SmoothQuadtree<T>;
};

#endif // SMOOTH_QUADTREE_BOX_H
