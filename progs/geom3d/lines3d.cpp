/***************************************************************
 * File: linesRelativePosition.cc
 * Author: Chee Yap <yap@cs.nyu.edu>
 *         Chen Li  <chenli@cs.nyu.edu>
 * Date:   12/98
 * Description:  
 *      Determine the relative position of the following two lines:
 *              L0 : (0, 0, 0) --- (sqrt(i), sqrt(j), 1) and
 *              L1 : (1, 1, 0) --- (sqrt(i)+1, sqrt(j)+1, 1)
 *      where i, j is in the range [0, N) (we set N=50).
 *      Here, the relative positions of two lines in space must be one
 *      of the following mutually exclusive outcomes:
 *              L0.isParallel(L1) is true
 *              L0.isSkew(L1) is true
 *              L0.intersects(L1) is true
 *      In our tests below, the two lines must always be parallel.
 * 
 * $Id: lines3d.cpp,v 1.2 2010/07/21 07:11:25 exact Exp $
 **************************************************************/

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/geometry3d.h"

using namespace CORE;
using namespace std;


static const int N = 25;

int main() {

#ifdef __REDUCE
  setRationalReduceFlag(true);
  cout << "rationalReduceFlag = true" << endl;
#endif

  Point3d p1(1.0, 1.0, 0.0);
  Point3d origin(0, 0, 0);
  int numPar= 0;
  int numSkew = 0;
  int numInt = 0;
  
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      Line3d L0(origin, Point3d(sqrt(double(i)), j, 1));
      Line3d L1(p1, Point3d(sqrt(double(i))+1, j+1, 1));
      if (L0.isParallel(L1)) numPar++;
      if (L0.isSkew(L1)) numSkew++;
      if (L0.intersects(L1) != -1) numInt++;
    }
  }
  
  cout << "par: " << numPar <<
                ", skew: " << numSkew <<
                ", intersect: " << numInt <<endl;
  if ((numPar < N*N) || (numSkew > 0) || (numInt > 0)) {
        cout << "INCORRECT!" ;
        if (CORE_LEVEL != 3)
                cout << " (But OK when not in CORE_LEVEL 3) " << endl;
        else
                cout << " (BAD!! should not happen in CORE_LEVEL 3)" << endl;
  } else
        cout << "CORRECT!" << endl;

  return 0;
}

