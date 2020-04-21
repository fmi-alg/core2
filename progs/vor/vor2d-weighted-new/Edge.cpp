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
#include <math.h>

#define DEBUG 0

namespace vor2d {

BiPoly Edge::make_dsegfun(Object* parent, const Point2d& p, const Point2d& q) {
  double a = parent->m()[0];
  double b = parent->m()[1];
  double c = parent->m()[2];

  Point2d v = q - p;
  double v_x = v[0];
  double v_y = v[1];

  BiPoly w_x;
  w_x.add_monomial(1.0, 1, 0);
  w_x.add_monomial(-p[0], 0, 0);
  
  BiPoly w_y;
  w_y.add_monomial(1.0, 0, 1);
  w_y.add_monomial(-p[1], 0, 0);

  // State changing assignments.
  qmw = (a * w_x * w_x) + (2 * b * w_x * w_y) + (c * w_y * w_y);
  vmw = v_x * (a * w_x + b * w_y) + v_y * (b * w_x + c * w_y);
  qmv = parent->qm(v);
  tstar = (1.0 / qmv) * vmw;

#if DEBUG
  cout << "p: " << p << " q: " << q << "\n";
  cout << "v: " << v << "\n";
  cout << "a: " << a << " b: " << b << " c: " << c << "\n";
  cout << "qmw: " << qmw.to_string() << "\n";
  cout << "vmw: " << vmw.to_string() << "\n";
  cout << "qmv: " << qmv << "\n";
  cout << "tstar: " << tstar.to_string() << "\n";
#endif

  return qmw - (tstar * vmw);
}

Edge::Edge(Corner* source, Corner* dest, Object* parent) :
  source_(source), dest_(dest) {
  parent_ = parent;
  dfun_seg_sq_ = make_dsegfun(parent, source->position(), dest->position());
  dfun_seg_sq_grad_ = dfun_seg_sq_.gradient();
  // source->set_next_edge(this);
  // dest->set_prev_edge(this);
}

Edge::Edge(const Point2d& p, const Point2d& q, Object* parent) :
  Edge(new Corner(p, parent), new Corner(q, parent), parent) {
}

BiPoly Edge::dfun_sq() {
  return dfun_seg_sq_;
}

pair<BiPoly, BiPoly> Edge::dfun_sq_grad() {
  return dfun_seg_sq_grad_;
}

BiPoly Edge::get_tstar() {
  return tstar;
}

bool Edge::interior_active(double x, double y) {
  double e_t = tstar.eval(x, y);
  return 0 <= e_t && e_t <= 1;
}

bool Edge::interior_active(Interval ix, Interval iy) {
  Interval e_t_range = tstar.eval(ix, iy);
  return e_t_range.contains(0) || e_t_range.contains(1);
}

Corner* Edge::dest() const {
  return dest_;
}

Corner* Edge::source() const {
  return source_;
}

double Edge::distance(const Point2d& point) {
  double x = point[0];
  double y = point[1];
  if (interior_active(x, y)) {
    return sqrt(dfun_sq().eval(x, y));
  }
  return fmin(source_->distance(point), dest_->distance(point));
}

} // namespace vor2d
