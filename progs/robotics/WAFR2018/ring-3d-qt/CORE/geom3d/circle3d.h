/*****************************************************************
 * File: circle3d.h
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
 * $Id: circle3d.h,v 1.1 2006/04/03 20:05:18 exact Exp $
 *****************************************************************/


#ifndef _CIRCLE3D_H_
#define _CIRCLE3D_H_

#include <CORE/geom3d/point3d.h>
#include <CORE/geom3d/line3d.h>
#include <CORE/geom3d/segment3d.h>
#include <CORE/geom3d/plane3d.h>

class Triangle3d;

class Circle3d : public GeomObj {

private:
  
  Point3d p;
  double r;
  Vector n;

public:

  /************************************************************
   *   constructors
   ************************************************************/

  Circle3d(const Point3d& pp, double rr, const Vector& nn): p(pp), r(rr), n(nn) {}

  Circle3d(const Circle3d& C): p(C.p), r(C.r), n(C.n) {}

  Circle3d(): p(ORIGIN_3D), r(0), n(0,0,0) {}
  //trivial triangle

  virtual ~Circle3d() {}
  /************************************************************
   *   member functions
   ************************************************************/

   // view a circle as a surface
  virtual int dim() const { return 2; }
   
  Point3d P() const { return p; }
  double R() const { return r; }
  Vector normal() const { return n; }
  void normalize() { n.set(n.X()/n.norm(), n.Y()/n.norm(), n.Z()/n.norm()); }

  // return normal of the plane containing this triangle

  Plane3d toPlane() const { return Plane3d(p, n); }
 
  /************************************************************
   *   predicates
   ************************************************************/

  bool do_intersect(Point3d p, double rB); // minkwoski sum a ball rB
  bool do_intersect(Segment3d seg, double rB);
  bool do_intersect(Triangle3d T, double rB);
  
  /************************************************************
   *  Intersection
   ************************************************************/

  void intersection(Line3d l, Point3d pro, Point3d &p1, Point3d &p2);
  void intersection(Line3d l, Point3d &p1, Point3d &p2);

  /************************************************************
   *   I/O 
   ************************************************************/

  friend std::istream& operator>>(std::istream& in, Circle3d& C);
  friend std::ostream& operator<<(std::ostream & out, const Circle3d& C);
  
}; //class Triangle3d

#endif
