/*****************************************************************
 * File: pentagon.ccp
 * Synopsis:
 *      pentagon test
 * Author: Shubin Zhao (2001)
 * 		(modified, Chee Yap, 2003)
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
 * $Id: pentagon.cpp,v 1.1 2006/04/03 18:55:31 exact Exp $
 *****************************************************************/

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"
using namespace std;

void inner( Point2d pentagon[5], Point2d * output) {
  for(int i=0; i<5; i++) {
    Line2d diagnal1(pentagon[i], pentagon[(i+2)%5]);
    Line2d diagnal2(pentagon[(i+1)%5], pentagon[(i+4)%5]);
    output[i] = *(Point2d *)diagnal1.intersection(diagnal2);
  }
}


void outer( Point2d pentagon[5], Point2d * output) {
  for(int i=0; i<5; i++) {
    Line2d edge1(pentagon[i], pentagon[(i+1)%5]);
    Line2d edge4(pentagon[(i+3)%5], pentagon[(i+4)%5]);
    output[i] = *(Point2d *)edge1.intersection(edge4);
  }
}

bool isEqual(Point2d pentA[5], Point2d pentB[5]) {
  for(int i=0; i<5; i++) {
    if( pentA[i] != pentB[i] )
      return false;
  }

  return true;
}

void assign( Point2d* pent_dst, Point2d* pent_src ) {
  for( int j=0; j<5; j++ ) 
  	pent_dst[j] = pent_src[j];
}

void pent_print(Point2d pentagon[5]) {
  cout << "\nPentagon:\n";
  cout << pentagon[0] << "\n" << pentagon[1] << "\n" << pentagon[2] << "\n" 
       << pentagon[3] << "\n" << pentagon[4] << "\n" ;
}


int main( int argc, char* argv[] ) {

  Point2d pentagon2d[5]; //= { Point2d(1.0,       0.0), 
                         //   Point2d(0.309017,  0.951), 
                         //   Point2d(-0.809017, 0.587785), 
                         //   Point2d(-0.809017, -0.587785), 
                         //   Point2d(0.309017,  -0.951) };

  loadPoints(cin, pentagon2d, 5);

  Point2d pentTmp[5];
  Point2d pentInner[5];
  Point2d pentOuter[5];

  EscapePrec = CORE_INFTY;
  int rounds = 2;

  if( argc > 1 )
    for( int i=1; i<argc; i++ ) {
      if( strcmp(argv[i], "-h" )==0 ) {
      	cout << "usage: pentagon_level3 [-escape int] [-round int] [-h]\n";
      	exit(0);
      }else if ( strcmp( argv[i], "-escape")==0 ) {
      	EscapePrec = atoi( argv[i+1] );
      }else if ( strcmp( argv[i], "-round")==0 ) {
      	rounds = atoi( argv[i+1] );
      }// else do thing
    }
      

  cout << "EscapePrec=" << EscapePrec << endl;
  cout << "rounds=" << rounds << endl;

    // keep the original
  assign( pentTmp, pentagon2d);

    //compute inner and outer repeatedly
  cout <<"\nComputing inner...";
  for( int r=0; r<rounds; r++ ) {
    inner( pentTmp, pentInner );
    assign(pentInner, pentTmp );
  }

  cout <<"\nComputing outer...";
  for( int r=0; r<rounds; r++ ) {
    inner( pentTmp, pentOuter );
    assign(pentOuter, pentTmp );
  }

  cout << "\nOriginal ";
  pent_print(pentagon2d);
  cout << "\nWe get ";
  pent_print(pentOuter);

  cout << "\nTesting...";
  if( isEqual( pentagon2d, pentOuter ) )
    cout << "\nCORRECT: The result pentagon is the same\n";
  else 
    cout << "\nINCORRECT: The result pentagon is different";

}

