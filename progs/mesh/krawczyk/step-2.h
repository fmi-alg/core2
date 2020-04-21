/*
 * step-2.h
 *
 *  Created on: Jul 2, 2010
 *      Author: narayan
 */

#ifndef STEP2_H_
#define STEP2_H_

#include <vector>

#include "krawczyk-defs.h"
#include "box.h"
#include "base-predicate-inl.h"
#include "algorithm-inl.h"
#include "krawczyk2D-inl.h"
#include "newton2D-inl.h"
#include "hs2D-inl.h"

using namespace std;

namespace step2 {

template <typename NT>
void ConvertBF(vector<const BoxT<NT> *> *input,
               vector<const BoxT<BigFloat> *> *output,
               const NT perturb = 0.005)  {
  for (unsigned int i = 0; i < input->size(); ++i) {
    const BoxT<NT> *box = (*input)[i];
    BoxT<BigFloat> *box_bf = new BoxT<BigFloat>(
        0,
        IntervalT<BigFloat>(
            box->x_range.getL() - perturb,
            box->x_range.getR() + perturb),
        IntervalT<BigFloat>(
            box->y_range.getL() - perturb,
            box->y_range.getR() + perturb));
    output->push_back(box_bf);
    delete box;
  }
  input->clear();
}

template <typename NT>
void ReverseConvertBF(vector<const BoxT<BigFloat> *> *input,
               vector<const BoxT<NT> *> *output)  {
  for (unsigned int i = 0; i < input->size(); ++i) {
    const BoxT<BigFloat> *box = (*input)[i];
    BoxT<NT> *box_bf = new BoxT<NT>(
        0,
        IntervalT<NT>(
            NT(box->x_range.getL().doubleValue()),
            NT(box->x_range.getR().doubleValue())),
        IntervalT<NT>(
            NT(box->y_range.getL().doubleValue()),
            NT(box->y_range.getR().doubleValue())));
    output->push_back(box_bf);
    delete box;
  }
  input->clear();
}

void CoalesceBoxes(vector<const BoxT<BigFloat> *> *input) {
  // For now just expand.
}

enum operator_type {
  NEWTON,
  KRAWCZYK,
  HANSEN,
};

template <typename NT>
void ConvertPoly(Polynomial<BigFloat> *output,
    const Polynomial<BigFloat> &in) {
  vector<BigFloat> coeff_array;
  for (int i = 0; i < in.getDegree() + 1; ++i) {
    coeff_array.push_back(in.coeff()[i]);
  }
  (*output) = Polynomial<BigFloat>(coeff_array);
}

template <typename NT>
BiPoly<BigFloat> ConvertPoly(const BiPoly<NT> &p) {
  vector<Polynomial<BigFloat> > coeff_array(p.getYdegree() + 1);
  for (int i = 0; i < p.getYdegree() + 1; ++i) {
    ConvertPoly<NT>(&coeff_array[i], p.coeffX[i]);
  }
  return BiPoly<BigFloat>(coeff_array);
}

template <typename NT>
void Process(vector<const BoxT<NT> *> *unresolved,
             vector<const BoxT<NT> *> *output,
             const BiPoly<NT> &fxy,
             const BiPoly<NT> &gxy,
             const NT &max_size,
             const NT &min_size,
             const operator_type op_type) {

  vector<const BoxT<BigFloat> *> unresolved_2;
  ConvertBF<NT>(unresolved, &unresolved_2);
  CoalesceBoxes(&unresolved_2);

  BiPoly<BigFloat> fxy_2 = ConvertPoly<NT>(fxy);
  BiPoly<BigFloat> gxy_2 = ConvertPoly<NT>(gxy);
  BasePredicate<BigFloat> *pred_2 = NULL;

  if (op_type == NEWTON) {
    pred_2 = new Newton2D<BigFloat>(fxy_2, gxy_2 , max_size,
        0, 4, 0);
  } else if (op_type == KRAWCZYK) {
    pred_2 = new Krawczyk2D<BigFloat>(fxy_2, gxy_2 , max_size,
        0, 4, 0);
  } else {
    pred_2 = new Hs2D<BigFloat>(fxy_2, gxy_2 , max_size,
        0, 4, 0);
  }

  vector<const BoxT<BigFloat> *> output_2;
  vector<const BoxT<BigFloat> *> ambiguous_2;
  for (unsigned int i = 0; i < unresolved_2.size(); ++i) {
    Algorithm::Run<BigFloat>(*pred_2, unresolved_2[i], &output_2, NULL, &ambiguous_2
        , false);
  }

  ReverseConvertBF<NT>(&output_2, output);

  delete pred_2;
}


}  // namespace step2;

#endif /* STEP2_H_ */
