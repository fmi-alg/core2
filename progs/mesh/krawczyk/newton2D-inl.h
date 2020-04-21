/*
 * predicates.h
 *
 *  Created on: May 25, 2010
 *      Author: narayan
 */

#ifndef KRAWCZYK_NEWTON2D_INL_H_
#define KRAWCZYK_NEWTON2D_INL_H_

#include "krawczyk-defs.h"

template <typename NT>
class Newton2D : public BasePredicate<NT> {
public:
  Newton2D(const Curve<NT> &fxy,
      const Curve<NT> &gxy,
      const NT &max_size,
      const NT &min_size,
      const unsigned int max_gen_id,
      const unsigned int T = 4) :
        BasePredicate<NT>(fxy, gxy, max_size, min_size, max_gen_id, T) {
  }

  typedef BoxT<NT> Box;
  typedef IntervalT<NT> Interval;

  // Check return types carefully.
  virtual OPERATOR_DECISION Include(const Box *region,
      vector<const Box *> *contraction) const {

    VectorT<Interval> output(2);

    const Interval &x_range = region->x_range;
    const Interval &y_range = region->y_range;
    bool succeeded = EvaluateNewton2D(x_range, y_range, &output[0], &output[1]);

    const unsigned int gen_id = region->generation_id + 1;

    if (!succeeded) {
      contraction->push_back(new Box(gen_id, Interval(x_range), Interval(y_range)));

      return OD_SUBDIVIDE;
    }

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
  bool EvaluateNewton2D(const Interval &x_range,
      const Interval &y_range,
      Interval *x_out,
      Interval *y_out) const {
    Interval x_mid(x_range.mid());
    Interval y_mid(y_range.mid());

    // Four evaluations. For a justification of using
    // y_mid check hansens paper
    // On Solving Systems of Equations Using Interval Arithmetic
    // Author(s): Eldon R. Hansen
    // Source: Mathematics of Computation, Vol. 22, No. 102 (Apr., 1968), pp. 374-384
    // Published by: American Mathematical Society

    const CenterRepT<NT> &centered_rep = this->centered_rep_;

    Interval a = centered_rep.jacobian(0, 0).eval<Interval>(
        centered_rep.x_mid - x_range, centered_rep.y_mid - y_mid) * Interval(-1, -1);
    Interval b = centered_rep.jacobian(0, 1).eval<Interval>(
        centered_rep.x_mid - x_range, centered_rep.y_mid - y_range) * Interval(-1, -1);
    Interval c = centered_rep.jacobian(1, 0).eval<Interval>(
        centered_rep.x_mid - x_range, centered_rep.y_mid - y_mid) * Interval(-1, -1);
    Interval d = centered_rep.jacobian(1, 1).eval<Interval>(
        centered_rep.x_mid - x_range, centered_rep.y_mid - y_range) * Interval(-1, -1);

    // And another one.
    Interval det = a*d - b*c;

    if (det.zero()) {
      return false;
    }

    // NOTE : (d/det) , (b/det) need to be rounded correctly
    // in level 2.
    Interval x_contraction = x_mid -
        (M_DIV(d, det)*this->f_.eval<NT>(x_mid.getL(), y_mid.getL()))
             + (M_DIV(b, det)*this->g_.eval<NT>(x_mid.getL(), y_mid.getL()));

    // NOTE : Should make sure to use x_contraction only if x_contraction
    // is really a contraction, otherwise there is no point.


    // Six more calculations, this seems rather avoidable.
    x_mid = x_contraction.mid();

    a = centered_rep.jacobian(0, 0).eval<Interval>(
        centered_rep.x_mid - x_contraction, centered_rep.y_mid - y_mid) * Interval(-1, -1);
    b = centered_rep.jacobian(0, 1).eval<Interval>(
        centered_rep.x_mid - x_contraction, centered_rep.y_mid - y_range) * Interval(-1, -1);
    c = centered_rep.jacobian(1, 0).eval<Interval>(
        centered_rep.x_mid - x_contraction, centered_rep.y_mid - y_mid) * Interval(-1, -1);
    d = centered_rep.jacobian(1, 1).eval<Interval>(
        centered_rep.x_mid - x_contraction, centered_rep.y_mid - y_range) * Interval(-1, -1);

    det = a*d - b*c;

    if (det.zero()) {
      return false;
    }

    // NOTE : (c/det) , (a/det) need to be rounded correctly
    // in level 2.
    Interval y_contraction = y_mid +
        (M_DIV(c, det)*this->f_.eval<NT>(x_mid.getL(), y_mid.getL()))
             - (M_DIV(a, det)*this->g_.eval<NT>(x_mid.getL(), y_mid.getL()));

    *x_out = x_contraction;
    *y_out = y_contraction;

    return true;
  }
};

#endif /* PREDICATES_H_ */
