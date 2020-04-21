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

#include "Feature.h"
#include "Object.h"
#include "Corner.h"
#include "Edge.h"
#include "Point.h"

#include <math.h>

#define SQ(x) ((x) * (x))

namespace vor2d {

Feature::Feature() {}

void Feature::set_parent(Object* parent) {
  parent_ = parent;
}

Object* Feature::parent() {
  return parent_;
}

// See derivation of Lipschitz constant in paper.
// TODO: Make this an Object method, not a Feature method.
double Feature::lipschitz() const {
  double* m = parent_->m();
  return (1.0 / sqrt(2)) * sqrt(m[0] + m[2] + sqrt(SQ(m[0] - m[2]) + 4 * SQ(m[1])));
}

// BiPoly* Feature::dfun_sq() {
//   return &dfun_sq_;
// }

// pair<BiPoly, BiPoly> Feature::dfun_sq_grad() {
//   return dfun_sq_grad_;
// }

double Feature::distance(const Point2d& point) {
  return sqrt(dfun_sq().eval(point[0], point[1]));
}

bool Feature::is_corner() {
  Corner* c = dynamic_cast<Corner*>(this);
  return c != nullptr;
}

bool Feature::is_edge() {
  Edge* e = dynamic_cast<Edge*>(this);
  return e != nullptr;
}

} // namespace vor2d
