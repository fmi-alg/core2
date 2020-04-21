/*****************************************************************
 * File: pentagon.cpp
 * Synopsis:
 *      example program for 2-dimensional geometry
 * Usage:
 *	% pentagon [EscapePrec] [NumberRounds = 1] [RatFlag = true]
 * E.g.,
 *	% pentagon 2000 5
 *
 * Author: Shubin Zhao (shubinz@cs.nyu.edu), 2001.
 *
 * Since Core Library 1.4
 * $Id: pentagon.cpp,v 1.6 2006/11/29 04:13:03 exact Exp $
 *****************************************************************/

#include "CORE.h"
#include "CORE/geometry3d.h"

extern void transform( double in[3], double *out );

void inner( Point3d pentagon[5], Point3d * output) {
  for(int i=0; i<5; i++) {
    Line3d diagnal1(pentagon[i], pentagon[(i+2)%5]);
    Line3d diagnal2(pentagon[(i+1)%5], pentagon[(i+4)%5]);
    Point3d* ppt = (Point3d*)diagnal1.intersection(diagnal2);
    if (!ppt) {
      std::cout << "INCORRECT!!! diagonal lines are disjoint or identical" << std::endl;
      exit(0);
    }
    output[i] = *ppt;
  }
}


void outer( Point3d pentagon[5], Point3d * output) {
  for(int i=0; i<5; i++) {
    Line3d edge1(pentagon[i], pentagon[(i+1)%5]);
    Line3d edge4(pentagon[(i+3)%5], pentagon[(i+4)%5]);
    Point3d* ppt = (Point3d*)edge1.intersection(edge4);
    if (!ppt) {
      std::cout << "INCORRECT!!! diagonal lines are disjoint or identical" << std::endl;
      exit(0);
    }
    output[i] = *ppt;
  }
}

bool isEqual(Point3d pentA[5], Point3d pentB[5]) {
  for(int i=0; i<5; i++) {
    if( pentA[i] != pentB[i] )
      return false;
  }

  return true;
}

void pent_print(Point3d pentagon[5]) {
  std::cout << "\nPentagon:\n";
  std::cout << pentagon[0] << "\n" << pentagon[1] << "\n" << pentagon[2] << "\n" 
       << pentagon[3] << "\n" << pentagon[4] << "\n" ;
}

#if CORE_LEVEL > 2
  double pentagon2d[5][3] = {{"1.0",       "0.0",   "0.0"},
			   {"0.309017",  "0.951", "0.0"},
			   {"-0.809017", "0.587785", "0.0"},
			   {"-0.809017", "-0.587785", "0.0"},
			   {"0.309017",  "-0.951",  "0.0"}};
#else
  double pentagon2d[5][3] = {{1.0,       0.0,   0.0},
			   {0.309017,  0.951, 0.0},
			   {-0.809017, 0.587785, 0.0},
			   {-0.809017, -0.587785, 0.0},
			   {0.309017,  -0.951,  0.0}};
#endif

int main( int argc, char* argv[] ) {

  set_cut_off_bound(30000);		// default escape precision
  int rounds = 1;		// default number of rounds
  bool ratFlag = true;		// default is to reduce Expr to Rationals

  // Read command line arguments:
  if( argc > 1 ) 
     set_escape_bound(atoi( argv[1] ));
  if( argc > 2 )
     rounds = atoi( argv[2] );
  if( argc > 3 )
     if (atoi( argv[3] ) <= 0) ratFlag = false;

  //setRationalReduceFlag(ratFlag);  // if ratFlag=true, reduce rational Expressions
      
  // Read command line arguments:
  double buff[3];
  Point3d pentagon3d[5];
  Point3d pentInner[5];
  Point3d pentOuter[5];

  std::cout << "CutOffPrec=" << get_cut_off_bound() << std::endl;
  std::cout << "round=" << rounds << std::endl;

  // create pentagon in 3d by linear transformation
   transform( pentagon2d[0], buff);
  pentagon3d[0] = Point3d(buff[0],      buff[1],   buff[2]);
   transform( pentagon2d[1], buff);
  pentagon3d[1] = Point3d(buff[0],      buff[1],   buff[2]);
   transform( pentagon2d[2], buff);
  pentagon3d[2] = Point3d(buff[0],      buff[1],   buff[2]);
   transform( pentagon2d[3], buff);
  pentagon3d[3] = Point3d(buff[0],      buff[1],   buff[2]);
   transform( pentagon2d[4], buff);
  pentagon3d[4] = Point3d(buff[0],      buff[1],   buff[2]);
	
    // keep the original
  for( int j=0; j<5; j++ ) 
  	pentOuter[j] = pentagon3d[j];
  	
    //compute inner and outer repeatedly
  for( int r=0; r<rounds; r++ ) {
    std::cout <<"\nComputing inner...\n";
    inner( pentOuter, pentInner );
    std::cout <<"\nComputing Outer...\n";
    outer( pentInner, pentOuter );
  }

  std::cout << "\nOriginal Pentagons:";
  pent_print(pentagon3d);

  std::cout << "\n\nComputed using an identity (in-out) operations";
  pent_print(pentOuter);

  std::cout << "\n\nTesting...";
  if( isEqual( pentagon3d, pentOuter ) )
    std::cout << "\nCORRECT! The resulting pentagons are all the same\n";
  else 
    std::cout << "\nERROR! Some resulting pentagon is different";
  return 0;
}

