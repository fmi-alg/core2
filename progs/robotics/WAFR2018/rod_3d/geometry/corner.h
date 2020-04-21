/*
 * corner.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 * Description:
 *  classification
 *  check collision
 *  find clearance (distance to the nearest feature)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef CORNER_H
#define CORNER_H

#include <cmath>
#include <string>
#include <vector>

#include "CORE/geom3d/point3d.h"
#include "geometry/edge.h"
#include "geometry/feature.h"
#include "geometry/wall.h"

class Edge;
class Wall;

class Corner : public Feature {
 private:
  Point3d p_;

 public:
  std::vector<Edge*> pre_edge;
  std::vector<Edge*> nxt_edge;

  std::vector<Wall*> incident_wall;

  Corner(Point3d point) : Feature(TYPE_CORNER), p_(point) {}
  Corner(const Corner& c) : Feature(TYPE_CORNER), p_(c.p_) {}

  Point3d point() { return p_; }
  void setPoint(double x, double y, double z) { p_ = Point3d(x, y, z); }
  bool same(const Corner& corner) { return (p_ == corner.p_); }

  void getHat() {
    double d = fabs(p_.X());
    if (d < fabs(p_.Y())) d = fabs(p_.Y());
    if (d < fabs(p_.Z())) d = fabs(p_.Z());
    if (d > 0) {
      p_.set(p_.X() / d, p_.Y() / d, p_.Z() / d);
    }
  }
};

#endif  // CORNER_H
