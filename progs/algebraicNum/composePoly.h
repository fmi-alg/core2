#ifndef __COMPOSE_POLY_H_INCLUDED__
#define __COMPOSE_POLY_H_INCLUDED__

/*

   File: composePoly.h

   Author : Philip Davidson

   Description : composition of polynomials

*/

#include "CORE/poly/Poly.h"


// Univariate = Univariate( Univariate ) 

// standard composition, for P = c_i X^i

template < class NT >
Polynomial<NT> composeNaive ( const Polynomial<NT>& P, const Polynomial<NT>& X ) { 

  Polynomial<NT> P_X;
  int i, deg_P;
  NT ci;
 
  deg_P = P.getTrueDegree();
  
  for ( i = 0 ; i <= deg_P ; i++ ) { 

    ci = P.getCoeff(i);      
    if ( ci == 0 ) continue;        // null term ! requires == 0 defined for NT .. ?isZero?

    // maybe move pKi outside and reuse, if assignment clears properly

    Polynomial<NT> pKi = X;         // initialize
    pKi.power(i);                   // generate term i
    P_X += pKi.mulScalar(ci);       // accumulate

  }

  return P_X;
  
}


// Univariate = Univariate( Univariate ) 

// standard composition, for P = c_i X^i

template < class NT >
Polynomial<NT> composeHorner ( const Polynomial<NT>& P, const Polynomial<NT>& X ) { 

  int i, deg_P;
  cerr << "degP " << P.degree() << " vs " << P.getTrueDegree() << endl;
  cerr << "degX " << X.degree() << " vs " << X.getTrueDegree() << endl;
  deg_P = P.getTrueDegree();
  if ( deg_P < 0 ) return Polynomial<NT>::polyZero();

  Polynomial<NT> P_X = Polynomial<NT>( P.getCoeff( deg_P ) );
  
  for ( i = deg_P-1 ; i >= 0 ; --i ) { 

    P_X *= X;
    P_X += Polynomial<NT>( P.getCoeff(i) );

  }

  return P_X;
  
}

// which should equal 

template < class NT >
Polynomial<NT> compose ( const Polynomial<NT>& P, const Polynomial<NT>& X ) { 
  return P(X);
}



template < class NT >
Polynomial<NT> composeBinary ( const Polynomial<NT>& P, const Polynomial<NT>& A, const Polynomial<NT>& B ) { 
  
  Polynomial<NT> P_AB;
  int i, deg_P;
  NT ci;
 
  deg_P = P.getTrueDegree();

  for ( i = 0 ; i <= deg_P ; i++ ) { 

    ci = P.getCoeff(i);      
    if ( ci == 0 ) continue;

    Polynomial<NT> pKA = A;    //initialize
    Polynomial<NT> pKB = B;
    pKA.power(i);              //generate term i
    pKB.power(deg_P-i);
    pKA *= pKB;                
    P_AB += pKA.mulScalar(ci); //accumulate

  }

  return P_AB;
 
}

#endif
