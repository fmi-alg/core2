/*
SubVor, a program for computing Voronoi diagrams via subdivision.
Copyright (C) 2016 Huck Bennett, Evanthia Papadopoulou, and Chee Yap.
For comments or questions, please contact Huck Bennett at hbennett@cs.nyu.edu.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Corner.h"
#include "Edge.h"
#include "Feature.h"
#include "Point.h"
#include "Interval.h"
#include "Point.h"
#include <vector>
#include <limits>

namespace vor2d {

class vor_box;

// A class to represent a collection of features.
class Object {
 public:
  Object(double weight);
  Object(double a, double b, double c);
  ~Object();

  // Add a feature to the object.
  void add_feature(Feature* feature);

  // Compute the distance between an object and this point.
  double distance(const Point2d& point) const;
  // double Interval box_distance(const Interval& int_x, const Interval& int_y);
  // tuple<Interval, Interval> box_grad(const Interval& int_x, const Interval& int_y);

  double qm(const Point2d& p);
  double qm2(const Point2d& p, const Point2d& q);
  Interval qm_b(const Interval& int_x, const Interval& int_y);
  Interval qm2_b(const Point2d& p, const Interval& int_x, const Interval& int_y);
  double* m();
  bool is_polygon() const;
  bool contains(const Point2d& p) const;
  bool contains(const vor_box& b) const;
  vector<Feature*>* get_features();

 private:
  // Determines whether r is to the left, right or on the line going through p, q.
  int in_on_out(Point2d p, Point2d q, Point2d r) const;
  
  vector<Feature*> features_;
  vector<Corner*> corners_;
  vector<Edge*> edges_;
  double m_[3]; // Weight metric
};

} // namespace vor2d
