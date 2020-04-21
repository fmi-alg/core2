/*
 * rounded-ia.h
 *
 *  Created on: 3 Aug 2010
 *      Author: narayan
 */

#ifndef ROUNDEDIA_H_
#define ROUNDEDIA_H_

#include <fenv.h>
#include "CORE/IntervalT.h"

// This namespace defines rounded division for intervals.
namespace rounded {

// Definition of the standard division operation
// (without extended interval arithmetic).
template <typename NT>
inline IntervalT<NT> Divide(const IntervalT<NT> &a,
		             const IntervalT<NT> &b) {
  return a/b;
}

// Specialization for the machine precision type.
template <>
inline IntervalT<DoubleWrapper> Divide(const IntervalT<DoubleWrapper> &a,
		                        const IntervalT<DoubleWrapper> &b) {
  int current_round = fegetround();

  fesetround(FE_DOWNWARD);
  DoubleWrapper left = std::min(
			std::min(a.getL() / b.getL(), a.getL() / b.getR()),
			std::min(a.getR() / b.getL(), a.getR() / b.getR()));
  fesetround(FE_UPWARD);
  DoubleWrapper right = std::max(
			std::max(a.getL() / b.getL(), a.getL() / b.getR()),
			std::max(a.getR() / b.getL(), a.getR() / b.getR()));

  fesetround(current_round);

  return IntervalT<DoubleWrapper>(left, right);
}



inline BigFloat r_div(const BigFloat &a, const BigFloat &b, rnd_t round) {
  BigFloat result;
  result.r_div(a, b, round);

  return result;
}

// Specialization for the bigfloat.
template <>
inline IntervalT<BigFloat> Divide(const IntervalT<BigFloat> &a,
		                        const IntervalT<BigFloat> &b) {
  BigFloat left = std::min(
			std::min(r_div(a.getL(), b.getL(), GMP_RNDD),
					 r_div(a.getL(), b.getR(), GMP_RNDD)),
			std::min(r_div(a.getR(), b.getL(), GMP_RNDD),
					 r_div(a.getR(), b.getR(), GMP_RNDD)));
  BigFloat right = std::max(
			std::min(r_div(a.getL(), b.getL(), GMP_RNDU),
					 r_div(a.getL(), b.getR(), GMP_RNDU)),
			std::min(r_div(a.getR(), b.getL(), GMP_RNDU),
					 r_div(a.getR(), b.getR(), GMP_RNDU)));

  return IntervalT<BigFloat>(left, right);
}

// Definition of the extended arithmetic division operation.
template <typename NT>
inline void Divide(const IntervalT<NT> &a,
		            const IntervalT<NT> &b,
		            vector<IntervalT<NT> > *output) {
  return interval_ext::Divide(a, b, output);
}

template <>
inline void Divide(const IntervalT<DoubleWrapper> &a,
		            const IntervalT<DoubleWrapper> &b,
		            vector<IntervalT<DoubleWrapper> > *output) {
	int current_round = fegetround();
	// To remain consistent with the paper.
	const DoubleWrapper &a1 = a.getL();
	const DoubleWrapper &a2 = a.getR();
	const DoubleWrapper &b1 = b.getL();
	const DoubleWrapper &b2 = b.getR();

	// For the sake of convenient.
	const DoubleWrapper &I_POS_INFTY = IntervalT<DoubleWrapper>::I_POS_INFTY;
	const DoubleWrapper &I_NEG_INFTY = IntervalT<DoubleWrapper>::I_NEG_INFTY;

	// Some of these cases could potentially be combined
	// but are left as is to be consistent with the paper.
	if (b.zero()) {
		if (a2 <= 0 && b2 == 0) {
			fesetround(FE_DOWNWARD);
			output->push_back(IntervalT<DoubleWrapper>(a2/b1 , I_POS_INFTY));
		} else if (a2 < 0 && b1 < 0 && b2 > 0) {
			fesetround(FE_UPWARD);
			output->push_back(IntervalT<DoubleWrapper>(I_NEG_INFTY, a2/b2));
			fesetround(FE_DOWNWARD);
			output->push_back(IntervalT<DoubleWrapper>(a2/b1, I_POS_INFTY));
		} else if (a2 <= 0 && b1 == 0) {
			fesetround(FE_UPWARD);
			output->push_back(IntervalT<DoubleWrapper>(I_NEG_INFTY, a2/b2));
		} else if (a1 >= 0 && b2 == 0) {
			fesetround(FE_UPWARD);
			output->push_back(IntervalT<DoubleWrapper>(I_NEG_INFTY, a1/b1));
		} else if (a1 > 0 && b1 < 0 && b2 > 0) {
			fesetround(FE_UPWARD);
			output->push_back(IntervalT<DoubleWrapper>(I_NEG_INFTY, a1/b1));
			fesetround(FE_DOWNWARD);
			output->push_back(IntervalT<DoubleWrapper>(a1/b2, I_POS_INFTY));
		} else if (a1 >= 0 && b1 == 0) {
			fesetround(FE_DOWNWARD);
			output->push_back(IntervalT<DoubleWrapper>(a1/b2, I_POS_INFTY));
		} else if (a1 < 0 && a2 > 0) {
			output->push_back(IntervalT<DoubleWrapper>(I_NEG_INFTY, I_POS_INFTY));
		} else {
			// Should never reach this spot.
			assert(false);
		}
	} else {
		// This is regular division when a and b have no zeroes.
		output->push_back(Divide(a, b));
	}

	fesetround(current_round);
}

template <>
inline void Divide(const IntervalT<BigFloat> &a,
		           const IntervalT<BigFloat> &b,
		           vector<IntervalT<BigFloat> > *output) {
	const BigFloat &a1 = a.getL();
	const BigFloat &a2 = a.getR();
	const BigFloat &b1 = b.getL();
	const BigFloat &b2 = b.getR();

	// For the sake of convenient.
	const BigFloat &I_POS_INFTY = IntervalT<BigFloat>::I_POS_INFTY;
	const BigFloat &I_NEG_INFTY = IntervalT<BigFloat>::I_NEG_INFTY;

	// Some of these cases could potentially be combined
	// but are left as is to be consistent with the paper.
	if (b.zero()) {
		if (a2 <= 0 && b2 == 0) {
			output->push_back(IntervalT<BigFloat>(r_div(a2,b1,GMP_RNDD), I_POS_INFTY));
		} else if (a2 < 0 && b1 < 0 && b2 > 0) {
			output->push_back(IntervalT<BigFloat>(I_NEG_INFTY, r_div(a2, b2, GMP_RNDU)));
			output->push_back(IntervalT<BigFloat>(r_div(a2,b1,GMP_RNDD), I_POS_INFTY));
		} else if (a2 <= 0 && b1 == 0) {
			output->push_back(IntervalT<BigFloat>(I_NEG_INFTY, r_div(a2, b2, GMP_RNDU)));
		} else if (a1 >= 0 && b2 == 0) {
			output->push_back(IntervalT<BigFloat>(I_NEG_INFTY, r_div(a1, b1, GMP_RNDU)));
		} else if (a1 > 0 && b1 < 0 && b2 > 0) {
			output->push_back(IntervalT<BigFloat>(I_NEG_INFTY, r_div(a1, b1, GMP_RNDU)));
			output->push_back(IntervalT<BigFloat>(r_div(a1,b2,GMP_RNDD), I_POS_INFTY));
		} else if (a1 >= 0 && b1 == 0) {
			output->push_back(IntervalT<BigFloat>(r_div(a1,b2,GMP_RNDD), I_POS_INFTY));
		} else if (a1 < 0 && a2 > 0) {
			output->push_back(IntervalT<BigFloat>(I_NEG_INFTY, I_POS_INFTY));
		} else {
			// Should never reach this spot.
			assert(false);
		}
	} else {
		// This is regular division when a and b have no zeroes.
		output->push_back(Divide(a, b));
	}
}

}


#endif /* ROUNDEDIA_H_ */
