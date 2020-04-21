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

#include "Interval.h"
#include "Point.h"
#include "BiPoly.h"

namespace vor2d {

class Object;

class Feature {
 public:
  Feature();
  virtual BiPoly dfun_sq() = 0;
  virtual pair<BiPoly, BiPoly> dfun_sq_grad() = 0;

  virtual double distance(const Point2d& point);
  double lipschitz() const;
  void set_parent(Object* parent);
  Object* parent();
  bool is_corner();
  bool is_edge();

 protected:
  Object* parent_;
};

} // namespace vor2d
