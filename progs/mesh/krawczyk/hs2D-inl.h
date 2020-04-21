/*
 * predicates.h
 *
 *  Created on: May 25, 2010
 *      Author: narayan
 */

#ifndef KRAWCZYK_HS2D_INL_H_
#define KRAWCZYK_HS2D_INL_H_

#include <vector>

#include "krawczyk-defs.h"

using namespace std;

template<typename NT>
class Hs2D : public BasePredicate<NT> {
 public:
  Hs2D(const Curve<NT> &fxy,
       const Curve<NT> &gxy,
       const NT &max_size,
       const NT &min_size,
       const unsigned int max_gen_id,
       const unsigned int T = 4) :
           BasePredicate<NT>(fxy, gxy, max_size, min_size, max_gen_id, T) {
  }

  // A typedef within the scope of this class.
  typedef IntervalT<NT> Interval;
  typedef BoxT<NT> Box;

  // Check return types carefully.
  virtual OPERATOR_DECISION Include(
      const BoxT<NT> *region,
      vector<const BoxT<NT> *> *contraction) const {
    // A list of intervals that the HS operator produced,
    // this can have more than one interval thanks to the
    // extended arithmetic division.
    vector<Interval> output_x;
    vector<Interval> output_y;

    const Interval &x_range = region->x_range;
    const Interval &y_range = region->y_range;
    bool extend = EvaluateHs2D(x_range, y_range, &output_x, &output_y);

    unsigned int num_intersects = HsIntersect(
        x_range, y_range, output_x, output_y,
        region->generation_id + 1, contraction);

    if (num_intersects == 0) {
      return OD_EXCLUDE;
    }

    if (!extend && HsContains(x_range, output_x) &&
        HsContains(y_range, output_y)) {
      return OD_INCLUDE;
    }

    ++this->fail_ctr_;
    return OD_SUBDIVIDE;
  }

 private:

  // Return true iff range contains each and every interval
  // contained in objects.
  inline bool HsContains(const Interval &range,
                         const vector<Interval> &objects) const {
    assert (objects.size() > 0);
    typename vector<Interval>::const_iterator it = objects.begin();
    while (it != objects.end()) {
      if (!Contains(range, *it)) {
        return false;
      }
      ++it;
    }

    return true;
  }

  // Return true iff range contains each and every interval
  // contained in objects.
  inline unsigned int HsIntersect(const Interval &x_range,
                                const Interval &y_range,
                                const vector<Interval> &x_objects,
                                const vector<Interval> &y_objects,
                                const unsigned int gen_id,
                                vector<const BoxT<NT> *> *output) const {
    unsigned int ctr = 0;
    for (unsigned int i = 0; i < x_objects.size(); ++i) {
      const Interval &x = x_objects[i];
      for (unsigned int j = 0; j < y_objects.size(); ++j) {
        const Interval &y = y_objects[j];
        if (Overlap(x_range, x) && Overlap(y_range, y)) {
          output->push_back(new Box(gen_id, Interval(Intersect(x_range, x)),
              Interval(Intersect(y_range, y))));
          ++ctr;
        }
      }
    }

    return ctr;
  }

  bool EvaluateHs2D(const Interval &x_range,
      const Interval &y_range, vector<Interval> *x_out,
      vector<Interval> *y_out) const {

    Interval x_mid(x_range.mid()), y_mid(y_range.mid());
    MatrixT<Interval> B(2), jacobian(2);
    // This matrix will always contain degenerate intervals.
    EvaluateBApprox(&B, x_mid.getL(), y_mid.getR());
    // 13jun2017(Remi&Chee): need to add "this->" to compile:
    this->EvaluateIntervalJacobian(&jacobian, x_range, y_range);
    MatrixT<Interval> M = B*jacobian;

    bool extend = false;
    if (M(0, 0).zero() || M(1, 1).zero()) {
      extend = true;
    }

    VectorT<Interval> f_eval(this->f_.eval<NT>(x_mid.getL(), y_mid.getR()),
        this->g_.eval<NT>(x_mid.getL(), y_mid.getR()));
    VectorT<Interval> b = B*f_eval;

    vector<Interval> x_contract, y_contract;
    Interval minus_1(-1, -1);

    DIV_NS::Divide(b[0]*minus_1 - M(0, 1)*(y_range - y_mid),  M(0, 0), &x_contract);

    for (unsigned int i = 0; i < x_contract.size(); ++i) {
      interval_ext::Add(x_contract[i], x_mid);
      x_out->push_back(x_contract[i]);
    }

    for (unsigned int i = 0; i < x_contract.size(); ++i) {
      const Interval &x = x_contract[i];
      const Interval &x_iter2 = (Contains(x_range, x)) ? x : x_range;

      DIV_NS::Divide(b[1]*minus_1 - M(1, 0)*(x_iter2 - x_mid),  M(1, 1), &y_contract);
    }

    for (unsigned int i = 0; i < y_contract.size(); ++i) {
       interval_ext::Add(y_contract[i], y_mid);
       // y_contract[i] = Interval::intersect(y_contract[i], y_range);
       if (y_out->size() == 0 || !(y_contract[i] == y_out->back())) {
         y_out->push_back(y_contract[i]);
       }
    }

    return extend;
  }

  // This is highly dubious code. This needs to be walked through
  // anddouble checked.
  void EvaluateBApprox(MatrixT<Interval> *output,
                       const NT &x,
                       const NT &y) const {
    MatrixT<Interval> &out_ref = *output;

    NT a = this->jacobian_(0, 0).eval<NT>(x, y);
    NT b = this->jacobian_(0, 1).eval<NT>(x, y);
    NT c = this->jacobian_(1, 0).eval<NT>(x, y);
    NT d = this->jacobian_(1, 1).eval<NT>(x, y);

    NT det = a*d - b*c;

    if ((det) == 0) {
      out_ref(0, 0) = out_ref(1, 1) = 1;
      out_ref(1, 0) = out_ref(0, 1) = 0;
      return;
    }

    out_ref(0, 0) =  d / det;
    out_ref(0, 1) = -b / det;
    out_ref(1, 0) = -c / det;
    out_ref(1, 1) = a / det;
  }
};

#endif /* PREDICATES_H_ */
