#pragma once
#include <vector>
#include "./geometry/Vector.h"
#include <iostream>
using namespace std;

class Box3d {
 public:
  Vector* origin;
  double width;
  vector<Box3d*> children;

  Box3d(double x, double y, double z, double width) {
    origin = new Vector(x, y, z);
    this->width = width;
  }

  bool approxEqual(double x, double y) {
    return abs(x - y) < 0.001;
  }

  bool containsPoint(double xx, double yy, double zz) {
    double x = origin->x;
    double y = origin->y;
    double z = origin->z;
    return
      !(xx > x + width / 2 || xx < x - width / 2 ||
        yy > y + width / 2 || yy < y - width / 2 ||
        zz > z + width / 2 || zz < z - width / 2);
  }

  bool intervalContains(double x, double xx, double w, double ww) {
    w += 0.0001;
    w /= 2;
    ww += 0.0001;
    ww /= 2;
    return (x + w > xx && x - w < xx) || (xx + ww > x && xx - ww < x);
  }

  bool includeFace(double x, double y, double w, double xx, double yy, double ww) {
    return intervalContains(x, xx, w, ww) && intervalContains(y, yy, w, ww);
  }

  bool isAdjacent(Box3d* other) {
    double dx = abs(other->origin->x - origin->x);
    double dy = abs(other->origin->y - origin->y);
    double dz = abs(other->origin->z - origin->z);
    double sw = width / 2 + other->width / 2;
    bool adj =
      (approxEqual(dx, sw) && includeFace(origin->y, origin->z, width, other->origin->y, other->origin->z, other->width)) ||
      (approxEqual(dy, sw) && includeFace(origin->x, origin->z, width, other->origin->x, other->origin->z, other->width)) ||
      (approxEqual(dz, sw) && includeFace(origin->x, origin->y, width, other->origin->x, other->origin->y, other->width));
    return adj;
  }

  bool isIdentical(Box3d* other) {
    return origin == other->origin && width == other->width;
  }

  bool split(double epsilon) {
    if (width < epsilon) {
      return false;
    }
    children.clear();
    for (int i = 0; i < 2; i++) {
      double nx = origin->x - width / 4 + width / 2 * i;
      for (int j = 0; j < 2; j++) {
        double ny = origin->y - width / 4 + width / 2 * j;
        for (int k = 0; k < 2; k++) {
          double nz = origin->z - width / 4 + width / 2 * k;
          children.push_back(new Box3d(nx, ny, nz, width / 2));
        }
      }
    }
    return true;
  }
};
