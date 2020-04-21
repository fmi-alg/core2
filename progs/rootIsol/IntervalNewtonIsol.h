/*
  File contains the code for real root isolation based upon interval
  Newton. The evaluation of polynomials on invertals is based upon the 
  centered form. The real root isolation algorithm is given in, 
  "A new method for real root isolation of univariate polynomials" by
  Ting Zhang and Bican Xia.

 */
#ifndef __INTERVALNEWTONISOL_H__
#define __INTERVALNEWTONISOL_H__

#define CORE_LEVEL 4
#include "CORE/CORE.h"

#include <queue>
#include "RootIsolationTools.h"


using namespace std;
using namespace CORE;

namespace IntervalNewtonIsol{

// Isolates the roots of P in the open interval I
template <typename T>
unsigned int isolateRoots(Polynomial <T> &P, BFInterval I, BFVecInterval &v)
{

  unsigned int deg = P.getTrueDegree();
  std::queue<BFInterval> Q;
  BFInterval seg, segtemp;
  BigFloat  midpoint;
  Polynomial<T> R(P);
  R.differentiate();

  Q.push(I);
  unsigned int count = 1;// Counts the size of the tree
  bool leftInter = false, rightInter = false;
  

  while(!Q.empty())
    {
      seg = Q.front(); Q.pop();

      segtemp = box(seg, R, deg-1);
      
      if(sign(segtemp.first) * sign(segtemp.second) > 0)
	{
	  //	      std::cout << " Derivative has no sign change" << std::endl;

	  if(sign(P.eval(seg.first)) * sign(P.eval(seg.second))< 0){
	    //		std::cout << " Pushing " << seg.first <<":"<< seg.second << std::endl;
	    v.push_back(seg);
	  }
	  if(v.size() == deg)
	    return count;
	}
      else
	{
	  // Check if the subdivision midpoint is a root...
	  midpoint = (seg.first + seg.second).div2() ;
	  if (sign(P.eval( midpoint) ) == 0)
	    v.push_back(BFInterval(midpoint, midpoint));

	  newton(seg, P, segtemp, deg, leftInter, rightInter);
	  if(leftInter){
	    Q.push(BFInterval(seg.first, midpoint));
	    count++;
	  }
	  if(rightInter){
	    Q.push(BFInterval(midpoint, seg.second));
	    count++;
	  }
	  leftInter = false; rightInter = false;
	}
    }
    
  return count; 
 }

// Isolate all the real roots of P. Returns the size of the root isolation tree
template <typename T>
unsigned int isolateRoots(Polynomial <T> &P, BRVecInterval &v)
{
  // We first isolate the real roots of P in [-1, 1]. Then consider the reverse
  // polynomial and isolate its roots in [-1, 1]; map the isolated intervals
  // back to [-infty, infty]. This way we avoid evaluations at the root bounds.

  if(P.coeff()[0] == 0){
    v.push_back(BRInterval(0, 0));
    P.mulXpower(-1);
  }
  if(P.eval(T(1)) == 0)
    v.push_back(BRInterval(1, 1));
  if(P.eval(T(-1)) == 0)
    v.push_back(BRInterval(-1, -1));

  unsigned int count = 1;

  BFVecInterval vT;
  count = IntervalNewtonIsol::isolateRoots(P, BFInterval(-1, 1), vT);
  //  cout<<"Isolated roots in (-1, 1)" <<endl;

  BRInterval J;
  for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it)
    v.push_back(BRInterval((*it)));
  vT.clear();

  Polynomial<T> Q(P);

  Q.reverse();
  count += IntervalNewtonIsol::isolateRoots(Q, BFInterval(0,1), vT);
  //  cout<<"Isolated roots in (1, infty)" <<endl;
  count += IntervalNewtonIsol::isolateRoots(Q, BFInterval(-1,0), vT);
  //cout<<"Isolated roots in (-infty, -1)" <<endl;

  BigFloat CB = CauchyUpperBound(P);

  BigRat tmp;

  for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it){
    if((*it).first >= 0){
      tmp = (*it).second;
      J.first = 1/tmp;
      tmp = (*it).first;
      if((*it).first != 0)
	J.second = 1/tmp;
      else
	J.second = CB;
    }else{
      tmp = (*it).first;
      J.second = 1/tmp;
      tmp = (*it).second;
      if((*it).second != 0)
	J.first = 1/tmp;
      else
	J.first = -CB;
    }
    v.push_back(J);
  }
  //  std::sort(v.begin(), v.end());
  return count;
}

// Isolate the real roots of P iteratively for n iterations. Then return the
// average tree size, time for n iterations.
template <typename NT>
void test(Polynomial<NT>& P, int n) {
  
  BRVecInterval v;
  
  int count=1;
  Timer t1;
  t1.start();
  for(int i=1; i <= n ; i++)
    count=isolateRoots(P, v);
  
  t1.stop();
  std::cout << "Size of recursion tree = " << count;
  std::cout << " #roots = " << v.size()/n;
  std::cout<<" Time " << t1.getSeconds()/n<<endl;
  //  std::cout<<" Time per evaluation " << t1.getSeconds()/(n*count) <<std::endl;
}

}// end namespace IntervalNewtonIsol


#endif //__INTERVALNEWTONISOL_H__
