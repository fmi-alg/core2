/*****************************************************************
 * File: segment3d.cc
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
 * $Id: segment3d.cpp,v 1.2 2006/04/03 19:38:35 exact Exp $
 *****************************************************************/

#include <CORE/geom3d/plane3d.h>
#include <CORE/geom3d/segment3d.h>

/************************************************************
 * *   constructors
************************************************************/
 //ray segment
Segment3d::Segment3d(const Point3d &p, const Vector &v) : p0(p), p1(p + v) { 
  directed = false;      
  open = false;          
}

Segment3d::Segment3d(const Point3d &_p0, const Point3d &_p1) : p0(_p0), p1(_p1) { 
  directed = false; 
  open = false;     
}

Segment3d::Segment3d(const Segment3d &s) : p0(s.startPt()), p1(s.stopPt()) { 
  directed = false; 
  open = false;     
}

Segment3d::Segment3d() : p0(0.0, 0.0, 0.0), p1(0.0, 0.0, 0.0) {
  directed = false; 
  open = false;     
}
 // line passes through the origin with direction 0.

/************************************************************
 * *   Member functions 
************************************************************/

  // test if two segments are coincident
bool Segment3d::isCoincident( const Segment3d& s) const {
  if( directed )
    return (p0==s.startPt() && p1==s.stopPt());
  else if( (p0==s.startPt() && p1==s.stopPt()) || (p1==s.startPt() && p0==s.stopPt()) )
    return true;
  else 
    return false;
}

bool Segment3d::isCoplanar( const Line3d& l) const {
  return coplanar(p0, p1, l.startPt(), l.stopPt() );
}

bool Segment3d::isCoplanar( const Segment3d& s) const {
  return coplanar(p0, p1, s.startPt(), s.stopPt() );
}
   
 //  reverses the direction of the segment
void Segment3d::reverse() {
  Point3d p = p0;
  p0 = p1;
  p1 = p0;
}
 
 // returns the Euclidean distance between this segment and point q
 // problem exists when segment is open
double Segment3d::distance( const Point3d& p ) const{
  double s1 = dotProduct(p1-p0, p-p0);
  double s2 = dotProduct(p1-p0, p-p1);
  if( s1*s2 <= 0)
    return toLine().distance( p );
  else
    return (s1>0)? p1.distance( p ) : p0.distance( p );
}

 // returns the point on segment closest to q
 /* NOTE:
  * problem exist when segment is open!
  * need to be solved later
  */
Point3d Segment3d::nearPt( const Point3d& p ) const {
  double s1 = dotProduct(p1-p0, p-p0);
  double s2 = dotProduct(p1-p0, p-p1);
  if( s1*s2 <= 0)
    return toLine().projection( p );
  else
    return (s1>0)? p1 : p0;
}

bool Segment3d::contains( const Point3d& p ) const {
  if( !toLine().contains( p ) )
    return false;
   
   // must be collinear here
  double sign = dotProduct(p-p0, p-p1);
  if( sign < 0 )
    return true;
  else if( sign==0 && !open )
    return true;
  else 
    return false;
}

 //decides whether this segment intersects t 
int Segment3d::intersects( const Line3d& l) const {
  
  int res = toLine().intersects(l);
  if( res == 0 ) {
   // lines intersect
   // test segment
    Vector normal = l.direction().cross(p1 - p0);
    Point3d Pn = l.startPt() + normal;
    int s1 = orientation3d( l.startPt(), l.stopPt(), Pn, p0 );
    int s2 = orientation3d( l.startPt(), l.stopPt(), Pn, p1 );
  
    if(s1==0 && s2==0)  //is on
      return 1;
    else if( s1*s2>0 )
      return -1;
    else
      return 0;

  }else  // not intersect or coincident
    return res;
}
  

  //return dim of intersection
int Segment3d::intersects( const Segment3d& s) const {
  int res = intersects( s.toLine());
  if( res == -1 )
    return -1;
  else if( res == 0 )
    if( s.intersects( toLine() ) == 0 )       return  0; //double check
    else                                      return -1;
  else {	// must be collinear
    double s1 = dotProduct( s.startPt()-p0, s.startPt()-p1 );  // o<-----*-->o
    double s2 = dotProduct( s.stopPt()-p0, s.stopPt()-p1 );  
// o<---------o<--*
    double s3 = dotProduct( p0-s.startPt(), p0-s.stopPt() );   // *<-----o-->*
    double s4 = dotProduct( p1-s.startPt(), p1-s.stopPt() );   // *<---------*<--o

    if( s1<0 || s2<0 || s3<0 || s4<0 ) //  -o---*--o----*-  OR: -o---*-----*-o-
                                       //one end point is contained
      return 1;                        // then must be a segment intersection
    else if( s1==0 && s2==0 )      // -8===========8-
      return 1;                    // coincident
    else if( s1>0 && s2>0 )    
      return -1;   //  -o--------o-*----*- or  *----*--o--------o-
    else     // one end point is coincident        -o--------8-------o-
      if( open || s.isOpen() ) return -1;
      else        return  0;
  }
}

  // return intersection point if this segment and l intersect at a single point
  // the intersection point is returned 
GeomObj* Segment3d::intersection( const Line3d& l ) const {
  if( !isCoplanar( l ) )
    return NULL;

  if( l.isCoincident( toLine() ) )
    return new Segment3d(*this);
    
  Point3d* p = (Point3d*)l.intersection( toLine() );
  if( p!=NULL && contains( *p ) )  return p;
  else              return  NULL; 
  
}

  // return intersection -- segment or point
GeomObj* Segment3d::intersection( const Segment3d& s ) const {
  if( !isCoplanar(s) )
    return NULL;
  
  if( toLine().isCoincident( s.toLine() ) ) {
     // colinear 
     // first intersect the segment s with ray starting from p0 
    Segment3d interSegment;
    double s1 = dotProduct(s.startPt() - p0, p1-p0 );
    double s2 = dotProduct(s.stopPt()  - p0, p1-p0 );
    if( s1<0 && s2<0 )
      return NULL;
    else if(s1>=0 && s2>=0)  // copy s
      interSegment = Segment3d( s );
    else if (s1>=0)
      interSegment = Segment3d(p0, s.startPt());
    else  
      interSegment = Segment3d(p0, s.stopPt());

     // then intersect the resulting segment with ray starting from p1
    double s3 = dotProduct(interSegment.startPt() - p1, p1-p0 );
    double s4 = dotProduct(interSegment.stopPt()  - p1, p1-p0 );
    if( s3>0 && s4>0 )
      return NULL;
    else if(s3<=0 && s4>0)
      interSegment = Segment3d(interSegment.startPt(), p1);
    else if(s4<=0 && s3>0)
      interSegment = Segment3d(interSegment.stopPt() , p1);
    //else -- s3<=0 && s4<=0
    //do nothing cause intersection remains unchanged

    if( interSegment.isTrivial() )  //intersection is point
      return new Point3d( interSegment.startPt() );
    else 
      return new Segment3d( interSegment );
  }
  else{  // use line-line intersection
    Point3d* p = (Point3d *)toLine().intersection( s.toLine() );
    if( p!=NULL && contains(*p) && s.contains(*p) )    // check
      return p;
    else 
      return NULL;
  }

}

 
 // return bisector plane
Plane3d Segment3d::bisect_plane() const { 
  Point3d midPt = p0+(p1-p0)*double(0.5);
  Vector  normal = p1-p0;
  return Plane3d( midPt, normal ); 
}


std::ostream& operator<<(std::ostream& out, const Segment3d & s) {
  return out << "Segment3d[" << s.p0 << "," << s.p1 << "; directed=" << s.directed 
  		   << "; open=" << s.open << "]";
}

std::istream& operator>>(std::istream& in, Segment3d& l) {
  Point3d pStart, pEnd;
  std::cout << "\nInput segment start point(-,-,-):";
  in >> pStart;
  std::cout << "Input segment end point(-,-,-):";
  in >> pEnd;
  l = Segment3d(pStart, pEnd); 
  return in;
}

