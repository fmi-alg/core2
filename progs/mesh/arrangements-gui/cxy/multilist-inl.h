/*
 * multilist-inl.h
 *
 * Implements a templated priority queue of boxes
 * 	(smaller box sizes have higher priority)
 *
 * It is used in algorithm_inl.h.
 *
 * The template parameter, P, is a predicate or function.
 * 	Strangely, in algorithm_inl.h, P is given as a number type.
 * 	What gives?  (Chee/Shang)
 *
 *
 *  Created on: Jun 16, 2010
 *      Author: narayan
 */

#ifndef MULTILIST_INL_H_
#define MULTILIST_INL_H_

#include <iostream>
#include <vector>
#include <list>

#include "cxy-defs.h"
#include "box.h"

using namespace std;

namespace cxy {

// A multilist is a container used for storing in-boxes
// that are generated as a result of the subdivision step. It
// is a priority queue type data structure, that provides the
// fundamental priority queue operations peek() that returns
// a pointer to the element with the highest priority, and pop()
// that can remove the element with the highest priority from
// this container.
//
// The priority of a box is the inverse of the size. Smaller boxes
// have higher priority. Within a list of boxes that have the
// same priority, boxes closer to the head of the list have higher
// priority (use push_back( ) / push_front( ) to control this). This
// fine grained level of control is needed to deal with ambiguous
// boxes that will have higher priority than non ambigious boxes within
// the list of boxes of the same size.
//
//
// This container supports the following operations. All of the operations
// except (Local/Global)balance return in constant time.
//
// peek() : returns (one of the) smallest boxes stored in this
// multi list. Always return the first box in the linked list of
// boxes of the smallest size.
//
// pop() : removes (one of the) smallest boxes stored in this multi
// list.
//
// push_back()
// push_front() :
// Push a box to the front or back of the list of boxes which are
// the SAME SIZE as the input box. There is no "global" view of the
// back or front, boxes are always returned in ascending order of
// size.
//
// size() : returns the number of boxes in this list.
// empty() : returns true iff the list has no boxes.
// erase() : erase a member of this data structure.
//
// LocalBalance()
// GlobalBalance() : Refer to function documentation.
//
template <typename P> class MultiList {
 public:
  // Needs the minimum and maximum generation id of
  // boxes that this structure should support.
  MultiList(unsigned int min_gen,
            unsigned int max_gen,
            vector<Box *> *rejects,
            const P &pred) :
    min_gen_(min_gen),
    max_gen_(max_gen + 1),
    lists_(vector<list<Box *> >(max_gen - min_gen + 2)),
    rejects_(rejects),
    pred_(&pred) {
    // This is the pointer to the current smallest box size
    // stored in this structure.
    min_ptr_ = lists_.size() + 1;
    size_ = 0;
  }

  // Push the box a to the end of the target list for the box a.
  void push_back(Box * a) {
    Push(a, false);
  }
  // Push the box a to the front of the target list for the box a.
  void push_front(Box *a) {
    Push(a, true);
  }
  // The total number of boxes in this container.
  unsigned int size() const {
    return size_;
  }
  // Return true iff there are no boxes in this container.
  bool empty() {
    return size_ < 1;
  }

  // Return a pointer to the box with the highest priority
  Box *peek() {
    // Maybe update the pointer to the list of minimum sized
    // boxes.
    MaybeUpdateMin();

    // Return the front of that list.
    return lists_[min_ptr_].front();
  }

  // Remove the highest priority box from the list.
  void pop() {
    MaybeUpdateMin();

    // Sanity check.
    assert(min_ptr_ < lists_.size() && !lists_[min_ptr_].empty());
    // Get rid of the box.
    lists_[min_ptr_].pop_front();
    // Decrement the size.
    --size_;
  }

  // Erase the input box from the list that contains it.
  void erase(Box *b) {
    // Fetch the target list.
    const unsigned int idx = Index(b);
    list<Box *> &target = lists_[idx];

    // Sanity check.
    assert(b->getMark(Box::IN_BOX));
    // Perform deletion.
    target.erase(b->getIterator());
    --size_;

    // The min_ptr might have to be updated if this was the
    // last box in that list.
    MaybeUpdateMin();
  }

  // Locally rebalance the container starting with a given element.
  // Changes are propagated outwards. The neighbours are split if
  // required, and then the neighbours of the neighbours etc. This
  // function will only modify boxes that are members of this
  // container, and not other boxes.
  void LocalBalance(Box *elem);
  // Balance the entire container. This is equivalent to calling
  // LocalBalance( ) in every box in the list of the smallest boxes.
  void GlobalBalance();

  ~MultiList() { }

 private:
  // Push an element to either the front or back of the list of
  // boxes that are the same size as the element. This is useful
  // to control the relative priority of a set of boxes of the
  // same size.
  void Push(Box *a, const bool list_front = false) {
    // Fetch the target list.
    const unsigned int idx = Index(a);
    list<Box *> &target = lists_[idx];

    // Mark this box as an in-box. This is very important, most
    // balancing procedures dont bother to change boxes are not
    // in boxes.
    a->setMark(Box::IN_BOX);

    // Add it either to the front or the back
    // of the list.
    if (list_front) {
      target.push_front(a);
      a->setIterator(target.begin());
    } else {
      target.push_back(a);
      // NOTE(narayan) :
      // Note the -- here. This cost me 2 hours of debugging.
      // target.end( ) points to a sentinel element : one element
      // after the end. We could have used rend( ) but that is
      // that is a reverse_iterator.
      a->setIterator(--target.end());
    }
    ++size_;

    // Update the min_ptr_ if required.
    if (idx < min_ptr_) {
      min_ptr_ = idx;
    }
  }

  // Fetch the index of the list containing boxes the
  // same size as a.
  unsigned int Index(const Box *a) const {
    // Note that lists of smaller boxes are stored at
    // lower indices.
    unsigned int ret =  max_gen_ - a->getGeneration();

    assert(ret >= 0 && ret < lists_.size());
    return ret;
  }

  // Update the pointer to the list of boxes of minimum size.
  void MaybeUpdateMin() {
    // Update the min_ptr_;
    unsigned int idx = min_ptr_;
    // Find the first non empty list.
    while (lists_[idx].empty()) {
      ++idx;
      if (idx == lists_.size()) {
        min_ptr_ = idx;
        return;
      }
    }

    // Update the min_ptr_.
    min_ptr_ = idx;
  }

  // The minimum generation id. (Corresponds to the largest boxes).
  const unsigned int min_gen_;
  // The maximum generation id. (Corresponds to the smallest boxes).
  const unsigned int max_gen_;
  // Pointer to the list of boxes of minimum size.
  unsigned int min_ptr_;
  // The size of this container.
  unsigned int size_;
  // The main storage , a vector of list of boxes. We use a list< >
  // to allow for constant time deletes, and pushes to the front and
  // back of the list.
  vector<list<Box *> > lists_;
  vector<Box *> *rejects_;
  const P *pred_;
};

template <typename P> void MultiList<P>::LocalBalance(Box *elem) {
  // We avoid a recursive implementation to prevent stack overflow
  // issues. process_queue( ) is used "unroll" the recursion.
  list<Box *> process_queue;
  process_queue.push_back(elem);

  while (!process_queue.empty()) {
    Box *elem = process_queue.front();

    // Keeps tack of whether this box caused a split, if it did not
    // we are done processing it - and it can be re-inserted into the
    // container.
    bool split_caused = false;

    // Fetch the neighbours, of this box.
    const vector<Box *> *neighbors = elem->getNeighbors();
    vector<Box *>::const_iterator it = neighbors->begin();

    // Iterate over the neighbour.
    while (it != neighbors->end()) {
      Box *neighbor = *it;

      // If the neighbour is an in-box AND the neighbour is larger
      // than twice of the current box, then we need to split it.
      if (neighbor->getMark(Box::IN_BOX) &&
          neighbor->getGeneration() < (elem->getGeneration() - 1)) {
        split_caused = true;

        // Split the neighbour, and erase it from the data structure.
        const vector<Box *> *children = neighbor->split();

        // We can get rid of the box as we never use it.
        erase(neighbor);

        // Push the children back into the queue for processing, their
        // neighbours might need to be split as well.
        vector<Box *>::const_iterator c_it = children->begin();
        while (c_it != children->end()) {
          process_queue.push_back(*c_it);
          ++c_it;
        }
      }

      ++it;
    }

    // If no split was caused, we are done processing.
    if (!split_caused) {
      push_front(elem);
      process_queue.pop_front();
    }
  }
}

template <typename P> void MultiList<P>::GlobalBalance() {
  list<Box *> &min_list = lists_[min_ptr_];
  list<Box *>::iterator it = min_list.begin();

  // 	Note(Chee): comment out the next line (assert)
  // 	to avoid warning that "start_size" is an unused variable
  // 	Probably because assertion below is not turned on.
  // unsigned int start_size = min_list.size();
  while (it != min_list.end()) {
    Box *box = (*it);
    list<Box *>::iterator delete_point = it;
    // Increment the iterator first so that we can delete
    // behind the iterator.
    ++it;
    // Delete behind the iterator, this way we can avoid
    // having to have Erase( ) return an iterator since
    // we dont invalidate the current one.
    //
    // We erase the box because the implementation of LocalBalance( )
    // will push the box back into the queue anyway.
    erase(box);
    // Balance around this box.
    LocalBalance(box);
  }

  // A rebalance should never result in more boxes added to the min_list.
  // 	Note(Chee): comment out the next line (assert)
  // 	to avoid warning of unused variable
  // assert(min_list.size() == start_size);
}


} //namespace cxy 

#endif /* MULTILISTINL_H_ */
