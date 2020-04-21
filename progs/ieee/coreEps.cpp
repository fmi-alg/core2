/************************************************************
 * file: coreEps.cpp
 * synopsis:	
 * 		Various tests with CORE_EPS, which is defined
 * 		to be 2^{-53}.  
 * 		This should be called "Machine Unit Roundoff",
 * 		not "Machine Epsilon" as our current name suggests.
 * 		Machine Epsilon would be 2^{-52}.
 *
 * usage: 
 *		% coreEps 
 *
 * author: Chee Yap
 * date: Aug 26, 2003
 * Since Core Library Version 1.6
 * $Id: coreEps.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 ************************************************************/

#include <CORE.h>
using namespace std;

void test(int baseVal) {

   cout << "   > Base Value            = " << baseVal << endl;
   Expr e = baseVal;
   e += 2*CORE_EPS;		// CORE_EPS = 2^{-53}
   Expr f = baseVal;
   f *= relEps;		// relEps = 1 + 2^{-52}

   cout << "   > Expr e = baseVal + 2*CORE_EPS    = " << e << endl;
   cout << "   >>> e.doubleValue()              = " << e.doubleValue();
   if (e.doubleValue() == baseVal)
	   cout << "  (ERROR!! This is same as baseVal" << endl;
   else
	   cout << "  (CORRECT!! This is different from baseVal" << endl;
   cout << "   > Expr f = baseVal * refEps      = " << f << endl;
   cout << "   >>> f.doubleValue()              = " << f.doubleValue();
   if (f.doubleValue() == baseVal)
	   cout << "  (ERROR!! This is same as baseVal" << endl;
   else
	   cout << "  (CORRECT!! This is different from baseVal" << endl;

   cout << endl;

   cout << "\n\n ANOTHER TEST: How many epsilons before we see a difference?"
   	<< endl;
   for (int i = 1; i< 10; i++) {
      e += CORE_EPS;
      cout << "   > e = baseVal + " << i+1 << " * CORE_EPS = " << e << endl;
      cout << "   >>> e.doubleValue()          = " << e.doubleValue() << endl;
      f *= relEps;
      cout << "   > f = baseVal * (relEps)^{" << i+1 << "} = " << f << endl;
      cout << "   >>> f.doubleValue()          = " << f.doubleValue() << endl;
      cout << " =======" << endl;
   }
}

int main(int argc, char ** argv) {

   int baseVal = 1;
   if (argc >= 2) baseVal = atoi(argv[1]);

   int prec = 17;
   if (argc >= 3) prec = atoi(argv[2]);

   machine_double eps = CORE_EPS;
   setDefaultOutputDigits(prec);

   cout << "================================================================\n";
   cout << "   OUTPUT PRECISION : " << prec << endl;
   cout << "   CORE_EPS         : " << eps << endl;
   cout << "   relEps           : " << relEps << endl;
   cout << "   =============================================================\n";
   test(baseVal);
   cout << "   =============================================================\n";

   return 0;
}
