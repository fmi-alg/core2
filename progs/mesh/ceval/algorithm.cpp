/*
 * File: ceval-algorithm.cpp
 *
 *  Created on: Jun 22, 2010
 *      Author: narayan
 *
 *  $Id: algorithm.cpp,v 1.10 2011/05/04 15:11:16 exact Exp $
 */

#include <list>
#include <iostream>

#include "algorithm.h"

using namespace std;

Box::Box(const Complex &min, const Complex &max) :
min_(min), max_(max) { }

void Box::Split(vector<const Box *> *output) const {
  // The boxes are named as
  // 1 | 2
  // -----
  // 3 | 4
  const Complex mid = (max_ + min_) / double(2);

  // min_x and max_x are the min/max values for
  // box x respectively.
  //
  // NOTE(narayan): This could have been less verbose
  // and perhaps a little more efficient.
  const Complex min_1(min_.re(), mid.im());
  const Complex max_1(mid.re() , max_.im());
  const Complex &min_2 = mid;
  const Complex &max_2 = max_;
  const Complex &min_3 = min_;
  const Complex &max_3 = mid;
  const Complex min_4(mid.re(), min_.im());
  const Complex max_4(max_.re(), mid.im());

  output->push_back(new Box(min_1, max_1));
  output->push_back(new Box(min_2, max_2));
  output->push_back(new Box(min_3, max_3));
  output->push_back(new Box(min_4, max_4));
}

const bool Algorithm::Size(const Box *box) const {
  if (box->max_.re() - box->min_.re() <= min_size_ ||
      box->max_.im() - box->min_.im() <= min_size_) {
    return true;
  }
  return false;
}

// This horrible approximation is for testing only.
const double Algorithm::SQRT_2(1.41421356);

Algorithm::Algorithm(const Predicates &p, const Box *b0,
    const double &min_size,
    const bool use_inclusion,
    const bool display) :
  pred_(p), min_size_(min_size),
  use_inclusion_(use_inclusion), display_(display) {
  queue_.push_back(b0);
}

// This is the standard subdivision algorithm
void Algorithm::Run() {
  // Statistics gathering.
  unsigned int num_iterations = 0;
  unsigned int num_excludes_T1 = 0;
  unsigned int num_excludes_8P = 0;
  unsigned int num_includes = 0;
  unsigned int num_splits = 0;

  while (!queue_.empty()) {
    ++num_iterations;

    const Box *b = queue_.back();
    queue_.pop_back();

    // Keep the mid and the radius handy.
    const Complex mid = b->mid();
    const double rad = b->radius();
    const double rad_degree_4 = pred_.GetDegree() * 4 * b->radius();
    const double rad_degree_2 = pred_.GetDegree() * 2 * b->radius();

    if (Size(b)) {
#ifdef DEBUG
cout << "ambiguous box = " << b->min_.re() << ", " << b->min_.im()
    << ", " << b->max_.re() << ", " << b->max_.im() << endl;
#endif
      if (!use_inclusion_) {
        // If no inclusion predicate is being used, then
        // if the box is too small - it is a part of the
        // output.
        ambiguous_.push_back(b);
      } else {
        // If an inclusion predicate is being used, then
        // if the box is too small - it is ambiguous.
        if (display_) {
          ambiguous_.push_back(b);
        } else {
          delete b;
        }
      }
      continue;
    }

    // This is the exclusion predicate.
    if (pred_.T(1, mid, rad)) {
      ++num_excludes_T1;
      if (display_) {
        rejects_.push_back(b);
      } else {
        delete b;
      }
    } else if (use_inclusion_ &&
               pred_.Tdash(6, mid, 4*rad) &&
               pred_.Tdash(1.5, mid, 8*rad)) {
      if (pred_.PointTest(mid, 4*rad)) {
        // A success ! . The region can be inserted into the output.
        InsertOutput(mid, 4*rad);
        ++num_includes;
        if (display_) {
          output_b_.push_back(b);
        }
      } else {
        // If the 8 point test fails the region can be
        // excluded.
        ++num_excludes_8P;
        if (display_) {
          rejects_.push_back(b);
        } else {
          delete b;
        }
      }
    } else if (!use_inclusion_ && pred_.Tdash(1.5, mid, rad_degree_4)) {
      // No inclusion test, but we can try with T'(sqrt{2}, 4*n*r)
      InsertOutput(mid, rad_degree_2);
      ++num_includes;
      if (display_) {
        output_b_.push_back(b);
      }
    } else {
      // Neither excluded or included, therefore split.
      ++num_splits;
      b->Split(&queue_);
      delete b;
    }
  }

  if (display_) {
    for (unsigned int i = 0; i < queue_.size(); ++i) {
      ambiguous_.push_back(queue_[i]);
    }
  }

  cout << "BOX STATISTICS: \n"
      << "   iters="<< num_iterations
      << ",   includes=" << num_includes
      << ",   splits=" << num_splits
      << ",\n   ambiguous=" << ambiguous_.size()
      << ",   exc_c0=" << num_excludes_T1
      << ",   exc_c1=" << num_excludes_8P;
  /*
  cout << "Total iterations : " << num_iterations << endl;
  cout << "Included boxes : " << num_includes << endl;
  cout << "Splits : " << num_splits << endl;
  cout << "Num excludes (T1) : " << num_excludes_T1 << endl;
  cout << "Num excludes (8P) : " << num_excludes_8P << endl;
  */
}

// A helper function to test if two disks intersect on
// the complex plane.
bool Intersect(const Disk &d1, const Disk &d2) {
  return (d1.radius + d2.radius) > (d1.centre - d2.centre).mod();
}

void Algorithm::InsertOutput(const Complex &mid, const double &rad) {
  list<const Disk *>::iterator it = output_.begin();
  // Create a new disk.
  Disk *new_d = new Disk;
  new_d->centre = mid;
  new_d->radius = rad;

  // Traverse the output list, this is O(n) in the size
  // of the output list which is likely to be small
  // anyway.
  while (it != output_.end()) {
    const Disk *d = *it;
    // If we find a disk that intersects, replace it with
    // the smaller of the two disks and return.
    if (Intersect(*d, *new_d)) {
      it = output_.erase(it);
      if (new_d->radius < d->radius) {
        output_.push_back(d);
        delete new_d;
      } else {
        output_.push_back(new_d);
        delete d;
      }
      return;
    }
    ++it;
  }

  // Add the disk to the output.
  output_.push_back(new_d);
  return;
}

const bool Adjacent(const Box *b1, const Box *b2) {
  const double &b1_xmin = b1->min_.re();
  const double &b1_xmax = b1->max_.re();
  const double &b1_ymin = b1->min_.im();
  const double &b1_ymax = b1->max_.im();

  const double &b2_xmin = b2->min_.re();
  const double &b2_xmax = b2->max_.re();
  const double &b2_ymin = b2->min_.im();
  const double &b2_ymax = b2->max_.im();

  // West or east
  if (b1_ymin == b2_ymin && (b1_xmax == b2_xmin || b1_xmin == b2_xmax)) {
    return true;
  }
  if (b1_xmin == b2_xmin && (b1_ymax == b2_ymin || b1_ymin == b2_ymax)) {
    return true;
  }

  return false;
}

class ConnectedComponent {
 public:
  ConnectedComponent(const Box *box) {
    member_boxes_.push_back(box);
    min = box->min_;
    max = box->max_;
  }

  void Add(const Box *box) {
    // Add this to the list of boxes that make up this
    // connected component.
    member_boxes_.push_back(box);

    // Update the extent of this connected component.
    if (box->min_.re() < min.re()) {
      min = Complex(box->min_.re(), min.im());
    }
    if (box->min_.im() < min.im()) {
      min = Complex(min.re(), box->min_.im());
    }
    if (box->max_.re() > max.re()) {
      max = Complex(box->max_.re(), max.im());
    }
    if (box->max_.im() > max.im()) {
      max = Complex(max.re(), box->max_.im());
    }
  }

  void AddAll(ConnectedComponent *c) {
    for (unsigned int i = 0; i < c->member_boxes_.size(); ++i) {
      Add(c->member_boxes_[i]);
    }
  }

  bool Connected(const Box *box) {
    vector<const Box *>::const_iterator it = member_boxes_.begin();
    while (it != member_boxes_.end()) {
      const Box *b = (*it);
      if (Adjacent(b, box)) {
        return true;
      }
      ++it;
    }

    return false;
  }

  const Box *BoundingBox() {
    return new Box(min, max);
  }

  vector<const Box *> member_boxes_;
  Complex min;
  Complex max;
};

// What is this trying to do?
//
void Algorithm::AttemptIsolation() {
  list<ConnectedComponent *> components;
  //  cerr << endl << endl << "HERE 2" << endl << endl;

  vector<const Box *>::const_iterator it = ambiguous_.begin();

  //  cerr << endl << endl << "HERE 3" << endl << endl;

  components.push_back(new ConnectedComponent(*it));

  //  cerr << endl << endl << "HERE 4" << endl << endl;

  ++it;

  //  cerr << endl << endl << "HERE 5" << endl << endl;

  vector<ConnectedComponent *> matching_comps;

  //  cerr << endl << endl << "HERE 6" << endl << endl;

  while (it != ambiguous_.end()) {
    const Box *b = (*it);
    matching_comps.clear();
    list<ConnectedComponent *>::iterator c_it = components.begin();
    while (c_it != components.end()) {
      ConnectedComponent *comp = (*c_it);
      if (comp->Connected(b)) {
        matching_comps.push_back(comp);
        c_it = components.erase(c_it);
        continue;
      }
      ++c_it;
    }

    if (matching_comps.size() == 1) {
      components.push_back(matching_comps[0]);
      matching_comps[0]->Add(b);
    } else if (matching_comps.size() != 0) {
      ConnectedComponent *comp = matching_comps.back();
      matching_comps.pop_back();

      for (unsigned int i = 0; i < matching_comps.size(); ++i) {
        comp->AddAll(matching_comps[i]);
        delete matching_comps[i];
      }

      comp->Add(b);
      components.push_back(comp);
    } else {
      components.push_back(new ConnectedComponent(b));
    }

    ++it;
  }

  ambiguous_.clear();
  list<ConnectedComponent *>::iterator c_it = components.begin();
  while (c_it != components.end()) {
    const Box *b = (*c_it)->BoundingBox();
    // Keep the mid and the radius handy.
    const Complex mid = b->mid();
    const double rad = b->radius();
    if (pred_.Tdash(6, mid, 4*rad) &&
        pred_.Tdash(1.5, mid, 8*rad)) {
      if (pred_.PointTest(mid, 4*rad)) {
        // A success ! . The region can be inserted into the output.
        InsertOutput(mid, 4*rad);
        if (display_) {
          output_b_.push_back(b);
        }
      } else {
        // If the 8 point test fails the region can be
        // excluded.
        if (display_) {
          rejects_.push_back(b);
        } else {
          delete b;
        }
      }
    } else {
      ambiguous_.push_back(b);
    }

    delete (*c_it);
    ++c_it;
  }
}

