/* **************************************
   File: Corner.h

   Description:

   HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang
   Update: July, 2017: Ching-Hsiang (Tom) Hsu

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once

#include <vector>
#include <string>
#include <cmath>

#include "CORE/geom3d/point3d.h"
#include "geometry/Edge.h"
#include "geometry/Wall.h"

using namespace std;

class Edge;
class Wall;

class Corner {
 public:
  string name;
  int id;
  Point3d p;

  vector<Edge*> preEdge;
  vector<Edge*> nextEdge;

  vector<Wall*> incidentWall;

  Corner(Point3d pp) : p(pp) {}
  Corner(const Corner& c) : p(c.p) {}

  bool same(Corner* a) {
    return (p == a->p);
  }

  void getHat(){
    double d = fabs(p.X());
    if(d < fabs(p.Y())) d = fabs(p.Y());
    if(d < fabs(p.Z())) d = fabs(p.Z());
    if(d > 0){
      p.set(p.X()/d, p.Y()/d, p.Z()/d);
    }
  }
};

class Sphere {
 public:
  double radius;
  Point3d p;

 Sphere(Point3d pp, double r) : p(pp), radius(r) {}

  double distance(Point3d pp) {
    return abs(p.distance(pp) - radius);
  }
};
