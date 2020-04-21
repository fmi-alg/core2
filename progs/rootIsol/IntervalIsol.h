/*
  File contains the code for real root isolation based upon interval
  box function. The EVAL algorithm with centered box functions
 */
#ifndef __INTERVALISOL_H__
#define __INTERVALISOL_H__

#define CORE_LEVEL 4
#include "CORE/CORE.h"

#include <queue>
#include "RootIsolationTools.h"


using namespace std;
using namespace CORE;

namespace IntervalIsol{

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
  
  while(!Q.empty())
    {
      seg = Q.front(); Q.pop();
      std::cout << "Interval is " << seg.first
		   << " : " << seg.second << std::endl;

      segtemp = box(seg, P, deg);
      cout <<"segtemp is " << segtemp.first << " : " << segtemp.second << endl;

      /*      if(segtemp.first * segtemp.second > 0)
	{
	 std::cout << "Rejected Box " << seg.first
		   << " : " << seg.second << std::endl;
		   }*/

      if(sign(segtemp.first) * sign(segtemp.second) < 0)
	{
	  //	  std::cout << "Eval Box has sign change on " << seg.first
	  //    << " : " << seg.second << std::endl;
	  
	  segtemp = box(seg, R, deg-1);
	  
	  if(sign(segtemp.first) * sign(segtemp.second) > 0)
	    {
	      std::cout << " Derivative has no sign change" << std::endl;

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
	      cout<<"midpoint = "<< midpoint<<endl;
	      if (sign(P.eval( midpoint) ) == 0)
		v.push_back(BFInterval(midpoint, midpoint));

		count += 2;
		cout<<"Adding the two intervals"<<endl;
		// If not, just add the BFIntervals
		Q.push(BFInterval(seg.first, midpoint));
		Q.push(BFInterval(midpoint, seg.second));
		//}
	    }
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

  cout<<"Inside root isolation"<<endl;
  unsigned int count = 1;

  BFVecInterval vT;
  count= IntervalIsol::isolateRoots(P, BFInterval(-1, 1), vT);
  cout<<"Isolated roots in (-1, 1)" <<endl;

  BRInterval J;
  for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it)
    v.push_back(BRInterval((*it)));
  vT.clear();

  Polynomial<T> Q(P);

  Q.reverse();
  count += IntervalIsol::isolateRoots(Q, BFInterval(0,1), vT);
  //  cout<<"Isolated roots in (1, infty)" <<endl;
  count += IntervalIsol::isolateRoots(Q, BFInterval(-1,0), vT);
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

}// end namespace IntervalIsol


#endif //__INTERVALISOL_H__
