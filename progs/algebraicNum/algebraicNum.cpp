/* **************************************
   File: desc1.cpp

   Author : Philip Davidson

   Description: 
   compiles to rootbound.exe, isolate.exe, algebra.exe

      rootbound : Descartes' method for root count estimation for a Polynomial P, interval [a,b]
      isolate   : under P, reduction of [a b] to isolating intervals of P
      algebra   : demonstrates addition, subtraction, multiplication, division, comparison
                  of two algebraic integers


 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif


#include "CORE/CORE.h"
// #include "localCurves.h"	// this is the local version of Curves.h
#include "CORE/poly/Curves.h"	// this is the official version of Curves.h


#include "descartes.h"
#include "AlgebraicNum.h"

// TYPEDEF

typedef BigInt	NT;
typedef Polynomial<NT>			PolyNT;
typedef BiPoly<NT>			BiPolyNT;
typedef Curve<NT>                       CurveNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;
typedef BigFloat2                       IntervalType;
typedef AlgebraicNum<NT>                AlgebraicNT;


// CONVENIENCE / FORMATTING 

// ROOTBOUND for closed Interval ival
int rootBound( const PolyNT& P, const IntervalType& ival ) { 
  int mR = intervalRootBound( P, ival );
  if ( P.eval(ival.getLeft()) == 0 ) mR++;
  if ( P.eval(ival.getRight()) == 0 ) mR++;
  return mR;
}

void pBF( BigFloat b ) { 
  cerr << b << endl;
}


template< class NumT > 
void showBiPoly( BiPoly<NumT> bp ) { 
  std::cout << "BiPoly<NT> ( deg = " << bp.ydeg << "/" << bp.getTrueYdegree() << ")" << endl;
  if ( bp.ydeg == -1 ) cerr << 0 << endl;
  for ( int i = 0 ; i <= bp.ydeg; ++i ) { 

    Polynomial<NumT> ci = bp.getCoeffY(i);

    if (zeroP(ci)) {
      continue;
    }

    NT cij = ci.getCoeff(0);

    if ( cij != 0 ) { 
      std::cout << " ";
      if ( cij > 0 ) std::cout << "+";
      std::cout << cij;
      if ( i == 1 ) std::cout << "y" ;
      else if ( i > 1 ) std::cout << "y^" << i;
    }

    for (int j = 1; j<= ci.getTrueDegree(); ++j) {

      NT cij = ci.getCoeff(j);
      
      if ( cij != 0 ) { 
	std::cout << " ";
	if ( cij > 0 ) std::cout << "+";
	std::cout << cij;
	if ( j == 1 ) std::cout << "x";
	else if ( j > 1 ) std::cout << "x^" << j;
	if ( i == 1 ) std::cout << "y";
	else if ( i > 1 ) std::cout << "y^" << i;
      }

    }

  }
  std::cout << endl;
}

// =========================================================
// MAIN
// =========================================================


// MAIN

BigFloat eps = 0;	// precision of the plot (i.e., delta-x)

enum appMode { ROOTBOUND_MODE, 
	       ISOLATE_MODE, 
	       ALGEBRA_MODE, 
	       INTERVAL_CHECK_MODE,
	       COMPOSE_MODE,
	       UNKNOWN_MODE };


int main(int argc, char* argv[])
{
  
    // SET INTERNAL PRECISION
    int prec = 30;	

    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits

    //    cerr.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cerr.precision(prec/3); 		// convert prec bits into prec/3 digits

    BigFloat eps2 = pow( 0.5, prec );

    if ( argc <= 1 ) {
      cerr << "usage: algebraicNum <algebra|rootbound|isolate|compose> [args]" << endl << endl;
      cerr << "algebraicNum algebra \"P-poly\" a b \"Q-poly\" c d " << endl;
      cerr << "\t X = ( P, [a,b] ), Y = ( Q, [c,d] ) // interval must be isolating" << endl;
      cerr << "\t compute : X+Y, X-Y, X*Y, X/Y, X=Y, P(X), P(Y)" << endl;
      cerr << endl;

      cerr << "algebraicNum rootbound \"P-poly\" a b " << endl;
      cerr << "\t# roots of P in [a,b] " << endl << endl;
      cerr << "algebraicNum isolate \"P-poly\" a b " << endl;
      cerr << "\tisolating intervals of P in (a,b) " << endl;
      cerr << endl;
      cerr << "algebraicNum compose <type> <args> [num]" << endl;
      cerr << "\tpop     \"P-poly\" \"A-poly\"              \tPoly  = A(B)" << endl;
      cerr << "\tpbinary \"P-poly\" \"A-poly\" \"B-poly\"   \tPoly  = B^n P(A/B)" << endl;
      cerr << "\tpob     \"P-poly\" \"A-bpoly\"             \tBPoly = A(B)" << endl;
      cerr << "\tbbinary \"P-poly\" \"A-bpoly\" \"B-poly\"  \tBPoly = B^n P(A/B)" << endl;
      cerr << "\tbobb    \"Q-bpoly\" \"A-bpoly\" \"B-bpoly\"\tBPoly = Q(A,B)" << endl;
      cerr << endl;
      return 1;
    }

   appMode mode = ALGEBRA_MODE;

   cerr << "mode: " << argv[1] << endl;
   if ( strstr( argv[1], "rootbound" ) != 0 ) mode = ROOTBOUND_MODE;
   else if ( strstr( argv[1], "isolate" ) != 0 ) mode = ISOLATE_MODE;
   else if ( strstr( argv[1], "algebra" ) != 0 ) mode = ALGEBRA_MODE;
   else if ( strstr( argv[1], "interval" ) != 0 ) mode = INTERVAL_CHECK_MODE;
   else if ( strstr( argv[1], "compose" ) != 0 ) mode = COMPOSE_MODE;
   PolyNT P, P2;
   BigFloat2 I, I2;

   if ( mode == INTERVAL_CHECK_MODE ) { 
     if ( argc < 6 ) { cerr << "not enough args" << endl; return 0; } 

     I  = BigFloat2 ( BigFloat( atof( argv[2] ) ), BigFloat( atof( argv[3] ) ) );
     I.check_exactness();
     I2 = BigFloat2 ( BigFloat( atof( argv[4] ) ), BigFloat( atof( argv[5] ) ) );
     I2.check_exactness();

     pBF( I.getLeft() );

     cerr << "input 1: " << BigFloat( atof( argv[2] ) ) << endl;
     cerr << "input 2: " << BigFloat( atof( argv[3] ) ) << endl;
     cerr << "input 3: " << BigFloat( atof( argv[4] ) ) << endl;
     cerr << "input 4: " << BigFloat( atof( argv[5] ) ) << endl;
     cerr << "Interval 1: "; ivlf ( cerr, I ) << " Interval 2" ; ivlf( cerr, I2 ) << endl;
     cerr << " left l: " << BigFloat( I.getLeft() ) << endl;
     cerr << " left r: " << BigFloat( I.getRight() ) << endl;
     cerr << " left l: " << I.getLeft() << endl;
     cerr << " left r: " << I.getRight() << endl;

     BigFloat2 il,ir;
     BigFloat m = split( I , il, ir );

     cerr << "I: center "; ivlf( cerr, m ) << endl;
     cerr << "split I, left: "; ivlf( cerr, il ) << endl; 
     cerr << "split I,right: "; ivlf( cerr, ir ) << endl; 
     
     BigFloat2 summ  = sum ( I, I2 );
     cerr << "sum: "; ivlf ( cerr, summ ) << endl;

     BigFloat2 diff = difference( I, I2 );
     cerr << "diff: "; ivlf ( cerr, diff ) << endl;

     BigFloat2 prod = product ( I, I2 );
     cerr << "prod: "; ivlf ( cerr, prod ) << endl;

     BigFloat2 quot = quotient( I, I2 );
     cerr << "quot: "; ivlf ( cerr, quot ) << endl;

     bool isect = intersects ( I, I2 );
     cerr << "intersects: " << isect << endl;


     return 0;
   }

   if ( mode == COMPOSE_MODE ) { 
     int reps = 1;

     if ( argc < 5 ) { cerr << "not enough args" << endl; return 0; } 
     int t = -1;

     if ( strcmp( "pop", argv[2] ) == 0 ) t = 0;
     if ( strcmp( "pob", argv[2] ) == 0 ) t = 1;
     if ( strcmp( "bobb", argv[2] ) == 0 ) t = 2;
     if ( strcmp( "pbinary", argv[2] ) == 0 ) t = 3;
     if ( strcmp( "bbinary", argv[2] ) == 0 ) t = 4;

     if ( strcmp( "popNaive", argv[2] ) == 0 ) t = 5;
     if ( strcmp( "pobNaive", argv[2] ) == 0 ) t = 6;
     if ( strcmp( "bobbNaive", argv[2] ) == 0 ) t = 7;

     if ( t == -1 ) t = atoi( argv[2] );
     
     if ( t == 0 ) {  // 1(1)
       if ( argc > 5 ) reps = atoi(argv[5]);
       PolyNT f = PolyNT( argv[3] );
       f.contract();
       cerr << "P " << f << endl;
       PolyNT g = PolyNT( argv[4] );
       g.contract();
       cerr << "X " << g << endl;
       PolyNT res;
       for ( int u = 0 ; u < reps; u++ ) res = f(g); 
       cout << res << endl; 
       res.mapleDump();
       cerr << "horner" << endl;
       PolyNT res2 = composeHorner(f,g);
       cout << res2 << endl;
     } 
     else if ( t == 1 ) { 
       if ( argc > 5 ) reps = atoi(argv[5]);
       PolyNT f = PolyNT( argv[3] );
       BiPolyNT g = BiPolyNT( argv[4] );
       cerr << "P " << f << endl << "x ";
       showBiPoly( g );
       BiPolyNT res;
       for ( int u = 0 ; u < reps; u++ ) res = f(g);
       //showBiPoly( res );
     }
     else if ( t == 5 ) {  // 1(1)
       if ( argc > 5 ) reps = atoi(argv[5]);
       PolyNT f = PolyNT( argv[3] );
       PolyNT g = PolyNT( argv[4] );
       PolyNT res;
       for ( int u = 0 ; u < reps; u++ ) res = composeNaive ( f, g ); 
       //res.mapleDump();
     } 
     else if ( t == 6 ) { 
       if ( argc > 5 ) reps = atoi(argv[5]);
       PolyNT f = PolyNT( argv[3] );
       BiPolyNT g = BiPolyNT( argv[4] );
       BiPolyNT res; 
       for ( int u = 0 ; u < reps; u++ ) res = composeNaive( f, g );
       //showBiPoly( res );

     }
     else { 
       if ( argc < 6 ) { cerr << "not enough args" << endl; return 0; } 
       if ( t == 2 ) { 
	 if ( argc > 6 ) reps = atoi(argv[6]);
	 BiPolyNT f = BiPolyNT( argv[3] );
	 BiPolyNT a = BiPolyNT( argv[4] );
	 BiPolyNT b = BiPolyNT( argv[5] );
	 BiPolyNT res;
	 for ( int u = 0 ; u < reps; u++ ) res = f( a, b );
	 //showBiPoly( res );
       }
       else if ( t == 7 ) { 
	 if ( argc > 6 ) reps = atoi(argv[6]);
	 BiPolyNT f = BiPolyNT( argv[3] );
	 BiPolyNT a = BiPolyNT( argv[4] );
	 BiPolyNT b = BiPolyNT( argv[5] );
	 BiPolyNT res;
	 for ( int u = 0 ; u < reps; u++ ) res = composeNaive( f, a, b );
	 //showBiPoly( res );
       }
       else if ( t == 3 ) { // 1(1,1)
	 if ( argc > 6 ) reps = atoi(argv[6]);
	 PolyNT f = PolyNT( argv[3] );
	 PolyNT a = PolyNT( argv[4] );
	 PolyNT b = PolyNT( argv[5] );
	 PolyNT res;
	 for ( int u = 0 ; u < reps; u++ ) res = composeBinary ( f, a, b ); 
	 //res.mapleDump();
       } 
       else if ( t == 4 ) { // 1(1,1)
	 if ( argc > 6 ) reps = atoi(argv[6]);
	 PolyNT f = PolyNT( argv[3] );
	 BiPolyNT a = PolyNT( argv[4] );
	 BiPolyNT b = PolyNT( argv[5] );
	 BiPolyNT res;
	 for ( int u = 0 ; u < reps; u++ ) res = composeBinary ( f, a, b ); 
	 //showBiPoly(res);
       } 
     }
     return 0;
   }
   
   if ( argc <= 2 ) { 
     cerr << argv[0] << ": no arguments! using default values" << endl;
   }

   if ( argc > 2 ) { 
     P = PolyNT ( argv[2] );
   }
   else { 
     cerr << "poly" << endl;
     P = PolyNT ( "12x^3 + 6x + 1" );
   }
   if ( argc > 4 ) { 
     I = BigFloat2 ( BigFloat( atof(argv[3] ) ), BigFloat( atof( argv[4] ) ) );
   }
   else { 
     I = BigFloat2 ( -1, 1 );
   }

   I2 = I ;
   if ( argc > 5 ) { 
     P2 = PolyNT ( argv[5] );
   }
   if ( argc > 7 ) {  
     I2 = BigFloat2 ( BigFloat( atof( argv[6] ) ), BigFloat( atof( argv[7] ) ) );
   }

   cerr << "P = " << P << endl;
   PolyNT gcdP = P.sqFreePart();

   if ( !unitP(gcdP) ) { 
     cerr << "=======P NOT SQUAREFREE=============" << endl;
     cerr << "->squarefree P =" << P << endl;
     cerr << "->gcd(P,P') =" << gcdP << endl;
     cerr << "==================================" << endl;
   }


   if ( mode == ROOTBOUND_MODE ) { 

     cerr << "===========ROOT BOUND==============" << endl;
     int mRoots = rootBound( P, I ); 
     cerr << mRoots << " >= # roots of P in [" << I.getLeft() << ", " << I.getRight() << "]" << endl;
     cerr << "==================================" << endl << endl ;
   }

   else if ( mode == ISOLATE_MODE ) { 

     cerr << "===========ISOLATING ==============" << endl;
     int mRoots = intervalRootBound( P, I ); 
     cerr << mRoots << " >= # roots of P(X) in " << I << endl;

     if ( mRoots > 0 ) { 
       BigFloat sep = 0;  // no sepBound function? 
       cerr << ": root separation bound " << sep << endl;

       vector < BigFloat2 > intervals;
       isolatingIntervals( P, I, intervals, sep );

       cerr << endl << "Isolating Intervals ("<< intervals.size() <<"): { " << endl;
       for ( unsigned int i = 0; i < intervals.size(); i++ ) { 
	 cout << "\t" << i+1 << ": "; ivlf( cout, intervals[i] ) << "\t-> ";
	 precPrint( cout, P, intervals[i], eps2 );
	 cout << endl; 
       }
       cerr << "}" << endl;
     }
     cerr << "===================================" << endl << endl ;

   }
   
   else if ( mode == ALGEBRA_MODE ) {

     cerr << "===========ALGEBRAIC OPERATIONS ==============" << endl;
     if ( argc <= 7 ) { 
       cerr << "Usage:  algebra \"Poly1\" [ a b ] \"Poly2\" [ c d ] "  << endl;
       return 1;
     }
     
     AlgebraicNT A ( P, I );
     AlgebraicNT B ( P2, I2 );
     
     if ( !A.valid() ) { 
       cerr << "value A is invalid..." << endl;
       return 1;
     }
     
     cerr << "value A: " << A << endl;

     if ( !B.valid() ) { 
       cerr << "value B is invalid..." << endl;
       return 1;
     }

     cerr << "value B: " << B << endl;

     cerr << "======ADDITION=============" << endl;
     AlgebraicNT plus     = A + B;
     cerr << "A + B: " << plus << endl << endl;

     cerr << "======SUBTRACTION==========" << endl;
     AlgebraicNT minus    = A - B;
     cerr << "A - B: " << minus << endl << endl;

     cerr << "======MULTIPLICATION=======" << endl;
     AlgebraicNT multiply = A * B;
     cerr << "A * B: " << multiply << endl << endl;

     cerr << "======DIVISION=============" << endl;
     AlgebraicNT divide   = A / B;     
     cerr << "A / B: " << divide << endl << endl;

     cerr << "=======COMPARISON==========" << endl;
     bool         eqs      = ( A == B );
     cerr << ( (eqs) ? "A eq B" : "A neq B" ) << endl << endl;

     cerr << "=======POLYNOMIAL EVAL==========" << endl;
     cerr << "Poly = " << P << endl;

     AlgebraicNT AofA = P.eval( A );
     cerr << "P(A)" << AofA << endl << endl;

     AlgebraicNT AofB = P.eval( B );
     cerr << "P(B)" << AofB << endl << endl;

     cerr << "===============================" << endl << endl;
   }
   return 0;
} 
