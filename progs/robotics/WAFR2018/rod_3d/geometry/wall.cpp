/*
 * wall.cpp
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "wall.h"

extern double EPS;

Wall::Wall(Corner *const aa, Corner *const bb, Corner *const cc)
    : Feature(TYPE_WALL), a(aa), b(bb), c(cc) {
  a->incident_wall.push_back(this);
  b->incident_wall.push_back(this);
  c->incident_wall.push_back(this);

  triangle_ = Triangle3d(a->point(), b->point(), c->point());
  triangle_.normalize();
}

bool Wall::insideWall(Point3d &p) { return (triangle_.distance(p) < EPS); }

bool Wall::same(Wall &ww) {
  return (a->point() == ww.a->point()) && (b->point() == ww.b->point()) &&
         (c->point() == ww.c->point());
}

bool Wall::isIntersected(Corner &p, Vector &ray, Corner &cc) {
  // Cramer's rule (from Wiki)
  // A B C  U   J
  // D E F  V = K
  // G H I  t   L
  double A = a->point().X() - b->point().X(),
         B = a->point().X() - c->point().X(), C = ray.X();
  double D = a->point().Y() - b->point().Y(),
         E = a->point().Y() - c->point().Y(), F = ray.Y();
  double G = a->point().Z() - b->point().Z(),
         H = a->point().Z() - c->point().Z(), I = ray.Z();
  double J = a->point().X() - p.point().X(), K = a->point().Y() - p.point().Y(),
         L = a->point().Z() - p.point().Z();

  // M = A*(EI-HF)+B*(GF-DI)+C*(DH-GE)
  double M = A * (E * I - H * F) + B * (G * F - D * I) + C * (D * H - G * E);

  if (M != 0) {
    // u = [J*(EI-HF)+B*(LF-KI)+C*(KH-LE)]/M
    double u = J * (E * I - H * F) + B * (L * F - K * I) + C * (K * H - L * E);
    u /= M;

    // v = [A*(KI-LF)+J*(GF-DI)+C*(DL-GK)]/M
    double v = A * (K * I - L * F) + J * (G * F - D * I) + C * (D * L - G * K);
    v /= M;

    // t = [A*(EL-HK)+B*(GK-DL)+J*(DH-GE)]/M
    double t = A * (E * L - H * K) + B * (G * K - D * L) + J * (D * H - G * E);
    t /= M;

    cc.setPoint(p.point().X() + t * ray.X(), p.point().Y() + t * ray.Y(),
                p.point().Z() + t * ray.Z());
    if (u >= 0 && u <= 1 &&  // 0 <= u <= 1
        v >= 0 && v <= 1 &&  // 0 <= v <= 1
        u + v <= 1 &&        // u+v <= 1
        t > 0) {             // t > 0
      return true;
    }
    return false;
  }
  return false;
}

int Wall::rayIntersectPlane(Corner &p, Vector &ray, Corner &cc) {
  Vector u(0, 0, 0), v(0, 0, 0), n(0, 0, 0);  // triangle vectors
  Vector w0(0, 0, 0), w(0, 0, 0);             // ray vectors
  float r, A, B;  // params to calc ray-plane intersect

  // get triangle edge vectors and plane normal
  u = Vector(b->point() - a->point());
  v = Vector(c->point() - a->point());
  n = u.cross(v);                              // cross product
  if (n.X() == 0 && n.Y() == 0 && n.Z() == 0)  // triangle is degenerate
    return -2;                                 // do not deal with this case

  w0 = Vector(p.point() - a->point());
  A = -n * w0;
  B = n * ray;
  if (fabs(B) < EPS) {  // ray is  parallel to triangle plane
    if (A == 0)         // ray lies in triangle plane
      return 2;
    else
      return -1;  // ray disjoint from plane
  }

  // get intersect point of ray with triangle plane
  r = A / B;
  if (r < 0.0)  // ray goes away from triangle
    return -3;  // => no intersect
  // for a segment, also test if (r > 1.0) => no intersect

  // intersect point of ray and plane
  cc.setPoint(p.point().X() + r * ray.X(), p.point().Y() + r * ray.Y(),
              p.point().Z() + r * ray.Z());

  // is I inside T?
  float uu, uv, vv, wu, wv, D;
  uu = u * u;
  uv = u * v;
  vv = v * v;
  w = Vector(cc.point() - a->point());

  wu = w * u;
  wv = w * v;
  D = uv * uv - uu * vv;

  // get and test parametric coords
  float s, t;
  s = (uv * wv - vv * wu) / D;
  if (s < 0.0 || s > 1.0)  // I is outside T
    return 0;
  t = (uv * wu - uu * wv) / D;
  if (t < 0.0 || (s + t) > 1.0)  // I is outside T
    return 0;

  return 1;  // I is in T
}
