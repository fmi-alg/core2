/*****************************************************************
 * File: circle3d.cpp
 * Synopsis:
 *      Basic 3-dimensional geometry
 * Author: Ching-Hsiang Hsu (chhsu@cs.nyu.edu), 2017.
 *
 *****************************************************************
 * CORE Library Version 1.4 (July 2001)
 *       Chee Yap <yap@cs.nyu.edu>
 *       Chen Li <chenli@cs.nyu.edu>
 *       Zilin Du <zilin@cs.nyu.edu>
 *
 * Copyright (c) 1995, 1996, 1998, 1999, 2000, 2001 Exact Computation Project
 *
 * WWW URL: http://cs.nyu.edu/exact/
 * Email: exact@cs.nyu.edu
 *
 * $Id: triangle3d.cpp,v 1.2 2010/05/19 04:48:20 exact Exp $
 *****************************************************************/

#include <CORE/geom3d/circle3d.h>
#include <CORE/geom3d/triangle3d.h>

/************************************************************
 *   constructors
 ************************************************************/
 
 
/************************************************************
 *   predicates
 ************************************************************/

bool Circle3d::do_intersect(Point3d pp, double rB){
  Point3d p_ = toPlane().projection(pp);
  double h = p_.distance(pp);
  double d = abs(p_.distance(p)-r);
  if(h*h+d*d <= rB*rB)
    return true;
  return false;
}

bool Circle3d::do_intersect(Segment3d seg, double rB){

  Point3d p0_ = toPlane().projection(seg.startPt());
  Point3d p1_ = toPlane().projection(seg.stopPt());

  Segment3d seg_(p0_, p1_);
  Point3d closestP_(seg_.nearPt(p));
  Line3d l(p, Vector(closestP_-p));

  Point3d pp1, pp2;
  intersection(l, pp1, pp2);
  double d1 = pp1.distance(p);
  double d2 = pp2.distance(p);
  if(d1 < d2){
    return do_intersect(seg.nearPt(pp1), rB);
  }
  else{
    return do_intersect(seg.nearPt(pp2), rB);
  }

//  Point3d p1 = seg.startPt();
//  Point3d p2 = seg.stopPt();
//  Point3d p1_ = toPlane().projection(p1);
//  Point3d p2_ = toPlane().projection(p2);
//  Vector p2p1_(p2_-p1_); // p2'-p1' vector
//  p2p1_.normalize();
//  double t = Vector(p-p1_)*p2p1_/(p2p1_*p2p1_);
//  if(t < 0 || t > 1) return false;

//  Vector p2p1(p2-p1); // p2-p1 vector
//  p2p1.normalize();
//  Point3d pp = seg.startPt()+t*p2p1;
//  return do_intersect(pp, rB);
}

bool Circle3d::do_intersect(Triangle3d T, double rB){

  Point3d near(T.nearPt(p));
  Point3d onCircle;
  Point3d onTriangle;

  Point3d pro(toPlane().projection(near));

  if(T.toPlane().isParallel(toPlane())){
    Point3d pp1, pp2;
    Line3d l(p, Vector(pro-p));
    this->intersection(l, pp1, pp2);

    if(pp1.distance(pro) < pp2.distance(pro))
      onCircle = pp1;
    else
      onCircle = pp2;
  }
  else{
    Point3d pp[4];
    double dd[4];
    Line3d l1(*(Line3d *)(T.toPlane().intersection(toPlane())));
    Line3d l2(p, Vector(pro-p));
    this->intersection(l1, pro, pp[0], pp[1]);
    this->intersection(l2, pp[2], pp[3]);

    Point3d closestP(pp[0]);
    double minD(T.distance(pp[0]));
    for(int i=1;i<4;++i){
      dd[i] = T.distance(pp[i]);
      if(minD > dd[i]){
        minD = dd[i];
        closestP = pp[i];
      }
    }

    onCircle = closestP;
  }

  onTriangle = T.nearPt(onCircle);

  return onTriangle.distance(onCircle)-rB <= 1e-8;
}

/************************************************************
 *  Intersection
 ************************************************************/

void Circle3d::intersection(Line3d l, Point3d pro, Point3d &p1, Point3d &p2){
  Vector v(l.direction());
  double a = v*v;
  double b = 2*(v*(pro-p));
  double c = Vector(pro-p)*Vector(pro-p)-r*r;

  double bb4ac = b*b-4*a*c;
  double t1 = (-b+sqrt(bb4ac))/(2*a);
  double t2 = (-b-sqrt(bb4ac))/(2*a);

  p1 = pro+t1*v;
  p2 = pro+t2*v;
}

void Circle3d::intersection(Line3d l, Point3d &p1, Point3d &p2){
  Vector v(l.direction());
  double c = r*r/(v*v);

  double t1 = sqrt(c);
  double t2 = -sqrt(c);

  p1 = p+t1*v;
  p2 = p+t2*v;
}


/***************************************************************
 *   I/O 
 ***************************************************************/

std::ostream &operator<<(std::ostream &o, const Circle3d& C) {
  return o << "Circle3d:\n" << "\t" << C.p << std::endl 
                            << "\t" << C.n;
}

std::istream& operator>>(std::istream& in, Circle3d& C) {
  Point3d pp;
  double rr;
  Vector nn;
  std::cout << "\nInput point(-,-,-):";
  in >> pp;
  std::cout << "\nInput radius:";
  in >> rr;
  std::cout << "\nInput normal vector(-,-,-):";
  in >> nn;
  C = Circle3d(pp, rr, nn);
  return in;
}

