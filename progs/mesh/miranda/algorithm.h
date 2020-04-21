/* *************************************************
 *  algorithm.h
 *  major algorithm
 *
 *  This contains a name space called Algorithm:
 *
 *  It contains only one method called Run.
 *
 *  Run takes arguments:
 *  	(1) An instance of the class MKPredicates  (which has 2 polynomials)
 *  	(2) An initial Box 
 *  	(3) 3 Queues: Q_output, Q_ambiguous, Q_exclude
 *  	
 *  Run has several while loops:
 *  	(1) Apply the C0 and JC test to put boxes into a queue called Q_confirm 
 *  	(2) Apply the MK test to boxes in Q_confirm to produce a priority queue
 *  		of root boxes, called Q_roots
 *  	(3) Finally, refine the roots in Q_root until they are pairwise
 *  		disjoint, and placed into Q_output
 *
 * Author: Shang Wang, July 17, 2011
 *
 * Since Core 2.1.
 *
************************************************** */

#ifndef MK_ALGORITHM_H_
#define MK_ALGORITHM_H_

#include <vector>
#include <iostream>

#include "mk-inl.h"
#include "rootbox.h"

using namespace std;

namespace Algorithm {

  template <typename DT,typename NT>
   void Run(
      const MKPredicates<DT,NT> &pred,
      const BoxT<DT> *initial,
      vector< RootBoxT<DT,NT> *> *Q_output,
      vector<const BoxT<DT> *> *Q_ambiguous,
      vector<const BoxT<DT> *> *Q_exclude) {

    // inner type definition
    typedef BoxT<DT> Box;
    typedef RootBoxT<DT,NT> RootBox;

    // main process queue
    vector<const Box *> Q_tmp;
    // Jacobian queue
    vector<const Box *> Q_confirm;
    // here we make a final queue for refinement processing
    // it is only used when we find a box that passes MK test(a.k.a root box)
    // we will make a Rootbox structure to hold vaious useful information
    vector<RootBox *> Q_final;

    if(!pred.Exclude(initial)) { // current box fails C0
      Q_tmp.push_back(initial);
    }
    else {
        Q_exclude->push_back(initial);
    }
    
    // main loop
    while(!Q_tmp.empty()) { 
      const Box *current = Q_tmp.back();
      Q_tmp.pop_back();
      // (current box)X2 passes the Jacobian test
      const Box *double_current = current->Dilate(2);
      if(pred.JTest(double_current)) {
        Q_confirm.push_back(current);  // wait for further confirmation
      }
      else {
        pred.Split_Exclude(current, &Q_tmp, Q_exclude); // split and test C0
      }
      delete double_current;

      // confirmation loop
      while(!Q_confirm.empty()) {
        const Box *box = Q_confirm.back();
        Q_confirm.pop_back();
        // box too small
        if(pred.Min(box)) {
          Q_ambiguous->push_back(box);
          continue;
        }
        // also do MK test on (box)X2
        const Box *double_box = box->Dilate(2);
        if(pred.MKTest(double_box)) {
          // already found a root box, put it in Q_final queue for refinement
          // rootbox creation based on double_box's demension
          Q_final.push_back(new RootBox(double_box)); 
          Q_exclude->insert(Q_exclude->end(), Q_confirm.begin(), Q_confirm.end());
          Q_confirm.clear();  // include area already found in region, clean Q_confirm
          // both box and double_box can be deleted because the constructor of RootBox 
          // will make new outer and inner box based on double_box's dimension
          delete box;
          delete double_box; 
          break;
        }
        else {
          pred.Split_Exclude(box, &Q_confirm, Q_exclude);
          delete double_box;
        }
      }//while (Q_confirm)
    }//while (Q_tmp)

    // by reaching here, the root boxes are all found, and stored in Q_final
    // the next step would be to refine root boxes until they are strongly isolated 
    // and place them into Q_output for display, and we are also cleaning
    // boxes in Q_final. In the end, Q_final should be empty
    while(!Q_final.empty()) {
      RootBox *current = Q_final.back();
      Q_final.pop_back();
      // fail to make the inner box strongly isolated
      // for now just put it back to Q_output???????
      if(!current->StrongIsol(pred)) {
        Q_output->push_back(current);
        continue;
      }
      else {
        // succeeded in making the root box strongly isolated
        // (all boxes in Q_output are the inner boxes for RootBoxes
        // because the Q_output has type of Box not RootBox)
        
        // this flag is to check whether a joint case happens. if found = true,
        // we can discard "current", because all boxes in Q_output are strongly isolated,
        // thus we can be sure that "current" contains the same root as the joint one, and
        // we can discard "current"
        bool found = false;
        for(unsigned int i = 0; i != Q_output->size(); i++) {
          if(!current->Disjoint(Q_output->at(i)->innerBox_)) { // not disjoint, discard current
            delete current;
            found = true;
            break;
          }
        }
        if(!found) { // gone though all elements in Q_output, still not found
          Q_output->push_back(current);
        }
      }

    } // (Q_final)

  }//Run
}  // end of namespace algorithm

#endif
