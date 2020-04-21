/*
 * algorithm.h
 *
 *  Created on: May 25, 2010
 *      Author: narayan
 */

#ifndef KRAWCZYK_ALGORITHM_H_
#define KRAWCZYK_ALGORITHM_H_

#include <vector>
#include <utility>

#include "krawczyk-defs.h"
#include "base-predicate-inl.h"
#include "box.h"

using namespace std;

namespace Algorithm {

template <typename NT> void Run(
    const BasePredicate<NT> &pred,
    const BoxT<NT> *initial,
    vector<const BoxT<NT> *> *output,
    vector<const BoxT<NT> *> *excluded,
    vector<const BoxT<NT> *> *ambiguous,
    const bool display = true) {

  // The main processing queue.
  vector<const BoxT<NT> *> queue;
  queue.push_back(initial);

  // Statistics gathering.
  unsigned int num_iterations = 0;
  unsigned int num_excludes_C0= 0;
  unsigned int num_excludes_C1 = 0;
  unsigned int num_includes = 0;
  unsigned int num_splits = 0;
  unsigned int num_ambiguous = 0;


  while (!queue.empty()) {
    const BoxT<NT> *region = queue.back();
    queue.pop_back();

    ++num_iterations;

    if (LOG_DEBUG) {
      cout << "============================================================" << endl;
      cout << "Considering : " << region->x_range << "," << region->y_range << endl;
    }

    // A test whether the region is too small to continue
    // processing, in which case it is pushed back into the
    // list of ambigious regions.
    if (pred.Min(region)) {
      ambiguous->push_back(region);
      if (LOG_DEBUG) { cout << "AMBIGUOUS" << endl; }
      ++num_ambiguous;
      continue;
    }

    // Exclusion predicate.
    if (pred.Exclude(region)) {
      if (excluded != NULL) {
        excluded->push_back(region);
      } else {
        delete region;
      }
      ++num_excludes_C0;
      if (LOG_DEBUG) { cout << "EXCLUDE" << endl; }
      continue;
    }

    // This is the contraction of the region under observation.
    // it is a subset of region if include == 1.
    vector<const BoxT<NT> *> contractions;

    // The operator decision can be one of
    // INCLUDE, SUBDIVIDE, or EXCLUDE.
    OPERATOR_DECISION ret = pred.Include(region, &contractions);

    // This will work well if we are already in a strong contraction
    // region, otherwise not so well.
    if (ret == OD_INCLUDE) {
      //if (pred.Max(contraction)) {
      //  pred.Split(contraction, &queue);
      //} else {

      // }
      typename vector<const BoxT<NT> *>::iterator it = contractions.begin();
      while (it != contractions.end()) {
        output->push_back(*it);
        ++it;
      }
      ++num_includes;
      if (LOG_DEBUG) { cout << "INCLUDE" << endl; }
    } else if (ret == OD_SUBDIVIDE) {
      typename vector<const BoxT<NT> *>::iterator it = contractions.begin();
      while (it != contractions.end()) {
        pred.Split(*it, &queue);
        delete (*it);
        //num_boxes+=3;
        ++it;
      }
      ++num_splits;
      if (LOG_DEBUG) { cout << "SUBDIVIDE" << endl; }
    } else if (ret == OD_EXCLUDE) {
      if (excluded != NULL) {
        excluded->push_back(region);
      } else {
        delete region;
      }
      ++num_excludes_C1;
      if (LOG_DEBUG) { cout << "EXCLUDE-C1" << endl; }
    }
  }

  for (unsigned int i = 0; i < queue.size(); ++i) {
    ambiguous->push_back(queue[i]);
  }

  if (display) {
    cout << num_iterations << " " << num_includes << " "
         << num_splits << " " << num_ambiguous << " "
         << num_excludes_C0 << " " << num_excludes_C1 << endl;
    /*
    cout << "Total iterations : " << num_iterations << endl;
    cout << "Included boxes : " << num_includes << endl;
    cout << "Ambiguous boxes : " << num_ambiguous << endl;
    cout << "Splits : " << num_splits << endl;
    cout << "Num excludes (C0) : " << num_excludes_C0 << endl;
    cout << "Num excludes (C1) : " << num_excludes_C1 << endl;
    */
  }
}
}

#endif /* KRAWCZYK_ALGORITHM_H_ */
