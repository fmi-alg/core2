/* **************************************
   File: orourke.cc

   Purpose: To illustrate the difference between
	incremental evaluation and non-incremental
	evaluation of expressions.

	We use two examples from O'Rourke:

	e = sqrt(10) + sqrt(11) - sqrt(5) - sqrt(18)
	  = 0.0001937859047052015
	f = sqrt(5) + sqrt(6) + sqrt(18) - sqrt(4) - sqrt(12) -sqrt(12)
 	  = -0.0000048228732563160

	These two sums are known to give the minimum value of r(20,2)
	and r(20,3), respectively.  In general, r(n,k) is the minimum
	achieved by the difference of two sums of k square roots,
	where the operands of the squareroots are at most n.
	
   Usage:
	% orourke

   Since CORE Library Version 1.2
   $Id: orourke.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ************************************** */


#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include <ctime>
#include "CORE.h"

int main() {

  clock_t starttime, endtime;
  int n = 10;

  setDefaultRelPrecision(100);
  std::cout.precision(20);

  setIncrementalEvalFlag(true);
  std::cout << "Orourke's Example: incremental evaluation \n";

  double e = sqrt((double)10) + sqrt((double)11) - sqrt((double)5) - sqrt((double)18);
  double f = sqrt((double)5) + sqrt((double)6) + sqrt((double)18) - sqrt((double)4) - sqrt((double)12) -sqrt((double)12);
  std::cout << " sqrt(10) + sqrt(11) - sqrt(5) - sqrt(18) : " << e << std::endl;
  std::cout << " sqrt(5) + sqrt(6) + sqrt(18) - sqrt(4) - sqrt(12) -sqrt(12) : "
	  << f << std::endl;
  
  starttime = clock();
  for (int i = 0; i<n; i++) {
    double e = sqrt((double)10) + sqrt((double)11) - sqrt((double)5) - sqrt((double)18);
    double f = sqrt((double)5) + sqrt((double)6) + sqrt((double)18) - sqrt((double)4) - sqrt((double)12) -sqrt((double)12);

    e.approx(); f.approx();
  }
  endtime = clock();

  std::cout << " User Time (x10) = " << (float)(endtime-starttime)/CLOCKS_PER_SEC << std::endl;
  std::cout << std::flush;

  setIncrementalEvalFlag(false);
  std::cout << "Orourke's Example: non-incremental evaluation \n";
  starttime = clock();
  for (int j=0; j<n; j++) {
    double ee = sqrt((double)10) + sqrt((double)11) - sqrt((double)5) - sqrt((double)18);
    double ff = sqrt((double)5) + sqrt((double)6) + sqrt((double)18) - sqrt((double)4) - sqrt((double)12) -sqrt((double)12);
    ee.approx();  ff.approx();
  }
  endtime = clock(); 

  std::cout << " User Time (10x) = " << (float)(endtime-starttime)/CLOCKS_PER_SEC << std::endl;
  std::cout << std::flush;
  
  return 0;
}

