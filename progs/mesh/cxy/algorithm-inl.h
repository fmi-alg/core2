/* **************************************
   File: algorithm-inl.h

   Description: 
	Algorithm code factored out from cxy.cpp.

   HISTORY: 2008: Long Lin (Java Code for cxy paper)
	    Apr, 2009: Shuxing Lu (converted Java code to Core2)
	    Jun, 2010: Narayan Kamath (refactored code into independent units)

   Since Core Library  Version 2.0
   $Id: algorithm-inl.h,v 1.10 2010/09/20 16:07:58 exact Exp $
 ************************************** */

#ifndef CXY2_ALGORITHM_INL_H_
#define CXY2_ALGORITHM_INL_H_

#include <list>
#include <vector>

#include "cxy-defs.h"
#include "box.h"
#include "multilist-inl.h"

using namespace std;

template <typename T> class Algorithm2D {
public:
  // Construct an algorithm class from a given predicate,
  // a bounding box bo and parameter N.
  Algorithm2D(const T *predicate,
      Box *b0, unsigned int max_gen, const unsigned int min_gen) :
        pred_(predicate),
        b0_(b0),
        max_gen_(max_gen),
        min_gen_(min_gen),
        max_subdivision_(0),
        q1_(MultiList<T>(min_gen_, max_gen_, &q_c0_, *pred_)) {

    cout << "Maximum subdivision depth : " << max_gen << endl;
    cout << "Minimum subdivision depth : " << min_gen << endl;
  }

  void run(bool draw_box) {
    Subdivide(draw_box);
    Construct();
  }

  // -----------------------------
  // Subdivision step
  // -----------------------------
  void Subdivide(bool draw_box) {
    unsigned int max_generation = 0;
    SignCondition sgn;
    // Push the initial box to the beginning of the list.
    q0_.push_back(b0_);
    while (!q0_.empty()) {
      // Fetch the box from the back of the list.
      Box *b = q0_.back();
      q0_.pop_back();
      sgn = pred_->Exclude(*b);
      if (sgn != _ZERO) {
        // The box has satisfied the exclusion predicate, this means
        // that it can be rejected from further consideration.
        //
        // If draw_box=true, we store the box for display, else delete it!
        if (draw_box) {
          q_c0_.push_back(b);
          // Mark the box appropriately, so that it can be drawn
          // the right color.
          if (sgn == _POS) {
        	  b->setMark(Box::OUT_BOX_POS);
          }
          if (sgn == _NEG) {
        	  b->setMark(Box::OUT_BOX_NEG);
          }
        } else {
          delete b;
        }
      } else if (b->getGeneration() >= min_gen_ && pred_->Include(*b)) {
        // The box has satisfied the inclusion predicate, and it can
        // be sent to the next stage of processing.
        // vector_ops::AddToPriorityQueue(&q1_, b);
        q1_.push_back(b);

        if (max_generation < b->getGeneration()) {
          max_generation = b->getGeneration();
        }
      } else if (b->getGeneration() <= max_gen_) {
        // Neither included nor excluded, which means that it needs
        // to be split.
        const vector<Box *> *split = b->split();
        // TODO : Fix these counts.
        // counter++;
        // cBox+=3;
        q0_.push_back(split->at(0));
        q0_.push_back(split->at(1));
        q0_.push_back(split->at(2));
        q0_.push_back(split->at(3));
        // We need BO for display purposes so we wont delete it.
        // Note that this delete is irrespective of draw_box because
        // we wont need this box even if we are drawing boxes.
        if (b != b0_) {
          delete b;
        }
      } else {
        // In this case, the box could not be included, nor excluded
        // but could not be subdivided either, because it was too small.
        q_unresolved_.push_back(b);
      }
    }

    cout << "Subdivide complete, maximum subdivision depth : " << max_generation << endl;
    Balance();
    cout << "Balance complete" << endl;

    // This is the maximum extent of subdivision (the maximum
    // depth of our quadtree of in-boxes) .
    max_subdivision_ = max_generation;
  }

  // Constructs the graph from the priority queue of in-boxes.
  void Construct() {
    while (!q1_.empty()) {
      // Gets the highest priority box from our priority queue. This
      // is one of the smallest boxes in the queue.
      Box *b = q1_.peek();
      // Pop it from the multilist.
      q1_.pop();

      // This box has been deemed ambiguous. (Sign +ve on all four
      // corners, and has two calculated curve vertices).
      if (pred_->Ambiguous(*b)) {
        // Full split this ambiguous box.
        const vector<Box *> *children = b->split();

        // Either exclude the split children, or push them
        // back into the queue for processing.
        for (unsigned int i = 0; i < children->size(); i++){
          Box *child = (*children)[i];

          if (pred_->Exclude(*child)) {
            q_c0_.push_back(child);
          } else {
            // Process this child. This involves computing curve vertices,
            // and promoting neighbouring in-boxes in the priority order
            // for processing.
            Process(child);
            // Rebalance neighbors around the child.
            Rebalance(child);
          }
        }

        // We have no further use for B. Delete it.
        delete b;
      } else {
        // We should never reach this branch on a box that
        // isnt an in-box.
        assert(b->getMark(Box::IN_BOX));

        // This seems like a winner, add vertices and be done.
        //pred_->ComputeVertices(b);
        //q3_.push_back(b);
        Process(b);
      }
    }
  }

  // Compute the curve vertices that lie on the box edges, and
  // promote ambiguous neighbours of this box.
  void Process(Box *box) {
    pred_->ComputeVertices(box);
    q3_.push_back(box);

    const vector<Box *> *neighbors = box->getNeighbors();

    for (unsigned int i = 0; i < neighbors->size(); ++i) {
      Box *neighbor = (*neighbors)[i];

      // If the computation of the curve vertices resulted in
      // this neighbor becoming ambiguous, then promote it
      // to the front of the priority list (of boxes of the same size).
      if (pred_->Ambiguous(*neighbor)) {
        if (neighbor->getMark(Box::IN_BOX)) {
          q1_.erase(neighbor);
        }
        q1_.push_front(neighbor);
      }
    }
  }

  // These functions are aliased to provide consistency with
  // names used in the paper.
  void Balance() {
    q1_.GlobalBalance();
  }
  void Rebalance(Box *box) {
    q1_.LocalBalance(box);
  }


  // Accessors for various data structures. The only data
  // structures that are of interest outside the algorithm are
  // the set of rejected boxes (c0) and the set of accepted
  // boxes (q3), and all boxes in q3 will have the curve passing
  // through them.
  vector<Box *> *mutable_c0() {
    return &q_c0_;
  }
  const vector<Box *> *c0() const {
    return &q_c0_;
  }
  const vector<Box *> *q3() const {
    return &q3_;
  }
  const vector<Box *> *unresolved() const {
    return &q_unresolved_;
  }

 private:
  // The input to algorithm, a predicate that decides what
  // to do with a given box, and can add vertices to the box
  // if required, and an initial box to run the algorithm on.
  const T *pred_;
  Box *b0_;
  const unsigned int max_gen_;
  const unsigned int min_gen_;
  unsigned int max_subdivision_;

  // Intermediate data structures used by the algorithm, are
  // accessible for debugging purposes etc. The names correspond
  // to the terminology used in the paper.
  vector<Box *> q0_;

  // A priority queue for inboxes. Provides support for fetching
  // the minimum box in constant time, and for global (tree wide)
  // and local (originating from a tree node) rebalancing.
  MultiList<T> q1_;

  vector<Box *> q3_;
  // Set of boxes rejected (by C0)
  vector<Box *> q_c0_;
  // Set of boxes that we could not make a decision on
  vector<Box *> q_unresolved_;
};


#endif  // CXY2_ALGORITHM_INL_H_
