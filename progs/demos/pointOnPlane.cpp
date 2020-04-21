/***************************************************************
 * File: pointOnPlane-dagstuhl.cpp
 * Author: Chee Yap <yap@cs.nyu.edu>
 *         Chen Li  <chenli@cs.nyu.edu>
 * Date:   12/98
 * Desc:   Determine the intersection point of the plane 
 *              P: sqrt(2)*x + y + z - 1 = 0
 *         and the line:
 *              L : (0, 0, 0) --- (sqrt(i), j*sqrt(j), 3)
 *                  where i, j is in the range [1, N).
 *         is really on the plane P.  (Set N=32 in this test)
 *
 * $Id: pointOnPlane.cpp,v 1.1 2006/03/07 04:51:23 exact Exp $
 **************************************************************/

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "CORE/geometry3d.h"
using namespace std;

const static int N = 32;

// The following program
//      (1) shoots N*N rays from the origin,
//      (2) compute their intersections with a plane P and
//      (3) check if the intersection points lie on the plane P
// In CORE_LEVEL 3, they ought to lie on P every time.
 
int main() {

  const Point3d origin(0, 0, 0);
  Plane3d P(sqrt(2.0), 1.0, 1.0, -1.0);

  int counter=0;
  for (int i=1; i<=N; i++) {
    for (int j=1; j<=N; j++) {
      Line3d L(origin, Vector(i, j*sqrt(double(j)), 3.0));
      
      Point3d* intp = (Point3d*)P.intersection(L);
      if (P.contains(*intp))
      		  counter ++;
    }
  }

  cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << ">  Equation of Plane P is  Sqrt(2)*X + Y + Z - 1 = 0" << endl;
  cout << ">  Number of points on plane P = " << counter << endl;
  cout << ">  Number of points NOT on Plane P = " << N*N-counter << endl;
  if (counter < N*N) {
     cout << "      INCORRECT!" ;
     if (CORE_LEVEL != 3)
        cout << "       (As expected, when not in CORE_LEVEL 3) " << endl;
     else
        cout << "       (BAD!! Should not happen in CORE_LEVEL 3)" << endl;
  } else
        cout << "      CORRECT! (not a single error)" << endl;
  cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  return 0;
}


