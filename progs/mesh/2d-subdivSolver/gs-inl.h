/* ***************************************************
*
* mk-inl.h
*
*    This file describes one class called GSPredicates.
*    The class has the following members:
*    	--fxy, gxy are 2 bivariate polynomials
*    	--Variable such as
*    		--max_size
*    		--min_size
*    		--max_gen_id
*
*    GS stands for Moore-Kioustelides, who gave a modified form of
*    the Miranda Test for existence of roots.
*    This class provided predicates for the
*
* 	C0 test: C0 is the exclusion predicate and holds if 
* 	JC test: JC is the Jacobian test, and if it succeeds, there is
* 		at most one root of fxy=gxy=0.
* 	GS test: This is performed only if JC holds.
* 		If it passes, there is a unique root.
*
*   We need two types of dyadic numbers:
*     DT : number type for box intervals.
*     NT : number type for intervals used by evaluation.
*     We assume that DT <= NT, i.e., DT is a subtype of NT.
*   Typically, both NT and DT can be DoubleWrapper.
*   But for high degree polynomials, we may need NT to be BigFloat.
*       while keeping DT to be DoubleWrapper
*
* Author: Shang Wang, July 16, 2011
*
* Since Core 2.1.
************************************************** */

#ifndef GS_GS_INL_H_
#define GS_GS_INL_H_

#include "mk-defs.h"
#include "box.h"

using namespace std;

template <typename DT, typename NT>
class GSPredicates {
public:
  // Constructor
  GSPredicates(const CORE::Curve<DT> &fxy, const CORE::Curve<DT> &gxy, 
          const DT &min_size, const DT &max_size,
	  const unsigned int max_gen_id) : fxy_(fxy), gxy_(gxy),
	  jacobian_(MatrixT<CORE::Curve<DT> >(2)), min_size_(min_size),
	  max_size_(max_size), max_gen_id_(max_gen_id)
  {
    // differentiate is "self-modified" operator
    CORE::Curve<DT> temp1 = fxy, temp2 = fxy;
    CORE::Curve<DT> temp3 = gxy, temp4 = gxy;
    jacobian_(0, 0) = temp1.differentiateX();
    jacobian_(0, 1) = temp2.differentiateY();
    jacobian_(1, 0) = temp3.differentiateX();
    jacobian_(1, 1) = temp4.differentiateY();
    debug_ = false;
  }

  // Inner type definition
  typedef BoxT<DT> Box;
  typedef CORE::IntervalT<NT> IntervalNT;
  typedef CORE::IntervalT<DT> IntervalDT;

  // Gauss-Seidel iteration
  //    Given a box B, it returns 
  // 1 if the existence and uniqueness is verified
  // 0 if one or more new box is produced
  // -1 if box B is discarded
  // -2 if split is suggested
  int GS_iteration(const Box *box, vector<Box* >* next) const {
	const IntervalNT &x_range = box->x_range;
	const IntervalNT &y_range = box->y_range;
	const NT &x_mid = x_range.mid();
	const NT &y_mid = y_range.mid();
    cout << "midpoints: " << x_mid << " " << y_mid << endl;
    // evaluate f and g at the center of the box
    NT f_center = fxy_.template eval<NT>(x_mid, y_mid),
       g_center = gxy_.template eval<NT>(x_mid, y_mid);
    cout << "f g at center " << f_center << " " << g_center << endl;
    // compute the interval slope matrix
    IntervalNT f_x = jacobian_(0,0).template eval<IntervalNT>(x_range, y_range),
               f_y = jacobian_(0,1).template eval<IntervalNT>(x_range, y_range),
               g_x = jacobian_(1,0).template eval<IntervalNT>(x_range, y_range),
               g_y = jacobian_(1,1).template eval<IntervalNT>(x_range, y_range);
	cout << "interval slope matrix (up left to down right)" << endl
        << f_x << " " << f_y << " " << g_x << " " << g_y << endl; 
    // compute the preconditioning matrix [C]. 
    // For the G-S iteration, it is usually chosen to be the 
    // midpoint inverse of the interval slope matrix
    // but to be consistent with the Miranda test, 
    // we first try the inverse of the jacobian at the box center 
	NT C00, C01, C10, C11;
	C11 = jacobian_(0, 0).template eval<NT>(x_mid, y_mid);
	C01 = -jacobian_(0, 1).template eval<NT>(x_mid, y_mid);
	C10 = -jacobian_(1, 0).template eval<NT>(x_mid, y_mid);
	C00 = jacobian_(1, 1).template eval<NT>(x_mid, y_mid);
	cout << "inverse slope matrix at center (up left to down right)" << endl
        << C00 << " " << C01 << " " << C10 << " " << C11 << endl; 
    // check whether the determinant of the preconditioning matrix is 0;
    // if yes, split the box
    NT determ = C00 * C11 - C01 * C10; 
    cout << "determinant = " << determ << endl;
    if( determ == 0)    
    {
        cout << "determinant of the precondition matrix vanishes" << endl;
        return -2;
    }
	// Compute the values obtained after pre-conditioning
	// that is, multiplying by the inverse of the Jacobian at midpoint.
	// Note that we have not divided by the determinant.
    NT f_center_precon = C00 * f_center + C01 * g_center,
       g_center_precon = C10 * f_center + C11 * g_center;
	IntervalNT intvl_C00(C00), intvl_C01(C01), 
               intvl_C10(C10), intvl_C11(C11),
               intvl_g_center_precon(g_center_precon),
               intvl_f_center_precon(f_center_precon);
    IntervalNT f_x_precon = intvl_C00 * f_x + intvl_C01 * g_x ,
               f_y_precon = intvl_C00 * f_y + intvl_C01 * g_y ,
               g_x_precon = intvl_C10 * f_x + intvl_C11 * g_x,
               g_y_precon = intvl_C10 * f_y + intvl_C11 * g_y;

    cout << "f_x_precon: " << f_x_precon << f_x_precon.zero() << endl; 
    cout << "g_y_precon: " << g_y_precon << g_y_precon.zero() << endl; 
    // compute the new box(es) after Gauss-Seidel iteration
    IntervalNT x_range_centered = x_range-IntervalNT(x_mid), 
               y_range_centered = y_range-IntervalNT(y_mid);
    if(!f_x_precon.zero() && !g_y_precon.zero())
    {
        cout << "0 NOT in f_x_precon and 0 NOT in g_y_precon "<< endl;
        IntervalNT numerator(IntervalNT(0)-intvl_f_center_precon - f_y_precon * y_range_centered);
        IntervalNT x_range_new = numerator/f_x_precon;
        cout << "x_range_centered: " <<  x_range_centered<< endl;
        cout << "x_range_new: " <<  x_range_new << endl;
        int contain_flag = 0;
        // set the new x_range, if it does not intersect the previous one, remove it
        if(Overlap(x_range_new,x_range_centered))
            if((x_range_new.getL() > x_range_centered.getL()) && 
               (x_range_new.getR() < x_range_centered.getR()))
                contain_flag++;
            else
            x_range_new = Intersect(x_range_new, x_range_centered);
        else
            return -1;
        // set the new y_range, if it does not intersect the previous one, remove it
        IntervalNT y_range_new = (IntervalNT(0)-intvl_g_center_precon - g_x_precon * x_range_new)
                                    /g_y_precon;
        if(Overlap(y_range_new,y_range_centered)){
            if(contain_flag == 1)
                if((y_range_new.getL() > y_range_centered.getL()) && 
                   (y_range_new.getR() < y_range_centered.getR()))
                    contain_flag++;    
            y_range_new = Intersect(y_range_new, y_range_centered);
        }
        else
            return -1;
        // if the new box is not the previous one, then set the new box
        // else return -2 to split the box 
        if((x_range_new != x_range_centered)||(y_range_new != y_range_centered))
        {
            
            next->push_back(new Box(0, IntervalNT(x_mid)+x_range_new, 
                                     IntervalNT(y_mid)+y_range_new));
            // if the new box is contained in the previous one, then return 1
            if(contain_flag == 2)
                return 1;
            else 
                return 0;
        }
        else 
            return -2;
    }        
    else if(!f_x_precon.zero() && g_y_precon.zero())
    {
        cout << "0 NOT in f_x_precon but in g_y_precon "<< endl;
        IntervalNT numerator(IntervalNT(0)-intvl_f_center_precon - f_y_precon * y_range_centered);
        cout << "numerator = " << numerator << endl;
        IntervalNT x_range_new = numerator/f_x_precon;
        cout << "x_range_centered: " <<  x_range_centered<< endl;
        cout << "x_range_new = " << x_range_new << endl; 
        if(Overlap(x_range_new,x_range_centered))
            x_range_new = Intersect(x_range_new, x_range_centered);
        else
            return -1;
        cout << "x_range_new after intersection = " << x_range_new << endl; 
        IntervalNT numerator2 = IntervalNT(0)-intvl_g_center_precon - g_x_precon * x_range_new;
        cout << "numerator2 = " << numerator2 << endl;
        vector<IntervalNT> *quotients = new vector<IntervalNT>;
        CORE::interval_ext::Divide(numerator2 , g_y_precon, quotients);
        cout << "size of quotients= " << quotients->size() << endl;
        for(typename vector<IntervalNT>::iterator it = quotients->begin(); 
                                         it != quotients->end(); it++) 
        {
            if(Overlap_ext(*it, y_range_centered))
            {
               IntervalNT y_range_new = Intersect_ext(*it, y_range_centered);
               if((x_range_new != x_range_centered) || (y_range_new != y_range_centered))
               {
                   next->push_back(new Box(0, IntervalNT(x_mid)+x_range_new,
                                        IntervalNT(y_mid)+y_range_new)); 
                   cout << x_range_new << endl;
                   cout << y_range_new << endl;
               }
               else 
                   return -2;
            }
        }
        delete quotients;
        cout << "---------- size of next after adding: " << next->size() << endl;
        if(next->size() > 0)
        {
            cout << "return 0" << endl;
            return 0;
        }
        else 
            return -1;
    }
    else if(f_x_precon.zero() && !g_y_precon.zero())
    {
        cout << "0 NOT in g_y_precon but in f_x_precon" << endl;
        IntervalNT numerator(IntervalNT(0)-intvl_g_center_precon - g_x_precon * x_range_centered);
        IntervalNT y_range_new = numerator/g_y_precon;
        if(Overlap(y_range_new,y_range_centered))
            y_range_new = Intersect(y_range_new, y_range_centered);
        else
            return -1;
        vector<IntervalNT> *quotients = new vector<IntervalNT>;
        IntervalNT numerator2 = IntervalNT(0)-intvl_f_center_precon - f_y_precon * y_range_new;
        CORE::interval_ext::Divide(numerator2, f_x_precon, quotients);
        for(typename vector<IntervalNT>::iterator it = quotients->begin(); 
                                         it != quotients->end(); it++) 
        {
            if(Overlap_ext(*it, x_range_centered))
            {
               IntervalNT x_range_new = Intersect_ext(*it, x_range_centered);
               if((x_range_new != x_range_centered)||(y_range_new != y_range_centered))
                   next->push_back(new Box(0, IntervalNT(x_mid)+x_range_new,
                                        IntervalNT(y_mid)+y_range_new)); 
               else 
                   return -2;
            }
        }
        delete quotients;
        if(next->size() > 0)
            return 0;
        else 
            return -1;
    }
    else
    {
        cout << "0 in both g_x_precon and f_x_precon" << endl;
        IntervalNT numerator(IntervalNT(0)-intvl_f_center_precon - f_y_precon * y_range_centered);
        vector<IntervalNT> *quotients_x = new vector<IntervalNT>;
        CORE::interval_ext::Divide(numerator , f_x_precon, quotients_x);
        for(typename vector<IntervalNT>::iterator it_x = quotients_x->begin();
                                         it_x != quotients_x->end(); it_x++)
        {
            if(Overlap_ext(*it_x, x_range_centered))
            {
                IntervalNT x_range_new = Intersect_ext(*it_x, x_range_centered);
                IntervalNT numerator2 = IntervalNT(0)-intvl_g_center_precon 
                                                - g_x_precon * x_range_new;
                vector<IntervalNT> *quotients_y = new vector<IntervalNT>;
                CORE::interval_ext::Divide(numerator2 , g_y_precon, quotients_y);
                for(typename vector<IntervalNT>::iterator it_y = quotients_y->begin(); 
                                                 it_y != quotients_y->end(); it_y++) 
                {
                    if(Overlap_ext(*it_y, y_range_centered))
                    {
                       IntervalNT y_range_new = Intersect_ext(*it_y, y_range_centered);
                       if((x_range_new != x_range_centered)||(y_range_new != y_range_centered))
                       {
                           next->push_back(new Box(0, IntervalNT(x_mid)+x_range_new,
                                                IntervalNT(y_mid)+y_range_new)); 
                           cout << x_range_new << endl;
                           cout << y_range_new << endl;
                       }
                       else 
                           return -2;
                    }
                }
            }
        }
        if(next->size() > 0)
            return 0;
        else 
            return -1;
    }
  }

  // the first interval may contain infinity, the second interval is normal
  bool Overlap_ext(const IntervalNT &a, const IntervalNT & b) const
  {
    if (a.getL()==IntervalNT::I_NEG_INFTY)
        return ((a.getR()==IntervalNT::I_POS_INFTY) || (a.getR() >= b.getL()));
    else if (a.getR()==IntervalNT::I_POS_INFTY)
        return (a.getL() <= b.getR());
    else
        return Overlap(a, b);
  }

  // the first interval may contain infinity, the second interval is normal
  IntervalNT Intersect_ext(const IntervalNT &a, const IntervalNT & b) const
  {
    if (a.getL()==IntervalNT::I_NEG_INFTY)
    {
        if(a.getR()!=IntervalNT::I_POS_INFTY)
            return IntervalNT(b.getL(), (std::min)(a.getR(), b.getR()));
        else 
            return b;
    }
    else if (a.getR()==IntervalNT::I_POS_INFTY)
        return IntervalNT((std::max)(a.getL(), b.getL()), b.getR());
    else
        return Intersect(a, b);
  }

  // check region size and generation, put into ambiguous if too small
  bool Min(const Box *region) const {
    if(region->x_range.width() <= min_size_ || 
        region->y_range.width() <= min_size_)
      return true;
    else if(region->generation_id > max_gen_id_)
      return true;
    else
      return false;
  }

  // return true if output box is too big 
  bool Max(const Box *region) const {
    if(((region->x_range.width() >= max_size_) || 
        (region->y_range.width() >= max_size_)) && 
        (region->generation_id < max_gen_id_))
   /* if((region->x_range.width() >= max_size_) || 
        (region->y_range.width() >= max_size_))*/
      return true;
    else
      return false;
  }

  // get absolute value(not sure if core implements this)
  NT absolute(const NT &input) const {
    if(input <= 0)
      return (NT(-1) * input);
    else
      return input;
  }

  // calculate magnitude of interval(should be put into library)
  NT Mag(const IntervalNT &interval) const {
    NT temp_left = interval.getL();
    NT temp_right = interval.getR();
    if(temp_left <= 0)  temp_left  = -temp_left;
    if(temp_right <= 0) temp_right = -temp_right;
    return ((temp_left>=temp_right) ? temp_left : temp_right);
  }

  /// Split(Box B, Queue Q)
  /// 	splits B to 4 children, and put them into the queue Q
  void Split(const Box *region, vector<const Box*> *queue) const 
  {
    bool r=const_cast<Box*>(region)->Split(0);

    assert(r);

    for(int i=0;i<4;i++) queue->push_back(region->pChildren[i]);

    // statistic collection only
    extern unsigned int largest_gen;
    const unsigned int gen_id = region->generation_id + 1;
    
    if(largest_gen < gen_id)
    {
        largest_gen = gen_id;
    }
  }

  /// Split_Exclude(Box B, Queue Q, Queue ExcludeQ, Queue ambiguous)
  ///   Splits B into 4 children, and push each child into Q if it fails the
  //	 C0 test, push into ExcludeQ if C0 holds, and into ambiguous queue if too small.
  void Split_Exclude(const Box *region,
    vector<const Box *> *queue,
    vector<const Box *> *exclude,
    vector<const Box *> *ambiguous) const 
  {

    // temp queue for processing
    vector<const Box *> temp;
    // split box into 4 children 
    Split(region, &temp);
    // Do Ambiguous and C0 test for each box in temp queue
    //cout<<"---------------- split exclude from : "<<(*region)<<endl;
    while(!temp.empty()) {
      const Box *box = temp.back();
      temp.pop_back();
      //cout<<(*box)<<endl;
	    if (Min(box)) 
      {
		    ambiguous->push_back(box);
	    }
      else if(!Exclude(box))   // C0 fails, put back to queue
      {
		    queue->push_back(box);
      }
      else 
      {
          exclude->push_back(box);
      }
    }
  }


// Members:
private:
  CORE::Curve<DT>           fxy_;       // f(x, y)
  CORE::Curve<DT>           gxy_;       // g(x, y)
  MatrixT<CORE::Curve<DT> > jacobian_;  // jacobian matrix of F = (f, g)
  const DT            min_size_;  // minimum size of a box
  const DT            max_size_;  // if output box greater that max, keep splitting
  const unsigned int max_gen_id_;// box generation
  bool               debug_;     // debug bit
};

#endif
