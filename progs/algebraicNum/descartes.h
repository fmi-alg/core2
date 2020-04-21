#ifndef __DESCARTES_H_INCLUDED__
#define __DESCARTES_H_INCLUDED__
/****
     File descartes.h
     Header for Descartes' Formulation
     
     Author : Philip Davidson

********************/


#include "CORE/poly/Poly.h"
#include "composePoly.h"

//temp
typedef BigRat     Rational;
typedef BigFloat2  IntervalType;
typedef BigFloat   IntervalValue;


// these are mostly convenience for transition from BFinterval
// and can go away eventually 


Rational leftRat( const IntervalType& I ) { 
  return I.get_q();
}

Rational rightRat( const IntervalType& I ) { 
  return I.getRight().get_q();
}

IntervalValue midPointI( const IntervalType& I ) { 
  if ( I.is_exact() ) return I.getLeft();
  IntervalValue m = ( I.getLeft() + I.getRight() );
  m.div_2exp(m, 1); 
  return m;
}

//TODO: Error Condition if I is exact? 
IntervalValue split( const IntervalType& I, IntervalType& right, IntervalType& left ) { 
  IntervalValue mid = midPointI( I );
  if ( I.is_exact() ) { 
    right = I;
    left = I;
  }
  else { 
    right = IntervalType( I.getLeft(), mid );
    left  = IntervalType( mid, I.getRight() );
  }
  return mid;
}

BigFloat2 sum( const BigFloat2& a, const BigFloat2& b ) { 
  BigFloat2 out;
  out.add( a, b );
  return out;
}

BigFloat2 difference( const BigFloat2& a, const BigFloat2& b ) { 
  BigFloat2 out;
  out.sub( a, b );
  return out;
}

BigFloat2 product( const BigFloat2& a, const BigFloat2& b ) { 
  BigFloat2 out;
  out.mul( a, b );
  return out;
}

BigFloat2 quotient( const BigFloat2& a, const BigFloat2& b ) { 
  BigFloat2 out;
  out.div( a, b );
  return out;
}

bool intersects( const BigFloat2& a, const BigFloat2& b ) { 
  if ( a.is_exact() && b.is_exact() ) { 
    if ( a.getLeft() == b.getLeft() ) return true;
    return false;  
  }
  else { 
    if ( a.getLeft() >= b.getRight() ) return false;
    if ( b.getLeft() >= a.getRight() ) return false;
    return true;
  }
  return false;
}

bool exact( const IntervalType& I ) { 
  return I.is_exact();
}


std::ostream& ivlf ( std::ostream& o, BigFloat2 I ) { 
  if ( I.is_exact() ) 
    return o << "[ " << I.getLeft() << " ]" ;
  else 
    return o << "( " << I.getLeft() << ", " << I.getRight() << " )";
}





// Moebius Transform
// [a/c, b/d] -> [0, \infty] 
// TODO: requires valid cast/constructor from BigInt to NT 
// TODO: is it an error condition if interval is exact? 

template< class NT > 
Polynomial<NT> moebiusTransform( const Polynomial< NT > &P,
				 const NT& ai,
				 const NT& bi,
				 const NT& ci,
				 const NT& di ) { 
  Polynomial<NT> PM_k; 

  //construct coefficient arrays
  NT dxpc_coeff[2];
  NT bxpa_coeff[2];

  bxpa_coeff[0] = ai;
  bxpa_coeff[1] = bi;

  dxpc_coeff[0] = ci;
  dxpc_coeff[1] = di; 

  //construct binary expressions
  Polynomial<NT> bxpa = Polynomial<NT>( 1, bxpa_coeff ); //bX + a 
  Polynomial<NT> dxpc = Polynomial<NT>( 1, dxpc_coeff ); //dX + c

  //compose binary expansion
  PM_k = composeBinary( P, bxpa, dxpc );

  //done
  return PM_k;

}

template< class NT > 
Polynomial<NT> moebiusTransform( const Polynomial< NT >& P, const IntervalType& I ) { 

  //left = a/c
  Rational lrat = leftRat(I);
  NT ai = lrat.num();  
  NT ci = lrat.den(); 

  //right = b/d
  Rational rrat = rightRat(I);
  NT bi = rrat.num(); 
  NT di = rrat.den();

  return moebiusTransform( P, ai, bi, ci, di );  

}


// Descartes' Rule
// upper limit for # roots of P in (0, \infty)
// counts # sign changes in coefficients of P 

template< class NT > 
unsigned int descartesBound( const Polynomial< NT >& P ) { 
  
  int i, deg_P;
  unsigned int maxRoots = 0;
  NT last_ci = 0;
  
  deg_P = P.getTrueDegree();
  
  for ( i = 0 ; i <= deg_P; i++ ) { 
    
    NT ci = P.getCoeff(i);
    
    if ( ci == 0 ) continue; //ignore zeroes
    
    if ( ci > 0 ) { 
      if ( last_ci < 0 ) 
	maxRoots++;
    }
    else if ( ci < 0 ) { 
      if ( last_ci > 0 ) 
	maxRoots++;
    }
    last_ci = ci ;
  }
  
  return maxRoots;

}

// Interval Root Bound
// Using Moebius and Descarte's Rule ( above ) 
// returns max roots of P in open interval I = (a, b);

template< class NT > 
unsigned int intervalRootBound( const Polynomial< NT >& P, const IntervalType& I ) { 

  Polynomial<NT> PM_k = moebiusTransform ( P, I ); //transform P-> PM_k;
  return descartesBound( PM_k );                   //get root bound of PM_k;

}


// ISOLATING INTERVALS

// TODO: if bound = 0, inserts exact intervals
// else, inserts intervals of dim 'bound' around exact roots 
// where bound should be an appropriate value. 
// this bound will also limit recursion

template< class NT >
unsigned int findIsolatingIntervals ( const Polynomial< NT >& P, 
				      const IntervalType& I, 
				      vector< IntervalType >& out,
				      const IntervalValue& bound = 0 ) { 

  int maxRoots = intervalRootBound( P, I );

  if ( maxRoots == 0 ) return 0;             //no roots - done
  if ( maxRoots == 1 ) out.push_back( I ); //one root - done
  else { 

    IntervalType        I_left, I_right;
    IntervalValue mid = split( I, I_left, I_right );  

    if ( P.eval(mid) != 0 ) { //search left and right 

      findIsolatingIntervals( P, I_left, out, bound ); 
      findIsolatingIntervals( P, I_right, out, bound );

    }
    else if ( bound == 0 ) { //midpoint with exact interval

      IntervalType middle = IntervalType( mid, mid );
      middle.set_exact(true);

      findIsolatingIntervals( P, I_left,  out,  bound ); 
      out.push_back( middle );
      findIsolatingIntervals( P, I_right, out,  bound );     

    }
    else if ( I.getLeft() - I.getRight() >= bound ) {  //midpoint with open interval
 
      I_left  = ( I_left.getLeft(), mid - bound * 0.5) ;
      I_right = ( mid + bound * 0.5, I_right.getRight() );
      
      IntervalType middle = IntervalType( I_left.getRight(), I_right.getLeft() );

      findIsolatingIntervals( P, I_left,  out,  bound ); 
      out.push_back( middle );
      findIsolatingIntervals( P, I_right, out,  bound );

    }
  }

  return 0;

}

// add isolating intervals to a vector of intervals 
// return a count of how many were found 

template< class NT > 
unsigned int isolatingIntervals ( const Polynomial< NT >& P, 
				  const IntervalType& I, 
				  vector< IntervalType >& out,
				  const IntervalValue& bound = 0) { 
  int tmp = out.size();
  findIsolatingIntervals( P, I, out, bound );
  return out.size()-tmp;

}


#endif
