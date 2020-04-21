/* **************************************
   File: Edge.h

   Description:

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang
   Update: July, 2017: Ching-Hsiang (Tom) Hsu

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once

#include <math.h>

#include "CORE/geom3d/line3d.h"
#include "CORE/geom3d/segment3d.h"
#include "geometry/Corner.h"
#include "geometry/Wall.h"

class Corner;
class Wall;

class Edge {
 public:

  bool convexEdge;
  Segment3d seg;

  Corner* src;
  Corner* dst;

  Wall* rightWall;
  Wall* oppositeWall;

  Edge(Corner* s, Corner* d);
  Edge(Corner s, Corner d);

  double distance(Point3d p, double &t, Point3d &proj);

  bool same(Corner* a, Corner* b);
  bool isConvex();

  void intersectDisc(Point3d c, double radius, Point3d &i1, bool &intersect1, Point3d &i2, bool &intersect2);
  bool intersectEdge(Edge* e, Point3d &rst);
};
