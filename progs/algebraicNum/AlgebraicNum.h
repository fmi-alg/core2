#ifndef __ALGEBRAIC_NUM_H_INCLUDED__
#define __ALGEBRAIC_NUM_H_INCLUDED__

//Algebraic Number Class

#include "composeBiPoly.h"
#include "descartes.h"


//proper resultants for arithmetic operations

template< class NT >
Polynomial<NT> algebraicSum( const Polynomial<NT>& A , const Polynomial<NT>& B ) { 
 
  BiPoly<NT> AY   = BiPoly<NT>( A, true );
  BiPoly<NT> XmY  = BiPoly<NT>( "x - y" );

  // compose ( B, XmY );
  BiPoly<NT> BXmY = B( XmY ); 

  return resY( AY, BXmY );

}

template< class NT >
Polynomial<NT> algebraicDifference( const Polynomial<NT>& A , const Polynomial<NT>& B ) { 

// "x + y" returns \beta_j - \alpha_i
// using "y - x" returns \alpha - \beta, which is our goal

  BiPoly<NT> AY   = BiPoly<NT>( A, true ); //make A(X) poly in Y
  BiPoly<NT> YmX  = BiPoly<NT>( "y - x" ); 

  //compose( B, YmX ); 
  BiPoly<NT> BXpY = B( YmX );

  return resY( AY, BXpY );

}

template< class NT >
Polynomial<NT> algebraicProduct( const Polynomial<NT>& A , const Polynomial<NT>& B ) { 

  BiPoly<NT> AY   = BiPoly<NT>( A, true );
  BiPoly<NT> Yn_B_XdivY = composeBinary( B, BiPoly<NT>("x"), BiPoly<NT>("y") ); 
  return resY( AY, Yn_B_XdivY );

}

template< class NT >
Polynomial<NT> algebraicQuotient( const Polynomial<NT>& A , const Polynomial<NT>& B ) { 
  
  BiPoly<NT> AY   = BiPoly<NT>( A, true );
  BiPoly<NT> Xn_B_YdivX = composeBinary( B, BiPoly<NT>("y"), BiPoly<NT>("x") ); 
  return resY( AY, Xn_B_YdivX );

}


// Algebraic Number Class

template< class NT >
class AlgebraicNum { 
public:

  Polynomial<NT>     poly;
  IntervalType ival; //isolating interval

  AlgebraicNum() {}
  AlgebraicNum( Polynomial<NT> p, IntervalType i ) { 
    poly = p; 
    setInterval( i );
  }    
  AlgebraicNum( const AlgebraicNum & b ) { 
    poly = b.poly; 
    ival = b.ival;
  }

  AlgebraicNum( NT c ) { 
    NT pcoeff[2];
    pcoeff[0] = -c;
    pcoeff[1] = 1;
    poly = Polynomial<NT>( 1, pcoeff );
    ival = IntervalType( c, c ); ival.set_exact(true);
  } 

  bool exact() const { return ( ival.is_exact() ); }

  bool valid() const { 
    if ( poly.getTrueDegree() <= 0 ) 
      return false; 

    if ( exact() ) 
      return ( poly.eval( ival.getLeft() ) == 0 );

    return ( intervalRootBound( poly, ival ) == 1 ); 
  }

  int has_solution( const IntervalType& IV ) { 
    if ( IV.is_exact() ) return ( poly.eval( IV.getLeft() ) == 0 ) ? 1 : 0 ;
    return intervalRootBound( poly, IV );
  }

  bool validInterval ( const IntervalType& IV ) { 
    if ( IV.is_exact() ) 
      return ( poly.eval( IV.getLeft() ) == 0 );

    return ( intervalRootBound( poly, IV ) == 1 ); 
  }

  //solution at zero, not zero polynomial
  bool isZero() const { 
    return ( exact() && ival.has_zero() == 0 ); 
  }
 
  void refine( IntervalType& I ) const { 
    IntervalType li, ri;
    IntervalValue mi = split ( I, li, ri );
    if ( poly.eval(mi) == 0 ) { 
      I.set( IntervalType( mi, mi ) );
      I.set_exact(true);
    }
    else { 
      if ( intervalRootBound( poly, li ) == 1 ) I = li;
      else if ( intervalRootBound( poly, ri ) == 1 ) I = ri;
      else { 
	cerr << "error! : refinement failed\n";
	cerr << poly;
	ivlf( cerr, I ) << endl;
      }
    }
  }   

  void precPrint( std::ostream& o, BigFloat eps ) const { 
    if ( !valid() ) { 
      o << "invalid value";
      return;
    }
    IntervalType ri = ival;
    while ( ri.getRight() - ri.getLeft() > eps ) {  //TODO: BigFloat2 - size accessor? 
      refine( ri ); 
    } 
    ri.centerize();
    o << ri.getLeft() ;
  }

  // NOTE : 
  // should be invariant that a valid interval
  // is either exactly zero, or does not contain zero;

  void checkZero( ) { 
    BigFloat z = 0;
    if ( ival.has_zero() ) { 
      if ( ival.is_exact() ) return;
      if ( poly.eval(z) == z ) { ival.set_zero(); } // if it's zero, it's zero
      else { 
	while ( ival.has_zero() ) 
	  refine( ival ); //otherwise we can refine until it doesn't
      }
    }
  }

  void setInterval( const IntervalType &i ) { 
    ival = i;
    checkZero();
  }

  AlgebraicNum operator + (const AlgebraicNum& b ) const { 

    AlgebraicNum out;

    out.poly = algebraicSum( poly, b.poly );
    out.poly.sqFreePart();

    //test degree of result ( must be > 0 !? ) 
    if ( out.poly.getTrueDegree() <= 0 ) { 
      cerr << "error, result is invalid ( degree <= 0 )" << endl;
      return out;
    }

    IntervalType iva = ival;
    IntervalType ivb = b.ival;

    //interval 
    IntervalType ivo = sum( iva, ivb );

    //refine until ivo is exact, or solution is isolated 
    while ( !ivo.is_exact() && out.has_solution( ivo ) > 1 ) { 
      refine( iva );
      b.refine( ivb );
      ivo = sum ( iva, ivb );
    }

    //final assignment
    out.setInterval( ivo );

    // sanity ( should be assertion )
    if ( !out.valid() ) { 
      cerr << "error, invalid result!" << endl;
      return AlgebraicNum();
    }

    return out;

  }

  AlgebraicNum operator - (const AlgebraicNum& b ) const { 

    AlgebraicNum out;

    out.poly = algebraicDifference( poly, b.poly );
    out.poly.sqFreePart();

    //test degree of result ( must be > 0 !? ) 
    if ( out.poly.getTrueDegree() <= 0 ) { 
      cerr << "error, result is invalid ( degree <= 0 )" << endl;
      return out;
    }

    IntervalType iva = ival;
    IntervalType ivb = b.ival;

    //interval 
    IntervalType ivo = difference( iva, ivb );

    //refine until ivo is exact, or solution is isolated 
    while ( !ivo.is_exact() && out.has_solution( ivo ) > 1 ) { 
      refine( iva );
      b.refine( ivb );
      ivo = difference ( iva, ivb );
    }

    //final assignment
    out.setInterval( ivo );

    // sanity ( should be assertion )
    if ( !out.valid() ) { 
      cerr << "error, invalid result!" << endl;
      return AlgebraicNum();
    }

    return out;

  }

  AlgebraicNum operator * (const AlgebraicNum& b ) const { 

    AlgebraicNum out;

    out.poly = algebraicProduct( poly, b.poly );
    out.poly.sqFreePart();


    //test degree of result ( must be > 0 !? ) 

    if ( out.poly.getTrueDegree() <= 0 ) { 
      cerr << "error, result is invalid ( degree <= 0 )" << endl;
      return out;
    }

    IntervalType iva = ival;
    IntervalType ivb = b.ival;

    //interval 
    IntervalType ivo = product( iva, ivb );

    //refine until ivo is exact, or solution is isolated 
    while ( !ivo.is_exact() && out.has_solution( ivo ) > 1 ) { 
      cerr << "r(" << out.has_solution(ivo) << ")" << endl;
      refine( iva );
      b.refine( ivb );
      ivo = product ( iva, ivb );
    }

    //final assignment
    out.setInterval( ivo );

    // sanity ( should be assertion )
    if ( !out.valid() ) { 
      cerr << "error, invalid result!" << endl;
      return AlgebraicNum();
    }

    return out;
  }

  AlgebraicNum operator / (const AlgebraicNum& b ) const { 

    AlgebraicNum out;

    if ( b.isZero() ) { 
      cerr << "error, Division by Zero!" << endl;
      return out;
    }

    out.poly = algebraicQuotient( poly, b.poly );
    out.poly.sqFreePart();

    //test degree of result ( must be > 0 !? ) 

    if ( out.poly.getTrueDegree() <= 0 ) { 
      cerr << "error, result is invalid ( degree <= 0 )" << endl;
      return out;
    }


    IntervalType iva = ival;
    IntervalType ivb = b.ival;

    //interval 
    IntervalType ivo = quotient( iva, ivb );

    //refine until ivo is exact, or solution is isolated 
    while ( !ivo.is_exact() && out.has_solution( ivo ) > 1 ) { 
      refine( iva );
      b.refine( ivb );
      ivo = quotient ( iva, ivb );
    }

    //final assignment
    out.setInterval( ivo );

    // sanity ( should be assertion )
    if ( !out.valid() ) { 
      cerr << "error, invalid result!" << endl;
      return AlgebraicNum();
    }

    return out;

  }

  bool operator == ( const AlgebraicNum & b ) const { 

    // A and B share a common root, 
    // since roots of GCD is set intersection of roots of this and b 
    // if either interval contains a root of gcd, it is equal to our value.
    // if a shared root exists in both intervals 
    // this is sufficient for equality

    if ( !intersects( ival, b.ival ) ) return false; //intervals must overlap, easy return

    Polynomial<NT> gcdAB = gcd ( poly, b.poly );             //common roots of this, b
    if ( gcdAB.getTrueDegree() == 0 ) return false; // gcd must be non-constant

    //interval contains a shared root
    if ( ival.is_exact() ) { 
      if ( gcdAB.eval( ival.getLeft() ) != 0 ) 
	return false; 
    }
    else 
      if ( intervalRootBound( gcdAB, ival ) != 1 ) 
	return false;

    //b.interval contains a shared root
    if ( b.exact() ) { 
      if ( gcdAB.eval( b.ival.getLeft() ) != 0 ) { 
	return false;
      }
    }
    else if ( intervalRootBound( gcdAB, b.ival ) != 1 ) { 
      return false;
    }
    return true;
  }

  int sign() { 
    if ( exact() ) { 
      if ( ival.getLeft() == 0 ) return 0;
      else if ( ival.getLeft() > 0 ) return 1 ;
      else return -1 ;
    }
    else { 
      if ( ival.getLeft() > 0 ) return 1;
      else return -1;
    }
    return 0;
  }
  
  bool operator < ( const AlgebraicNum& b ) const { 
    if ( exact() && b.exact() ) {  //exact-exact case
      if ( ival.getLeft() < b.ival.getLeft() ) return true;
      return false;
    }
    else if ( !intersects( ival, b.ival ) ) { //non-intersecting intervals
      if ( ival.getRight() <= b.ival() ) return true;
      return false;
    }
    // sign of difference
    AlgebraicNum diff = *this - b;
    if ( diff.sign() < 0 ) 
      return true;
  }

  AlgebraicNum& operator += ( const AlgebraicNum& b ) { 
    AlgebraicNum sum = *this + b;
    *this = sum;
    return *this;
  }

  AlgebraicNum& operator *= ( const AlgebraicNum& b ) { 
    AlgebraicNum prod = *this * b;
    *this = prod;
    return *this;
  }

}; // end AlgebraicNum type


template< class NT >
std::ostream& operator << ( std::ostream& o, const AlgebraicNum<NT>& A ) {
  BigFloat epss = pow( 0.5, 30 );
  o << A.poly << endl;
  if ( A.exact() ) { 
    return ivlf( o, A.ival ) << ( ( A.exact() ) ? "(exact)" : "" ) << endl;
  }
  else { 
    ivlf( o, A.ival ) << " -> ";
    A.precPrint( o, epss );
    return o << endl;
  }
}

template< class NT >
void precPrint( std::ostream& o, const Polynomial<NT>& p, const IntervalType& i, BigFloat eps ) { 
  AlgebraicNum<NT> a = AlgebraicNum<NT>( p, i );
  a.precPrint( o, eps );
}


#endif
