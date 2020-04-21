/*
 * edge.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef EDGE_H
#define EDGE_H

#include <math.h>

#include "CORE/geom3d/line3d.h"
#include "CORE/geom3d/segment3d.h"
#include "geometry/corner.h"
#include "geometry/feature.h"
#include "geometry/wall.h"

class Corner;
class Wall;

class Edge : public Feature {
 private:
  bool convex_;
  Segment3d segment_;

 public:
  Corner* a;
  Corner* b;

  Wall* right_wall;
  Wall* opposite_wall;

  Edge(Corner* const s, Corner* const d);

  Segment3d segment() { return this->segment_; }
  void setSegment(Segment3d segment) { this->segment_ = segment; }

  void setConvexity(bool convex) { this->convex_ = convex; }
  bool convex() { return this->convex_; }

  double distance(Point3d p, double& t, Point3d& proj);

  bool same(Corner* aa, Corner* bb);
  bool isConvex();
  bool intersectEdge(Edge* e, Point3d& rst);
};

#endif  // EDGE_H
