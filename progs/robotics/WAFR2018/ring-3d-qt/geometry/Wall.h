/* **************************************
   File: Wall.h

   Description: A wall is a triangle defined by vertices (corners) a, b, c.

   HISTORY: July, 2013: Surin Ahn
   Update: July, 2017: Ching-Hsiang (Tom) Hsu

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once

#include <vector>
#include <string>
#include <cmath>

#include "CORE/geom3d/point3d.h"
#include "CORE/geom3d/line3d.h"
#include "CORE/geom3d/segment3d.h"
#include "CORE/geom3d/plane3d.h"
#include "CORE/geom3d/triangle3d.h"
#include "geometry/Corner.h"
#include "geometry/Edge.h"

using namespace std;

class Corner;
class Edge;

class Wall {
 public:
  string name;
  int id;
  Triangle3d tri;
  Plane3d pln;

  Corner* a;
  Corner* b;
  Corner* c;

  Edge* ab;
  Edge* bc;
  Edge* ca;

  Wall();
  Wall(Corner* aa, Corner* bb, Corner* cc);
  Wall(Corner aa, Corner bb, Corner cc);

  bool insideWall(Point3d p);
  bool sameSide(Point3d q);
  bool same(Wall *ww);
  bool isIntersected(Corner *p, Vector ray, Corner *cc);
  int intersect3D_RayPlane(Corner *p, Vector ray, Corner *cc);
};
