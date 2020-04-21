/*
 * box3d.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef BOX3D_H
#define BOX3D_H

#include <iostream>
#include <vector>

#include "CORE/geom3d/point3d.h"

extern double EPS;

class Box3d {
 public:
  Point3d* origin;
  double width;
  std::vector<Box3d*> children;

  Box3d(Point3d point, double width) {
    origin = new Point3d(point);
    this->width = width;
  }

  bool approxEqual(double a, double b) { return fabs(a - b) < EPS; }

  bool containsPoint(Point3d p) {
    return !(p.X() > origin->X() + width / 2.0 ||
             p.X() < origin->X() - width / 2.0 ||
             p.Y() > origin->Y() + width / 2.0 ||
             p.Y() < origin->Y() - width / 2.0 ||
             p.Z() > origin->Z() + width / 2.0 ||
             p.Z() < origin->Z() - width / 2.0);
  }

  bool intervalContains(double x, double xx, double w, double ww) {
    w += EPS;
    w /= 2.0;
    ww += EPS;
    ww /= 2.0;
    return (x + w > xx && x - w < xx) || (xx + ww > x && xx - ww < x);
  }

  bool includeFace(double x, double y, double w, double xx, double yy,
                   double ww) {
    return intervalContains(x, xx, w, ww) && intervalContains(y, yy, w, ww);
  }

  bool isAdjacent(Box3d* other) {
    double dx = fabs(other->origin->X() - origin->X());
    double dy = fabs(other->origin->Y() - origin->Y());
    double dz = fabs(other->origin->Z() - origin->Z());
    double sw = width / 2.0 + other->width / 2.0;
    bool adj = (approxEqual(dx, sw) &&
                includeFace(origin->Y(), origin->Z(), width, other->origin->Y(),
                            other->origin->Z(), other->width)) ||
               (approxEqual(dy, sw) &&
                includeFace(origin->X(), origin->Z(), width, other->origin->X(),
                            other->origin->Z(), other->width)) ||
               (approxEqual(dz, sw) &&
                includeFace(origin->X(), origin->Y(), width, other->origin->X(),
                            other->origin->Y(), other->width));
    return adj;
  }

  bool isIdentical(Box3d* other) {
    return (fabs(origin->X() - other->origin->X()) < EPS) &&
           (fabs(origin->Y() - other->origin->Y()) < EPS) &&
           (fabs(origin->Z() - other->origin->Z()) < EPS) &&
           (fabs(width - other->width) < EPS);
  }

  bool split(double epsilon) {
    if (width < epsilon) {
      return false;
    }
    children.clear();
    for (int i = 0; i < 2; i++) {
      double nx = origin->X() - width / 4.0 + width / 2.0 * i;
      for (int j = 0; j < 2; j++) {
        double ny = origin->Y() - width / 4.0 + width / 2.0 * j;
        for (int k = 0; k < 2; k++) {
          double nz = origin->Z() - width / 4.0 + width / 2.0 * k;
          children.push_back(new Box3d(Point3d(nx, ny, nz), width / 2.0));
        }
      }
    }
    return true;
  }
};

#endif  // BOX3D_H
