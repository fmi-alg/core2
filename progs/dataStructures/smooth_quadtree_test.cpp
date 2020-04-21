/* **************************************
   File: smooth_quadtree_test.ccp

   Description: 
 	An implementation of the smooth quadtree data structure.
 
   Author: Huck Bennett, 9/21/2014
 
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

#include "smooth_quadtree.h"
#include <assert.h>
#include <iostream>

using namespace std;

struct Pair {
  Pair (int x, int y) : x_(x), y_(y) {}
  int x_;
  int y_;
};

int main(int argc, char** argv) {
  SmoothQuadtree<Pair>* tree = new SmoothQuadtree<Pair>(2, 1.0);
  SmoothQuadtreeBox<Pair>* root = tree->root();
  cout << root->center()[0] << " " << root->center()[1] << "\n";

  root->smooth_split();
  root->children()[0]->smooth_split();
  SmoothQuadtreeBox<Pair>* cur_child = root->children()[1];

  Pair p1 (3, 4);
  Pair p2 (4, 5);

  root->set_data(&p1);
  cur_child->set_data(&p2);

  double point_array[] = {0.5, -0.5};
  vector<double> point(point_array, point_array + sizeof(point_array) / sizeof(double));
  SmoothQuadtreeBox<Pair>* point_box = tree->get_box(point);
  assert(point_box != nullptr);
  cout << point_box->center()[0] << " " << point_box->center()[1] << "\n";
  assert(point_box == cur_child);

  assert(cur_child->is_leaf());
  root->children()[0]->children()[3]->smooth_split();
  assert(!cur_child->is_leaf());
  root->children()[0]->children()[3]->children()[3]->smooth_split();
  assert(!cur_child->is_leaf());
  assert(!root->children()[3]->is_leaf());
  cout << "Data: " << cur_child->data()->x_ << " " << cur_child->data()->y_ << "\n";

  cout << "Dir 1 neighbors:\n";
  shared_ptr<vector<SmoothQuadtreeBox<Pair>*>> neighbors = cur_child->leaf_neighbors_dir(1);
  for (SmoothQuadtreeBox<Pair>* neighbor : *neighbors) {
    cout << neighbor->center()[0] << " " << neighbor->center()[1] << "\n";
  }

  cout << "Dir -1 neighbors:\n";
  neighbors = cur_child->leaf_neighbors_dir(-1); 
  for (SmoothQuadtreeBox<Pair>* neighbor : *neighbors) {
    cout << neighbor->center()[0] << " " << neighbor->center()[1] << "\n";
  }

  cout << "Dir 2 neighbors:\n";
  neighbors = cur_child->leaf_neighbors_dir(2);
  for (SmoothQuadtreeBox<Pair>* neighbor : *neighbors) {
    cout << neighbor->center()[0] << " " << neighbor->center()[1] << "\n";
  }

  assert(tree->splits() == 9);
  cout << "# splits: " << tree->splits() << "\n";
  cout << "# smooth splits: " << tree->smooth_splits() << "\n";

  delete tree;
  exit(0);
}
