/*
  File contains the code for real root isolation based upon the monomial representation
  of intermediate polynomials. This is Collins-Akritas' algorithm.
 */
#ifndef __DESCARTESP_H__
#define __DESCARTESP_H__

#define CORE_LEVEL 4
#include<stack>
#include "CORE/CORE.h"
#include"RootIsolationTools.h"

using namespace CORE;
using namespace std;

namespace DescartesP{
int COUNTER = 0;  

//Isolates real roots of P in the interval (0,1). The coefficients represent
// the power basis coefficients.
//The roots of P in (0,1) are in bijective correspondence with the roots of
//the original input polynomial P_{in} in I. Thus
//P has a root in (0,1) iff P_{in} has a root in I.
//We also ensure that the list of isolating intervals in v
//is sorted. The intervals in v are of two types:
//1) Both the end points of the interval are the same. This happens
//only if the end point is a root of P.
//2) Otherwise the interval represents an open interval which contains a root.
// Note, the end points may be roots but they will be distinct in this case.
//Advantage of this representation is that we don't have to compute the separation
//bound. 

template <typename T>
void isolateRoots(Polynomial<T> P, const BFInterval I, int deg, 
		BFVecInterval &v) {

  typedef SubDivData <BigFloat, T> SubDivData;  
  
  int num = shiftAndSigncount(P.coeff(), deg);
  //  cout << "sign variations after shift " << num << endl;
  //cout <<"Interval is ["<<I.first << ":"<< I.second << "]"<<endl;
  //cout << "Polynomial is "; P.dump() ; cout <<endl;
  if(num == 0) 
    return;
  if(num == 1){
    v.push_back(I);
    return;
  }

  std::stack< SubDivData > S;
  SubDivData SD(deg);
  S.push(SubDivData(I.first, I.second, P));

  while(!S.empty()){
    COUNTER++;
    SD = S.top(); S.pop();
    SubDivData SDL(deg), SDR(deg);

    subdividePow(SD, SDL, SDR, SD.P.getTrueDegree());

    if(SDR.P.coeff()[0] == 0){
      v.push_back(std::make_pair(SDR.a, SDR.a));
      SDR.P.mulXpower(-1);
    }

    if(SDL.P.coeff()[0] == 0)
      SDL.P.mulXpower(-1);
    
    
    num = shiftAndSigncount(SDL.P.coeff(), SDL.P.getTrueDegree());
    //    cout << "sign variations in left " << num << endl;
    if(num == 1)
      v.push_back(BFInterval(SDL.a, SDL.b));
    else if(num > 1)
      S.push(SDL);

    num = shiftAndSigncount(SDR.P.coeff(), SDR.P.getTrueDegree());
    //    cout << "sign variations in right  " << num << endl;
    //    std::cout<<"Sign variations in right half " << num << std::endl;
    if(num == 1)
      v.push_back(BFInterval(SDR.a, SDR.b));
    else if(num > 1)
      S.push(SDR);
  }

}


//Isolates roots of P in the CLOSED interval I; assumes P is square-free.
//This is achieved by computing a polynomial Q whose roots in the
//unit interval are in bijective correspondence with the roots of
//P in I. More precisely, Q(X) = P((b-a)X + a) where I=(a,b).
//This is obtained by first computing the Taylor shift by a of the polynomial
//and then doing a contraction by b-a.
//Again v contains the sorted list of intervals.
template <typename T>
void isolateRoots(Polynomial<T>& P, BFInterval I, BFVecInterval& v){
  int deg = P.getTrueDegree();
  if(deg == 0)
    return;
  
  //    std::cout<< "Polynomial is a constant" << endl;
  
  
  BigFloat a = I.first, b=I.second;
  
  BigFloat temp1[deg + 1], temp2[deg+1];

  shift(P.coeff(), deg, a, temp1);
  contract(temp1, deg, b-a, temp2);



  if(P.eval(a) == 0)
    v.push_back(std::make_pair(a, a));

  Polynomial<BigFloat> R(deg, temp2);
  //cout <<"Corresponding polynomial with roots in unit interval ";R.dump();
  //cout <<endl;
  //COUNTER=0;
  
  isolateRoots(R, I, deg, v);

  //  std::cout << "Number of recursive steps = " << COUNTER << std::endl;
  
  if(P.eval(b) == 0)
    v.push_back(std::make_pair(b, b));
  //std::sort(v.begin(), v.end());

}

// Isolates all real roots of P. Assumes that the polynomial is square-free.
// v will contain the isolating intervals for P in sorted order.
// We calculate an upper bound B on the absolute value of the roots of P
// and call isolateRoots(P, (-B, B), v). This approach is slow!
/*template <typename T>
void isolateRoots(Polynomial<T>& P, BFVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;

  //Compute an upper bound on the positive roots of P
  T B = P.CauchyBound();

  BFInterval I(-B, B);
  isolateRoots(P, I, v);
}
*/

// An alternative approach to isolating all roots. Here we separate the positive
// and negative roots separately. This is slightly more efficient than the method
// above. Also we avoid using the Cauchy bound in our computations, except to get
// the final isolating intervals.
template <typename T>
void isolateRoots(Polynomial<T>& P, BRVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    return;
    //    cout<< "Polynomial is a constant" << endl;

  Polynomial<T> Q(P);// Copy P
  Q.reverse();
  BFVecInterval vT;

  if(P.coeff()[0] == 0){
    v.push_back(BRInterval(0,0));
    P.mulXpower(-1); // Changes the true degree
  }

  T B = CauchyUpperBound(P).get_z(GMP_RNDU);
  // First isolate the roots of P in the unit interval
  DescartesP::isolateRoots(P, BFInterval(0,1), P.getTrueDegree(), vT);
  getInterval(vT, v, B, true, true);
  vT.clear();

  // Check if one is a root of P
  if(P.eval(T(1)) == 0)
    v.push_back(BRInterval(1,1));

// Then isolate the roots of P in (1, infty) or the roots of Q in (0,1)
  DescartesP::isolateRoots(Q, BFInterval(0,1), Q.getTrueDegree(), vT);
  getInterval(vT, v, B, true, false);
  vT.clear();

  Polynomial<T> R(deg);
  // Flip the signs of the odd coefficients of R and Q
  for(int i=0; i<= deg; i++){
    if(i % 2 != 0){
      R.coeff()[i] =-1*P.coeff()[i]; Q.coeff()[i]*=-1;
    }else
      R.coeff()[i] = P.coeff()[i];
  }
    
  // Isolate the roots of P in (-1, 0), i.e., the roots of R in (0,1)
  //  cout<<"Isolate roots in (-1, 0)"<<endl;
  //  R.dump();
  DescartesP::isolateRoots(R, BFInterval(0,1), R.getTrueDegree(), vT);
  getInterval(vT, v, B, false, true);
  vT.clear();

  // Check if -one is a root of P
  if(P.eval(T(-1)) == 0)
    v.push_back(BRInterval(-1,-1));

  // Isolate the roots of P in [-infty, -1], i.e., the roots of Q in (0,1)
  DescartesP::isolateRoots(Q, BFInterval(0,1), Q.getTrueDegree(), vT);
  getInterval(vT, v, B, false, false);
  // std::sort(v.begin(), v.end());
  
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
}

}//namespace Descartes

#endif //__DESCARTESP_H__
