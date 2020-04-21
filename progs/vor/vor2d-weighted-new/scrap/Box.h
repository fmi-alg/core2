#pragma once

#include "Corner.h"
#include "Edge.h"
#include "../../dataStructures/smooth_quadtree.h"

namespace vor2d {

enum Status {IN, OUT, ON, UNKNOWN};

class VorBox : public SmoothQuadTreeBox<void> {
 public:
  VorBox(int depth, int ind, double center[], SmoothQuadTree<void>* tree)
    : SmoothQuadTreeBox<void>(depth, ind, center, tree) {
  }

  void add_corner(Corner* corner) {
    corners_.push_back(corner);
  }

  void add_edge(Edge* edge) {
    edges_.push_back(edge);
  }

  vector<Corner*>* corners() {
    return &corners_;
  }

  vector<Edge*>* edges() {
    return &edges_;
  }

 private:
  Status status_;
  vector<Corner*> corners_;
  vector<Edge*> edges_;
};

} // namespace vor2d
