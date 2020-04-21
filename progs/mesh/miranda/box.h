/*
 * box.h
 * box structure
 * July 15, 2011
 */

#ifndef MK_BOX_H_
#define MK_BOX_H_

#include <vector>

#include "mk-defs.h"

template <typename NT> class BoxT {
public:
  BoxT() : generation_id(0) {
  }
  BoxT(unsigned int generation_id,
       const IntervalT<NT> &x_range,
       const IntervalT<NT> &y_range) :
        generation_id(generation_id),
        x_range(x_range),
        y_range(y_range) {
  }

  ~BoxT() { }

  void Split(vector<const BoxT<NT> *> *output) const {
    const NT &x_start = x_range.getL();
    const NT &x_end = x_range.getR();
    const NT &x_mid = x_range.mid();
    const NT &y_start = y_range.getL();
    const NT &y_end = y_range.getR();
    const NT &y_mid = y_range.mid();

    const unsigned int gen_id = generation_id + 1;

    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_start, x_mid), IntervalT<NT>(y_start, y_mid)));
    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_mid, x_end), IntervalT<NT>(y_start, y_mid)));
    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_start, x_mid), IntervalT<NT>(y_mid, y_end)));
    output->push_back(new BoxT<NT>(gen_id,
        IntervalT<NT>(x_mid, x_end), IntervalT<NT>(y_mid, y_end)));
  }

  BoxT * Dilate(const NT factor) const {
    IntervalT<NT> new_x = x_range.Expand(factor);
    IntervalT<NT> new_y = y_range.Expand(factor);
    return (new BoxT(0, new_x, new_y));
  }

  NT x_mid() const { return x_range.mid(); }
  NT y_mid() const { return y_range.mid(); }

  const unsigned int generation_id;
  const IntervalT<NT> x_range;
  const IntervalT<NT> y_range;
};

#endif /* BOX_H_ */
