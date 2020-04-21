/* *************************************************
*    rootbox.h
* 
*    This file describes the templated class called RootBoxT.
*
*       The template parameter DT is some number type,
*         typically DoubleWrapper, machine_double, or BigFloat.
*       This type is used for the dimension of the boxes.
*
*    The class has the following members:
*       --Boxes: OuterBox and InnerBox
*       --Boolean: StrongIsolFlag, FailRefineFlag
*       
*    OuterBox is known to contain a unique root of fxy, gxy in its interior
*    InnerBox is contained in Outerbox, and also
*       known to contain a unique root of fxy, gxy.
*       Thus, the annulus region between Outerbox and InnterBox has no roots.
*
*    Initially, InnerBox=OuterBox.
*       We will preserve the OuterBox, but the InnerBox can shrink
*       by calling the Refine Procedure.
*       
*    StrongIsolFlag is true if we know that 3*InnerBox contains a unique root.
*       Initially, StrongIsolFlag = false.
*    FailRefineFlag is true if our attempt to refine the InnerBox fails.
*       Note that this can only happen because our Min_Size global variable
*       prevents further subdivision during refinement.
*       Initially, FailRefineFlag = false.
*
* Author: Shang Wang and Chee Yap, Aug 30, 2011
*
* Since Core 2.1.
************************************************** */

#ifndef MK_ROOTBOX_H
#define MK_ROOTBOX_H

#include "mk-defs.h"
#include "mk-inl.h"

//template <typename DT>
//inline const bool Overlap(const IntervalT<DT> &s, const IntervalT<DT> &t);

/* rootbox class  */
template <typename DT, typename NT>
class RootBoxT {
public:
  // Constructor
  RootBoxT(const BoxT<DT> *B) : 
    strongIsolFlag_(false),
    failRefineFlag_(false) {
      unsigned int gen_id = B->generation_id;
      const IntervalT<DT> &xRange = B->x_range;
      const IntervalT<DT> &yRange = B->y_range;
      innerBox_ = new BoxT<DT>(gen_id, xRange, yRange);
      outerBox_ = new BoxT<DT>(gen_id, xRange, yRange);
  }

	// Constructor from a pair of inner box and outer box
	RootBoxT(const BoxT<DT> *IB, const BoxT<DT> *OB) : 
    strongIsolFlag_(false),
    failRefineFlag_(false) {
		unsigned int gen_id = IB->generation_id;
		const IntervalT<DT> &xRange = IB->x_range;
		const IntervalT<DT> &yRange = IB->y_range;
		innerBox_ = new BoxT<DT>(gen_id, xRange, yRange);
		const IntervalT<DT> &xxRange = OB->x_range;
		const IntervalT<DT> &yyRange = OB->y_range;
		outerBox_ = new BoxT<DT>(gen_id, xxRange, yyRange);
	}
  ~RootBoxT() {
    delete outerBox_;
    delete innerBox_;
  }

  // Inner type definition
  typedef BoxT<DT> Box;
  typedef IntervalT<DT> Interval;

  /***************** methods ***********************/  
  void Cover(const Box *box, 
             vector<const Box*> *queue) {
    const DT &x_start     = box->x_range.getL();
    const DT &x_end       = box->x_range.getR();
    const DT &x_mid       = box->x_range.mid();
    const DT &y_start     = box->y_range.getL();
    const DT &y_end       = box->y_range.getR();
    const DT &y_mid       = box->y_range.mid();
    const DT &x_halfwidth = box->x_range.width() / 4;
    const DT &y_halfwidth = box->y_range.width() / 4;

    // statistic collection for final output
    extern unsigned int largest_gen;
    const unsigned int gen_id = box->generation_id + 1;
    if(largest_gen < gen_id)
      largest_gen = gen_id;

    queue->push_back(new Box(gen_id, // q_1
      Interval(x_start, x_mid), Interval(y_start, y_mid)));
    queue->push_back(new Box(gen_id, // q_2
      Interval(x_mid, x_end), Interval(y_start, y_mid)));
    queue->push_back(new Box(gen_id, // q_3
      Interval(x_start, x_mid), Interval(y_mid, y_end)));
    queue->push_back(new Box(gen_id, // q_4
      Interval(x_mid, x_end), Interval(y_mid, y_end)));
    queue->push_back(new Box(gen_id, // q_1/2
      Interval(x_mid, x_end), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
    queue->push_back(new Box(gen_id, // q_3/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_start, y_mid)));
    queue->push_back(new Box(gen_id, // q_5/2
      Interval(x_start, x_mid), Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
    queue->push_back(new Box(gen_id, // q_7/2
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), Interval(y_mid, y_end)));
    queue->push_back(new Box(gen_id, // q_0
      Interval(x_mid-x_halfwidth, x_mid+x_halfwidth), 
      Interval(y_mid-y_halfwidth, y_mid+y_halfwidth)));
  }
  // release all pointers that stay in the queue
  void Clear_queue(vector<const Box *> *queue) {
    while(!queue->empty()) {
      const Box *b = queue->back();
      queue->pop_back();
      delete b;
    }
  } // Clear_queue

  // Cover_Exclude() calls Cover() to split the box into 9 children
  // and use C0 to test each of them, put children into queue if it fails C0 test
  void Cover_Exclude(const MKPredicates<DT,NT> &pred,
      const Box *box, vector<const Box *> *queue) {
    // temp queue for processing
    vector<const Box *> temp;
    // cover box into 9 children
    Cover(box, &temp);
    // Do C0 test for each box
    while(!temp.empty()) {
      const Box *b = temp.back();
      temp.pop_back();
      if(!pred.Exclude(b)) { // C0 fails, keep the box in the queue
        queue->push_back(b);
      }
      else {
        // annulus region doesn't belong to anything, just release memory
        delete b;  
      }
    }
  }

  const bool Refinement(const MKPredicates<DT,NT> &pred) {
    vector<const Box *> Qtmp;
    Cover_Exclude(pred, innerBox_, &Qtmp);
    // loop started
    while(true) {
      // it might be that all children holds C0 test, queue empty
      if(Qtmp.empty()) {
cout << "all passed C0 test, fail refine" << endl;
        return false;
      }
      const Box *box = Qtmp.back();
      Qtmp.pop_back();
      // too small
      if(pred.Min(box)) {
        // we cannot go any further, set this box as the most precise one
        delete innerBox_;
        innerBox_ = box;
cout << "reached the minimum size, fail refine" << endl;
        Clear_queue(&Qtmp);
        return false;
      }
      // keep using MK test to test boxes, 
      // if MK holds, set new inner box to current one
      if(pred.MKTest(innerBox_)) {
        delete innerBox_;
        innerBox_ = box;
        Clear_queue(&Qtmp);
        return true;
      }
      else {  // cannot pass MK test, use cover to split 9 children
        Cover_Exclude(pred, box, &Qtmp);
        delete box;
      }
    }
  }

  // This routine will make an inner box a strong rootbox,
  // return false means the inner box cannot be refined anymore
  const bool StrongIsol(const MKPredicates<DT,NT> &pred) {
    Box *triple_box = innerBox_->Dilate(3);
    while(!pred.JTest(triple_box)) {
      if(!Refinement(pred)) {
        failRefineFlag_ = true;
        delete triple_box;
        return false;
      }
      else {  // can refine, free previous 3B and make another
              // 3B based on the new inner box created by Refinement()
        delete triple_box;
        triple_box = innerBox_->Dilate(3);
      }
    }
    // out of loop, which indecates that JTest holds
    delete triple_box;
    strongIsolFlag_ = true;
    return true;
  }
  
  // check if two boxes are disjoint
  // return true if either innerBox.x_range isn't overlap with other.x_range
  // or innerBox.y_range isn't overlap with other.y_range
  const bool Disjoint(const Box *other) {
    const Interval &inner_x = innerBox_->x_range;
    const Interval &inner_y = innerBox_->y_range;
    const Interval &other_x = other->x_range;
    const Interval &other_y = other->y_range;
    return (!Overlap(inner_x, other_x) || !Overlap(inner_y, other_y));
  }

	// check if two root boxes are disjoint
	const bool Disjoint(const RootBoxT *other) {
		return Disjoint(other->innerBox_);
	}


	
	
  /**************  members  ***********************/
  const Box *outerBox_;
  const Box *innerBox_;
  bool      strongIsolFlag_;
  bool      failRefineFlag_;
};

template <typename DT, typename NT>
inline std::ostream& operator<<(std::ostream& o, const RootBoxT<DT, NT>& RB) {
	//o << RB.innerBox_->x_range << " * "<< RB.innerBox_->y_range;
	o << (*RB.innerBox_);
	return o;
	
}
#endif
