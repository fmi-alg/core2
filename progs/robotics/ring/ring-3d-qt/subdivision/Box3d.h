#pragma once

#include <vector>
#include <iostream>

#include "CORE/geom3d/point3d.h"

using namespace std;

extern bool verbose;
extern FILE* g_fptr;

class Box3d {
 public:
  Point3d* origin;
  double width;
  vector<Box3d*> children;

  Box3d(Point3d pp, double width) {
    origin = new Point3d(pp);
    this->width = width;
  }

  bool approxEqual(double a, double b) {
    return abs(a - b) < 1e-8;
  }

  bool containsPoint(Point3d p) {
    return
      !(p.X() > origin->X() + width / 2 || p.X() < origin->X() - width / 2 ||
        p.Y() > origin->Y() + width / 2 || p.Y() < origin->Y() - width / 2 ||
        p.Z() > origin->Z() + width / 2 || p.Z() < origin->Z() - width / 2);
  }

  bool intervalContains(double x, double xx, double w, double ww) {
    w  += 1e-8;
    w  /= 2;
    ww += 1e-8;
    ww /= 2;
    return (x + w > xx && x - w < xx) || (xx + ww > x && xx - ww < x);
  }

  bool includeFace(double x, double y, double w, double xx, double yy, double ww) {
    return intervalContains(x, xx, w, ww) && intervalContains(y, yy, w, ww);
  }

  bool isSameDirectionAdjacent(Box3d* other, int idx){
    double dx = abs(other->origin->X() - origin->X());
    double dy = abs(other->origin->Y() - origin->Y());
    double dz = abs(other->origin->Z() - origin->Z());
    double sw = width / 2 + other->width / 2;
    bool adj =
      (idx == 1 && approxEqual(dx, sw) && includeFace(origin->Y(), origin->Z(), width, other->origin->Y(), other->origin->Z(), other->width)) ||
      (idx == 2 && approxEqual(dy, sw) && includeFace(origin->X(), origin->Z(), width, other->origin->X(), other->origin->Z(), other->width)) ||
      (idx == 3 && approxEqual(dz, sw) && includeFace(origin->X(), origin->Y(), width, other->origin->X(), other->origin->Y(), other->width));
    return adj;
  }

  bool isAdjacent(Box3d* other) {
    double dx = abs(other->origin->X() - origin->X());
    double dy = abs(other->origin->Y() - origin->Y());
    double dz = abs(other->origin->Z() - origin->Z());
    double sw = width / 2 + other->width / 2;
    bool adj =
      (approxEqual(dx, sw) && includeFace(origin->Y(), origin->Z(), width, other->origin->Y(), other->origin->Z(), other->width)) ||
      (approxEqual(dy, sw) && includeFace(origin->X(), origin->Z(), width, other->origin->X(), other->origin->Z(), other->width)) ||
      (approxEqual(dz, sw) && includeFace(origin->X(), origin->Y(), width, other->origin->X(), other->origin->Y(), other->width));
    return adj;
  }

  bool isIdentical(Box3d* other) {
    return  (abs(origin->X()-other->origin->X()) < 1e-8) &&
            (abs(origin->Y()-other->origin->Y()) < 1e-8) &&
            (abs(origin->Z()-other->origin->Z()) < 1e-8) &&
            (abs(width-other->width) < 1e-8);
  }

  bool split(double epsilon) {
    //fprintf(g_fptr, "split Bt\n");
    if (width < epsilon) {
      //fprintf(g_fptr, "QQQ\n");
      return false;
    }
    children.clear();
    for (int i = 0; i < 2; i++) {
      double nx = origin->X() - width / 4 + width / 2 * i;
      for (int j = 0; j < 2; j++) {
        double ny = origin->Y() - width / 4 + width / 2 * j;
        for (int k = 0; k < 2; k++) {
          double nz = origin->Z() - width / 4 + width / 2 * k;
          children.push_back(new Box3d(Point3d(nx, ny, nz), width / 2));
        }
      }
    }
    return true;
  }
};
