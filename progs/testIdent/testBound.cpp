/* ************************************************
   File: testBound.ccp

   Purpose:  To test functions based on inequalities.

   	Currently, we test the following functions in Expr class:
		ceil, floor, ceilLg, floorLg
	Ultimately, should extend to other number types as well.

   Usage:
        % testBound

   Author: Chee Yap

   Since Core Library 1.7

   $Id: testBound.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */        

#define CORE_LEVEL 4
#include "CORE.h"
using namespace std;

// THESE ARE NOT DEFINED BY CORE:
// already in gmp:  #define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MIN(l,o) ((l) < (o) ? (l) : (o))
#define MAX(h,i) ((h) > (i) ? (h) : (i))


// boundTest:
// 	Note that ceil and floor return BigInts
// 	But ceilLg and floorLg return long
void boundTest(const Expr& a) {
	assert(a <= ceil(a));
	assert(-a <= ceil(-a));
	assert(a >= floor(a));
	assert(-a >= floor(-a));
	int e;
	if (a > 0) {
	   e = ceilLg(a);	// we assume int and long are equal!!
	   assert(Expr(BigFloat::exp2(e)) >= a);
	   e = floorLg(a);	// we assume int and long are equal!!
cout << "floorLg(a) = " << e << endl;
	   assert(Expr(BigFloat::exp2(e)) <= a);
	}
}

int main(int argc, char* argv[])
{

   cout << " ====================== Start Bound Test ===================\n";
   // Boundary Tests:
   cout << "   -- testing 0" << endl;
   boundTest(Expr(0));
   cout << "   -- testing 1" << endl;
   boundTest(Expr(1));
   cout << "   -- testing -1" << endl;
   boundTest(Expr(-1));
   cout << "   -- testing 0.5" << endl;
   boundTest(Expr(0.5));

   // Some Tests:
   cout << "   -- testing 9876543210" << endl;
   boundTest(Expr("9876543210"));
   cout << "   -- testing 12321.9876" << endl;
   boundTest(Expr("12321.9876"));
   cout << "   -- testing 98765/43210" << endl;
   boundTest(Expr("98765/4321"));

// bug!  test fails:
#ifdef DEBUG
   cout << "   -- testing 9876/543210" << endl;
   boundTest(Expr("9876/54321"));
#endif

   // if more tests wanted:
   if (argc == 1) cout <<
	 "  -- For more test, call testBound with any argument" << endl;
   else {

   	   cout << "   -- testing loop" << endl;
	   for (int i=2; i<20; i++) 
		   boundTest(Expr(i));

   }

   cout << " ====================== End Bound Test =====================\n";
   return 0;
}
