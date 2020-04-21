/* **************************************
   File: Edge.cpp

   Description:

   HISTORY: July, 2013: Surin Ahn
   Update: July, 2017: Ching-Hsiang (Tom) Hsu

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#include "Edge.h"

Edge::Edge(Corner* s, Corner* d):src(s), dst(d) {
  src->nextEdge.push_back(this);
  dst->preEdge.push_back(this);
  convexEdge = false;

  seg = Segment3d(s->p, d->p);
}

Edge::Edge(Corner s, Corner d):src(&s), dst(&d) {
  src->nextEdge.push_back(this);
  dst->preEdge.push_back(this);
  convexEdge = false;

  seg = Segment3d(s.p, d.p);
}

double Edge::distance(Point3d p, double &t, Point3d &proj) {

  double t_num = Vector(p-src->p)*Vector(dst->p-src->p); // <q-a, b-a>
  double t_denom = Vector(dst->p-src->p).norm(); // |b-a|^2
  t = t_num / t_denom;
  t = t < 0 ? 0 : t;
  t = t > 1 ? 1 : t;

  // parametric form
  proj = dst->p * t + src->p * (1 - t);
  return p.distance(proj);
}

bool Edge::same(Corner *a, Corner *b) {
  return a->same(src) && b->same(dst);
}

bool Edge::isConvex() {
  Vector e = Vector(dst->p-src->p);
  if(rightWall == NULL)    return true;
  Vector normalRightWall = rightWall->tri.normal();
  if(oppositeWall == NULL) return true;
  Vector normalOppositeWall = oppositeWall->tri.normal();
  Vector crossNormal = normalRightWall.cross(normalOppositeWall);
  if(fabs(crossNormal.X()) > 0 || fabs(crossNormal.Y()) > 0 || fabs(crossNormal.Z()) > 0){
    crossNormal.normalize();
    return (e*crossNormal > 0);
  }
  else{
    return true;
  }
}

void Edge::intersectDisc(Point3d c, double radius, Point3d &i1, bool &intersect1, Point3d &i2, bool &intersect2){

  Point3d a = src->p;
  Vector b = Vector(dst->p-src->p);
  b.normalize();

  Vector t(0,0,0);
    t.set((b.X()*(c.X()-a.X()))*(-1+radius)/(b.X()*b.X()),
          (b.Y()*(c.Y()-a.Y()))*(-1+radius)/(b.Y()*b.Y()),
          (b.Z()*(c.Z()-a.Z()))*(-1+radius)/(b.Z()*b.Z()));
    intersect1 = true;
    if(t.X() > 1 || t.X() < 0) intersect1 = false;
    if(t.Y() > 1 || t.Y() < 0) intersect1 = false;
    if(t.Z() > 1 || t.Z() < 0) intersect1 = false;
    i1.set(a.X()+b.X()*t.X(),
           a.Y()+b.Y()*t.Y(),
           a.Z()+b.Z()*t.Z());

    t.set((b.X()*(c.X()-a.X()))*(-1-radius)/(b.X()*b.X()),
          (b.Y()*(c.Y()-a.Y()))*(-1-radius)/(b.Y()*b.Y()),
          (b.Z()*(c.Z()-a.Z()))*(-1-radius)/(b.Z()*b.Z()));
    intersect2 = true;
    if(t.X() > 1 || t.X() < 0) intersect1 = false;
    if(t.Y() > 1 || t.Y() < 0) intersect1 = false;
    if(t.Z() > 1 || t.Z() < 0) intersect1 = false;
    i2.set(a.X()+b.X()*t.X(),
           a.Y()+b.Y()*t.Y(),
           a.Z()+b.Z()*t.Z());
}

bool Edge::intersectEdge(Edge* e, Point3d &rst){
  Point3d p = src->p;
  Vector r(dst->p-src->p);
  Point3d q = e->src->p;
  Vector s(e->dst->p-e->src->p);

  Vector r_s = r.cross(s);
  Vector q_p = q-p;
  q_p = q_p.cross(s);

  Vector s_r = s.cross(r);
  Vector p_q = p-q;
  p_q = p_q.cross(r);

  Vector t(0,0,0), u(0,0,0);

  if(fabs(r_s.X()) > 0 || fabs(r_s.Y()) > 0 || fabs(r_s.Z()) > 0){
    t.set(q_p.X()/r_s.X(),
          q_p.Y()/r_s.Y(),
          q_p.Z()/r_s.Z());

    rst.set(p.X()+t.X()*r.X(),
            p.Y()+t.Y()*r.Y(),
            p.Z()+t.Z()*r.Z());

//    u.x = p_q.x/s_r.x;
//    u.x = p_q.x/s_r.x;
//    u.x = p_q.x/s_r.x;

    return true;
  }
  else{
    return false;
  }
}
