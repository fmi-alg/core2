#ifndef __COMPOSE_BIPOLY_H_INCLUDED__
#define __COMPOSE_BIPOLY_H_INCLUDED__

/*

   File: composeBiPoly.h

   Author : Philip Davidson

   Description : composition of bivariate polynomials

*/

#include "CORE/poly/Poly.h"
#include "CORE/poly/Curves.h"
 

// Bivariate = Univariate( Bivariate ) 

// standard composition, for P = c_i X^i
template < class NT >
BiPoly<NT> composeNaive ( const Polynomial<NT>& P, const BiPoly<NT>& X ) { 

  BiPoly<NT> P_X;
  int i, deg_P;
  NT ci;
 
  deg_P = P.getTrueDegree();
  
  for ( i = 0 ; i <= deg_P ; i++ ) { 

    ci = P.getCoeff(i);      
    if ( ci == 0 ) continue;  // ! requires == 0 defined for NT .. ?isZero?

    BiPoly<NT> pKi = X;         // maybe move pKi outside and reuse, if assignment clears properly
    pKi.pow(i);
    P_X += pKi.mulScalar(ci);

  }

  return P_X;

}



// Bivariate = Univariate( Bivariate ) 

// standard composition, for P = c_i X^i
template < class NT >
BiPoly<NT> composeHorner ( const Polynomial<NT>& P, const BiPoly<NT>& X ) { 

  int i, deg_P;
  NT ci;
 
  deg_P = P.getTrueDegree();
  if ( deg_P < 0 ) { return BiPoly<NT>(); } //return zero polynomial

  int cd = 0;
  ci = P.getCoeff(deg_P);

  BiPoly<NT> P_X = BiPoly<NT>( 0, &cd, &ci );

  for ( i = deg_P-1 ; i >= 0 ; --i ) { 
    P_X *= X;
    ci = P.getCoeff(i);      
    P_X += BiPoly<NT>( 0, &cd, &ci );
  }

  return P_X;

}

template < class NT >
BiPoly<NT> compose ( const Polynomial<NT>& P, const BiPoly<NT>& X ) { 
  return P(X);
}


// compose sequence c_i A^i B^{deg-i}, using c_i of P 
// used in Moebius transform

template < class NT >
BiPoly<NT> composeBinary ( const Polynomial<NT>& P, const BiPoly<NT>& A, const BiPoly<NT>& B ) { 

  BiPoly<NT> P_AB;
  int i, deg_P;
  NT ci;
  
  deg_P = P.getTrueDegree();

  for ( i = 0 ; i <= deg_P ; i++ ) { 

    ci = P.getCoeff(i);      
    if ( ci == 0 ) continue;

    BiPoly<NT> pKA = A;
    BiPoly<NT> pKB = B;
    pKA.pow(i);
    pKB.pow(deg_P-i);
    pKA *= pKB;
    P_AB += pKA.mulScalar(ci);

  }

  return P_AB;

}


#include "composePoly.h"


// Bivariate from Bivariate 
template< class NT >
BiPoly<NT> composeNaive ( const BiPoly<NT>& P, const BiPoly<NT>& A, const BiPoly<NT>& B ) { 

  BiPoly<NT> P_XY;

  //P is a Bivariate Polynomial
  //  cerr << "deg(y)" << P.getYdegree() << endl;

  int deg_P = P.getYdegree();

  Polynomial<NT> ci;
  
  for ( int i = 0 ; i <= deg_P ; i++ ) { 

    ci = P.getCoeffY( i );                     //x-Polynomial coefficient
    //    cerr << "i: " << i << "->" << ci << endl;
    if ( ci.getTrueDegree() == -1 ) continue;  //zero-case

    // xCi = ci(A);
    // bivariate = univariate( bivariate )
    BiPoly<NT> xCi = composeNaive( ci, A );         //as above


    // yCi = B^i
    BiPoly<NT> yCi = B;                        //init with Y-argument
    yCi.pow(i);                                //raise to power

    // yCi = ci(A) B^i
    // multiply the two BiPoly
    yCi  *= xCi; 


    //accumulate in result;
    P_XY += yCi; 

  }  

  return P_XY;

}



// Bivariate from Bivariate 
template< class NT >
BiPoly<NT> composeHorner ( const BiPoly<NT>& P, const BiPoly<NT>& A, const BiPoly<NT>& B ) { 


  //P is a Bivariate Polynomial

  int deg_P = P.getYdegree();

  if ( deg_P < 0 ) return BiPoly<NT>(); //return zero polynomial

  Polynomial<NT> ci;
  
  std::vector< Polynomial<NT> > civ;
  civ.push_back( P.getCoeffY(deg_P) );

  BiPoly<NT> P_XY = compose( civ[0], A );

  for ( int i = deg_P - 1 ; i >= 0 ; --i ) { 

    P_XY *= B;

    civ[0] = P.getCoeffY( i );                     //x-Polynomial coefficient
    if ( civ[0].getTrueDegree() == -1 ) continue;
    P_XY += compose( civ[0], A );

  }  

  return P_XY;

}


// Bivariate from Bivariate 
template< class NT >
BiPoly<NT> compose ( const BiPoly<NT>& P, const BiPoly<NT>& A, const BiPoly<NT>& B ) { 
  return P( A, B );
}


#endif
