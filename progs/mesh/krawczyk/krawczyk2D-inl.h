/*
 * predicates.h
 *
 *  Created on: May 25, 2010
 *      Author: narayan
 */

#ifndef KRAWCZYK_KRAWCZYK2D_INL_H_
#define KRAWCZYK_KRAWCZYK2D_INL_H_

#include "krawczyk-defs.h"
#include "base-predicate-inl.h"


template <typename NT>
class Krawczyk2D : public BasePredicate<NT> {
 public:
  Krawczyk2D(const Curve<NT> &fxy,
             const Curve<NT> &gxy,
             const NT &max_size,
             const NT &min_size,
             const unsigned int max_gen_id,
             const unsigned int T = 4) :
    BasePredicate<NT>(fxy, gxy, max_size, min_size, max_gen_id, T),
    I(MatrixT<Interval>(2)) {
    I(0, 0) = 1.0;
    I(1, 1) = 1.0;
    I(1, 0) = 0.0;
    I(0, 1) = 0.0;
  }
  virtual ~Krawczyk2D() {}

  typedef IntervalT<NT> Interval;
  typedef BoxT<NT> Box;

  // Check return types carefully.
  virtual OPERATOR_DECISION Include(const Box *region,
                            vector<const Box *> *contraction) const {
    VectorT<Interval> output;

    const Interval &x_range = region->x_range;
    const Interval &y_range = region->y_range;
    EvaluateKrawczyk(x_range, y_range, &output);

    const unsigned int gen_id = region->generation_id + 1;

    if (Contains(x_range, output[0]) &&
        Contains(y_range, output[1])) {
      contraction->push_back(
          new Box(gen_id,
              Interval(Intersect(x_range, output[0])),
              Interval(Intersect(y_range, output[1]))));

      return OD_INCLUDE;
    }

    if (!Overlap(x_range, output[0]) ||
        !Overlap(y_range, output[1])) {
      return OD_EXCLUDE;
    }

    if (Overlap(x_range, output[0]) &&
        Overlap(y_range, output[1])) {
      contraction->push_back(
          new Box(gen_id,
              Interval(Intersect(x_range, output[0])),
              Interval(Intersect(y_range, output[1]))));

      ++this->fail_ctr_;
      return OD_SUBDIVIDE;
    }

    cout << "Should never reach this spot" << endl;
    exit(1);
  }

 private:
  void EvaluateKrawczyk(const Interval &x_range,
                        const Interval &y_range,
                        VectorT<Interval> *output) const {
    MatrixT<NT> Y(2);
    VectorT<NT> mid(x_range.mid(), y_range.mid());
    EvaluateYApprox(&Y, mid[0], mid[1]);

    VectorT<NT> f_eval(this->f_.eval<NT>(mid[0], mid[1]),
                       this->g_.eval<NT>(mid[0], mid[1]));

    // Section 1 :
    // This computes the first two terms of the krawczyk operator. which are
    // y - Yf(y) where y = mid(X).
    VectorT<NT> newton = mid - (Y)*(f_eval);
    VectorT<Interval> newton_int(Interval(newton[0], newton[0]),
                                 Interval(newton[1], newton[1]));

    // Section 2 :
    // Calculates (X - y) where y = mid(X)
    VectorT<Interval> mid_interval(Interval(mid[0], mid[0]),
                                   Interval(mid[1], mid[1]));
    VectorT<Interval> range_interval(x_range, y_range);
    VectorT<Interval> diff = range_interval - mid_interval;
    //cout << "X - y : " << diff << endl;

    // Section 3 :
    // Calculates {I - YF'(X)}(X - y)
    MatrixT<Interval> jacobian_interval(2), y_interval(2);
    //12Jun2017(Remi&Chee): added "this->" to compile:
    this->EvaluateIntervalJacobian(&jacobian_interval,
        x_range, y_range);  // This is F'(X)
    ConvertDegenerate(Y, &y_interval);
    VectorT<Interval> result = (I - y_interval*jacobian_interval) * diff;

    // Calculates and returnsy - Yf(y) + {I - YF'(X)}(X - y)
    (*output) = newton_int + result;
  }

  // This is highly dubious code. This needs to be walked through
  // and double checked.
  bool EvaluateYApprox(MatrixT<NT> *output,
                       const NT &x,
                       const NT &y) const {
    MatrixT<NT> &out_ref = *output;
    NT a = this->jacobian_(0, 0).eval<NT>(x, y);
    NT b = this->jacobian_(0, 1).eval<NT>(x, y);
    NT c = this->jacobian_(1, 0).eval<NT>(x, y);
    NT d = this->jacobian_(1, 1).eval<NT>(x, y);
    NT det = a*d - b*c;

    if ((det) == 0) {
      out_ref(0, 0) = 1;
      out_ref(0, 1) = 0;
      out_ref(1, 0) = 0;
      out_ref(1, 1) = 1;
      return true;
    }

    // Dont worry about correct rounding, this needs to
    // be only approximate.
    out_ref(0, 0) = d / det;
    out_ref(0, 1) = -b / det;
    out_ref(1, 0) = -c / det;
    out_ref(1, 1) = a / det;

    return true;
  }

  void ConvertDegenerate(const MatrixT<NT> &input,
                         MatrixT<Interval> *output) const {
    MatrixT<Interval> &out_ref = *output;
    out_ref(0, 0) = Interval(input(0, 0), input(0, 0));
    out_ref(0, 1) = Interval(input(0, 1), input(0, 1));
    out_ref(1, 0) = Interval(input(1, 0), input(1, 0));
    out_ref(1, 1) = Interval(input(1, 1), input(1, 1));
  }

private:
  MatrixT<IntervalT<NT> > I;
};


#endif /* PREDICATES_H_ */
