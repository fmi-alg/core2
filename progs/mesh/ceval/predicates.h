/*
 * file:  predicates.h
 *
 * DESCRIPTION:
 *	 The predicates required for the CEVAL algorithm.
 *	 There are three predicates.
 *	 	(i) 	Tk(r,m)
 * 		(ii) 	T'k(r,m)
 *		(iii) 8-point-test on disk Dr(m)
 *	These predicates are implicitly dependent on the polynomial
 *	f whose roots are being isolated. This polynomial is the input
 *	parameter for the constructor.
 *
 *	Tk(r,m) and T'k(r,m) are computed using a common function
 *
 *			Calculate(k, r, m, i)
 *
 *	where i=1 gives Tk, i=2 gives T'k.
 *
 *  Created on: Jun 21, 2010
 *      Author: Narayan Kamath
 *      Since Core 2.1.
 */

#ifndef PREDICATES_H_
#define PREDICATES_H_

#include <vector>

#include "ceval-defs.h"
#include "CORE/poly/Poly.h"

class Predicates {
 public:
  Predicates(const Polynomial<PolyType> &poly);
  // Tk(m, r)
  inline bool T(const double &k, const Complex &m,
      const double &r) const {
    return Calculate(k, m, r, 1);
  }
  // T'k(m, r)
  inline bool Tdash(const double &k, const Complex &m,
      const double &r) const {
    return Calculate(k, m, r, 2);
  }

  inline unsigned int GetDegree() const {
    return degree_;
  }

  // The 8 point test on the disk centered at m with
  // radius r.
  bool PointTest(const Complex &m, const double &r) const;
 private:
  // Modify this to use BigRat numbers for evaluating the
  // points NE, NW, SW, SE. E, W, N , S will be dyadic
  // for dyadic m and r.
  void EvaluateAtCompassPoints(Complex *results,
      const Complex &m, const double &r) const;
  // Calculate Tk(m, r) for a given value of k, m and r.
  bool Calculate(const double &k, const Complex &m, const double &r,
                 const unsigned int start) const;
  // Perform all the "preprocessing" , this is essentially computing
  // all the derivatives
  void Init(const Polynomial<PolyType> &poly);

  // This contains the (ordered) series f
  // f' , f'', f''' ..... , the last polynominal
  // in this series is always the constant polynomial.
  Polynomial<PolyType> *series_;

  // The degree of the complex polynomial.
  const unsigned int degree_;
  // Factorial terms for the RHS, rounded UPWARDS
  vector<double> fact_terms_u_;
  // The factorial term for the RHS rounded DOWNWARDS.
  vector<double> fact_terms_d_;
};


#endif /* PREDICATES_H_ */
