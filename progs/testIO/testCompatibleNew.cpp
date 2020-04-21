/* ************************************************
   File: testCompatibleNew.cpp

   Purpose:  Tests if two decimal strings, are they compatible?
   		Strings can be in scientific or positional notation
	     In this new version, we generalize the predicate "isCompatible(s1, s2)"
	        to a new function "compatible(s1,s2)" that returns the first position
                where the two strings becomes incompatible.   Then
                the original "isCompatible(s1,s2)" is equivalent to
	        "compatible(s1,s2) = min(length(s1),length(s2))".
              This theory is correct as long as s1 and s2 are not zero (because
	       there is no "msb" in a zero to begin the comparison.
               In this case, we use the old definition.

   Usage:
        % testCompatibleNew
	% testCompatibleNew [string1] [string2] [compatible]
	% testCompatibleNew 12.34 1234e-2 true

   Theory: Here is the definition of compatibility
     For any decimal string SX, it has a nominal value X
     and also an uncertainty UX.

     E.g., SX= 1.234 then X=1.234 and UX=0.001
     E.g., SX= 1200e-2 then X=12.00 and UX=0.01
     E.g., SX= 12 then X=12 and UX=1

 	The string SX then defines an interval [X-UX, X+UX],
     which I call the "implicit interval" of SX.
 
     When we say two strings, SX and SY are compatible, we mean
     that their implicit intervals overlap.

     To decide compatibility of SX and SY, we first compute
     X, UX, Y, UY.  Wlog, let X <= Y.  Then
     SX and SY are compatible iff
           X + UX >= Y - UY.
     REMARK: we could try to work with the stricter notion with
     	   X + UX > Y - UY
     but it might cause trouble with some of our other implementations.

     E.g., 1.200 is compatible with 1.19999 and 1.201
     E.g., 1.234 is compatible with 1.235 and 1.233

   Author: Jihun and Chee Yap (Jan 2010)

   Since Core Library 2.0
   $Id: testCompatibleNew.cpp,v 1.3 2010/06/16 15:59:48 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

//////////////////////////////////////////////////
// test routine
//////////////////////////////////////////////////
void testIsCompatible(string str_1, string str_2) {
  bool compatibleNew = isCompatible(str_1,str_2);
  bool compatibleOld = isCompatibleEfficient(str_1,str_2);
//  cout << "Answer = " << compatibleNew << endl;
  if(compatibleNew == compatibleOld)
    cout << "CORRECT! " << endl << str_1 << " and " << endl << str_2;
  else
    cout << "ERROR! " << endl << str_1 << " and " << endl << str_2;
  cout << (compatibleNew ? " are compatible." : " are not compatible.") << endl; 
}


void testCompatible(string str_1, string str_2, int ans) {
  int position = compatible(str_1,str_2);
  cout << "Answer = " << position << endl;
  if(position == ans) {
    cout << "CORRECT! " << endl << str_1 << " and " << endl << str_2
	<< " are compatible to " << ans << " positions." << endl; 
  } else {
    cout << "ERROR! " << endl << str_1 << " and " << endl << str_2
	     << " are not compatible to " << ans << " positions." << endl; 
  }
  testIsCompatible(str_1, str_2);
  cout << "=======================================================" << endl;
}

//////////////////////////////////////////////////
// main
//////////////////////////////////////////////////
int main( int argc, char *argv[] ) {

// For user given test inputs:
  if (argc>1) {
	string stringIn, stringAns;
	int answer = 0; // set to 0 to avoid warning
    stringIn=argv[1];
    if (argc>2) stringAns=argv[2];
    if (argc>3) answer=atoi(argv[3]);
    testCompatible(stringIn, stringAns, answer);
	return 0;
  }

  cout << "==================" << endl;
  cout << "Test of compatible" << endl;
  cout << "==================" << endl;
  
  testCompatible("123.456",
                 "123.45555", 6);
  testCompatible("123.45555",
                 "123.456", 6);
  testCompatible("123.459999999",
                 "123.456000000", 5);
  testCompatible("123.459999999",
                 "123.46000000", 11);

  testCompatible("1",
                 "1", 1);
  testCompatible("1",
                 "2", 1);
  testCompatible("1",
                 "3", 0);
  testCompatible("0",
                 "1", -1);

  testCompatible("1.23400e-5",
                 "12.3399e-6", 6);

  testCompatible("1.23400e-5",
                 "1.23398e-5", 5);
  testCompatible("1.23400e-5",
                 "1.23397e-5", 5);

  testCompatible("+12",
                 "13", 2);
  testCompatible("+12",
                  "14", 1);
  testCompatible("+12",
                 "-12", 0);
  testCompatible("-12", 
                 "+15", 0);

  testCompatible("+00012.5",
                 "013", 2);
  
  testCompatible("0.0",
                 "0.12", -1);

  testCompatible("0.9999995000000416666652777778025793648037918892129", 
		 "0.99999950000004166666527777780257936480379188921289614586985", 49);

  testCompatible("0.9999995000000416666652777708025793648037918892129", 
	         "0.99999950000004166666527777780257936480379188921289614586985", 26);
  
  testCompatible("0.00", "123", -1);
  testCompatible("0", "123", -1);
}//main
