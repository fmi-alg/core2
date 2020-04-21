/* **************************************
   File: Edge.cpp

   Description:

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#include "Edge.h"
#include "Corner.h"
#include "Vector.h"

Edge::Edge(Corner* s, Corner* d):src(s), dst(d) {
  src->nextEdge = this;
  dst->preEdge = this;
}

Edge::Edge(Corner s, Corner d):src(&s), dst(&d) {
  src->nextEdge = this;
  dst->preEdge = this;
}

double Edge::distance(double x, double y, double z) {
  Vector src(*(this->src));
  Vector dst(*(this->dst));
  Vector point(x, y, z);

  double t_num = (point - src) * (dst - src); // <q-a, b-a>
  double t_denom = (dst - src) * (dst - src); // |b-a|^2
  double t = t_num / t_denom;
  t = t < 0 ? 0 : t;
  t = t > 1 ? 1 : t;
  Vector proj = dst * t + src * (1 - t);
  return point.distance(proj);
}
