/*
  File contains the code for real root isolation based upon the Bernstein
  basis representation of intermediate polynomials. 
  This is Lane-Reisenfeld's algorithm.
 */
#ifndef __DESCARTESB2_H__
#define __DESCARTESB2_H__

#define CORE_LEVEL 4
#include<stack>
#include "CORE/CORE.h"
#include"RootIsolationTools.h"

using namespace CORE;
using namespace std;

namespace DescartesB2 {
  
int COUNTER = 0;

/* This approach is the Descartes method based upon Bernstein basis.
   The use of Bernstein basis helps avoiding the use of root bounds in
   the computation. We avoid the standard approach of constructing the
   Bernstein basis coefficients over the interval (-B, B), where 
   B is an upper bound on the real roots of the polynomial. Instead we
   reduce the problem to isolating real roots in the unit interval.
*/




// Isolates real roots of P in the interval (0,1). The coefficients are the
// Bernstein basis coefficients.
// The roots of P are in bijective correspondence with the roots of
// the original input polynomial P_{in}. More precisely,
// P has a root in (0,1) iff P_{in} has a root in I.
// We also ensure that the list of isolating intervals in v
// is sorted. The intervals in v are of two types:
// 1) Both the end points of the interval are the same; this happens
// only if the end point is a root of P.
// 2) Otherwise the interval represents an open interval which contains a root.
// Note, the end points may be roots but they will be distinct in this case.
// Advantage of this representation is that we don't have to compute the separation
// bound. 

template <typename T>
void isolateRoots(const Polynomial<T>& P, const BFInterval I, int deg, 
		BFVecInterval &v) {
  
  typedef SubDivData <BigFloat, T> SubDivData;
  
  
  int num = signVar(P.coeff(), deg);
  //  cout << "sign variations is at least " << num << endl;
  //  cout <<"Interval is ["<<I.first << ":"<< I.second << "]"<<endl;
  //  couts << "Polynomial is "; P.dump() ; cout <<endl;
  if(num == 0) return;
  if(num == 1){
    v.push_back(I);
    return;
  }

  std::stack< SubDivData > S;
  SubDivData SD(deg);
  S.push(SubDivData(I.first, I.second, P));
  BigFloat B=0;
  BigRat BB;
 
  while (!S.empty()){
    COUNTER++;
    SD = S.top(); S.pop();

    SubDivData SDL(deg), SDR(deg);

    //    B = lowerBoundB(SD.P, deg);// B is of the form m. 2^n
    //    std::cout<<"Lower bound is "<< B << " Mantissa = "<< B.m() << " Exp= "<<B.exp()<< std::endl; 

    
    /*    if(B >= BigFloat(0.5)){// The bound is worth it. More than half the interval has no roots.
      BB = BigRat(B);
      //      cout << "Numerator = "<<  numerator(BB) <<" Denominator "<< denominator(BB)<<endl;
      subdivide(SD, SDL, SDR, deg, numerator(BB), denominator(BB));//Only right part is interesting
      num = signVar(SDR.P.coeff(), deg);
      if(num == 1)
	v.push_back(BFInterval(SDR.a, SDR.b));
      else if(num > 1)
	S.push(SDR);
	}else{// subdivide at the midpoint*/
    subdivide(SD, SDL, SDR, deg);
    //      std::cout<<"After subdivision " << std::endl;
    //      std::cout<<"Left polynomial is " << std::endl;
    //      SDL.P.dump();cout<<endl;
    //    std::cout<<"Right polynomial is " << std::endl;
    //    SDR.P.dump();    cout<<endl;
    
    if(SDR.P.coeff()[0] == 0){// midpoint is a root
      //	std::cout<<"Midpoint is a root " << SDR.a << std::endl;
      v.push_back(BFInterval(SDR.a, SDR.a));
    }
    num = signVar(SDL.P.coeff(), deg);
    //    std::cout<<"Sign variations in left half " << num << std::endl;
    
    if(num == 1){
      //      cout<<"pushing "<< SDL.a <<" : "<< SDL.b <<endl;
      v.push_back(BFInterval(SDL.a, SDL.b));
    }else if(num > 1)
      S.push(SDL);
    //    cout<<"Left interval "<< SDL.a <<" : "<< SDL.b <<endl;
    
    num = signVar(SDR.P.coeff(), deg);
    //    std::cout<<"Sign variations in right half " << num << std::endl;
    if(num == 1){
      //      cout<<"pushing "<< SDR.a <<" : "<< SDR.b <<endl;
      v.push_back(BFInterval(SDR.a, SDR.b));
    }else if(num > 1)
      S.push(SDR);
    
    //    cout<<"Right interval "<< SDR.a <<" : "<< SDR.b <<endl;
  } 
 }

//Isolates roots of P in the CLOSED interval I; assumes P is square-free.
//This is achieved by computing a polynomial Q whose roots in the
//unit interval are in bijective correspondence with the roots of
//P in I. More precisely, Q(X) = P((b-a)X + a) where I=(a,b).
//This is obtained by first computing the Taylor shift by a of the polynomial
//and then doing a contraction by b-a.
//Again v contains the sorted list of intervals.
//template <typename T>
//void isolateRoots(const Polynomial<T>& P, BFInterval I, BFVecInterval& v){
//}




// We separate the positive
// and negative roots separately, and reduce the problem to isolating in 
// the unit interval.
template <typename T>
void isolateRoots(const Polynomial<T>& P, BRVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    return;
  //    cout<< "Polynomial is a constant" << endl;

  Polynomial<T> Q(deg);

  std::vector<int> Binomial;

  T B = CauchyUpperBound(P).get_z(GMP_RNDU);

  BFVecInterval vT;//Temporary storage of intervals containing roots
  //std::cout<<std::endl;Q.dump();cout<<endl;

  /***** STANDARD PROCDURE: MAP THE INTERVAL [-B, B] to [0,1] -- INEFFICIENT
  Polynomial<T> R(deg);
  shift(P.coeff(), deg, -B, R.coeff());
  contract(R.coeff(), deg, 2*B, R.coeff());
  convertToBernstein(R, Q, deg, Binomial, true, true);
  DescartesB2::isolateRoots(Q, BFInterval(0,1), deg, vT);  
  ****/

  /***** ISOLATE ROOTS IN [0, 1] *****/

  // First get the Bernstein coefficients w.r.t. [0,1]
  convertToBernstein(P, Q, deg, Binomial, true, true);
  // Isolating roots.
  isolateRoots(Q, BFInterval(0,1), deg, vT);
  // Getting back the intervals on [0,1]
  getInterval(vT, v, B, true, true);
  vT.clear();
  //  std::cout<<"Isolated roots in [0,1] "<<std::endl;

  /***** ISOLATE ROOTS IN [1,infty] *****/

  // First reverse the polynomial
  Polynomial <T> R(P); R.reverse();
  // Get the Bernstein coefficients w.r.t. [0,1] of the reversed polynomial
  convertToBernstein(R, Q, deg, Binomial, true, true);
  // Then isolate the roots of P in (1, infty) or the roots of R in (0,1)
  isolateRoots(Q, BFInterval(0,1), deg, vT);
  // Convert the intervals in [0,1] to [1, infty]
  getInterval(vT, v, B, true, false);
  vT.clear();// Clear the temporary vector for reuse

  /***** ISOLATE ROOTS IN [-1, 0] *****/
  // First get the Bernstein coefficients w.r.t. [0,1] of P(-X)
  convertToBernstein(P, Q, deg, Binomial, false, true);
  // Isolate the roots of P in (-1, 0), i.e., the roots of Q in (0,1)
  isolateRoots(Q, BFInterval(0,1), deg, vT);
  // Convert the intervals in [0,1] to [-1,0]
  getInterval(vT, v, B, false, true);
  vT.clear();

  /***** ISOLATE ROOTS IN [-infty, -1] *****/
  // First get the Bernstein coefficients w.r.t. [0,1] of P(-1/X)
  convertToBernstein(R, Q, deg, Binomial, false, true);
  // Isolate the roots of P in [-infty, -1], i.e., the roots of Q in (0,1)
  isolateRoots(Q, BFInterval(0,1), deg, vT);
  // Convert the intervals in [0,1] to [-infty, -1]  
  getInterval(vT, v, B, false, false);
  
  // Should sort the vector 
  //  std::sort(v.begin(), v.end());
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
    isolateRoots(P, v);
  
  t1.stop();
  std::cout << "Size of recursion tree = " << COUNTER/n;
  std::cout << " #roots = " << v.size()/n;
  std::cout<<" Time " << t1.getSeconds()/n <<endl;
  //  std::cout<<" Time per evaluation " << t1.getSeconds()/(COUNTER) <<std::endl;
  int i=0;
  for (BRVecInterval::const_iterator it = v.begin();
       i != v.size()/n; ++it) {
    std::cout << ++i << "th Root is in ["
	      << it->first << " ; " << it->second << "]" << std::endl;
    }

}

}// end namespace DescartesB2
#endif // __DESCARTESB2_H__
