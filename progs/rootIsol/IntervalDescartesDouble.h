/*
  File contains the code for BITSTREAM real root isolation based upon the Bernstein
  basis representation of intermediate polynomials. 
 */
#ifndef __INTERVALDESCARTESDOUBLE_H__
#define __INTERVALDESCARTESDOUBLE_H__

#define CORE_LEVEL 4
#include<stack>
#include "CORE/CORE.h"
#include"RootIsolationTools.h"

using namespace CORE;
using namespace std;

namespace IntervalDescartesDouble {
  float INIT_TIME=0;

  int COUNTER = 0;
  prec_t DEFAULT_PREC = DOUBLE_PREC;
  typedef double DEFAULT_TYPE;
  bool ExactBigFloatArith = false;
  // Our initial sleeve has error 2*EPS_DOUBLE=2*2^{1-DOUBLE_PREC}}.

  template <typename Vec, typename FT> 
    unsigned int checkSleeve(Vec& upSlv, Vec& lowSlv, int deg, FT EPS){
    //    cout << " Inside check Sleeve: EPS = " << EPS << endl;
  
    int type = sleeveVar(upSlv, lowSlv, deg);
    if(type < 2)
      return type;
    
    // Compute a sleeve for the derivative.
    Vec upSlvDeriv(deg), lowSlvDeriv(deg);
    for(int i=0; i < deg; i++){
      upSlvDeriv[i] = upSlv[i] - lowSlv[i+1];
      lowSlvDeriv[i] = lowSlv[i] - upSlv[i+1];
    }

    type = sleeveVar(upSlvDeriv, lowSlvDeriv, deg-1);
    if(type == 0){// Polynomial is monotone
      // First check if we can infer the sign from the sleeve endpoints
      if((sgn(upSlv[0]) * sgn(lowSlv[0]) > 0) && 
	 (sgn(upSlv[deg]) * sgn(lowSlv[deg]) > 0)){
	if(sgn(upSlv[0]) * sgn(upSlv[deg]) < 0)
	  return 1;
	else// WHAT IF THE SIGN IS ZERO AT ONE OF THE ENDPOINTS
	  return 0;
      }else{// Signal we need to evaluate the polynomial at the endpoints.
	return 2;// The check will be performed later.
      }
    }

    //    cout.precision(DEFAULT_PREC);    
    //    cout <<"Inside CheckSleeve "<<endl;
    // Check if the sleeve is small
    bool coeffSmall = true;
    for(int i=0; i <= deg; i++){
      //cout << " slv["<<i<<"] = ["<< lowSlv[i] << " , " << upSlv[i] << "]" <<endl;
      //	   << (slv[i].getLeft()).uMSB() << "; 3- EPS = "<< 3-EPS<<endl;
      //      cout <<"slv[ "<< i <<" ] =" << slv[i] << " log |slv["<<i<<"]| = "
      //	   << (slv[i].getLeft()).uMSB() << "; 3- EPS = "<< 3-EPS<<endl;
      if( core_max(ceillg(upSlv[i]),  ceillg(lowSlv[i])) < 3-EPS)
	// That is, |slv[i]| < 8*2^{-EPS}
	;
      else{
	coeffSmall=false;
	//	break;
      }
    }
    if(coeffSmall) return 3;

    return 4;
    
  }
  
  /*
    slv is the sleeve of the polynomial on the interval I. By construction
    the entries are doubles.
    We are assuming the sleeve is "interesting" and has "sign variation" greater
    than one. The aim is to subdivide the sleeve for a constant subdivision depth.
    We identify four types of intervals: first that contain a root (these are
    stored in v), second are those for which the polyomial is monotone (stored
    in monotone), third are those for sleeve is small (stored in reinit
    with added DEF_SCALE), 
    and fourth are those for which we have done the fixed number of subdivisions
    (stored in reinit). 
    
    scale is the total amount of scaling required to I, and prec is the
    precision.
    
  */
  template <typename Vec, typename IntvData>
    void isolateRoots(Vec& upSlv, Vec& lowSlv, int deg, IntvData &ID,
		      BFVecInterval &v, BFVecInterval &monotone, 
		      std::stack<IntvData> &reinit,
		      int EPS) {

    //    cout<<"Inside sleeve solver: EPS= "<< EPS << endl;    
    typedef SlvSubDivDataDouble<BigFloat, DEFAULT_TYPE> SlvSubDivDataDouble;
    
    std::stack< SlvSubDivDataDouble > S;
    SlvSubDivDataDouble SD(deg+1);

    S.push(SlvSubDivDataDouble(ID.a, ID.b, upSlv, lowSlv));
    int scale = ID.s;
    prec_t prec = ID.p;

    BigFloat m;
    unsigned int type; // Identifies the type of the interval
    int EPS_DEPTH;
    int logDeg = ceillg(deg);
    bool isExactArith = ExactBigFloatArith;
    

    while (!S.empty()){
      COUNTER++;
      SD = S.top(); S.pop();

      m = (SD.a + SD.b).div2();
      
      SlvSubDivDataDouble SDL(deg+1), SDR(deg+1);
      deCasteljau(SD.upSlv, SD.lowSlv, SDL.upSlv, SDL.lowSlv, 
		  SDR.upSlv, SDR.lowSlv, deg);

      SDL.a = SD.a; SDL.b = m; SDR.a = m; SDR.b = SD.b;
      SDL.depth = SD.depth+1; SDR.depth=SDL.depth;
      //      cout.precision(SDL.a.get_prec());
      // At SDL.depth we have an EPS_DEPTH sleeve.
      // Since error at depth h is 2^{-EPS} * 4*deg*h.
      EPS_DEPTH = EPS-2- logDeg - ceillg((long)SDL.depth);

      type = checkSleeve(SDL.upSlv, SDL.lowSlv, deg, EPS_DEPTH);
      //            cout <<"Type of left sleeve "<< type <<" DEPTH = "<< SDL.depth
      //            	   << " Interval is [" << SDL.a << " , "<< SDL.b << "]"
      //      	   << " EPS_DEPTH = "<< EPS_DEPTH <<endl;
      
      if(type != 0){
	if(type == 1) // Found a root
	  v.push_back(BFInterval(SDL.a, SDL.b));
	else if(type == 2)// Polynomial is monotone
	  monotone.push_back(BFInterval(SDL.a, SDL.b));
	else if(type == 3) // Sleeve is smaller than 8*2^{-EPS_DEPTH}.
	  reinit.push(IntvData(SDL.a, SDL.b, scale+EPS_DEPTH-3, prec));
	else if(SDL.depth > EPS)// Done sufficient subdivisions
	  reinit.push(IntvData(SDL.a, SDL.b, scale, prec));
	else
	  S.push(SDL);
      }
      
      type = checkSleeve(SDR.upSlv, SDR.lowSlv, deg, EPS_DEPTH);
      //            cout <<"Type of right sleeve "<< type
      //            	   << " Interval is [" << SDR.a << " , "<< SDR.b << "]"
      //      	   << " EPS_DEPTH = "<< EPS_DEPTH <<endl;

      if(type != 0){
	if(type == 1) // Found a root
	  v.push_back(BFInterval(SDR.a, SDR.b));
	else if(type == 2)// Polynomial is monotone
	  monotone.push_back(BFInterval(SDR.a, SDR.b));
	else if(type == 3) // Sleeve is small, must scale.
	  reinit.push(IntvData(SDR.a, SDR.b, scale+EPS_DEPTH-3, prec));
	else if(SDR.depth > EPS)// Done sufficient subdivisions
	  reinit.push(IntvData(SDR.a, SDR.b, scale, prec));
	else
	  S.push(SDR);
      }
      
    }
    //    cout<<"Exiting sleeve solver "<< endl;    
  }
  
  /*
    Isolate the roots of P in the interval I \ib [0,1]. P is assumed to be
    in the Bersntein basis wr.t. [0,1].
  */
  template <typename T, typename Vec>
    void isolateRoots(Polynomial<T> &P, int deg, BFInterval I, BFVecInterval &v,
		      BFVecInterval monotone, Vec &Binomial) {

    //    cout<<"Inside unit interval root solver "; 
    //    P.dump(); cout << endl;    
    std::vector<BigFloat2> slv(deg+1);
    std::vector<DEFAULT_TYPE> upSlv(deg+1);// The sleeve for P on I. 
    std::vector<DEFAULT_TYPE> lowSlv(deg+1);// The sleeve for P on I. 
    //By default the prec is double precision
    

    typedef IntvData<BigFloat, int, prec_t> IntvData;
    std::stack<IntvData> reinit;


    reinit.push(IntvData(I.first, I.second, 0, DEFAULT_PREC));
    IntvData ID;
    //    cout<<"Pused data on the stack "<< endl;    

    // The initializeSleeve computes a 2^{-EPS_INIT} sleeve.
    int EPS_INIT = DEFAULT_PREC - 2;
    //    cout <<"Initialization precision is " << EPS_INIT << endl;
    int type;
    bool isExactArith = ExactBigFloatArith;
    Timer2 init;

    while(!reinit.empty()){
      ID = reinit.top(); reinit.pop();
      
      init.start();
      initializeSleeve(P, deg, ID.a, ID.b, ID.s, ID.p, slv, Binomial, 
		       isExactArith, EPS_INIT);// Modifies ID.p
      init.stop();
      INIT_TIME += init.get_seconds();

      for(int i=0; i<=deg; i++){
	upSlv[i] = (slv[i].getRight()).get_d(GMP_RNDU);
	lowSlv[i] = (slv[i].getLeft()).get_d(GMP_RNDD);
      }
      //      cout<<"Initialization successful: Sleeve is "<< endl;    
      //      for(int i=0; i<= deg; i++)
      //      	cout <<"slv["<<i<<"] = [ "<< upSlv[i] << " , "<< lowSlv[i] << "]" <<endl;
      
      type = checkSleeve(upSlv, lowSlv, deg, EPS_INIT);
      //      cout<<"Sleeve type "<< type<< endl;    
      
      if(type != 0){
	if(type == 1) // Found a root
	  v.push_back(BFInterval(ID.a, ID.b));
	else if(type == 2)// Polynomial is monotone
	  monotone.push_back(BFInterval(ID.a, ID.b));
	else if(type == 3) // Sleeve is small, must scale.
	  reinit.push(IntvData(ID.a, ID.b, ID.s+EPS_INIT-3, ID.p));
	else
	  IntervalDescartesDouble::isolateRoots(upSlv, lowSlv, deg, ID, v, monotone, reinit, EPS_INIT);

      }
    }
    
  }

  /* Filtered real root isolation */
  template <typename POL, typename VEC1, typename VEC2>
    void filterIsolRoots(POL& P, int deg, 
			 VEC1 &v, VEC1 &checkMonotone, 
			 BigFloat B, VEC2 &Binomial,
			 bool isPos) {

    Polynomial<BigRat> Q(P);
    BFVecInterval vT;//Temporary storage of intervals containing roots
    BFVecInterval monotone;// Storage of intervals for which Bernstein 
    // polynomial is monotone

    //std::cout<<std::endl;Q.dump();cout<<endl;
    // Construct the Bernstein coefficients of polynomial Q(x) whose roots in
    // [0,1] correspond with the roots of P(x) in [-infty, 0]. 

    //  std::cout<<"Flipping signs "<<std::endl;
    // First flip the signs of odd coeffs.
    if(!isPos){
      for(int i=1; i<= deg; i++){
	if(i % 2 != 0)
	  Q.coeff()[i] *=-1;
      }
    }

    for(int i = 1; i < deg; i++){
      if(i <= deg/2)
	Q.coeff()[i] /= (Binomial[i]);
      else
	Q.coeff()[i] /= (Binomial[deg-i]);
    }

    long logH = (height(P).getRight()).uMSB();
    Q.coeff()[0] >>= logH; Q.coeff()[deg] >>= logH;
    for(int i = 1; i < deg; i++)
	Q.coeff()[i] >>= logH;
    
    /*    cout << " Converted to Bernstein: After normalization  "<< endl;
    for(int i = 0; i <= deg; i++){
      cout << Q.coeff()[i] << " ";
    }
    cout << endl;
    */  
    // Now we know that the height of Q is smaller than one.

    /*    if(isPos)
      cout<<"Calling root isolation on positive half "<< endl;
    else
      cout<<"Calling root isolation on negative half "<< endl;
    */
    IntervalDescartesDouble::isolateRoots(Q, deg, BFInterval(0,1),
				    vT, monotone, Binomial);

    // Map the intervals in vT back to [0, infty], flip the signs and add to v.
    // The mapping back yields us intervals with rational endpoints.
    getInterval(vT, v, B, isPos);// mapping to [-infty, 0]
    getInterval(monotone, checkMonotone, B, isPos);
    
  }

  /*
    Isolate the roots of P. P is assumed to be in the power basis.
    
  */
  template <typename T>
    void isolateRoots(Polynomial<T> &P, BRVecInterval &v) {
    
    int deg = P.getTrueDegree();
    if(deg == 0)
      return;
    //    cout<< "Polynomial is a constant" << endl;

    //Compute an upper bound on the positive roots of P
    BigFloat B = CauchyUpperBound(P);

    std::vector< int > Binomial;// Does it have enough precision?
    computeMatrix(deg, Binomial);

    BRVecInterval checkMonotone, small;

    // Isolate negative roots
    IntervalDescartesDouble::filterIsolRoots(P, deg, v, checkMonotone, 
					B, Binomial, false);
    if(P.coeff()[0] == 0)
      v.push_back(std::make_pair(0,0));

    // Isolate positive roots
    IntervalDescartesDouble::filterIsolRoots(P, deg, v, checkMonotone, 
					B, Binomial, true);

    int sa, sb;
    //    cout << "Number of monotone intervals " << checkMonotone.size() << endl;
    for (BRVecInterval::const_iterator it = checkMonotone.begin();
       it != checkMonotone.end(); ++it) {
      
      sa = sign(P.eval(it->first)); sb=sign(P.eval(it->second));
      if(sa * sb <= 0)
	v.push_back(BFInterval(it->first, it->second));
    }

  }



//Given the count n of the number of roots, this function isolates
//the roots of P and checks if they are equal to n.
template <typename NT>
void testDescartes(Polynomial<NT>& P, int n = -1) {

  BRVecInterval v;
  isolateRoots(P,v);
  std::cout << "   Number of roots is " << v.size() <<std::endl;
  if ((n >= 0) & (v.size() == (unsigned)n))
    std::cout << " (CORRECT!)" << std::endl;
  else
    std::cout << " (ERROR!) " << std::endl;
  int i = 0;
  for (BRVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
}// testDescartes

// Isolate the real roots of P iteratively for n iterations. Then return the
// average tree size, time for n iterations.
template <typename NT>
void test(Polynomial<NT>& P, int n) {
  
  BRVecInterval v;
  
  Timer2 t1;
  t1.start();
  for(int i=1; i <= n ; i++)
    IntervalDescartesDouble::isolateRoots(P, v);

  t1.stop();
  std::cout << "Size of recursion tree = " << COUNTER/n;
  std::cout << " #roots = " << v.size()/n;
  std::cout<<" Time " << t1.getSeconds()/n;
  std::cout<<" Initialization Time " << INIT_TIME/n <<endl;

  int i=0;
  for (BRVecInterval::const_iterator it = v.begin();
       i != v.size()/n; ++it) {
    std::cout << ++i << "th Root is in ["
	      << it->first << " ; " << it->second << "]" << std::endl;
    }

  //  std::cout<<" Time per evaluation " << t1.getSeconds()/(COUNTER) <<std::endl;
}

}// end namespace IntervalDescartesDouble
#endif // __INTERVALDESCARTESDOUBLE_H__
