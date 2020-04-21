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

#include "Object.h"
#include "Corner.h"
#include "Feature.h"
#include "Point.h"
#include "BiPoly.h"

#include <float.h>
#include <math.h>

namespace vor2d {

class Edge : public Feature {
 public:
  Edge(Corner* source, Corner* dest, Object* parent);
  Edge(const Point2d& p, const Point2d& q, Object* parent);
  BiPoly dfun_sq() override;
  pair<BiPoly, BiPoly> dfun_sq_grad() override;
  BiPoly get_tstar();
  bool interior_active(double x, double y);
  bool interior_active(Interval ix, Interval iy);
  double distance(const Point2d& point) override;
  
  Corner* source() const;
  Corner* dest() const;
  
 private:
  BiPoly make_dsegfun(Object* parent, const Point2d& p, const Point2d& q);

  double qmv;
  BiPoly qmw;
  BiPoly vmw;
  BiPoly tstar;
  Corner* source_;
  Corner* dest_;

  // These polynomials represent the distance to the line through p and q.
  BiPoly dfun_seg_sq_;
  pair<BiPoly, BiPoly> dfun_seg_sq_grad_;
};

} // namespace vor2d
