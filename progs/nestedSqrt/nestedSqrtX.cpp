/****************************************************************
 File: nestedSqrtX.ccp
 Purpose:
 	Generalization of nestedSqrt.cpp,
		where the base number "x" can be arbitrary.
	Experiment to demonstrate the speedup effects of the
		k-ary rootbounds from Yap and Pion

	For any real x > 0, and natural number k, we test the sign of
	the following two expressions:

		E0 = E0(x,k) = SQRT( SQRE( x, k), k) - x
		E1 = E1(x,k) = SQRT( SQRE( x, k) + 1, k) - x

	where SQRE( x, k) = SQRE( x, k-1)**2    when k>0, and SQRE( x, 0)= x,
	and   SQRT( x, k) = sqrt( SQRT( x, k-1) when k>0, and SQRT( x, 0)= x,

	Clearly E0 is identically zero.
	For large k, E1 is a very small positive number.

 Author: Chee Yap (Sep 30, 2002)
 Usage:
	% nestedSqrtX [k=4] [x="2.25"]
	
	-- where the two optional arguments have the following meaning:
	-- k is the nesting depth of the sqrt expression
		k = 4 is the default
	-- x is the base number in decimal.
		x = 2.25 is the default

 Since CORE Library Version 1.5
 $Id: nestedSqrtX.cpp,v 1.4 2010/06/11 17:49:30 exact Exp $
****************************************************************/

#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

using namespace std;

#include "CORE.h"

int main(int argc, char* argv[])
{
   cout << "================== NestedSqrtX ================" << endl;
   cout << "===============================================" << endl;

   // PRECISION PARAMETERS
   // ========================================================================
	cout.precision(40);		// printout precision
	setDefaultRelPrecision(20);		// default relative precision

   // COMMAND LINE ARGUMENTS
   // ========================================================================
        double E, x;
        int i,k;
	if (argc == 1) {
	   k = 4;
	   cout << "   Nesting depth is 4 (default)" << endl;
	} else {
           k = atoi(argv[1]);
	   cout << "   Nesting depth is " << k << endl;
	}
	if (argc == 3) {
		x = argv[2];
	        cout << "   x = " << x << endl;
	} else {
		x = "2.25";			// default base value
	        cout << "   x = " << x << " (default) " << endl;
	}

   // EVALUATION OF EXPRESSION E1 
   // ========================================================================
	E = x;
        for (i=0;i<k;i++)
                E = E*E;
	cout << "   E = SQRE(" << x << ", " << k << ") \n";
	cout << "     = " << E << endl;
	cout << "       where SQRE(x,k)= SQRE(x,k-1)**2 and SQRE(x,0)=x\n";
        double E1 = E+1;
        for (i=0;i<k;i++)
                E1 = sqrt(E1);
        E1 = E1 - x;
	if (E1>0) {
	  setScientificFormat();
	  cout << "   E1 = SQRT(E + 1, " << k << ") - x \n";
	  cout << "      = " << E1 << endl;
	  cout << "        (N.B. E1 should be very small positive number)\n";
	  cout << "        ( lg(E1) ~ " <<E1.rep()->get_uMSB()<< ")"<< endl;
	} else 
	  cerr  << "   ERROR!! E1 is not positive " << endl;

   // EVALUATION OF EXPRESSION E0
   // ========================================================================
        for (i=0;i<k;i++)
                E = sqrt(E);
        E = E-x;
	if (E == 0) {
	cout << "   E0 = SQRT(E, " << k << ") - x \n";
	cout << "      = " << E << endl;
	cout << "        (CORRECT!! E0 is identically zero)\n";
	} else
	  cerr  << "   ERROR!! E0 is not zero " << endl;

   cout << "===============================================" << endl;
   return 0;
}
