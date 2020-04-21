/*****************************************************************
 * File: triangle3d.cc
 * Synopsis:
 *      Basic 3-dimensional geometry
 * Author: Shubin Zhao (shubinz@cs.nyu.edu), 2001.
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

#include <CORE/geom3d/triangle3d.h>

/************************************************************
 *   constructors
 ************************************************************/

Triangle3d::Triangle3d(const Point3d& v1, const Point3d& v2, const Point3d& v3)
    : p0(v1), p1(v2), p2(v3) {
  n = (p1 - p0).cross(p2 - p0);
}
// given three vertices

Triangle3d::Triangle3d(const Triangle3d& T)
    : p0(T.V1()), p1(T.V2()), p2(T.V3()) {
  n = (p1 - p0).cross(p2 - p0);
}
// given a triangle

/************************************************************
 *   predicates
 ************************************************************/

bool Triangle3d::contains(const Point3d& p) const {
  // take the end of normal vector as viewpoint
  Point3d viewpoint(normal());
  int s1, s2, s3;
  s1 = orientation3d(viewpoint, p, p0, p1);
  s2 = orientation3d(viewpoint, p, p1, p2);
  s3 = orientation3d(viewpoint, p, p2, p0);

  if (s1 == s2 && s2 == s3)
    // p is inside
    return true;
  else if (s1 * s2 * s3 == 0)
    // p is on edge
    return true;
  else
    return false;
}

bool Triangle3d::isOnEdge(const Point3d& p) const {
  return Segment3d(p0, p1).contains(p) || Segment3d(p1, p2).contains(p) ||
         Segment3d(p0, p2).contains(p);
}

bool Triangle3d::inside(const Point3d& p) const {
  assert(isCoplanar(p));

  // use normal vector as viewpoint
  Point3d viewpoint(normal());
  int s1, s2, s3;
  s1 = orientation3d(viewpoint, p, p0, p1);
  s2 = orientation3d(viewpoint, p, p1, p2);
  s3 = orientation3d(viewpoint, p, p2, p0);

  if (s1 == s2 && s2 == s3)
    // is inside
    return true;
  else
    return false;
}

// distance from a point to triangle
double Triangle3d::distance(const Point3d& point) const {
  //  Point3d projection_point = this->toPlane().projection(point);
  //  double d0 = dotProduct(p0 - projection_point, p1 - projection_point);
  //  double d1 = dotProduct(p1 - projection_point, p2 - projection_point);
  //  double d2 = dotProduct(p2 - projection_point, p0 - projection_point);
  //  if (d0 < 0) {
  //    return Segment3d(p0, p1).distance(point);
  //  } else if (d1 < 0) {
  //    return Segment3d(p1, p2).distance(point);
  //  } else if (d2 < 0) {
  //    return Segment3d(p2, p0).distance(point);
  //  } else {
  //    return this->toPlane().distance(point);
  //  }
  Vector vB = Vector(p0.X(), p0.Y(), p0.Z());        // B = TRI(1,:);
  Vector vE0 = Vector(p1.X(), p1.Y(), p1.Z()) - vB;  // E0 = TRI(2,:) - B
  Vector vE1 = Vector(p2.X(), p2.Y(), p2.Z()) - vB;  // E1 = TRI(3,:) - B
  Vector vD = vB - Vector(point.X(), point.Y(), point.Z());  // D = B - P;

  double a = vE0 * vE0;
  double b = vE0 * vE1;
  double c = vE1 * vE1;
  double d = vE0 * vD;
  double e = vE1 * vD;
  double f = vD * vD;

  double det = a * c - b * b;
  double s = b * e - c * d;
  double t = b * d - a * e;

  double sqr_distance;
  double numer;
  double denom;
  double tmp0;
  double tmp1;

  if ((s + t) <= det) {
    if (s < 0) {
      if (t < 0) {
        // region4
        if (d < 0) {
          t = 0;
          if (-d >= a) {
            s = 1;
            sqr_distance = a + 2 * d + f;
          } else {
            s = -d / a;
            sqr_distance = d * s + f;
          }
        } else {
          s = 0;
          if (e >= 0) {
            t = 0;
            sqr_distance = f;
          } else {
            if (-e >= c) {
              t = 1;
              sqr_distance = c + 2 * e + f;
            } else {
              t = -e / c;
              sqr_distance = e * t + f;
            }
          }
        }  // end of region 4
      } else {
        // region 3
        s = 0;
        if (e >= 0) {
          t = 0;
          sqr_distance = f;
        } else {
          if (-e >= c) {
            t = 1;
            sqr_distance = c + 2 * e + f;
          } else {
            t = -e / c;
            sqr_distance = e * t + f;
          }
        }
      }  // end of region 3
    } else {
      if (t < 0) {
        // region 5
        t = 0;
        if (d >= 0) {
          s = 0;
          sqr_distance = f;
        } else {
          if (-d >= a) {
            s = 1;
            sqr_distance = a + 2 * d + f;
          } else {
            s = -d / a;
            sqr_distance = d * s + f;
          }
        }
      } else {
        // region 0
        double invDet = 1 / det;
        s = s * invDet;
        t = t * invDet;
        sqr_distance =
            s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
      }
    }
  } else {
    if (s < 0) {
      // region 2
      tmp0 = b + d;
      tmp1 = c + e;
      if (tmp1 > tmp0) {
        numer = tmp1 - tmp0;
        denom = a - 2 * b + c;
        if (numer >= denom) {
          s = 1;
          t = 0;
          sqr_distance = a + 2 * d + f;
        } else {
          s = numer / denom;
          t = 1 - s;
          sqr_distance =
              s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
        }
      } else {
        s = 0;
        if (tmp1 <= 0) {
          t = 1;
          sqr_distance = c + 2 * e + f;
        } else {
          if (e >= 0) {
            t = 0;
            sqr_distance = f;
          } else {
            t = -e / c;
            sqr_distance = e * t + f;
          }
        }
      }  // end of region 2
    } else {
      if (t < 0) {
        // region6
        tmp0 = b + e;
        tmp1 = a + d;
        if (tmp1 > tmp0) {
          numer = tmp1 - tmp0;
          denom = a - 2 * b + c;
          if (numer >= denom) {
            t = 1;
            s = 0;
            sqr_distance = c + 2 * e + f;
          } else {
            t = numer / denom;
            s = 1 - t;
            sqr_distance =
                s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
          }
        } else {
          t = 0;
          if (tmp1 <= 0) {
            s = 1;
            sqr_distance = a + 2 * d + f;
          } else {
            if (d >= 0) {
              s = 0;
              sqr_distance = f;
            } else {
              s = -d / a;
              sqr_distance = d * s + f;
            }
          }
        }
        // end region 6
      } else {
        // region 1
        numer = c + e - b - d;
        if (numer <= 0) {
          s = 0;
          t = 1;
          sqr_distance = c + 2 * e + f;
        } else {
          denom = a - 2 * b + c;
          if (numer >= denom) {
            s = 1;
            t = 0;
            sqr_distance = a + 2 * d + f;
          } else {
            s = numer / denom;
            t = 1 - s;
            sqr_distance =
                s * (a * s + b * t + 2 * d) + t * (b * s + c * t + 2 * e) + f;
          }
        }  // end of region 1
      }
    }
  }

  // account for numerical round-off error
  if (sqr_distance < 0) {
    sqr_distance = 0;
  }

  return sqrt(sqr_distance);
}

Point3d Triangle3d::nearPoint(const Point3d& p) const {
  Point3d proj(this->toPlane().projection(p));
  if (this->distance(proj) < 1e-8) {
    return proj;
  } else {
    Point3d seg_p[3];
    double seg_d[3];
    Segment3d ab(p0, p1);
    seg_p[0] = ab.nearPoint(p);
    Segment3d bc(p1, p2);
    seg_p[1] = bc.nearPoint(p);
    Segment3d ca(p2, p0);
    seg_p[2] = ca.nearPoint(p);

    double minD(p.distance(seg_p[0]));
    Point3d closest_point(seg_p[0]);
    for (int i = 1; i < 3; ++i) {
      seg_d[i] = p.distance(seg_p[i]);
      if (minD > seg_d[i]) {
        minD = seg_d[i];
        closest_point = seg_p[i];
      }
    }

    return closest_point;
  }
}

Polygon3d* Triangle3d::toPolygon() const {
  Polygon3d* plg = new Polygon3d();
  plg->addPoint(p0);
  plg->addPoint(p1);
  plg->addPoint(p2);
  return plg;
}

/************************************************************
 *   Intersections
 ************************************************************/

bool Triangle3d::do_intersect(const Segment3d& s) const {
  if (isCoplanar(s)) {
    if (contains(s.startPt()) || contains(s.stopPt()))
      return true;  // segment might lies inside the triangle

    if (s.intersects(Segment3d(p0, p1))) return true;  // stop
    if (s.intersects(Segment3d(p1, p2))) return true;  // stop
    if (s.intersects(Segment3d(p0, p2))) return true;  // stop

    return false;
  } else {
    if (toPlane().intersects(s) == -1) return false;

    // make sure intersects inside this triangle
    Point3d pStart = s.startPt();
    Point3d pStop = s.stopPt();
    int s1 = orientation3d(pStart, pStop, p0, p1);
    int s2 = orientation3d(pStart, pStop, p1, p2);
    int s3 = orientation3d(pStart, pStop, p2, p0);

    if (s1 * s2 * s3 == 0) return true;
    if (s1 == s2 && s2 == s3)
      // intersects inside
      return true;
    else
      return false;
  }
}

bool Triangle3d::do_intersect(const Line3d& l) const {
  if (isCoplanar(l)) {
    if (Segment3d(p0, p1).intersects(l)) return true;  // stop
    if (Segment3d(p1, p2).intersects(l)) return true;  // stop
    if (Segment3d(p0, p2).intersects(l)) return true;  // stop

    return false;
  } else {
    Point3d pStart = l.startPt();
    Point3d pStop = l.stopPt();
    int s1 = orientation3d(pStart, pStop, p0, p1);
    int s2 = orientation3d(pStart, pStop, p1, p2);
    int s3 = orientation3d(pStart, pStop, p2, p0);

    if (s1 * s2 * s3 == 0) return true;
    if (s1 == s2 && s2 == s3)
      // intersects inside
      return true;
    else
      return false;
  }
}

bool Triangle3d::do_intersect(const Plane3d& pl) const {
  // compute the signed distances for vertices
  double dist1 = pl.apply(p0);
  double dist2 = pl.apply(p1);
  double dist3 = pl.apply(p2);

  if (dist1 * dist2 <= 0 || dist2 * dist3 <= 0 || dist1 * dist3 <= 0)
    return true;
  // different signs
  else
    return false;
}

bool Triangle3d::do_intersect(const Triangle3d& T) const {
  // at least one edge must intersect the other triangle
  if (T.intersects(Segment3d(p0, p1))) return true;
  if (T.intersects(Segment3d(p1, p2))) return true;
  if (T.intersects(Segment3d(p0, p2))) return true;

  return false;
}

int Triangle3d::intersects(const Segment3d& s) const {
  GeomObj* interObj = intersection(s);
  return (interObj == NULL) ? -1 : interObj->dim();
}

int Triangle3d::intersects(const Line3d& l) const {
  GeomObj* interObj = intersection(l);
  return (interObj == NULL) ? -1 : interObj->dim();
}

int Triangle3d::intersects(const Plane3d& pl) const {
  GeomObj* interObj = intersection(pl);
  return (interObj == NULL) ? -1 : interObj->dim();
}

int Triangle3d::intersects(const Triangle3d& T) const {
  GeomObj* interObj = intersection(T);
  return (interObj == NULL) ? -1 : interObj->dim();
}

GeomObj* Triangle3d::intersection(const Segment3d& s) const {
  if (isCoplanar(s))
    return coplanar_intersection(s);
  else {
    GeomObj* interObj = toPlane().intersection(s);
    if (interObj == NULL)
      return NULL;
    else
      return contains(*(Point3d*)interObj) ? interObj : NULL;
  }
}

GeomObj* Triangle3d::intersection(const Line3d& l) const {
  if (isCoplanar(l)) {
    return coplanar_intersection(l);
  } else {
    GeomObj* interObj = toPlane().intersection(l);
    if (interObj == NULL) {
      return NULL;
    } else {
      return contains(*(Point3d*)interObj) ? interObj : NULL;
    }
  }
}

// return intersection with a plane
GeomObj* Triangle3d::intersection(const Plane3d& pl) const {
  // being contained in plane
  if (isCoplanar(pl)) return new Triangle3d(*this);

  // must not be coplanar
  GeomObj* interObjs[3] = {NULL, NULL, NULL};
  interObjs[0] = pl.intersection(Segment3d(p0, p1));
  if (interObjs[0] != NULL && interObjs[0]->dim() == 1)  // segment
    return interObjs[0];

  interObjs[1] = pl.intersection(Segment3d(p1, p2));
  if (interObjs[1] != NULL && interObjs[1]->dim() == 1)  // segment
    return interObjs[1];

  interObjs[2] = pl.intersection(Segment3d(p0, p2));
  if (interObjs[2] != NULL && interObjs[2]->dim() == 1)  // segment
    return interObjs[2];

  if (interObjs[0] == NULL && interObjs[1] == NULL && interObjs[2] == NULL)
    return NULL;

  Point3d* firstPt =
      (Point3d*)((interObjs[0] != NULL) ? interObjs[0] : interObjs[1]);
  Point3d* secondPt =
      (Point3d*)((interObjs[2] != NULL) ? interObjs[2] : interObjs[1]);

  assert(firstPt != NULL && secondPt != NULL);

  // special care when we get three intersection points
  // there must be two duplicates
  if (interObjs[0] != NULL && interObjs[1] != NULL && interObjs[2] != NULL)
    if (*firstPt == *secondPt) secondPt = (Point3d*)interObjs[1];

  if (*firstPt == *secondPt) {  // a point
    return firstPt;
    delete secondPt;
  } else {  // a segment
    return new Segment3d(*firstPt, *secondPt);
    delete firstPt;
    delete secondPt;
  }
}

GeomObj* Triangle3d::intersection(const Triangle3d& T) const {
  Plane3d PL1 = toPlane();
  Plane3d PL2 = T.toPlane();

  GeomObj* T1_in_PL2 = intersection(PL2);
  GeomObj* T2_in_PL1 = T.intersection(PL1);
  if (T1_in_PL2 == NULL || T2_in_PL1 == NULL) return NULL;

  switch (T1_in_PL2->dim()) {
    case 2:  // triangle
      return coplanar_intersection(T);
    case 1: {  // segment
      Segment3d s1 = *(Segment3d*)T1_in_PL2;
      if (T2_in_PL1->dim() == 0)
        return s1.contains(*(Point3d*)T2_in_PL1) ? T2_in_PL1 : NULL;
      else if (T2_in_PL1->dim() == 1)
        return s1.intersection(*(Segment3d*)T2_in_PL1);
      else {
        std::cerr << "\nShould not reach here";
        return NULL;
      }
    }
    case 0:  // point
      return T.contains(*(Point3d*)T1_in_PL2) ? T1_in_PL2 : NULL;
    default:
      std::cerr << "\nShould not reach here";
      return NULL;
  }
}

// coplanar segment intersection
GeomObj* Triangle3d::coplanar_intersection(const Segment3d& s) const {
  Polygon3d plg1;
  plg1.addPoint(s.startPt());
  plg1.addPoint(s.stopPt());

  Polygon3d plg2 = *in_half_plane(p0, p1, p2, plg1);
  plg1 = *in_half_plane(p1, p2, p0, plg2);
  plg2 = *in_half_plane(p2, p0, p1, plg1);

  // now plg2 contains the intersection object
  switch (plg2.getSize()) {
    case 0:
      return NULL;
    case 1:
      return plg2[0];  // a point
    case 2:
      return new Segment3d(*plg2[0], *plg2[1]);
    default:
      std::cout << "Internal Error: intersects with " << s << std::endl;
      return NULL;
  }
}

// coplanar line intersection
// maybe there is a cleaner way to do this
GeomObj* Triangle3d::coplanar_intersection(const Line3d& l) const {
  Point3d* interPts[3] = {NULL, NULL, NULL};
  int count = 0;
  GeomObj* interObj;
  interObj = Segment3d(p0, p1).intersection(l);
  if (interObj != NULL && interObj->dim() == 0)
    interPts[count++] = (Point3d*)interObj;
  interObj = Segment3d(p1, p2).intersection(l);
  if (interObj != NULL && interObj->dim() == 0)
    interPts[count++] = (Point3d*)interObj;
  interObj = Segment3d(p2, p0).intersection(l);
  if (interObj != NULL && interObj->dim() == 0)
    interPts[count++] = (Point3d*)interObj;

  switch (count) {
    case 0:
      return NULL;
    case 1:
      return interPts[0];
    case 2:
      if (*interPts[0] == *interPts[1])
        return interPts[0];  // a point
      else
        return new Segment3d(*interPts[0], *interPts[1]);
    case 3:  // there must be two points that are the same
      if (*interPts[0] == *interPts[1])
        return new Segment3d(*interPts[0], *interPts[2]);
      else
        return new Segment3d(*interPts[0], *interPts[1]);
    default:
      return NULL;
  }
}

// coplanar triangle intersection
GeomObj* Triangle3d::coplanar_intersection(const Triangle3d& T) const {
  Polygon3d plg1;
  plg1.addPoint(T.V1());
  plg1.addPoint(T.V2());
  plg1.addPoint(T.V3());

  Polygon3d plg2 = *in_half_plane(p0, p1, p2, plg1);
  plg1 = *in_half_plane(p1, p2, p0, plg2);
  plg2 = *in_half_plane(p2, p0, p1, plg1);

  // now plg2 contains the intersection object
  switch (plg2.getSize()) {
    case 0:
      return NULL;
    case 1:
      return new Point3d(*plg2[0]);  // a point
    case 2:
      return new Segment3d(*plg2[0], *plg2[1]);
    default:
      return new Polygon3d(plg2);  // a polygon
  }
}

Polygon3d* Triangle3d::in_half_plane(const Point3d& pa, const Point3d& pb,
                                     const Point3d& pSide,
                                     Polygon3d& plg) const {
  Polygon3d::Iterator I = plg.getIterator();
  Polygon3d* outPlg = new Polygon3d();
  for (int i = 0; i < plg.getSize(); i++, I++) {
    Point3d* pPoint = I.getPoint();
    int s1 = coplanar_orientation(pa, pb, pSide, *pPoint);
    if (s1 != -1)  // not on opposite sides
      outPlg->addPoint(*pPoint);

    if (plg.getSize() == 1) return outPlg;  // stop here if no other points

    if (s1 != 0) {  // not collinear
      Point3d* nextPoint = plg.nextPoint(*pPoint);
      int s2 = coplanar_orientation(pa, pb, pSide, *nextPoint);
      if (s2 * s1 < 0) {  // different side
        Line3d l(pa, pb);
        Point3d* interPt =
            (Point3d*)Segment3d(*pPoint, *nextPoint).intersection(l);
        if (interPt == NULL)
          std::cerr << "\nInternal Error: coplanar_intersection";
        outPlg->addPoint(*interPt);
      }
    }

  }  // for loop

  return outPlg;
}

// test orientation of p against line formed by pa and pb
// return 0 if p is collinear with pa and pb
// return -1 if p is on opposite side of point ps
// return  1 if p is on the same side of point ps
int Triangle3d::coplanar_orientation(const Point3d& pa, const Point3d& pb,
                                     const Point3d& ps,
                                     const Point3d& p) const {
  if (Line3d(pa, pb).contains(p)) return 0;  //
  Vector normal = (pb - pa).cross(ps - pa);
  int s1 = orientation3d(pa, pb, ps, normal);  // s1 = 1 or -1
  int s2 = orientation3d(pa, pb, p, normal);   // s2 = 1 or -1

  return s1 * s2;
}

double Triangle3d::separation(const Segment3d& s) const {
  double t_s1 = Segment3d(p0, p1).separation(s);
  double t_s2 = Segment3d(p1, p2).separation(s);
  double t_s3 = Segment3d(p2, p0).separation(s);

  double t_p1 = this->distance(s.startPt());
  double t_p2 = this->distance(s.stopPt());

  double min_dist = t_s1;
  if (min_dist > t_s2) min_dist = t_s2;
  if (min_dist > t_s3) min_dist = t_s3;
  if (min_dist > t_p1) min_dist = t_p1;
  if (min_dist > t_p2) min_dist = t_p2;
  return min_dist;
}

// CHECK: (intersection of ball and cone)
// The first 4 half-space are the side of the cone.
// The last half-sapce is the horizontal plane H0 in the paper.
bool Triangle3d::intersectHalfspaces(const std::vector<Plane3d>& half_spaces,
                                     const Point3d& translation_center,
                                     const Point3d& rotation_center_on_sphere,
                                     const double outer_distance) const {
  Segment3d segment_p0p1(this->p0, this->p1);
  Segment3d segment_p1p2(this->p1, this->p2);
  Segment3d segment_p2p0(this->p2, this->p0);

  if (segment_p0p1.intersectHalfspaces(half_spaces, translation_center,
                                       rotation_center_on_sphere,
                                       outer_distance) ||
      segment_p1p2.intersectHalfspaces(half_spaces, translation_center,
                                       rotation_center_on_sphere,
                                       outer_distance) ||
      segment_p2p0.intersectHalfspaces(half_spaces, translation_center,
                                       rotation_center_on_sphere,
                                       outer_distance)) {
    return true;
  }

  // triangle line intersection
  Line3d line(half_spaces.back().point(), -half_spaces.back().normal());
  GeomObj* intersection = this->intersection(line);
  if (intersection != NULL) {
    if (intersection->dim() == 0) {
      Point3d intersection_point = *(Point3d*)intersection;
      return intersection_point.intersectHalfspaces(
          half_spaces, translation_center, outer_distance);
    } else {  // intersection->dim() == 1 (segment)
      Segment3d intersection_segment = *(Segment3d*)intersection;
      return intersection_segment.intersectHalfspaces(
          half_spaces, translation_center, rotation_center_on_sphere,
          outer_distance);
    }
  }

  return false;
}

/***************************************************************
 *   I/O
 ***************************************************************/

std::ostream& operator<<(std::ostream& o, const Triangle3d& T) {
  return o << "Triangle3d:\n"
           << "\t" << T.p0 << std::endl
           << "\t" << T.p1 << std::endl
           << "\t" << T.p2;
}

std::istream& operator>>(std::istream& in, Triangle3d& T) {
  Point3d pv1, pv2, pv3;
  std::cout << "\nInput first point(-,-,-):";
  in >> pv1;
  std::cout << "\nInput second point(-,-,-):";
  in >> pv2;
  std::cout << "\nInput third point(-,-,-):";
  in >> pv3;
  T = Triangle3d(pv1, pv2, pv3);
  return in;
}
