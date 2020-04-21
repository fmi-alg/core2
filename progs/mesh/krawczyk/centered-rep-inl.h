/*
 * default-functions-inl.h
 *
 *
 * Synopsis:
 *
 * 	ConstructCentered(xrange, yrange, f, g, S)
 * 		and sets the structure S 
 * 		to contain the Jacobian of the functions 
 *
 * 		f(x-x0, y-y0), g(x-x0, y-y0)
 *
 * 		where (x0,y0) is the mid point of the box (xrange, yrange).
 *
 * 	The structure S is of type CenterRepT.
 *
 *
 *  Created on: Jun 1, 2010
 *      Author: narayan
 */

#ifndef KRAWCZYK_DEFAULT_FUNCTIONSINL_H_
#define KRAWCZYK_DEFAULT_FUNCTIONSINL_H_

#include <vector>
#include <utility>

#include "krawczyk-defs.h"


template <typename NT>
struct CenterRepT {
  IntervalT<NT> x_mid;
  IntervalT<NT> y_mid;
  BiPoly<NT> centered_f;
  BiPoly<NT> centered_g;
  MatrixT<BiPoly<NT> > jacobian;

  CenterRepT() : jacobian(MatrixT<BiPoly<NT> >(2)) {}
};

template <typename NT>
void ConstructCentered(const IntervalT<NT> &x_range,
    const IntervalT<NT> &y_range,
    const BiPoly<NT> &f, const BiPoly<NT> &g,
    CenterRepT<NT> *rep) {
  NT x_mid = x_range.mid();
  NT y_mid = y_range.mid();
  rep->x_mid = IntervalT<NT>(x_mid);
  rep->y_mid = IntervalT<NT>(y_mid);

  // Construct the polynomials
  // h(x,y) = mid_x - x
  // g(x,y) = mid_y - y
  //
  // g(x, y) is straightforward, construct a univariate
  // polynomial and transform it to a bivariate polynomial
  // with no "x" terms.
  Polynomial<NT> py(1);
  py.coeff()[0] = y_mid;
  py.coeff()[1] = -1;
  BiPoly<NT> y_transform(py, true);

  // h(x, y), construct a univariate polynomial in x and
  // this will be the constant term.
  Polynomial<NT> px(1);
  px.coeff()[0] = x_mid;
  px.coeff()[1] = -1;
  vector<Polynomial<NT> > coeffs;
  coeffs.push_back(px);
  BiPoly<NT> x_transform(coeffs);

  // The centered forms are compositions of the original
  // polynomial with h(x,y) and g(x,y)
  rep->centered_f = composeHorner<NT>(
      BiPoly<NT>(f), x_transform, y_transform);
  rep->centered_g = composeHorner<NT>(
      BiPoly<NT>(g), x_transform, y_transform);

  BiPoly<NT> tmp = rep->centered_f, tmp1 = rep->centered_f;
  rep->jacobian(0, 0) = tmp.differentiateX();
  rep->jacobian(0, 1) = tmp1.differentiateY();
  tmp = rep->centered_g, tmp1 = rep->centered_g;
  rep->jacobian(1, 0) = tmp.differentiateX();
  rep->jacobian(1, 1) = tmp1.differentiateY();
}

#endif /* DEFAULTFUNCTIONSINL_H_ */
