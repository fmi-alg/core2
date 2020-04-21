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
