/*
  File contains the code for real root isolation based upon
  the continued fraction algorithm. 
  This is the scaled version of Akritas' algorithm.
 */
#ifndef __CONTFRAC_H__
#define __CONTFRAC_H__
#define CORE_LEVEL 4
#include "CORE/CORE.h"
#include"RootIsolationTools.h"

using namespace CORE;
using namespace std;

namespace ContFrac{
int COUNTER = 0;

// Isolates the positive roots of P; assumes P is square-free.
// v contains the sorted list of intervals. The four BigFloat's a,b,c and d
// are error-free and
// represent the Moebius transformation (aX+b)/(cX+d). Initially this
// is the identity transformation, i.e., a=d=1 and b=c=0.
// CB is the Cauchy bound for the initial polynomial, it is used when
// sign variations are one and a/c = \infty, i.e., c=0.
// sv is the number of sign variations in P. If P is the polynomial
// corresponding to the right child of a node, then we have already
// calculated the sign variations of P inside its parent node, and hence
// avoid re-calculating it by passing it as a parameter; sv = -1 signals
// that we have not yet computed the sign variations.
// The intervals returned have to be rationals.
template <typename RT, typename T, typename FT>
void isolateRoots(CFDivData<RT,T>& SD, std::vector<std::pair<FT, FT> >& v,
		  BigInt CB, int deg){

  typedef CFDivData<RT, T> CFDivData;
  //  cout<<"Sign variations = " << SD.sv << endl;
  if(SD.sv == 0) 
    return;
  if(SD.sv == 1){
    getInterval(SD, v, CB);
    return;
  }

  if(SD.P.coeff()[0] == 0){
    v.push_back(std::make_pair(FT(SD.b,SD.d), FT(SD.b,SD.d)));
    //    cout << "Found a root " << b/d <<endl;
    SD.P.mulXpower(-1);
  }


  std::stack< CFDivData > S;
  S.push(SD);

  while(!S.empty()){
    COUNTER++;
    SD = S.top(); S.pop();
    CFDivData SDL(deg), SDR(deg);
    //    cout <<"a= "<< SD.a << "b= "<< SD.b<< "c= "<<SD.c <<"d= " <<SD.d << endl;

    bool leftCreated = subdivide(SD, SDL, SDR, SD.P.getTrueDegree());
    // Passing the exact degree is crucial; otherwise we access junk memory

    if(SDR.P.coeff()[0] == 0){
      v.push_back(std::make_pair(FT(SDR.b,SDR.d), FT(SDR.b,SDR.d)));
      SDR.P.mulXpower(-1);// Reduces the degree
    }
    
    //    cout<<"Sign variations in right = " << SDR.sv << endl;
    if(SDR.sv == 1)
      getInterval(SDR, v, CB);      
    if(SDR.sv > 1)
      S.push(SDR);

    if(leftCreated){
      //      cout<<"Left created "<<endl;
      //      cout<<"Sign variations in left  = " << SDL.sv << endl;
      if(SDL.P.coeff()[0] == 0)
	SDL.P.mulXpower(-1); // Reduces the degree
      if(SDL.sv == 1)
	getInterval(SDL, v, CB);      
      if(SDL.sv > 1)
	S.push(SDL);
    }

  }

}

//Isolate all the real roots of P.
//The intervals returned in v are of two types:
//1) Both the end points of the interval are the same. This happens
// if the end point is a root of P.
//2) Otherwise the interval represents an open interval which contains a root.
// Note, the end points may be roots but they will be distinct in this case.
//Advantage of this representation is that we don't have to compute the separation
//bound.
template <typename T, typename FT>
void isolateRoots(const Polynomial<T>& P, std::vector<FT>& v){
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;

  Polynomial<T>Q(P);
  //  cout << "Isolating positive roots of P"<< endl;
  BigInt CB=CauchyUpperBound(P).get_z(GMP_RNDU);
  //  cout << "Cauchy's bound is " << CB << endl;
  BigInt one(1), zero(0), mone(-1);
  CFDivData<BigInt, T> SD =
      CFDivData<BigInt, T>(one, zero, zero, one, signVariation(Q), Q);

  isolateRoots(SD, v, CB, deg ); //Isolate the positive roots of P.

  //Assign Q as the polynomial whose positive roots correspond 
  // with the negative roots of P.
  for(int i=1; i<= deg; i++){
    if(i % 2 != 0)
      Q.coeff()[i] *= -1;
  }
  //  cout << "Isolating negative roots of P"<< endl;

  SD = CFDivData<BigInt, T>(mone, zero, zero, one, signVariation(Q), Q);
  isolateRoots(SD, v, CB, deg);//Isolate the negative roots of P.
  //  std::sort(v.begin(), v.end());
  //  std::cout << "Number of recursive steps = " << COUNTER << std::endl;
}


// Isolate the real roots of P iteratively for n iterations. Then return the
// average tree size, time for n iterations.
template <typename NT>
void test(const Polynomial<NT>& P, int n=10) {
  
  BRVecInterval v;
  
  Timer2 t1;
  t1.start();
  for(int i=1; i <= n ; i++)
    isolateRoots(P, v);
  
  t1.stop();
  std::cout << "Size of recursion tree = " << COUNTER/n;
  std::cout << " #roots = " << v.size()/n;
  std::cout<<" Time " << t1.getSeconds()/n <<endl;
  //  std::cout<<" Time per evaluation "
  //           << t1.getSeconds()/(COUNTER) <<std::endl;
}

} // end namespace ContFrac



#endif //__CONTFRAC_H__
