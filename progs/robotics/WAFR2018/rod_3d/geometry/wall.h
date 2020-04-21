/*
 * wall.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 * Description:
 *  a wall is a triangle defined by vertices (corners) a, b, c.
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef WALL_H
#define WALL_H

#include <cmath>
#include <string>
#include <vector>

#include "CORE/geom3d/line3d.h"
#include "CORE/geom3d/plane3d.h"
#include "CORE/geom3d/point3d.h"
#include "CORE/geom3d/segment3d.h"
#include "CORE/geom3d/triangle3d.h"
#include "geometry/corner.h"
#include "geometry/edge.h"
#include "geometry/feature.h"

class Corner;
class Edge;

class Wall : public Feature {
 private:
  Triangle3d triangle_;
  Plane3d plane_;

 public:
  Corner* a;
  Corner* b;
  Corner* c;

  Edge* ab;
  Edge* bc;
  Edge* ca;

  Wall(Corner* const aa, Corner* const bb, Corner* const cc);

  Triangle3d triangle() { return this->triangle_; }
  Plane3d plane() { return this->plane_; }

  bool same(Wall& ww);

  bool insideWall(Point3d& p);
  bool isIntersected(Corner& p, Vector& ray, Corner& cc);
  int rayIntersectPlane(Corner& p, Vector& ray, Corner& cc);
};

#endif  // WALL_H
