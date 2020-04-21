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

#include "Graphics.h"
#include <math.h>

// TODO: Convert magic numbers into global parameters.

Graphics::Graphics(const Point2d& ctr, double scale) {
  update(ctr, scale);
}

void Graphics::update(double scale) {
  update(ctr_, scale);
}

void Graphics::update(const Point2d& ctr, double scale) {
  ctr_ = Point2d{mxi(ctr[0]), myi(ctr[1])};
  ctr_[0] = fmax(ctr_[0], -1.0 + 1 / scale);
  ctr_[0] = fmin(ctr_[0], 1.0 - 1 / scale);
  ctr_[1] = fmax(ctr_[1], -1.0 + 1 / scale);
  ctr_[1] = fmin(ctr_[1], 1.0 - 1 / scale);
  scale_ = fmax(1.0, scale);
}

double Graphics::mxi(double x) {
  return x / scale_ + ctr_[0];
}

double Graphics::myi(double y) {
  return y / scale_ + ctr_[1];
}

double Graphics::mx(double x) {
  return scale_ * (x - ctr_[0]);
}

double Graphics::my(double y) {
  return scale_ * (y - ctr_[1]);
}

void Graphics::draw_vor_seg(const vor_seg& seg) {
  glColor3f(1.0, 0.0, 0.0); // Red.
  glLineWidth(3.0);
  glBegin(GL_LINES);
  glVertex2d(mx(seg.p_[0]), my(seg.p_[1]));
  glVertex2d(mx(seg.q_[0]), my(seg.q_[1]));
  glEnd();
}

void Graphics::draw_box(const vor_box& box, bool show_grid) {
  double cx = box.center()[0];
  double cy = box.center()[1];
  double hw = box.width() / 2;

  // Draw mesh boundaries.
  if (show_grid) {
    glColor3f(0.7, 0.7, 0.7); // Gray
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(mx(cx - hw), my(cy - hw));
    glVertex2d(mx(cx - hw), my(cy + hw));
    glVertex2d(mx(cx + hw), my(cy + hw));
    glVertex2d(mx(cx + hw), my(cy - hw));
    glEnd();
  }

  if (box.is_degen()) {
    glColor3f(0, 0, 1.0); // Blue
    glBegin(GL_POLYGON);
    glVertex2d(mx(cx - hw), my(cy - hw));
    glVertex2d(mx(cx - hw), my(cy + hw));
    glVertex2d(mx(cx + hw), my(cy + hw));
    glVertex2d(mx(cx + hw), my(cy - hw));
    glEnd();
  }

  // Display Voronoi segments.
  for (vor_seg* seg : *box.get_segments()) {
    draw_vor_seg(*seg);
  }
}

void Graphics::draw_box_rec(const vor_box& box, bool show_grid) {
  draw_box(box, show_grid);
  if (!box.is_leaf()) {
    vor_box** children = box.children();
    for (int i = 0; i < box.num_children(); i++) {
      draw_box_rec(*children[i], show_grid);
    }
  }
}

void Graphics::draw_edge(const Edge& edge) {
  const Point2d s_pos = edge.source()->position();
  const Point2d d_pos = edge.dest()->position();
  glColor3f(0.0, 0.0, 0.0); // Black.
  glLineWidth(3.0);
  glBegin(GL_LINES);
  glVertex2d(mx(s_pos[0]), my(s_pos[1]));
  glVertex2d(mx(d_pos[0]), my(d_pos[1]));
  glEnd();
}

void Graphics::draw_corner(const Corner& corner) {
  const Point2d point = corner.position();
  glColor3f(0.0, 0.0, 0.0); // Black.
  glPointSize(7.0);
  glBegin(GL_POINTS);
  glVertex2d(mx(point[0]), my(point[1]));
  glEnd();
}
