/*
 * base-predicate-inl.h
 *
 *  Created on: Jul 2, 2010
 *      Author: narayan
 */

#ifndef BASEPREDICATEINL_H_
#define BASEPREDICATEINL_H_

#include "krawczyk-defs.h"
#include "centered-rep-inl.h"

template <typename NT>
class BasePredicate {
protected:
  BasePredicate(const Curve<NT> &fxy,
      const Curve<NT> &gxy, const NT &max_size,
      const NT &min_size, const unsigned int max_gen_id,
      const unsigned int T = 4) :
      f_(fxy), g_(gxy), jacobian_(MatrixT<Curve<NT> >(2)),
      max_size_(max_size), min_size_(min_size),
      max_gen_id_(max_gen_id), T_(T) {

    // This ugly code is needed only because "differentiate is
    // a self modifying operator.
    Curve<NT> tmp = fxy, tmp1 = fxy;
    jacobian_(0, 0) = tmp.differentiateX();
    jacobian_(0, 1) = tmp1.differentiateY();
    tmp = gxy, tmp1 = gxy;
    jacobian_(1, 0) = tmp.differentiateX();
    jacobian_(1, 1) = tmp1.differentiateY();

    ConstructCentered<NT>(Interval(0),  Interval(0), f_, g_, &centered_rep_);
  }

  typedef IntervalT<NT> Interval;

  void EvaluateIntervalJacobian(MatrixT<Interval> *output,
      const Interval &x_range, const Interval &y_range) const {
    // to make the syntax cleaner.
    MatrixT<Interval> &out_ref = *output;

    out_ref(0, 0) = centered_rep_.jacobian(0, 0).eval<Interval>(
        centered_rep_.x_mid - x_range, centered_rep_.y_mid - y_range) * Interval(-1, -1);
    out_ref(0, 1) = centered_rep_.jacobian(0, 1).eval<Interval>(
        centered_rep_.x_mid - x_range, centered_rep_.y_mid - y_range) * Interval(-1, -1);
    out_ref(1, 0) = centered_rep_.jacobian(1, 0).eval<Interval>(
        centered_rep_.x_mid - x_range, centered_rep_.y_mid - y_range) * Interval(-1, -1);
    out_ref(1, 1) = centered_rep_.jacobian(1, 1).eval<Interval>(
        centered_rep_.x_mid - x_range, centered_rep_.y_mid - y_range) * Interval(-1, -1);
  }

public:

  bool Min(const BoxT<NT> *region) const {
    if (region->x_range.width() <= min_size_ ||
        region->y_range.width() <= min_size_) {
      return true;
    }


    if (region->generation_id > max_gen_id_) {
      return true;
    }

    return false;
  }

  bool Max(const BoxT<NT> *region) const {
    if (region->x_range.width() >= max_size_ ||
        region->y_range.width() >= max_size_) {
      return true;
    }

    return false;
  }

  void Split(const BoxT<NT> *region,
      vector<const BoxT<NT> *> *outputs) const {
    region->Split(outputs);
  }

  bool Exclude(const BoxT<NT> *region) const {
    if (fail_ctr_ >= T_) {
      fail_ctr_ = 0;
      ConstructCentered(region->x_range, region->y_range,
          f_, g_, &centered_rep_);
    }

    IntervalT<NT> f_range = centered_rep_.centered_f.eval<IntervalT<NT> >(
        centered_rep_.x_mid - region->x_range,
        centered_rep_.y_mid - region->y_range);
    IntervalT<NT> g_range = centered_rep_.centered_g.eval<IntervalT<NT> >(
        centered_rep_.x_mid - region->x_range,
        centered_rep_.y_mid - region->y_range);

    return !(f_range.zero() && g_range.zero());
  }

  // Check return types carefully.
  virtual OPERATOR_DECISION Include(
      const BoxT<NT> *region,
      vector<const BoxT<NT> *> *contraction) const = 0;

protected:
  Curve<NT> f_;
  Curve<NT> g_;
  MatrixT<Curve<NT> > jacobian_;
  // Change the design so that these dont need
  // to be mutable.
  mutable unsigned int fail_ctr_;
private:
  const NT max_size_;
  const NT min_size_;
  const unsigned int max_gen_id_;
  const unsigned int T_;
  // TODO : Change the newton code to use EvaluateIntervalJacobian( )
  // in order to keep this private.
protected:
  mutable CenterRepT<NT> centered_rep_;
};


#endif /* BASEPREDICATEINL_H_ */
