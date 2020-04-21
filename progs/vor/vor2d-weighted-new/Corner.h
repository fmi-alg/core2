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

#include "Feature.h"
#include "Point.h"

namespace vor2d {
class Edge;

class Corner : public Feature {
 public:
  Corner(const Point2d& position, Object* parent);
  ~Corner();
  BiPoly dfun_sq() override;
  pair<BiPoly, BiPoly> dfun_sq_grad() override;
  
  Point2d position() const;

  friend ostream& operator<<(ostream& os, const Corner& c) {
    return os << "(" << c.position()[0] << ", " << c.position()[1] << ")";
  }

 private:
  BiPoly make_dfun(Object* parent, const Point2d& p);
  
  BiPoly dfun_sq_;
  pair<BiPoly, BiPoly> dfun_sq_grad_;
  const Point2d position_;
};

} // namespace vor2d
