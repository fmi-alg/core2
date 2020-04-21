#pragma once
#include <math.h>

class Vector {
 public:
  double x, y, z;
  // Constructor from (x,y,z):
  Vector(double xx, double yy, double zz):x(xx), y(yy), z(zz) { }
  Vector(const Vector& c): x(c.x), y(c.y), z(c.z) { }

  Vector operator+(const Vector& other) {
    return Vector(x + other.x, y + other.y, z + other.z);
  }
  Vector operator-(const Vector& other) {
    return Vector(x - other.x, y - other.y, z - other.z);
  }
  double operator*(const Vector& other) {
    return x * other.x + y * other.y + z * other.z;
  }
  Vector operator*(const double alpha) {
    return Vector(alpha * x, alpha * y, alpha * z);
  }

  double distance(double x2, double y2, double z2) {
    return
      sqrt((x - x2) * (x - x2) +
           (y - y2) * (y - y2) +
           (z - z2) * (z - z2) );
  }
  double distance(Vector other) {
    return distance(other.x, other.y, other.z);
  }
};
