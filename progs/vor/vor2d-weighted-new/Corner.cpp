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

#include "Corner.h"
#include "Edge.h"
#include "Point.h"

namespace vor2d {

BiPoly Corner::make_dfun(Object* parent, const Point2d& p) {
  double a = parent->m()[0];
  double b = parent->m()[1];
  double c = parent->m()[2];
  
  BiPoly w_x;
  w_x.add_monomial(1.0, 1, 0);
  w_x.add_monomial(-p[0], 0, 0);
  
  BiPoly w_y;
  w_y.add_monomial(1.0, 0, 1); 
  w_y.add_monomial(-p[1], 0, 0);
  
  return (a * w_x * w_x) + (2 * b * w_x * w_y) + (c * w_y * w_y);
}

Corner::Corner(const Point2d& position, Object* parent) :
  position_(position[0], position[1]) {
  dfun_sq_ = make_dfun(parent, position);
  dfun_sq_grad_ = dfun_sq_.gradient();
  parent_ = parent;
}

Corner::~Corner() {}

// TODO: Make this return a non-pointer.
BiPoly Corner::dfun_sq() {
  return dfun_sq_;
}

pair<BiPoly, BiPoly> Corner::dfun_sq_grad() {
  return dfun_sq_grad_;
}

// double Corner::dist_sq(double x, double y) {
//   return dfun_sq_.eval(x, y);
// }

// pair<double, double> Corner::dist_sq_grad(double x, double y) {
//   return pair<double, double>{
//     dfun_sq_grad_.first.eval(x, y),
//       dfun_sq_grad_.second.eval(x, y)};
// }

// Interval Corner::box_dist_sq(const Interval& int_x, const Interval& int_y) {
//   return dfun_sq_.eval(int_x, int_y);
// }

// pair<Interval, Interval> Corner::box_dist_sq_grad(
//   const Interval& int_x, const Interval& int_y) {
//   return pair<Interval, Interval>{
//     dfun_sq_grad_.first.eval(int_x, int_y),
//       dfun_sq_grad_.second.eval(int_x, int_y)};
// }

Point2d Corner::position() const {
  return position_;
}

// bool Corner::operator==(const Corner& other) {
//   return position_ == other.position();
// }

} // namespace vor2d
