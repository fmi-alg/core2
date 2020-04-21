/*****************************************************************
 * File: test.cc
 * Synopsis:
 *      3-dimensional geometrytest program
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
 * $Id: test.cpp,v 1.1 2006/04/03 18:55:31 exact Exp $
 *****************************************************************/


#include "CORE.h"

#include "geometry3d.h"
#include <iostream.h>

int num_of_errors = 0;


void polygon3d_test() {
  cout << "\n\n############# Test Polygon3d ###############\n";
  Triangle3d T1, T2; 

  T1 = Triangle3d( Point3d(0,1,0), Point3d(1,2,1), Point3d(4,4,1) );
  T2 = Triangle3d( Point3d(3,0,0), Point3d(0,1,0), Point3d(3,1,2) );
  GeomObj* pInterObj = T1.intersection( T2 );
  Point3d ansPt = Point3d(0,1,0);
  if( pInterObj == NULL || pInterObj->dim() != 0 || ansPt!= *((Point3d*)pInterObj) ) {
    cout << "ERROR! --intersection( Triangle3d & )  -- test 1" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 1" << endl;

  if( T1.do_intersect( T2 ) != true ) {
    cout << "ERROR! --do_intersect( Triangle3d & )  -- test 1.1" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --do_intersect( Triangle3d & ) -- test 1.1" << endl;

  T1 = Triangle3d( Point3d(1,1,-1), Point3d(1,1,1), Point3d(4,0,-1) );
  GeomObj* pInterObj1 = T2.intersection( T1 );
  GeomObj* pInterObj2 = T1.intersection( T2 );

  if( pInterObj1 == NULL || pInterObj2 == NULL ) {
    cout << "ERROR! --intersection( Triangle3d & )" << endl;
    num_of_errors++;
  }
  else if( *((Segment3d *)pInterObj1) != *((Segment3d *)pInterObj2) ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 2" << endl;
    num_of_errors++;
  }
  else {
    //cout << *((Segment3d *)pInterObj1) << endl;
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 2" << endl;
    delete pInterObj1;
    delete pInterObj2;
  }


  T1 = Triangle3d( Point3d(0,0,-1), Point3d(6,0,-1), Point3d(4,5,-1) );
  T2 = Triangle3d( Point3d(4,0,-1), Point3d(5,0,5), Point3d(3,1,4) );
  pInterObj = T1.intersection( T2 );
  ansPt = Point3d(4, 0, -1);
  if( pInterObj == NULL || *(Point3d *)pInterObj != ansPt ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 3" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 3" << endl;

  T1 = Triangle3d( Point3d(4,5,1), Point3d(2,3,1), Point3d(5,4,1) );
  T2 = Triangle3d( Point3d(4,5,1), Point3d(9,6,1), Point3d(6,7,1) );
  pInterObj = T1.intersection( T2 );
  ansPt = Point3d(4, 5, 1);
  if( pInterObj == NULL || *(Point3d *)pInterObj != ansPt ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 4" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 4" << endl;

  T1 = Triangle3d( Point3d(1,1,-1), Point3d(4,1,-1), Point3d(5,6,-1) );
  T2 = Triangle3d( Point3d(3,1,-1), Point3d(6,1,-1), Point3d(4,4,3) );
  pInterObj = T1.intersection( T2 );
  Segment3d ansSeg(Point3d(3, 1, -1), Point3d(4,1,-1) );
  if( pInterObj == NULL || *(Segment3d *)pInterObj != ansSeg ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 5" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 5" << endl;

  if( T1.intersects( T2 ) != true ) {
    cout << "ERROR! --do_intersect( Triangle3d & ) -- test 5.1" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --do_intersect( Triangle3d & ) -- test 5.1" << endl;

  T1 = Triangle3d( Point3d(1,1,1), Point3d(7,1,1), Point3d(5,6,1) );
  T2 = Triangle3d( Point3d(4,3,1), Point3d(3,5,4), Point3d(4,5,6) );
  pInterObj = T1.intersection( T2 );
  ansPt = Point3d(4, 3, 1);
  if( pInterObj == NULL || *(Point3d *)pInterObj != ansPt ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 6" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 6" << endl;

  T1 = Triangle3d( Point3d(1,2,3), Point3d(7,2,3), Point3d(5,7,3) );
  T2 = Triangle3d( Point3d(4,2,3), Point3d(3,6,6), Point3d(4,2,8) );
  pInterObj = T1.intersection( T2 );
  ansPt = Point3d(4, 2, 3);
  if( pInterObj == NULL || *(Point3d *)pInterObj != ansPt ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 7" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 7" << endl;

  T1 = Triangle3d( Point3d(1,0,1), Point3d(6,0,1), Point3d(5,6,1) );
  T2 = Triangle3d( Point3d(1,0,-1), Point3d(5,6,3), Point3d(4,0,5) );
  pInterObj = T1.intersection( T2 );
  ansSeg = Segment3d( Point3d(2,0,1), Point3d(3,3,1) );
  if( pInterObj == NULL || *(Segment3d *)pInterObj != ansSeg ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 8" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 8" << endl;

  T1 = Triangle3d( Point3d(3,2,2), Point3d(9,2,2), Point3d(7,7,2) );
  T2 = Triangle3d( Point3d(6,4,2), Point3d(5,9,2), Point3d(9,8,2) );
  pInterObj1 = T1.intersection( T2 );
  pInterObj2 = T2.intersection( T1 );
  //cout<<*(Polygon3d *)pInterObj2 << endl;
  if( pInterObj1 == NULL || pInterObj2 == NULL || 
        *(Polygon3d *)pInterObj1 != *(Polygon3d *)pInterObj2 ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 9" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 9" << endl;


  T1 = Triangle3d( Point3d(1,0,2), Point3d(6,0,2), Point3d(5,6,2) );
  T2 = Triangle3d( Point3d(2,0,2), Point3d(1,2,5), Point3d(4,6,2) );
  pInterObj = T1.intersection( T2 );
  ansSeg = Segment3d( Point3d(2,0,2), Point3d(3,3,2) );
  if( pInterObj == NULL || *(Segment3d *)pInterObj != ansSeg ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 10" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 10" << endl;

  pInterObj = T1.intersection( T1 );
  if( pInterObj == NULL || *(Polygon3d *)pInterObj != *T1.toPolygon() ) {
    cout << "ERROR! --intersection( Triangle3d & ) -- test 11" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Triangle3d & ) -- test 11" << endl;

  T1 = Triangle3d( Point3d(3,2,2), Point3d(9,2,4), Point3d(7,7,2) );
  ansSeg = Segment3d( Point3d(7,7,2), Point3d(6,2,3));
  pInterObj = T1.intersection( Segment3d(Point3d(5,-3,4), Point3d(7,7,2)) );
  //cout << *(Segment3d *)pInterObj << endl;
  if( pInterObj == NULL || *(Segment3d *)pInterObj != ansSeg ) {
    cout << "ERROR! --intersection( Segment3d & ) -- test 12" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Segment3d & ) -- test 12" << endl;

  T1 = Triangle3d( Point3d(3,2,2), Point3d(9,2,4), Point3d(7,7,2) );
  ansSeg = Segment3d( Point3d(7,7,2), Point3d(6,2,3));
  pInterObj = T1.intersection( Line3d(Point3d(8,12,1), Point3d(5,-3,4)) );
  //cout << *(Segment3d *)pInterObj << endl;
  if( pInterObj == NULL || *(Segment3d *)pInterObj != ansSeg ) {
    cout << "ERROR! --intersection( Line3d & ) -- test 13" << endl;
    num_of_errors++;
  }
  else
    cout << "CORRECT! --intersection( Line3d & ) -- test 13" << endl;

  cout << "\n############# End of Polygon3d Test ###############\n";
}


void plane3d_test() {
  cout << "\n\n############# Test Plane3d ###############\n";
  Plane3d plane1( Point3d(3.0, 0.0, 0.0), Point3d(0.0, 1.0, 0.0), 
                  Point3d(3.0, 3.0, 0.0) );  //XY plane
  Plane3d plane2( Point3d(1.0, 0.0, 0.0), Point3d(0.0, 1.0, 0.0), 
                  Point3d(0.0, 0.0, 1.0) );

  double dist = plane2.distance( ORIGIN_3D );
  if( dist != sqrt( double(3) )/ double(3) ) {
	cout << "ERROR! -- distance( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! -- distance( Point3d &)" << endl;

  Point3d image = plane2.projection( ORIGIN_3D );
  Point3d ansPt = Point3d(BigRat(1,3), BigRat(1,3), BigRat(1,3));
  if( image != ansPt ) {
	cout << "ERROR! --projection( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --projection( Point3d &)" << endl;

  image = plane1.projection( Point3d(2.5, 1.0, 5.0) );
  ansPt = Point3d(2.5, 1.0, 0.0);
  if( image != ansPt ) {
	cout << "ERROR! --projection( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --projection( Point3d &)" << endl;

  if( !plane2.contains( Point3d(0.5, 0.5, 0.0) ) ) {
	cout << "ERROR! --contains( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --contains( Point3d &)" << endl;

  Line3d* resLine = (Line3d *)plane1.intersection(plane2);
  Line3d ansLine( Point3d(1.0, 0.0, 0.0), Point3d(0.0, 1.0, 0.0) );
   //cout << *resLine << endl;
  if( !ansLine.isCoincident(*resLine) ) {
	cout << "ERROR! --intersection( Plane3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersection( Plane3d &)" << endl;

  int resInt = plane1.intersects(plane2);
  //cout << resInt << endl;
  if( resInt != 1 ) {
	cout << "ERROR! --intersects( Plane3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersects( Plane3d &)" << endl;

  
  Segment3d s1( Point3d(1,1,1), Point3d(-1,-1,-1) );
  Point3d* pResPt = (Point3d *)plane1.intersection(s1);
  if( *pResPt != ORIGIN_3D ) {
	cout << "ERROR! --intersection( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersection( Segment3d &)" << endl;

  resInt = plane1.intersects(s1);
  //cout << resInt << endl;
  if( resInt != 0 ) {
	cout << "ERROR! --intersects( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersects( Segment3d &)" << endl;

  cout << "############# End of Plane3d Test ###############\n";
}


void segment3d_test() {
  cout << "############# Test  Segment3d ###############\n";
  Segment3d s1( Point3d(0.0, 0.0, 3.0), Point3d(5.0, 0.0, 3.0));
  Segment3d s2( Point3d(0.0, 1.0, 3.0), Point3d(3.0, 3.0, 3.0));
  
  Point3d* pResPt = (Point3d* )s1.toLine().intersection( s2.toLine() );
  Point3d ansPt(-1.5, 0.0, 3.0);
  //cout << *pResPt << endl;
  if (ansPt != *pResPt) {
	cout << "ERROR! --intersection( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersection( Segment3d &)" << endl;

  Point3d resPt = s1.nearPt( Point3d(3.0, 3.0, 3.0) );
  ansPt = Point3d(3.0, 0.0, 3.0);
  //cout << resPt << endl;
  if (ansPt != resPt) {
	cout << "ERROR! --nearPt( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --nearPt( Point3d &)" << endl;  

  resPt = s1.nearPt( s1.stopPt() );
  //cout << resPt << endl;
  if (resPt != s1.stopPt() ) {
	cout << "ERROR! --nearPt( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --nearPt( Point3d &)" << endl;  

  Point3d p(12.33, 3.5, 0.02);
  resPt = s1.nearPt( p );
  //cout << resPt << endl;
  if( s1.distance(p) != resPt.distance(p) ) {
	cout << "ERROR! --nearPt( Point3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --nearPt( Point3d &)" << endl;  

  int resInt = s1.intersects( s2.toLine() );
  cout << resInt << endl;
  if (resInt != -1) {
	cout << "ERROR! --intersect( Line3d &)" << endl;
        num_of_errors++;
  } else {
	cout << "CORRECT! --intersect( Line3d &)" << endl;  
  }

  resInt = s1.intersects( s2 );
  //cout << resInt << endl;
  if (resInt != -1) {
	cout << "ERROR! --intersect( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersect( Segment3d &)" << endl;  

  resInt = s1.intersects( s1 );
  //cout << resInt << endl;
  if (resInt != 1) {
	cout << "ERROR! --intersect( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersect( Segment3d &)" << endl;  

  resInt = s2.intersects( Segment3d( s2.startPt(), ORIGIN_3D ) );
  //cout << resInt << endl;
  if (resInt != 0) {
	cout << "ERROR! --intersect( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersect( Segment3d &)" << endl;  

  pResPt = (Point3d *)s2.intersection( Line3d( s2.startPt(), ORIGIN_3D ));
  //cout << *pResPt << endl;
  if( pResPt==NULL || *pResPt != s2.startPt() ) {
	cout << "ERROR! --intersection( Line3d &)" << endl;
    num_of_errors++;
  }
  else 
	cout << "CORRECT! --intersection( Line3d &)" << endl;  

  Segment3d * pinterSeg = (Segment3d *)s1.intersection( Segment3d( Point3d(-1, 0, 3.0), \
 					Point3d(2, 0, 3.0)));
  Segment3d ansSeg( Point3d(0,0,3.0), Point3d(2,0,3.0) );
  if( pinterSeg==NULL || *pinterSeg != ansSeg ) {
	cout << "ERROR! --intersection( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersection( Segment3d &)" << endl;  

  pinterSeg = (Segment3d *)s1.intersection( s1 );
  if( pinterSeg==NULL || *pinterSeg != s1 ) {
	cout << "ERROR! --intersection( Segment3d &)" << endl;
    num_of_errors++;
  }
  else
	cout << "CORRECT! --intersection( Segment3d &)" << endl;  

  cout << "############# End of Segment3d Test ###############\n";
}

int intersects( const Segment3d& s, const Line3d& l) {
  Point3d p0= s.startPt();
  Point3d p1= s.stopPt();

//  double v = signed_volume(p0, p1, l.startPt(), l.stopPt());

  if( !coplanar(p0, p1, l.startPt(), l.stopPt() ) )
    return -1;
  if( l.contains(p0) && l.contains(p1) ) // this segment is on l
    return 1;

   // not collinear
   // use normal as a viewpoint
  Vector normal = l.direction().cross(p1 - p0);
  Point3d Pn = l.startPt() + normal;
  //cout << Pn << endl;
  double vol =  signed_volume( l.startPt(), l.stopPt(), Pn, p1 );
  cout << vol;
  if( vol > 0 )
    cout << " is greater than 0\n";
  else
    cout << " is less than 0\n";
  int s1 = orientation3d( l.startPt(), l.stopPt(), Pn, p0 );
  int s2 = orientation3d( l.startPt(), l.stopPt(), Pn, p1 );
  
  if(s1==0 && s2==0)  //is on
    return 1;
  else if( s1*s2>0 )
    return -1;
  else
    return 0;
}

void bug_prog() {
 Segment3d s1( Point3d(0.0, 0.0, 3.0), Point3d(5.0, 0.0, 3.0));
 Line3d l( Point3d(0.0, 1.0, 3.0), Point3d(3.0, 3.0, 3.0));
 
 // cout << "s1=" << s1 << ", l=" << l << endl;
  intersects( s1, l );
  //cout << "s1=" << s1 << ", l=" << l << endl;

  Point3d p0= s1.startPt();
  Point3d p1= s1.stopPt();

  Vector normal = l.direction().cross(p1 - p0);
  Point3d Pn = l.startPt() + normal;
  double vol =  signed_volume( l.startPt(), l.stopPt(), Pn, p1 );
  cout << vol <<"(sign=" <<vol.getSign()<<")";
  if( vol > 0 )
    cout << " is greater than 0\n";
  else
    cout << " is less than 0\n";
}


int main(int argc, char* argv[]) {
  
  segment3d_test();
  cout << num_of_errors << " errors occured" << endl;
  plane3d_test();
  cout << num_of_errors << " errors occured" << endl;
  polygon3d_test();
  cout << "Test finished" << endl;
  cout << num_of_errors << " errors occured" << endl;
  
  cout << "\n########But report test program#########\n";
  cout << "First, turn on filter..." << endl;
  setFpFilterFlag(true);
  bug_prog();
  cout << "now, turn off filter..." << endl;
  setFpFilterFlag(false);
  bug_prog();
}
