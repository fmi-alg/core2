/*
 * edge.cpp
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "edge.h"

Edge::Edge(Corner *const s, Corner *const d) : Feature(TYPE_EDGE), a(s), b(d) {
  a->nxt_edge.push_back(this);
  b->pre_edge.push_back(this);
  convex_ = false;

  segment_ = Segment3d(s->point(), d->point());
}

double Edge::distance(Point3d point, double &t, Point3d &proj) {
  double t_num = Vector(point - a->point()) *
                 Vector(b->point() - a->point());           // <q-a, b-a>
  double t_denom = Vector(b->point() - a->point()).norm();  // |b-a|^2
  t = t_num / t_denom;
  t = t < 0 ? 0 : t;
  t = t > 1 ? 1 : t;

  // parametric form
  proj = b->point() * t + a->point() * (1 - t);
  return point.distance(proj);
}

bool Edge::same(Corner *aa, Corner *bb) { return aa->same(*a) && bb->same(*b); }

bool Edge::isConvex() {
  Vector e = Vector(b->point() - a->point());
  if (right_wall == nullptr) return true;
  Vector normal_right_wall = right_wall->triangle().normal();
  if (opposite_wall == nullptr) return true;
  Vector normal_opposite_wall = opposite_wall->triangle().normal();
  Vector cross_normal = normal_right_wall.cross(normal_opposite_wall);
  if (fabs(cross_normal.X()) > 0 || fabs(cross_normal.Y()) > 0 ||
      fabs(cross_normal.Z()) > 0) {
    cross_normal.normalize();
    return (dotProduct(e, cross_normal) >= 0);
  } else {
    return true;
  }
}

bool Edge::intersectEdge(Edge *e, Point3d &rst) {
  Point3d p = a->point();
  Vector r(b->point() - a->point());
  Point3d q = e->a->point();
  Vector s(e->b->point() - e->a->point());

  Vector r_s = r.cross(s);
  Vector q_p = q - p;
  q_p = q_p.cross(s);

  Vector s_r = s.cross(r);
  Vector p_q = p - q;
  p_q = p_q.cross(r);

  Vector t(0, 0, 0), u(0, 0, 0);

  if (fabs(r_s.X()) > 0 || fabs(r_s.Y()) > 0 || fabs(r_s.Z()) > 0) {
    t.set(q_p.X() / r_s.X(), q_p.Y() / r_s.Y(), q_p.Z() / r_s.Z());

    rst.set(p.X() + t.X() * r.X(), p.Y() + t.Y() * r.Y(),
            p.Z() + t.Z() * r.Z());

    //    u.x = p_q.x/s_r.x;
    //    u.x = p_q.x/s_r.x;
    //    u.x = p_q.x/s_r.x;

    return true;
  } else {
    return false;
  }
}
