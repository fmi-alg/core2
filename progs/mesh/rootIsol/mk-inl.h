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
*       while keeping DT to be DoubleWrapper
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
	jacobian = jacobian_(0,0) * jacobian_(1,1) - jacobian_(0,1)*jacobian_(1,0);
//	std::cout<<" Jacobian is "<< jacobian.toString() << std::endl;
    debug_ = false;
  }

  // Inner type definition
  typedef BoxT<DT> Box;
  typedef IntervalT<NT> IntervalNT;
  typedef IntervalT<DT> IntervalDT;

  // This is essentially the C0 test for F=(f, g) on a box B
  // uses centered form to achieve quadratic convergence 
  bool Exclude(const Box *box) const {
    // zero not in f(B) or 0 not in g(B)
	// This would be invoked for most boxes and hence
	// we need a very tight implementation of interval evaluation
//	std::cout << "Inside Exclude Test " << (*box) << std::endl;
    const IntervalNT &x_range = box->x_range;
    const IntervalNT &y_range = box->y_range;
	IntervalNT c_f = fxy_.eval(x_range, y_range);// Faster than eval2
    IntervalNT c_g = gxy_.eval(x_range, y_range);
	if(!c_f.zero() || !c_g.zero()){ 
//		std::cout << "Sign of F " << sign(c_f.getL()) << " , "<< sign(c_f.getR()) 
//					<< std:: endl;
//		std::cout << "Sign of G " << sign(c_g.getL()) << " , "<< sign(c_g.getR()) 
//		<< std:: endl;

		return true;
    }else
      return false;
  }

  // This is the Jacobian Test on a box B
  //   It returns true iff 0 is not in det(JF(B))
  //   If true, then B has at most one root.
  bool JTest(const Box *box) const {
   const IntervalNT &x_range = box->x_range;
   const IntervalNT &y_range = box->y_range;
	// Becomes slower if we switch from eval to eval3 
	// Also, eval apparently gives a tighter bound
/*   IntervalNT a = jacobian_(0, 0).eval<IntervalDT>(x_range, y_range);
   IntervalNT b = jacobian_(0, 1).eval<IntervalDT>(x_range, y_range);
   IntervalNT c = jacobian_(1, 0).eval<IntervalDT>(x_range, y_range);
   IntervalNT d = jacobian_(1, 1).eval<IntervalDT>(x_range, y_range);
   IntervalNT det = a*d - b*c;
   std::cout << "Inside Jtest " << (*box) << std::endl;
   std::cout <<" Det sign [" << sign(det.getL()) << " , "
			 << sign(det.getR()) << "]" << std::endl;
*/			 
	IntervalNT temp = jacobian.eval(x_range, y_range);
//	  std::cout<<" On box "<< (*box) << " jacobian sign [" << sign(temp.getL()) << " , "
//	  << sign(temp.getR()) << "]" << std::endl;
   if(temp.zero())
     return false;
   else
     return true;
  }

  // MK test  (MK stands for Moore-Kioustelides)
  //    Given a box B, it returns 
  // 1 if MK(B) holds
  // 0 if B can be excluded; this is the case when either one curve changes
  //						 sign across the box and the other curve
  //						 does not, or both do not change sign. 
  //						Since the Jacobian condition holds
  //						 on the box, it means that the two curves
  //						 do not intersect. 
  // -1 if MK(B) fails
int MKTest(const Box *box) const {
	const IntervalNT &x_range = box->x_range;
	const IntervalNT &y_range = box->y_range;
	const NT &x_l = x_range.getL();
	const NT &x_r = x_range.getR();
	const NT &y_l = y_range.getL();
	const NT &y_r = y_range.getR();
	const NT &x_mid = x_range.mid();
	const NT &y_mid = y_range.mid();
	
	NT j00, j01, j10, j11;
		// evaluate at center of the box
	j00 = jacobian_(0, 0).eval<NT>(x_mid, y_mid);
	j01 = jacobian_(0, 1).eval<NT>(x_mid, y_mid);
	j10 = jacobian_(1, 0).eval<NT>(x_mid, y_mid);
	j11 = jacobian_(1, 1).eval<NT>(x_mid, y_mid);
//	std::cout<<"Evaluated jacobian at midpoint " << std::endl;
	NT det = j00*j11 - j01*j10;
	// if det = 0, fail
//	std::cout << "Inside MK-test sign of det at center = "<< sign(det)<< std::endl;
//	if(det == 0) {// This cannot be possible since Jacobian test passed
			//cout << "sign = 0" << endl;
//		return -1;
//	}
//	std::cout<<"Box  " << x_range << " * " << y_range << " "<<std::endl;
	
	
	// F, G is the system obtained after pre-conditioning
	// that is, multiplying by the inverse of the Jacobian at midpoint.
	// Note that we have not divided by the determinant.
	BiPoly<DT> F, G, tmp1(fxy_), tmp2(gxy_), tmp3(fxy_), tmp4(gxy_);

//	std::cout << "tmp2 = "<< tmp2.toString() << std::endl;	
//	std::cout << "tmp4 = "<< tmp4.toString() << std::endl;
	tmp1.mulScalar(j11);
//	std::cout << "tmp1 mulscalar done" << std::endl;
	tmp2.mulScalar(j01);
//	std::cout << "tmp2 mulscalar done" << std::endl;
//	for (int i = 0; i<=tmp3.getYdegree() ; i++)
	tmp3.mulScalar(j10);
//	std::cout << "tmp3 mulscalar done" << std::endl;
//	for (int i = 0; i<=tmp4.getYdegree() ; i++)
	tmp4.mulScalar(j00);
//	std::cout << "tmp4 after mulscalar = "<< tmp4.toString() << std::endl;

/*	std::cout << "j00 j01 = "<< j00 << " "<<j01 << std::endl;
	std::cout << "j10 j11 = "<< j10 << " "<< j11 << std::endl;

	std::cout << "tmp1 = "<< tmp1.toString() << std::endl;
	std::cout << "tmp2 = "<< tmp2.toString() << std::endl;
	std::cout << "tmp3 = "<< tmp3.toString() << std::endl;
	std::cout << "tmp4 = "<< tmp4.toString() << std::endl;
*/
//	std::cout << "Computing F and G " << std::endl;
	F = tmp1 - tmp2;
	G = tmp4 - tmp3;
	
//	std::cout << "Computed F and G " << std::endl;	
//	std::cout << "F = "<< F.toString() << std::endl;
//	std::cout << "G = "<< G.toString() << std::endl;
	
	// These are the univariate polynomials corresponding to the boundaries
/*	Polynomial<NT> f_l = F.yPolynomial(x_l);
	Polynomial<NT> f_r = F.yPolynomial(x_r);
	Polynomial<NT> g_l = G.xPolynomial(y_l);
	Polynomial<NT> g_r = G.xPolynomial(y_r);

*/	
	// FILTER: Check if f_l or f_r changes sign on y_range,
	// or f_l and f_r have the same sign on y_range;
	// similarly check if  g_l, g_r change sign on x_range.
	// If so then we do not need to do interval evaluation.
/*	NT f_ltop = sign(F.eval<NT>(x_l, y_r));
	NT f_lbot = sign(F.eval<NT>(x_l, y_l));
	NT f_rtop = sign(F.eval<NT>(x_r, y_r));
	NT f_rbot = sign(F.eval<NT>(x_r, y_l));
	
//	std::cout << "fltop flbot frtop frbot = "<< f_ltop <<" " 
//				<< f_lbot<< " " << f_rtop << " "<< f_rbot<< std::endl;
	if (f_ltop * f_lbot <= 0 || // f_l changes sign on y_range
		f_rtop * f_rbot <= 0 {// f_r changes sign on y_range
//		f_ltop * f_rtop >=0) {  // f_l and f_r have same sign at corners THIS IS NOT CORRECT! CAN BE USED TO EXCLUDE
		return -1;
	}

	NT g_ltop = sign(G.eval<NT>(x_r, y_l));
	NT g_lbot = sign(G.eval<NT>(x_l, y_l));
	NT g_rtop = sign(G.eval<NT>(x_r, y_r));
	NT g_rbot = sign(G.eval<NT>(x_l, y_r));

//	std::cout << "gltop glbot grtop grbot = "<< g_ltop<< " " 
//	<< g_lbot <<" " << g_rtop << " "<< g_rbot<< std::endl;

	if (g_ltop * g_lbot <= 0 || // g_l changes sign on y_range
		g_rtop * g_rbot <= 0 {	// g_r changes sign on y_range
//		g_ltop * g_rtop >=0  ) {// g_l and g_r have same sign at corners
		return -1;
	}
*/	// END OF FILTER
	
	// Evaluate F, G at the boundaries: F on the left and right
	// boundaries, and G on the top and bottom.
	IntervalNT I_l, I_r, I_u, I_d;
	
	Polynomial<DT> P;
	P = F.fixX(x_l);
	I_l = P.evalCenteredForm(y_range);
	
	P = F.fixX(x_r);
	I_r = P.evalCenteredForm(y_range);
	
	P = G.fixY(y_l);
	I_d = P.evalCenteredForm(x_range);
	
	P = G.fixY(y_r);
	I_u = P.evalCenteredForm(x_range);
	
	/*
	IntervalNT I_l = F.eval(IntervalNT(x_l), y_range);
	IntervalNT I_r = F.eval(IntervalNT(x_r), y_range);
	IntervalNT I_u = G.eval(x_range, IntervalNT(y_l));
	IntervalNT I_d = G.eval(x_range, IntervalNT(y_r));
	 */

/*	std::cout << "Il = " << I_l << std:: endl;
	std::cout << "Ir = " << I_r << std:: endl;
	std::cout << "Iu = " << I_u << std:: endl;
	std::cout << "Id = " << I_d << std:: endl;
*/
	if (!I_l.zero() && !I_r.zero() && !I_d.zero() && !I_u.zero()) {
//		std::cout << "Boundaries have fixed sign "<< std::endl;
		
		if (sign(I_l.getR()) *sign(I_r.getR()) < 0 &&
			sign(I_d.getR()) *sign(I_u.getR()) < 0 )  {
//			std::cout <<" MK Test successful " << std::endl;
				return 1;
		}else if (sign(I_l.getR()) *sign(I_r.getR()) > 0 ||
				  sign(I_d.getR()) *sign(I_u.getR()) > 0 ) {
			return 0;
		}else {
			return -1;
		}
	}
	
	if (!I_l.zero() && !I_r.zero()) {
		if (sign(I_l.getR())*sign(I_r.getR()) > 0) {
			return 0;
		}
	}
			
	if (!I_d.zero() && !I_u.zero()) {
		if (sign(I_d.getR())*sign(I_u.getR()) > 0) {
			return 0;
		}
	}
					
	/* 
	 if (sign(I_l.getR()) *sign(I_r.getR()) < 0 &&
	 sign(I_d.getR()) *sign(I_u.getR()) > 0) {
	 return 0;
	 } else if (sign(I_l.getR()) *sign(I_r.getR()) > 0 &&
	 sign(I_d.getR()) *sign(I_u.getR()) < 0) {
	 return 0;	
	 }
	 */
	 
	return -1;

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
	if(largest_gen < gen_id){
		largest_gen = gen_id;
//		std::cout << "Depth Updated" << largest_gen << std::endl;
	}
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

  /// Split_Exclude(Box B, Queue Q, Queue ExcludeQ, Queue ambiguous)
  ///   Splits B into 4 children, and push each child into Q if it fails 
  //	C0 test, push into ExcludeQ if C0 holds, and into ambiguous queue if too small.
  void Split_Exclude(const Box *region,
    vector<const Box *> *queue,
    vector<const Box *> *exclude,
    vector<const Box *> *ambiguous) const {

    // temp queue for processing
    vector<const Box *> temp;
    // split box into 4 children 
    Split(region, &temp);
    // Do Ambiguous and C0 test for each box in temp queue
    while(!temp.empty()) {
      const Box *box = temp.back();
      temp.pop_back();
	  if (Min(box)) {
		  ambiguous->push_back(box);
	  }else if(!Exclude(box)) {  // C0 fails, put back to queue
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
  Curve<DT>			  jacobian;	// the detrminant of jacobian_ matrix
  const DT            min_size_;  // minimum size of a box
  const DT            max_size_;  // if output box greater that max, keep splitting
  const unsigned int max_gen_id_;// box generation
  bool               debug_;     // debug bit
};

#endif
