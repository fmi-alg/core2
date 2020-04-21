/* **************************************
   File: Wall.cpp

   Description:

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
 ************************************** */

#include "Wall.h"

using namespace std;

extern FILE* g_fptr;

Wall::Wall(){
}

Wall::Wall(Corner* aa, Corner* bb, Corner* cc): a(aa), b(bb), c(cc) {
  aa->incidentWall.push_back(this);
  bb->incidentWall.push_back(this);
  cc->incidentWall.push_back(this);

  tri = Triangle3d(aa->p, bb->p, cc->p);
  tri.normalize();
}

Wall::Wall(Corner aa, Corner bb, Corner cc) {
  a = new Corner(aa);
  b = new Corner(bb);
  c = new Corner(cc);
  a->incidentWall.push_back(this);
  b->incidentWall.push_back(this);
  c->incidentWall.push_back(this);

  tri = Triangle3d(aa.p, bb.p, cc.p);
  tri.normalize();
}

bool Wall::insideWall(Point3d p){
  return (tri.distance(p) <= 1e-8);
}

bool Wall::sameSide(Point3d q) {

  Vector qa = Vector(q-a->p);
  return (qa*tri.normal() >= 0);

  double a0[3] = {a->p.X(), a->p.Y(), a->p.Z()};
  double b0[3] = {b->p.X(), b->p.Y(), b->p.Z()};
  double c0[3] = {c->p.X(), c->p.Y(), c->p.Z()};

  // Subtract a from b, c, q so we get a 3x3 matrix
  // (p stands for "prime")

  double bp[3] = {b0[0]-a0[0], b0[1]-a0[1], b0[2]-a0[2]};
  double cp[3] = {c0[0]-a0[0], c0[0]-a0[0], c0[2]-a0[2]};
  double qp[3] = {q.X()-a0[0], q.Y()-a0[1], q.Z()-a0[2]};

  // Calculate the determinant of the 3x3 matrix whose columns are given by
  // b', c', and q'
  double det =
    (bp[0]*cp[1]*qp[2]) + (cp[0]*qp[1]*bp[2]) + (qp[0]*bp[1]*cp[2]) -
    (qp[0]*cp[1]*bp[2]) - (cp[0]*bp[1]*qp[2]) - (bp[0]*qp[1]*cp[2]);
  return !(det < 0);
}

bool Wall::same(Wall *ww){
  return (a->p == ww->a->p) &&
         (b->p == ww->b->p) &&
         (c->p == ww->c->p);
}

bool Wall::isIntersected(Corner *p, Vector ray, Corner *cc){
  // Cramer's rule (from Wiki)
  // A B C  U   J
  // D E F  V = K
  // G H I  t   L
  double A = a->p.X()-b->p.X(), B = a->p.X()-c->p.X(), C = ray.X();
  double D = a->p.Y()-b->p.Y(), E = a->p.Y()-c->p.Y(), F = ray.Y();
  double G = a->p.Z()-b->p.Z(), H = a->p.Z()-c->p.Z(), I = ray.Z();
  double J = a->p.X()-p->p.X(), K = a->p.Y()-p->p.Y(), L = a->p.Z()-p->p.Z();

  // M = A*(EI-HF)+B*(GF-DI)+C*(DH-GE)
  double M = A*(E*I-H*F)+B*(G*F-D*I)+C*(D*H-G*E);

  if(M != 0){
      // u = [J*(EI-HF)+B*(LF-KI)+C*(KH-LE)]/M
      double u = J*(E*I-H*F)+B*(L*F-K*I)+C*(K*H-L*E);
      u /= M;

      // v = [A*(KI-LF)+J*(GF-DI)+C*(DL-GK)]/M
      double v = A*(K*I-L*F)+J*(G*F-D*I)+C*(D*L-G*K);
      v /= M;

      // t = [A*(EL-HK)+B*(GK-DL)+J*(DH-GE)]/M
      double t = A*(E*L-H*K)+B*(G*K-D*L)+J*(D*H-G*E);
      t /= M;

      cc->p.set(p->p.X() + t*ray.X(),
                p->p.Y() + t*ray.Y(),
                p->p.Z() + t*ray.Z());
      if(u >= 0 && u <= 1 &&  // 0 <= u <= 1
         v >= 0 && v <= 1 &&  // 0 <= v <= 1
         u+v <= 1 &&          // u+v <= 1
         t > 0){              // t > 0
        return true;
      }
      return false;
  }
  return false;
}

int Wall::intersect3D_RayPlane(Corner *p, Vector ray, Corner *cc){
    Vector    u(0,0,0), v(0,0,0), n(0,0,0);              // triangle vectors
    Vector    w0(0,0,0), w(0,0,0);                // ray vectors
    float     r, A, B;              // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = Vector(b->p-a->p);
    v = Vector(c->p-a->p);
    n = u.cross(v);              // cross product
    if (n.X() == 0 && n.Y() == 0 && n.Z() == 0) // triangle is degenerate
        return -2;                              // do not deal with this case

    w0 = Vector(p->p-a->p);
    A = -n*w0;
    B = n*ray;
    if (fabs(B) < 1e-8) {     // ray is  parallel to triangle plane
        if (A == 0)                  // ray lies in triangle plane
            return 2;
        else return -1;              // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = A / B;
    if (r < 0.0)                    // ray goes away from triangle
        return -3;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    // intersect point of ray and plane
    cc->p.set(p->p.X() + r*ray.X(),
              p->p.Y() + r*ray.Y(),
              p->p.Z() + r*ray.Z());

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u*u;
    uv = u*v;
    vv = v*v;
    w = Vector(cc->p-a->p);

    wu = w*u;
    wv = w*v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return 0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)   // I is outside T
        return 0;

    return 1;                       // I is in T
}
