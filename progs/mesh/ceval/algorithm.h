/*
 * ceval-algorithm.h
 *
 *  Created on: Jun 22, 2010
 *      Author: narayan
 */

#ifndef CEVAL_ALGORITHM_H_
#define CEVAL_ALGORITHM_H_

#include <list>
#include <vector>

#include "ceval-defs.h"
#include "predicates.h"

// The datastructure that stores an isolating disk in
// complex space.
struct Disk {
  Complex centre;
  double radius;
};

// A Box represents a rectangular region in the
// complex plane, and is
// represented by two complex numbers.
//
// min : The bottom left hand corner of the region.
// max : The top right hand corner of the region.
class Box {
 public:
  // Construct a box.
  Box(const Complex &min, const Complex &max);
  // Split the rectangular region into for equal
  // sized children. The children are added to
  // output.
  void Split(vector<const Box *> *output) const;
  // The center of this region.
  inline Complex mid() const {
    return (max_ + min_) / double(2);
  }
  // This is the radius of the circle that would pass
  // through the four corners of the region.
  //
  // NOTE(narayan): This can be replaced by the dyadic
  // approximation of 3/4w(b).
  inline double radius() const {
    return 0.75*(max_.re() - min_.re());
    //return (max_ - mid()).mod();
  }
 //private:
  // This is the SW (bottom left) corner of the box.
  Complex min_;
  // This is the NE (top right) corner of the box.
  Complex max_;
};

class Algorithm {
 public:
  // Constructs an instance of this class with the specified
  // predicates and specified starting region.
  Algorithm(const Predicates &p, const Box *b0,
      const double &min_size_,
      const bool use_inclusion = true,
      const bool display = false);

  // Run the algorithm. The output is stored as a list of Disks.
  void Run();

  // Attempt to isolate the roots *IF* the algorithm has been run
  // without the inclusion predicate. This step takes each connected
  // component of output boxes and, attempts to run the 8 point test
  // on the bounding box for each connected component.
  void AttemptIsolation();

  // Get the output of the CEval algorithm.
  const list<const Disk *> *output() const {
    return &output_;
  }
  const vector<const Box *> *reject() const {
    return &rejects_;
  }
  const vector<const Box *> *output_boxes() const {
    return &output_b_;
  }
  const vector<const Box *> *ambiguous() const {
    return &ambiguous_;
  }
 private:
  const bool Size(const Box *box) const;
  // Insert an isolating disk into the output. This is an O(n)
  // operation because if a disk is found that intersects with
  // this one, the smaller of the two disks is inserted into the
  // queue.
  void InsertOutput(const Complex &mid, const double &rad);

  // The frequently used value of the SQRT_2 , used in the Tk
  // predicates.
  //
  // TODO: This can be removed and replaced with 3/2.
  static const double SQRT_2;
  // Instance of the prdicates class.
  const Predicates &pred_;
  // The processing queue.
  vector<const Box *> queue_;
  // The output list of disks;
  list<const Disk *> output_;

  // Minimum size
  const double min_size_;
  // Use the inclusion predicate
  const bool use_inclusion_;
  // Display ?
  const bool display_;

  // For processing.
  vector<const Box *> output_b_;
  vector<const Box *> rejects_;
  vector<const Box *> ambiguous_;
};

#endif /* CEVAL_ALGORITHM_H_ */
