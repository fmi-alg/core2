#pragma once
#include "./Vector.h"
#include "./Obstacle.h"
#include <cmath>
class Edge;

class Corner : public Vector, public Obstacle {
 public:
  Edge* preEdge;
  Edge* nextEdge;
  // Constructor from (x,y,z):
 Corner(double xx, double yy, double zz) : Vector(xx, yy, zz) { }
 Corner(const Corner& c) : Vector(c) { }

  Corner operator+(const Corner& other) {
    return Corner(x + other.x, y + other.y, z + other.z);
  }
  Corner operator-(const Corner& other) {
    return Corner(x - other.x, y - other.y, z - other.z);
  }

  double distance(double x2, double y2, double z2) {
    return
      sqrt((x - x2) * (x - x2) +
           (y - y2) * (y - y2) +
           (z - z2) * (z - z2) );
  }
};

class Sphere : public Corner {
 public:
  double radius;

 Sphere(double xx, double yy, double zz, double r):
  Corner(xx, yy, zz), radius(r) { }

  double distance(double xx, double yy, double zz) {
    return
      std::abs(sqrt((x - xx) * (x - xx) +
               (y - yy) * (y - yy) +
               (z - zz) * (z - zz) ) - radius);
  }
};
