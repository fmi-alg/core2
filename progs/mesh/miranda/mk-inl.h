/* ***************************************************
*
* mk-inl.h
*
*    This file describes one class called MKPredicates.
*    The class has the following members:
*    	--fxy, gxy are 2 bivariate polynomials
*    	--Variable such as
*    		--max_size
*    		--min_size
*    		--max_gen_id
*
*    MK stands for Moore-Kioustelides, who gave a modified form of
*    the Miranda Test for existence of roots.
*    This class provided predicates for the
*
* 	C0 test: C0 is the exclusion predicate and holds if 
* 	JC test: JC is the Jacobian test, and if it succeeds, there is
* 		at most one root of fxy=gxy=0.
* 	MK test: This is performed only if JC holds.
* 		If it passes, there is a unique root.
*
*   We need two types of dyadic numbers:
*     DT : number type for box intervals.
*     NT : number type for intervals used by evaluation.
*     We assume that DT <= NT, i.e., DT is a subtype of NT.
*   Typically, both NT and DT can be DoubleWrapper.
*   But for high degree polynomials, we may need NT to be BigFloat.
*       while keeping DT to be DouleWrapper
*
* Author: Shang Wang, July 16, 2011
*
* Since Core 2.1.
************************************************** */

#ifndef MK_MK_INL_H_
#define MK_MK_INL_H_

#include "mk-defs.h"
#include "box.h"

using namespace std;

template <typename DT, typename NT>
class MKPredicates {
public:
  // Constructor
  MKPredicates(const Curve<DT> &fxy, 
	  const Curve<DT> &gxy, const DT &min_size, const DT &max_size,
	  const unsigned int max_gen_id) : fxy_(fxy), gxy_(gxy),
	  jacobian_(MatrixT<Curve<DT> >(2)), min_size_(min_size),
	  max_size_(max_size), max_gen_id_(max_gen_id)
  {
    // differentiate is "self-modified" operator
    Curve<DT> temp1 = fxy, temp2 = fxy;
    Curve<DT> temp3 = gxy, temp4 = gxy;
    jacobian_(0, 0) = temp1.differentiateX();
    jacobian_(0, 1) = temp2.differentiateY();
    jacobian_(1, 0) = temp3.differentiateX();
    jacobian_(1, 1) = temp4.differentiateY();
    debug_ = false;
  }

  // Inner type definition
  typedef BoxT<DT> Box;
  typedef IntervalT<NT> IntervalNT;
  typedef IntervalT<DT> IntervalDT;

  // This is essentially the C0 test for F=(f, g) on a box B
  // uses centered form to achieve quadratic convergence 
  bool Exclude(const Box *box) const {
    // o not in f(B) \/ 0 not in g(B)
    const IntervalNT &x_range = box->x_range;
    const IntervalNT &y_range = box->y_range;
    IntervalNT c_f = fxy_.eval2(x_range, y_range);
    IntervalNT c_g = gxy_.eval2(x_range, y_range);
    if(!c_f.zero() || !c_g.zero()) 
      return true;
    else
      return false;
  }

  // This is the Jacobian Test on a box B
  //   It returns true iff 0 is not in det(JF(B))
  //   If true, then B has at most one root.
  bool JTest(const Box *box) const {
   const IntervalNT &x_range = box->x_range;
   const IntervalNT &y_range = box->y_range;
   IntervalNT a = jacobian_(0, 0).eval<IntervalDT>(x_range, y_range);
   IntervalNT b = jacobian_(0, 1).eval<IntervalDT>(x_range, y_range);
   IntervalNT c = jacobian_(1, 0).eval<IntervalDT>(x_range, y_range);
   IntervalNT d = jacobian_(1, 1).eval<IntervalDT>(x_range, y_range);
   IntervalNT det = a*d - b*c;
   if(det.zero())
     return false;
   else
     return true;
  }

  // MK test  (MK stands for Moore-Kioustelides)
  //    Given a box B, it returns true iff MK(B) holds.
  bool MKTest(const Box *box) const {
    const IntervalNT &x_range = box->x_range;
    const IntervalNT &y_range = box->y_range;
    const NT &x_l = x_range.getL();
    const NT &x_r = x_range.getR();
    const NT &y_l = y_range.getL();
    const NT &y_r = y_range.getR();
    const NT &x_mid = x_range.mid();
    const NT &y_mid = y_range.mid();
    // width of the box on x and y directions
    const NT &w_x   = x_range.getR() - x_mid;
    const NT &w_y   = y_range.getR() - y_mid;

    MatrixT<NT> j_center(2); // JF(m(B))
    int sign = JSign(&j_center, box);
    // if det = 0, fail
    if(sign == 0) {
//cout << "sign = 0" << endl;
      return false;
    }
    /*================== MK prediate No.1 ================*/
    /*       
              +x2
           _________
          |    |    |       a = fx(m)   b = fy(m)
      -x1 |____|____| +x1   
          |    |    |       c = gx(m)   d = gy(m)
          |____|____|
              -x2
    */
    NT f_left_cen  = fxy_.eval<DT>(x_l, y_mid); // f(m(-x1))
    NT g_left_cen  = gxy_.eval<DT>(x_l, y_mid); // g(m(-x1))
    NT f_right_cen = fxy_.eval<DT>(x_r, y_mid); // f(m(+x1))
    NT g_right_cen = gxy_.eval<DT>(x_r, y_mid); // g(m(+x1))

    // ^f- = d*f(m(-x1)) - b*g(m(-x1))
    NT f_left  = j_center(1, 1)*f_left_cen -
      j_center(0, 1)*g_left_cen;
    // ^f+ = d*f(m(+x1)) - b*g(m(+x1))
    NT f_right = j_center(1, 1)*f_right_cen -
      j_center(0, 1)*g_right_cen;

    NT f_down_cen = fxy_.eval<DT>(x_mid, y_l);  // f(m(-x2))
    NT g_down_cen = gxy_.eval<DT>(x_mid, y_l);  // g(m(-x2))
    NT f_up_cen   = fxy_.eval<DT>(x_mid, y_r);  // f(m(+x2))
    NT g_up_cen   = gxy_.eval<DT>(x_mid, y_r);  // g(m(+x2))

    // ^g- = a*g(m(-x2)) - c*f(m(-x2))
    NT g_left = j_center(0, 0)*g_down_cen - 
      j_center(1, 0)*f_down_cen;
    // ^g+ = a*g(m(+x2)) - c*f(m(+x2))
    NT g_right = j_center(0, 0)*g_up_cen - 
      j_center(1, 0)*f_up_cen;

    // print debug information
    if(debug_) {
      cout << "^f- = " << f_left << endl;
      cout << "^f+ = " << f_right << endl;
      cout << "^g- = " << g_left << endl;
      cout << "^g+ = " << g_right << endl;
    }
    // ^f- and ^f+ same sign || ^g- and ^g+ same sign
    if((f_left * f_right >= 0) || (g_left * g_right >= 0))
      return false;

    /*================== MK prediate No.2 ================*/

    IntervalNT fy_l = jacobian_(0, 1).eval<IntervalDT>(
      IntervalNT(x_l), y_range);  // fy(-x1)
    IntervalNT gy_l = jacobian_(1, 1).eval<IntervalDT>(
      IntervalNT(x_l), y_range);  // gy(-x1)
    // ^fy(-x1) = d*fy(-x1) - b*gy(-x1)
    IntervalNT fy_left = fy_l*j_center(1, 1) - gy_l*j_center(0, 1);

    IntervalNT fy_r = jacobian_(0, 1).eval<IntervalDT>(
      IntervalNT(x_r), y_range);  // fy(+x1)
    IntervalNT gy_r = jacobian_(1, 1).eval<IntervalDT>(
      IntervalNT(x_r), y_range);  // gy(+x1)
    // ^fy(+x1) = d*fy(+x1) - b*gy(+x1)
    IntervalNT fy_right = fy_r*j_center(1, 1) - gy_r*j_center(0, 1);

    IntervalNT gx_l = jacobian_(1, 0).eval<IntervalDT>(
      x_range, IntervalNT(y_l));  // gx(-x2)
    IntervalNT fx_l = jacobian_(0, 0).eval<IntervalDT>(
      x_range, IntervalNT(y_l));  // fx(-x2)
    // ^gx(-x1) = a*gx(-x2) - c*fx(-x2)
    IntervalNT gx_left = gx_l*j_center(0, 0) - fx_l*j_center(1, 0);

    IntervalNT gx_r = jacobian_(1, 0).eval<IntervalDT>(
      x_range, IntervalNT(y_r));  // gx(+x2)
    IntervalNT fx_r = jacobian_(0, 0).eval<IntervalDT>(
      x_range, IntervalNT(y_r));  // gx(+x2)
    // ^gx(+x1) = a*gx(+x2) - c*fx(+x2)
    IntervalNT gx_right = gx_r*j_center(0, 0) - fx_r*j_center(1, 0);
    
    if(debug_) {
      cout << "mag(^fy(-x1)) = " << Mag(fy_left) << endl;
      cout << "mag(^fy(+x1)) = " << Mag(fy_right) << endl;
      cout << "mag(^gx(-x1)) = " << Mag(gx_left) << endl;
      cout << "mag(^gx(+x1)) = " << Mag(gx_right) << endl;
    }

    if((absolute(f_left)  <= Mag(fy_left)*w_x)  ||
       (absolute(f_right) <= Mag(fy_right)*w_x) || 
       (absolute(g_left)  <= Mag(gx_left)*w_y)  || 
       (absolute(g_right) <= Mag(gx_right)*w_y)  )
      return false;
    return true;
  }

  // compute JF(m(B)), m(B) is the center of box, and return the sign of det(JF(m(B)))
  int JSign(MatrixT<NT> *output,
    const Box *box) const {
    // construct mid point for x and y
    const NT &x_mid = box->x_mid();
    const NT &y_mid = box->y_mid();
    // keep things handy
    MatrixT<NT> &temp = *output;
    // evaluate at center of the box
    temp(0, 0) = jacobian_(0, 0).eval<NT>(x_mid, y_mid);
    temp(0, 1) = jacobian_(0, 1).eval<NT>(x_mid, y_mid);
    temp(1, 0) = jacobian_(1, 0).eval<NT>(x_mid, y_mid);
    temp(1, 1) = jacobian_(1, 1).eval<NT>(x_mid, y_mid);
    NT det = temp(0, 0)*temp(1, 1) - temp(0, 1)*temp(1, 0);
    if(det > 0) 
      return 1;
    else if(det == 0)
      return 0;
    else
      return -1;
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
  void Split(const Box *region,
      vector<const Box*> *queue) const {
    const DT &x_start = region->x_range.getL();
    const DT &x_end   = region->x_range.getR();
    const DT &x_mid   = region->x_range.mid();
    const DT &y_start = region->y_range.getL();
    const DT &y_end   = region->y_range.getR();
    const DT &y_mid   = region->y_range.mid();

    // statistic collection only
    extern unsigned int largest_gen;
    const unsigned int gen_id = region->generation_id + 1;
    if(largest_gen < gen_id)
      largest_gen = gen_id;

    queue->push_back(new Box(gen_id, 
      IntervalDT(x_start, x_mid), IntervalDT(y_start, y_mid)));
    queue->push_back(new Box(gen_id, 
      IntervalDT(x_mid, x_end), IntervalDT(y_start, y_mid)));
    queue->push_back(new Box(gen_id, 
      IntervalDT(x_start, x_mid), IntervalDT(y_mid, y_end)));
    queue->push_back(new Box(gen_id, 
      IntervalDT(x_mid, x_end), IntervalDT(y_mid, y_end)));
    delete region;
  }

  /// Split_Exclude(Box B, Queue Q, Queue ExcludeQ)
  ///   Splits B into 4 children, and push each child into Q if it fails C0 test
  ///	and push into ExcludeQ if C0 holds.
  void Split_Exclude(const Box *region,
    vector<const Box *> *queue,
    vector<const Box *> *exclude) const {

    // temp queue for processing
    vector<const Box *> temp;
    // split box into 4 children 
    Split(region, &temp);
    // Do C0 test for each box in temp queue
    while(!temp.empty()) {
      const Box *box = temp.back();
      temp.pop_back();
      if(!Exclude(box)) {  // C0 fails, put back to queue
        queue->push_back(box);
      }
      else {
          exclude->push_back(box);
      }
    }
  }


// Members:
private:
  Curve<DT>           fxy_;       // f(x, y)
  Curve<DT>           gxy_;       // g(x, y)
  MatrixT<Curve<DT> > jacobian_;  // jacobian matrix of F = (f, g)
  const DT            min_size_;  // minimum size of a box
  const DT            max_size_;  // if output box greater that max, keep splitting
  const unsigned int max_gen_id_;// box generation
  bool               debug_;     // debug bit
};

#endif
