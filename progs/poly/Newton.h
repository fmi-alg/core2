/* **************************************
   File: Newton.h

   Synopsis: 
	Class Newton supports Newton iteration for polynomial roots
	This is based on the Polynomial class

   Author:  Chee Yap and Sylvain Pion
   Date:    July 20, 2002

   Since Core Library  v1.4
   $Id: Newton.h,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */

#ifndef _CORE_NEWTON_H
#	define _CORE_NEWTON_H

#define CORE_LEVEL 4

#include "CORE.h"

// ==================================================
// Newton Class
// NOTE: if bad input, return -1.
// ==================================================

template < class NT >
class Newton {
public:
  Polynomial<NT> p;	// this is the polynomial being iterated
  Polynomial<NT> q;	// this is derivative of p
  BigFloat val;		// curr approximate value
  BigFloat del;		// difference from previous value
  BigFloat olddel;	// old difference from previous value
  int count;		// counter for number of iterations
  extLong uMSB;		// this is del.uMSB() when you are in the
  			//   Newton zone, otherwise, you need some
			//   other means of estimating how close you are
			//   to the root
  static int N_STOP_ITER; // Stop IterE after this many iterations
  			// This is initialized below, outside the Newton class 

  //=======================================================
  // CONSTRUCTORS
  //=======================================================

  Newton(Polynomial<NT> pp, BigFloat e = 1):
     p(pp), val(e), del(e), olddel(0), count(0)
  {
    q = differentiate(pp);
    uMSB = e.uMSB();
  }

  //=======================================================
  // METHODS
  //=======================================================

  // Resetting the state of Newton object:

  void reset( BigFloat v = 1) {
    val=v; del=v;
    olddel=0; count=0;
  }

  // Iterate n times:

  BigFloat IterN(int n = 1) {
    for (int i=0; i<n; i++) {
	olddel = del;
	// Expr e(val);
	// compute to absolute precision of -uMSB
	// del = p.eval(e).approx(CORE_INFTY,-uMSB).getBigFloat().makeExact()/
	//         q.eval(e).approx(CORE_INFTY,-uMSB).getBigFloat().makeExact();

	del = p.eval(val).makeExact()/q.eval(val).makeExact();

	del.makeExact();
	val -= del;		// this subtraction introduces error!
	val.makeExact();	// May 15, 2003: fixed a bug here (Chee/Vikram)
	uMSB = del.uMSB();
	count ++;
    } 
    return val;
  }

  // iterate until epsilon step size (i.e., epsilon < 2^{-e})

  BigFloat IterE(int prec){		// usually, prec is positive
    count = 0;
    do { IterN(); 
    } while ((uMSB >= -prec) && (count < N_STOP_ITER)) ;
				// N_STOP_ITER to prevent infinite loops
    return val;
  }

// ==================================================
// Test Routine
// ==================================================
  // testN: 	iterate n times
  BigFloat testN( int n){
    return IterN(n);
  }

  // testE: 	iterate until less than Epsilon =2^{-prec} change
  BigFloat testE(int prec){	// prec is normally >0
    return IterE(prec);
  }

};// Newton class

// ==================================================
// Static initialization
// ==================================================
template <class NT>
  int Newton<NT>:: N_STOP_ITER = 10000;	// stop IterE after this many loops
  					// Reset this as needed

#endif

