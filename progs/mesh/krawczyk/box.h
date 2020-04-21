/*
 * box.h
 *
 *  Created on: Jul 1, 2010
 *      Author: narayan
 */

#ifndef BOX_H_
#define BOX_H_

#include <vector>

#include "krawczyk-defs.h"

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

  ~BoxT() {

  }

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


  const unsigned int generation_id;
  const IntervalT<NT> x_range;
  const IntervalT<NT> y_range;
};

#endif /* BOX_H_ */
