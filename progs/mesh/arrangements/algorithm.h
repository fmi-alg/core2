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
	   
	   
	int mk_ret;// Used to store the return value of MK-test

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
      // (current box)*2 passes the Jacobian test
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
			  //box too small
		/*if(pred.Min(box)) {
          Q_ambiguous->push_back(box);
          continue;
        }*/
        // also do MK test on (box)*2
        const Box *double_box = box->Dilate(2);
		mk_ret = pred.MKTest(double_box);
		if(mk_ret == 1) {// MK-test succeeded

          // already found a root box, put it in Q_final queue for refinement
          // rootbox creation based on double_box's dimension
		  Q_final.push_back(new RootBox(box, double_box)); // Why do we push dilated box?
          Q_exclude->insert(Q_exclude->end(), Q_confirm.begin(), Q_confirm.end());
          Q_confirm.clear();  // include area already found in region, clean Q_confirm
          // both box and double_box can be deleted because the constructor of RootBox 
          // will make new outer and inner box based on double_box's dimension
          delete box;
          delete double_box; 
          break;
        }else if (mk_ret == 0) {// MK-test detected an exclusion box
			Q_exclude->push_back(box);
		}
        else {// MK-test failed
          pred.Split_Exclude(box, &Q_confirm, Q_exclude);
          delete double_box;
        }
      }//while (Q_confirm)
    }//while (Q_tmp)

    // by reaching here, the root boxes are all found, and stored in Q_final
    // the next step would be to refine root boxes until they are strongly isolated 
    // and place them into Q_output for display, and we are also cleaning
    // boxes in Q_final. In the end, Q_final should be empty

	// Vikram: Why do we need to do strong root isolation? Since the 
	// sign of the jacobian is the same on overlaping boxes, we 
	// only have to output one box from a set of overlapping 
	// boxes corresponding to a root.
	typename std::vector <RootBox *>::iterator new_end;
	vector <RootBox *> Q_temp;
	RootBox *current;
	/*	
	std::cout <<"Printing Final Queue"<<std::endl;
	for (unsigned int i=0; i<Q_final.size(); i++) {
		std::cout<< (*Q_final[i]) << std::endl;
	}
	std::cout <<"Clearing Final Queue"<<std::endl;
	*/
	while(!Q_final.empty()) {
		current = Q_final.back();
		Q_final.pop_back();
	//std::cout << " current box = "<< (*current) << std::endl;
		for (unsigned int i=0; i<Q_final.size(); i++) {
			if (current->Disjoint(Q_final[i])) {
				Q_temp.push_back(Q_final[i]);
			}
		}

	//	for (unsigned int i=0; i<Q_temp.size(); i++) {
	//		std::cout<< (*Q_temp[i]) << std::endl;
	//	}
		
			// Search if any other box in Q_final overlaps with current.
			// Discard any such box.
			// It will help to start with smaller boxes first, but now
			// we do a starightforward quadratic time algorithm
		Q_output->push_back(current);
		Q_final=Q_temp;
		Q_temp.clear();				
	} // (Q_final)
  }//Run
}  // end of namespace algorithm

#endif

