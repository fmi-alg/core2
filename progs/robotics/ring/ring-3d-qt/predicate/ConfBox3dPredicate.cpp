#include "predicate/ConfBox3dPredicate.h"

extern FILE* g_fptr;
extern bool verbose;

bool inBound(double innerBound, double val, double outerBound){
  return (innerBound-1e-8 <= val && val <= outerBound+1e-8);
}

// Find the nearest feature, and check
// b is its parent
Status ConfBox3dPredicate::classification(ConfBox3d* b, Circle3d cir) {

  Point3d closestP(cir.P());

  if(cir.R() > 0){
    double minDist = std::numeric_limits<float>::max();
    for (list<Wall*>::iterator iterW = b->Walls.begin(); iterW != b->Walls.end(); ++iterW) {
      Wall* w = (*iterW);

      Point3d* p_ = w->tri.separation_circle(cir);
      double dist(w->tri.separation_circleL(cir));

      if(p_ != NULL && minDist > dist){
        closestP = *p_;
        minDist = dist;
      }
    }

    for (list<Edge*>::iterator iterE = b->Edges.begin(); iterE != b->Edges.end(); ++iterE) {
      Edge *e = *iterE;

      Point3d* p_ = e->seg.separation_circle(cir);
      double dist(e->seg.separation_circleL(cir));

      if(p_ != NULL && minDist > dist){
        closestP = *p_;
        minDist = dist;
      }
    }

    for (list<Corner*>::iterator iterC = b->Corners.begin(); iterC != b->Corners.end(); ++iterC) {
      Corner* c = (*iterC);

      Point3d* p_ = c->p.separation_circle(cir);
      double dist(c->p.separation_circleL(cir));

      if(p_ != NULL && minDist > dist){
        closestP = *p_;
        minDist = dist;
      }
    }
  }

  // should use Obstacle as a parent class
  // and need one copy to record the min distance and the corresponding feature
  Wall* nearestWall = NULL;
  double minDistW = std::numeric_limits<float>::max();
  for (list<Wall*>::iterator iterW = b->Walls.begin(); iterW != b->Walls.end(); ++iterW) {
    Wall* w = (*iterW);
    double dist(w->tri.distance(closestP));

    if (dist < minDistW) {
      minDistW = dist;
      nearestWall = w;
    }
  }

  Edge* nearestEdge = NULL;
  double minDistE = std::numeric_limits<float>::max();
  for (list<Edge*>::iterator iterE = b->Edges.begin(); iterE != b->Edges.end(); ++iterE) {
    Edge *e = *iterE;
    double dist(e->seg.distance(closestP));

    if (dist < minDistE) {
      minDistE = dist;
      nearestEdge = e;
    }
  }

  Corner* nearestCorner = NULL;
  double minDistC = std::numeric_limits<float>::max();
  for (list<Corner*>::iterator iterC = b->Corners.begin(); iterC != b->Corners.end(); ++iterC) {
    Corner* c = *iterC;
    double dist(c->p.distance(closestP));

    if (dist < minDistC) {
      minDistC = dist;
      nearestCorner = c;
    }
  }

  bool isFree = false;

  //// Box_t
  // if the nearest feature is a wall...
  // check the orientation of m(B) with respect to the wall
  if (nearestWall != NULL && minDistW <= minDistE+1e-8 && minDistW <= minDistC+1e-8) {
    if (nearestWall->sameSide(closestP)) {
      isFree = true;
    }
  }
  else if (nearestEdge != NULL && minDistE <= minDistC+1e-8 && minDistE <= minDistW+1e-8) {
    if(nearestEdge->convexEdge){
      isFree = true;
    }
  }
  else if (nearestCorner != NULL && minDistC <= minDistE+1e-8 && minDistC <= minDistW+1e-8) {
    int convex_cnt = 0, concave_cnt = 0;
    for(unsigned i=0;i<nearestCorner->preEdge.size();++i){
      if(nearestCorner->preEdge[i]->convexEdge) ++convex_cnt;
      else                                      ++concave_cnt;
    }
    if(concave_cnt == 0) { // convex corner
      isFree = true;
    }
    else if(convex_cnt > 0 && concave_cnt > 0) { // mixed corner
      Wall* incidentWall = NULL;
      double dot_v = -2;
      Vector pw = Vector(closestP-nearestCorner->p);
      pw.normalize();
      for(unsigned i=0;i<nearestCorner->incidentWall.size();++i){
        // minimam angle implies that dot product has maximam value
        double v = pw*nearestCorner->incidentWall[i]->tri.normal();
        if (dot_v < v) {
          dot_v = v;
          incidentWall = nearestCorner->incidentWall[i];
        }
      }
      if (incidentWall != NULL) {
        if (incidentWall->sameSide(closestP)) {
          isFree = true;
        }
      }
    }
    // else concave corner
    // isFree remains the same value
  }
  // else, the nearest feature is either an edge or corner, and the box must be free (since all obstacles are convex)
  else isFree = true;

  return isFree ? FREE : STUCK;
}

bool testPolytope(Segment3d seg, Plane3d top, Plane3d bot, double innerDomain, double outerDomain, Point3d mB){
  if(top.apply(seg.startPt())*bot.apply(seg.startPt()) > 0 &&
     top.apply(seg.stopPt())*bot.apply(seg.stopPt()) > 0){
    return false;
  }

  Circle3d cir(mB, innerDomain, top.normal());
  Point3d *seg_p = NULL, *cir_p = NULL;
  seg.separation_circle(cir, seg_p, cir_p);
  if(seg_p != NULL && cir_p != NULL){
    double h = Plane3d(mB, top.normal()).distance(*seg_p);
    double d = seg_p->distance(*cir_p);
    double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
    return inBound(innerDomain, dist, outerDomain);
  }
  return false;
}

bool testPolytope(Triangle3d T, Plane3d top, Plane3d bot, double innerDomain, double outerDomain, Point3d mB){
  deque<Point3d> poly;
  poly.push_back(T.V1()); poly.push_back(T.V2()); poly.push_back(T.V3());
  Plane3d* H[2];
  H[0] = new Plane3d(top);
  H[1] = new Plane3d(bot);
  for(int i=0;i<2;++i){
    // There is an intersection
    if(H[i]->apply(T.V1())*H[i]->apply(T.V2()) < 0 ||
       H[i]->apply(T.V2())*H[i]->apply(T.V3()) < 0 ||
       H[i]->apply(T.V3())*H[i]->apply(T.V1()) < 0){

      GeomObj* tmp = H[i]->intersection(T.toPlane());
      Line3d* l;
      if(tmp != NULL) l = (Line3d*)tmp;

      vector<Point3d> pp;
      if(tmp != NULL){
        poly.push_back(poly[0]);
        for(unsigned j=0;j<poly.size()-1;++j){
          Segment3d seg(poly[j], poly[j+1]);
          GeomObj* intersection = H[i]->intersection(seg.toLine());
          if(intersection != NULL && intersection->dim() == 0 &&
             (seg.contains(*((Point3d*)intersection)) || seg.distance(*((Point3d*)intersection)) < 1e-8)){
            pp.push_back(*((Point3d*)intersection));
          }
        }
        poly.pop_back();
      }

      if(pp.size() == 2){

        deque<Point3d> l1, l2;
        bool breakpoint(false);
        unsigned k = 0;
        for(;k<poly.size();++k){
          if(H[i]->apply(poly[k]) > 0){
            if(!breakpoint){
              l1.push_back(pp[0]);
              l2.push_back(pp[1]);
              l1.push_back(pp[1]);
              l2.push_back(pp[0]);
            }
            breakpoint = true;
          }
          else{
            if(breakpoint) break;
            l1.push_back(poly[k]);
            l2.push_back(poly[k]);
          }
        }
        for(;k<poly.size();++k){
          if(H[i]->apply(poly[k]) <= 0){
            l1.push_back(poly[k]);
            l2.push_back(poly[k]);
          }
        }

        bool ok1(true), ok2(true);
        for(unsigned j=0;j<l1.size()-3;++j){
          Point3d p0(l1[j]);
          Point3d p1(l1[j+1]);
          Point3d p2(l1[j+2]);
          Vector v1(p1-p0);
          Vector v2(p2-p1);
          Vector v3(v1.cross(v2));
          if(v3*T.normal() < 0){
            ok1 = false;
            break;
          }
        }
        for(unsigned j=0;j<l2.size()-3;++j){
          Point3d p0(l2[j]);
          Point3d p1(l2[j+1]);
          Point3d p2(l2[j+2]);
          Vector v1(p1-p0);
          Vector v2(p2-p1);
          Vector v3(v1.cross(v2));
          if(v3*T.normal() < 0){
            ok2 = false;
            break;
          }
        }

        poly.clear();
        if(ok1)
          for(unsigned j=0;j<l1.size();++j)
            poly.push_back(l1[j]);
        else
          for(unsigned j=0;j<l2.size();++j)
            poly.push_back(l2[j]);
      }
      else{
        deque<Point3d> l;
        for(unsigned j=0;j<poly.size();++j){
          if(H[i]->apply(poly[j]) < 0){
            l.push_back(poly[j]);
          }
        }

        poly.clear();
        for(unsigned j=0;j<l.size();++j){
          poly.push_back(l[j]);
        }
      }
    }
  }

  if(poly.size() > 0){
    Circle3d circleIn(mB, innerDomain, top.normal());
    Circle3d circleOut(mB, outerDomain, top.normal());


    bool outsideTop(true), outsideBot(true);
    for(unsigned i=0;i<poly.size();++i){
      if(top.apply(poly[i]) <= 0){
        outsideTop = false;
      }
      if(bot.apply(poly[i]) <= 0){
        outsideBot = false;
      }
    }


    if(!outsideTop && !outsideBot){
      if(poly.size() > 3){
        {
          Triangle3d T(poly[0], poly[1], poly[2]);
          T.normalize();
          double dist = T.separation_circleL(circleIn)+T.separation_circleL(circleOut);
          if(inBound(0, dist, outerDomain-innerDomain))
            return true;
        }
        {
          Triangle3d T(poly[0], poly[2], poly[3]);
          T.normalize();
          double dist = T.separation_circleL(circleIn)+T.separation_circleL(circleOut);
          if(inBound(0, dist, outerDomain-innerDomain))
            return true;
        }
      }
      else if(poly.size() > 2){
        Triangle3d T(poly[0], poly[1], poly[2]);
        T.normalize();
        double dist = T.separation_circleL(circleIn)+T.separation_circleL(circleOut);
        if(inBound(0, dist, outerDomain-innerDomain))
          return true;
      }
      else if(poly.size() > 1){
        Segment3d S(poly[0], poly[1]);
        double dist = S.separation_circleL(circleIn)+S.separation_circleL(circleOut);
        if(inBound(0, dist, outerDomain-innerDomain))
          return true;
      }
      else{
        Point3d P(poly[0]);
        double dist = P.separation_circleL(circleIn)+P.separation_circleL(circleOut);
        if(inBound(0, dist, outerDomain-innerDomain))
          return true;
      }
    }
  }
  return false;
}

void ConfBox3dPredicate::checkCollisionDetectionFeatureSet(ConfBox3d* b) {

  if (b->status != UNKNOWN) return ;

  double innerDomain = (ConfBox3d::r0-b->rB > 0)?(ConfBox3d::r0-b->rB):0;
  double outerDomain = ConfBox3d::r0+b->rB;

  // Translational and ratational box
  if(b->rot_width != -2){
    // Do PI 1 intersection
    // 0) find the theta
    // 1) a top ring and a bot ring
    // 2) double cone
    // 2.1) two half-space
    // 3) a ball with radius r0-rB: innerDomain and do complement
    // 4) a ball with radius r0+rB: outerDomain

    // 0)
    Vector circle_n(Rot3dSide::cube2sphere(b->rot_mB)-Point3d(0,0,0));
    circle_n.normalize();

    // 1)
    double sin = ((b->rot_width/2)*sqrt(2))/3;
    double cos = sqrt(1-(b->rot_width/2)*(b->rot_width/2)*2/9);
    double rcos = ConfBox3d::r0*cos;
    double rsin = ConfBox3d::r0*sin;
    Circle3d topCircle(Point3d(b->mB+circle_n*rsin), rcos, circle_n);
    Circle3d botCircle(Point3d(b->mB-circle_n*rsin), rcos, -circle_n);

    // 2)
    Plane3d top(topCircle.toPlane());
    Plane3d bot(botCircle.toPlane());

    for (list<Corner*>::iterator it = b->Corners.begin(); it != b->Corners.end();) {
      Corner* c = *it;

      Circle3d cir(b->mB, innerDomain, top.normal());
      bool intersected(topCircle.do_intersect(c->p, b->rB) ||
                       botCircle.do_intersect(c->p, b->rB));
      Point3d P(c->p);
      Point3d* p = P.separation_circle(cir);
      if(p != NULL){
        double h = Plane3d(b->mB, top.normal()).distance(P);
        double d = P.distance(*p);
        double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
        intersected |= inBound(innerDomain, dist, outerDomain);
      }

      if(intersected){
        b->status = MIXED;
        ++it;
      }
      else{
        it = b->Corners.erase(it);
      }
    }

    for (list<Edge*>::iterator it = b->Edges.begin(); it != b->Edges.end();) {
      Edge* e = *it;

      Circle3d cir(b->mB, innerDomain, top.normal());
      bool intersected(false);

      if(!intersected){
        intersected |= topCircle.do_intersect(e->seg.startPt(), b->rB) ||
                       botCircle.do_intersect(e->seg.startPt(), b->rB);
        Point3d P(e->seg.startPt());
        Point3d* p = P.separation_circle(cir);
        if(p != NULL){
          double h = Plane3d(b->mB, top.normal()).distance(P);
          double d = P.distance(*p);
          double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
          intersected |= inBound(innerDomain, dist, outerDomain);
        }
      }
      if(!intersected){
        intersected |= topCircle.do_intersect(e->seg.stopPt(), b->rB) ||
                       botCircle.do_intersect(e->seg.stopPt(), b->rB);
        Point3d P(e->seg.startPt());
        Point3d* p = P.separation_circle(cir);
        if(p != NULL){
          double h = Plane3d(b->mB, top.normal()).distance(P);
          double d = P.distance(*p);
          double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
          intersected |= inBound(innerDomain, dist, outerDomain);
        }
      }

      if(!intersected){
        // annulus test
        intersected |= topCircle.do_intersect(e->seg, b->rB) ||
                       botCircle.do_intersect(e->seg, b->rB) ||
                       testPolytope(e->seg, top, bot, innerDomain, outerDomain, b->mB);
      }

      if(intersected){
        b->status = MIXED;
        ++it;
      }
      else{
        it = b->Edges.erase(it);
      }
    }

    for (list<Wall*>::iterator it = b->Walls.begin(); it != b->Walls.end();) {
      Wall* w = *it;

      Circle3d cir(b->mB, innerDomain, top.normal());
      bool intersected(false);

      if(!intersected){
        intersected |= topCircle.do_intersect(w->a->p, b->rB) ||
                       botCircle.do_intersect(w->a->p, b->rB);
        Point3d P(w->a->p);
        Point3d* p = P.separation_circle(cir);
        if(p != NULL){
          double h = Plane3d(b->mB, top.normal()).distance(P);
          double d = P.distance(*p);
          double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
          intersected |= inBound(innerDomain, dist, outerDomain);
        }
      }
      if(!intersected){
        intersected |= topCircle.do_intersect(w->b->p, b->rB) ||
                       botCircle.do_intersect(w->b->p, b->rB);
        Point3d P(w->b->p);
        Point3d* p = P.separation_circle(cir);
        if(p != NULL){
          double h = Plane3d(b->mB, top.normal()).distance(P);
          double d = P.distance(*p);
          double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
          intersected |= inBound(innerDomain, dist, outerDomain);
        }
      }
      if(!intersected){
        intersected |= topCircle.do_intersect(w->c->p, b->rB) ||
                       botCircle.do_intersect(w->c->p, b->rB);
        Point3d P(w->c->p);
        Point3d* p = P.separation_circle(cir);
        if(p != NULL){
          double h = Plane3d(b->mB, top.normal()).distance(P);
          double d = P.distance(*p);
          double dist = sqrt((innerDomain+sqrt(d*d-h*h))*(innerDomain+sqrt(d*d-h*h))+h*h);
          intersected |= inBound(innerDomain, dist, outerDomain);
        }
      }

      if(!intersected){
        Segment3d ab(w->a->p, w->b->p);
        intersected |= topCircle.do_intersect(ab, b->rB) ||
                       botCircle.do_intersect(ab, b->rB)||
                       testPolytope(ab, top, bot, innerDomain, outerDomain, b->mB);
        Segment3d bc(w->b->p, w->c->p);
        intersected |= topCircle.do_intersect(bc, b->rB) ||
                       botCircle.do_intersect(bc, b->rB) ||
                       testPolytope(bc, top, bot, innerDomain, outerDomain, b->mB);
        Segment3d ca(w->c->p, w->a->p);
        intersected |= topCircle.do_intersect(ca, b->rB) ||
                       botCircle.do_intersect(ca, b->rB) ||
                       testPolytope(ca, top, bot, innerDomain, outerDomain, b->mB);
      }

      if(!intersected){
        intersected |= topCircle.do_intersect(w->tri, b->rB) ||
                       botCircle.do_intersect(w->tri, b->rB);
      }

      if(!intersected){
        intersected = testPolytope(w->tri, top, bot, innerDomain, outerDomain, b->mB);
      }

      if(intersected){
        b->status = MIXED;
        ++it;
      }
      else{
        it = b->Walls.erase(it);
      }
    }
  }
  // Translational box only
  else{
    for (list<Corner*>::iterator it = b->Corners.begin(); it != b->Corners.end();) {
      Corner* c = *it;
      double distCorner = c->p.distance(b->mB);

      if(distCorner <= outerDomain) {
        b->status = MIXED;
        ++it;
      }
      else{
        it = b->Corners.erase(it);
      }
    }

    for (list<Edge*>::iterator it = b->Edges.begin(); it != b->Edges.end();) {
      Edge* e = *it;
      double distEdge = e->seg.distance(b->mB);

      if(distEdge <= outerDomain){
        b->status = MIXED;
        ++it;
      }
      else{
        it = b->Edges.erase(it);
      }
    }

    for (list<Wall*>::iterator it = b->Walls.begin(); it != b->Walls.end();) {
      Wall* w = *it;
      double distWall = w->tri.distance(b->mB);

      if(distWall <= outerDomain){
        b->status = MIXED;
        ++it;
      }
      else{
        it = b->Walls.erase(it);
      }
    }
  }

  if (b->Corners.empty() && b->Edges.empty() && b->Walls.empty()) {
    if (!b->parent) {
      b->status = FREE;
    }
    else {
      b->status = classification(b->parent, b->ring);
    }
  }
}


// Sep(mB, f) <= 2rB + clearance(mB)
void ConfBox3dPredicate::checkVoronoiFeatureSet(ConfBox3d *b){
  double sep = 2*b->rB+findCleanrance(b, b->ring);

  if(b->vorCorners.size()){
    for(list<Corner*>::iterator it = b->vorCorners.begin(); it != b->vorCorners.end();){
      Corner* c = *it;
      double dist = c->p.distance(b->mB);

      if(dist > sep){
        it = b->vorCorners.erase(it);
      }
      else{
        ++it;
      }
    }
  }

  if(b->vorEdges.size()){
    for(list<Edge*>::iterator it = b->vorEdges.begin(); it != b->vorEdges.end();){
      Edge* e = *it;
      double dist = e->seg.distance(b->mB);

      if(dist > sep){
        it = b->vorEdges.erase(it);
      }
      else{
        ++it;
      }
    }
  }

  if(b->vorWalls.size()){
    for(list<Wall*>::iterator it = b->vorWalls.begin(); it != b->vorWalls.end();){
      Wall* w = *it;
      double dist = w->tri.distance(b->mB);

      if(dist > sep){
        it = b->vorWalls.erase(it);
      }
      else{
        ++it;
      }
    }
  }
} // checkVoronoiFeatureSet

//find the nearest feature of ring, and return the distance
double ConfBox3dPredicate::findCleanrance(ConfBox3d *b, Circle3d cir){

  double mindistW = std::numeric_limits<double>::max();
  Wall* nearestWall = NULL;
  for(list<Wall*>::iterator iterW = b->Walls.begin(); iterW != b->Walls.end(); ++iterW){
    Wall* w = *iterW;
    double dist = w->tri.separation_circleL(cir);
    if(dist < mindistW){
      mindistW = dist;
      nearestWall = *iterW;
    }
  }

  double mindistE = std::numeric_limits<double>::max();
  Edge* nearestEdge = NULL;
  for(list<Edge*>::iterator iterE = b->Edges.begin(); iterE != b->Edges.end(); ++iterE){
    Edge* e = *iterE;
    double dist = e->seg.separation_circleL(cir);
    if(dist < mindistE){
      mindistE = dist;
      nearestEdge = *iterE;
    }
  }

  double mindistC = std::numeric_limits<double>::max(); // mindistC may not exist, so init to a bigger number
  Corner* nearestCorner = NULL;
  for(list<Corner*>::iterator iterC = b->Corners.begin(); iterC != b->Corners.end(); ++iterC){
    Corner* c = *iterC;
    double dist = c->p.separation_circleL(cir);
    if (dist < mindistC){
      mindistC = dist;
      nearestCorner = *iterC;
    }
  }

  double clearance = mindistW;
  if(clearance > mindistE) clearance = mindistE;
  if(clearance > mindistC) clearance = mindistC;
  return clearance;
} // findCleanrance

//find the nearest voronoi feature of mB and record the nearest feature
void ConfBox3dPredicate::findCleanrance2(ConfBox3d *b, Circle3d cir){

  double mindistW = std::numeric_limits<double>::max();
  b->nearestW = NULL;
  for(list<Wall*>::iterator iterW = b->vorWalls.begin(); iterW != b->vorWalls.end(); ++iterW){
    Wall* w = *iterW;
    double dist = w->tri.separation_circleL(cir);
    if(dist < mindistW){
      mindistW = dist;
      b->nearestW = w;
    }
  }

  double mindistE = std::numeric_limits<double>::max();
  b->nearestE = NULL;
  for(list<Edge*>::iterator iterE = b->vorEdges.begin(); iterE != b->vorEdges.end(); ++iterE){
    Edge* e = *iterE;
    double dist = e->seg.separation_circleL(cir);
    if(dist < mindistE){
      mindistE = dist;
      b->nearestE = e;
    }
  }

  double mindistC = std::numeric_limits<double>::max(); // mindistC may not exist, so init to a bigger number
  b->nearestC = NULL;
  for(list<Corner*>::iterator iterC = b->vorCorners.begin(); iterC != b->vorCorners.end(); ++iterC){
    Corner* c = *iterC;
    double dist = c->p.separation_circleL(cir);
    if (dist < mindistC){
      mindistC = dist;
      b->nearestC = c;
    }
  }
} // findCleanrance2
