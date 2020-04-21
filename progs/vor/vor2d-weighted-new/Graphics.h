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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "vor2d.h"
#include "vor_qt.h"
#include "vor_box.h"
#include "Corner.h"
#include "Edge.h"
#include "Object.h"
#include "Point.h"

using vor2d::vor_box;
using vor2d::vor_qt;
using vor2d::vor_seg;
using vor2d::Corner;
using vor2d::Edge;
using vor2d::Feature;
using vor2d::Object;

class Graphics {
public:
  Graphics(const Point2d& ctr, double scale);
  void draw_vor_seg(const vor_seg& seg);
  void draw_box(const vor_box& box, bool show_grid);
  void draw_box_rec(const vor_box& box, bool show_grid);
  void draw_edge(const Edge& edge);
  void draw_corner(const Corner& corner);
  void update(const Point2d& ctr, double scale);
  void update(double scale);
  double mxi(double x);
  double myi(double y);
  double mx(double x);
  double my(double y);

private:
  Point2d ctr_;
  double scale_;
};

#endif // GRAPHICS_H
