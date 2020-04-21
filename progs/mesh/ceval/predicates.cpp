/*
 * file:  predicates.cpp
 *
 *  Created on: Jun 22, 2010
 *      Author: Narayan Kamath
 *      Since Core 2.1.
 */

#include <fenv.h>
#include <iostream>

#include "predicates.h"

// Initialize the predicates with the given polynomial.
Predicates::Predicates(const Polynomial<PolyType> &poly) :
    degree_(poly.getTrueDegree()) {
  Init(poly);
}

bool Predicates::PointTest(const Complex &m, const double &r) const {
  // Contains the results of the evaluation of
  // f at the 8 compass points.
  Complex results[8];
  EvaluateAtCompassPoints(results, m, r);

  vector<unsigned int> u_crossings;
  vector<unsigned int> v_crossings;

  // Perform the sign calculation at each at the
  // compass points.
  for (unsigned int i = 0; i < 8; ++i) {
    const Complex &start = results[i];
    const Complex &end = results[(i + 1) % 8];

    if (start.re() == 0 || start.re() * end.re() < 0) {
      u_crossings.push_back(i);
    }
    if (start.im() == 0 || start.im() * end.im() < 0) {
      v_crossings.push_back(i);
    }
  } 
 
  // There must be exactly two u crossings and two v crossings.
  if (u_crossings.size() != 2 || v_crossings.size() != 2) {
    return false;
  }

  if (u_crossings[0] < v_crossings[0]) {
    return ((v_crossings[0] < u_crossings[1]) &&
        (u_crossings[1] < v_crossings[1]));
  } else if (v_crossings[0] < u_crossings[0]) {
    return ((u_crossings[0] < v_crossings[1]) &&
        (v_crossings[1] < u_crossings[1]));
  }

  // This is the case that u_crossings[0] == v_crossings[0
  return false;
}

template <typename NT> void EvaluateOrdinals(
    const Polynomial<PolyType> &f,
    const ComplexT<NT> &m,
    const NT &r,
    ComplexT<NT> *results) {
  // Note that this is in general inexact. Refer to the
  // template specialization for Level 2 in terms of BigRat below
  // which will in general be exact.
  const Complex NE(machine_double(20)/29, machine_double(21)/29);
  const Complex NW(machine_double(-20)/29, machine_double(21)/29);
  const Complex SW(machine_double(-20)/29, machine_double(-21)/29);
  const Complex SE(machine_double(20)/29, machine_double(-21)/29);

  results[1] = f.eval<Complex>(m + (NE * r));
  results[3] = f.eval<Complex>(m + (NW * r));
  results[5] = f.eval<Complex>(m + (SW * r));
  results[7] = f.eval<Complex>(m + (SE * r));
}

// Calculate the modulus  of the given complex number, round
// either towards zero or away from zero.
template <typename NT> NT RoundedMod(const ComplexT<NT> &complex,
		                             bool round_towards_minf) {
  return complex.mod();
}

template <> DoubleWrapper RoundedMod(const ComplexT<DoubleWrapper> &complex,
		                             bool round_towards_minf) {
  int current_round = fegetround();
  if (round_towards_minf) {
	fesetround(FE_DOWNWARD);
  } else {
	fesetround(FE_UPWARD);
  }

  DoubleWrapper mod = complex.mod();
  fesetround(current_round);

  return mod;
}

/*
 * The statement below contains a body of code to operate
 * at Level 2. In particular, it rounds terms correctly to
 * ensure that the algorithm has no implementation gaps.
 */
#if CORE_LEVEL == 2 || CORE_LEVEL >= 4
template <> BigFloat RoundedMod(const ComplexT<BigFloat> &complex,
		                        bool round_towards_minf) {
  BigFloat mod;
  if (round_towards_minf) {
    mod.sqrt((complex.re() * complex.re() + complex.im() * complex.im()),
        CORE::getDefaultBFradicalPrec(), GMP_RNDD);
  } else {
    mod.sqrt((complex.re() * complex.re() + complex.im() * complex.im()),
         CORE::getDefaultBFradicalPrec(), GMP_RNDU);
  }

  return mod;
}

template <> void EvaluateOrdinals(
    const Polynomial<PolyType> &f,
    const ComplexT<BigFloat> &m_bf,
    const BigFloat &r_bf,
    ComplexT<BigFloat> *results) {
  const ComplexT<BigRat> m(BigRat(m_bf.re()), BigRat(m_bf.im()));
  const BigRat r = r_bf;

  const ComplexT<BigRat> NE(BigRat(20, 29), BigRat(21, 29));
  const ComplexT<BigRat> NW(BigRat(-20, 29), BigRat(21, 29));
  const ComplexT<BigRat> SW(BigRat(-20, 29), BigRat(-21, 29));
  const ComplexT<BigRat> SE(BigRat(20, 29), BigRat(-21, 29));

  ComplexT<BigRat> array[8];

  array[1] = f.eval<ComplexT<BigRat> >(m + (NE * r));
  array[3] = f.eval<ComplexT<BigRat> >(m + (NW * r));
  array[5] = f.eval<ComplexT<BigRat> >(m + (SW * r));
  array[7] = f.eval<ComplexT<BigRat> >(m + (SE * r));


  for (unsigned int i = 1; i <=7; i+=2) {
    int real_sign = sign(array[i].re());
    int imag_sign = sign(array[i].im());
    results[i] = ComplexT<BigFloat>(real_sign, imag_sign);
  }
}

// NOTE: this is the BigFloat version of CalculateFactTerms
void CalculateFactTerms(const int degree,
    vector<BigFloat> *terms_u,
    vector<BigFloat> *terms_d) {
  terms_u->reserve(degree);
  terms_d->reserve(degree);
  for (int i = 0; i < degree; ++i) {
    terms_u->push_back(0);
    terms_d->push_back(0);
  }

  (*terms_u)[degree-1] = 1;
  (*terms_d)[degree-1] = 1;

  for (int i = 1; i < degree; ++i) {
    BigFloat prev = (*terms_u)[degree-1-(i - 1)];
    (*terms_u)[degree-1 - i].r_mul(prev, (degree - i+ 1), GMP_RNDU);
    // cout << "Set : " << (*terms)[degree -1 - i] << endl;
  }

  for (int i = 1; i < degree; ++i) {
    BigFloat prev = (*terms_d)[degree-1-(i - 1)];
    (*terms_d)[degree-1 - i].r_mul(prev, (degree - i+ 1), GMP_RNDD);
    // cout << "Set : " << (*terms)[degree -1 - i] << endl;
  }
}

#endif

#if CORE_LEVEL == 3 || CORE_LEVEL >= 4
void CalculateFactTerms(const int degree,
		                vector<Expr> terms,
		                Expr *n_fact) {
  cout << "Calculating factorial terms not supported for level 3 !!" << endl;
  assert(false);
}
#endif


// Modify this to use BigRat numbers for evaluating the
// points NE, NW, SW, SE. E, W, N , S will be dyadic
// for dyadic m and r.
void Predicates::EvaluateAtCompassPoints(Complex *results,
    const Complex &m, const double &r) const {
  const Polynomial<PolyType> &f = series_[0];
  // This is r + i(0), 0 + i(r) respectively, This
  // can be used to generate the value of f at the
  // 8 main compass points.
  const Complex r_real(r, 0), r_complex(0, r);

  // These 4 points are always dyadic,
  results[0] = f.eval<Complex>(m + r_real);
  results[2] = f.eval<Complex>(m + r_complex);
  results[4] = f.eval<Complex>(m - r_real);
  results[6] = f.eval<Complex>(m - r_complex);

  // This is handled differently for Level 1
  // and Level 2.
  EvaluateOrdinals(f, m, r, results);
}


// NOTE: this is the machine float version of CalculateFactTerms
//
// Invariant: 
//
//      terms_u[ k ] =  d! / (d-k+1)!   
//            for k=0,...,d-2     
//
// d = degree                                                                        
// term[k] = d * (d-1) * ... * (d-k+2)                                               
//         = d! / (d-k+1)!                                                           
// k ranges from 0 to d-2 inclusive                                                  
// for both the upper and lower series                                               
// see loop for i from 1 to d-1                                                      
// term[d-1]=1 
void CalculateFactTerms(const int degree,
		                vector<DoubleWrapper> *terms_u,
		                vector<DoubleWrapper> *terms_d) {
  int current_round = fegetround();

  // Calculate the factorial terms rounded upwards.
  terms_u->reserve(degree);
  terms_d->reserve(degree);
  // To ensure correct construction
  for (int i = 0; i < degree; ++i) {
    terms_u->push_back(0);
    terms_d->push_back(0);
  }
  (*terms_u)[degree-1] = 1;
  (*terms_d)[degree-1] = 1;

  fesetround(FE_UPWARD);
  for (int i = 1; i < degree; ++i) {
    DoubleWrapper prev = (*terms_u)[degree-1-(i - 1)];
    (*terms_u)[degree-1 - i] = prev*(degree - (i- 1));
  }

  // k! is always rounded upwards.
  fesetround(FE_DOWNWARD);
  for (int i = 1; i < degree; ++i) {
    DoubleWrapper prev = (*terms_d)[degree-1-(i - 1)];
    (*terms_d)[degree-1 - i] = prev*(degree - (i -1));
  }

/*
  for (int i = 0; i < degree; ++i) {
    cout << "i=" << i << ":" << (*terms_u)[i] << "," << (*terms_d)[i] << endl;
  }
*/
  fesetround(current_round);
}

// Calculate Tk(m, r) for a given value of k, m and r.
bool Predicates::Calculate(const double &k, const Complex &m, const double &r,
    const unsigned int start) const {
  Complex mod = series_[start - 1].eval<Complex>(m);

  // This is rounded towards zero.
  // This is not necessarily dyadic is it ?
  double mod_fm = RoundedMod<double>(mod, true);

  // The r_term.
  double r_term = r;
  // The overall RHS
  double R = 0;
  unsigned int fact_ctr = start - 1;

  for (unsigned int i = start; i < degree_ + 1; ++i) {
    // Terms on the RHS are rounded towards +inf.
    double term = RoundedMod<double>(series_[i].eval<Complex>(m), false);

    // Chee (June 2011):
    // ORIGINAL:
    // 		term = (term * r_term *fact_terms_u_[fact_ctr]);
    //
    // 		This multiplies the i-th Taylor coefficient by d!/(d-i+1)! .
    // 		After normalization (dividing by d!), the i-th Taylor coefficient
    // 		is divided by  (d-i+1)!   which seems wrong.   We want to
    // 		divide by i! instead.
    //
    // CORRECTION?  (NOT CONFIRMED)
    // 		term = (term * r_term *fact_terms_u_[degree_ - fact_ctr + 1]);
    //
    //		This multiplie the i-th Taylor coefficient by d!/(i+1)! .
    //		After normalization (dividing by d!), the i-th Taylor coefficient
    // 		is divided by  (i+1)! .   This is still slightly off.
    //
#if (ORIGINAL==0)
    term = (term * r_term *fact_terms_u_[fact_ctr]);
#else
    term = (term * r_term *fact_terms_u_[degree_ - fact_ctr]);
#endif

    R +=term;
    r_term *= r;
    ++fact_ctr;
  }

  unsigned int index = degree_ - fact_ctr;
  index += (start - 1);

  return (fact_terms_d_[index] * mod_fm) > (R * k);
}

//cout << "Evaluating at : " << m << endl;
//cout << "MOD = " << mod << endl;

//cout << "Fact : " << fact << endl;
//cout << "R-term : " << r_term << endl;
//cout << "RHS : " << R << endl;

// term = |fk(m)|

//cout << "Term : " << term << endl;
//cout << "RHS (+Term): " << R << endl;

//cout << "MOD FM : " << mod_fm << endl;
//cout << "factorial = " << fact_terms_d_[index] << endl;
/*
 *     //cout << "Fact term=" << fact_terms_u_[fact_ctr] << endl;
 *
  //cout << "-------" << endl;
  //cout << "Start=" << start << endl;
if (false) {
  cout << "R=" << R << endl;
  cout << "mod_fm=" << mod_fm << endl;
  cout << "k=" << k << ",start=" << start << endl;
}
*/



// Perform all the "preprocessing" , this is essentially computing
// all the derivatives
void Predicates::Init(const Polynomial<PolyType> &poly) {
  assert(degree_ >= 0);
  // Needed to counter the self modifying behaviour
  // of the differentiate operator.
  Polynomial<PolyType> temp;

  // degree + 1 because the first element is the polynomial
  // itself.
  series_ = new Polynomial<PolyType>[degree_ + 1];
  series_[0] = poly;

  // Performed the "obvious ways" , there are probably more
  // efficient ways to calculate all n derivatives at the same
  // time.
  for (unsigned int i = 1; i < degree_ + 1; ++i) {
    temp = series_[i - 1];
    temp.differentiate();
    series_[i] = temp;
  }

  CalculateFactTerms(degree_, &fact_terms_u_, &fact_terms_d_);
 
// CHECKing factTerms: (set this DEBUG flag in Makefile)
#ifdef DEBUG
  cout << "Checking FactTerms" << endl;
  for (unsigned int i = 0; i < degree_ ; i++)
      cout << setprecision(20) << "i = " << i << ": " << fact_terms_u_[i]
	  << ", " << fact_terms_d_[i] << endl;
#endif
}
