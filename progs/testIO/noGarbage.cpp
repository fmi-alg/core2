/* ************************************************
   File: noGarbage.cpp

   Purpose:  We illustrate the property of Core2 to never print garbage digits.  

   Usage:
        % noGarbage
        % noGarbage m    -- test an integer m  (compute sqrt(m))
        % noGarbage m n  -- test a rational number m/n  (compute sqrt(m/n))

   Theory:
	Internal approximations have the correct
	number of bits (that is a given in all arbitrary precision packages).
	But it does not mean the printout will not
	show garbage digits.  The reason is the difference between
	internal representation in binary, and output in decimals.
	For instance, suppose the internal value in binary is
			x=(0.1111).  
	I will put a binary representation within
	a parenthesis, to distinguish it from a decimal representation.

	You can print x exactly:
		 x = 15/16 = 0.9375.
	Are the printed digits "correct" ?  In some sense, yes.
	BUT suppose we view x as an approximation, where the last bit is
	uncertain.   So x is really
		x = (0.1111) +/- (0.0001)
	so x lies in the range
		[ (0.111), (1.000) ] 
	which is equal to [0.875, 1.0] in decimal.
	So ONLY the first digit of x is reliable, and should be printed as
		x = 0.9
	as this tells us that the real value of x lies in the range
	x= 0.9 +/- 0.1.  This is correct: x lies in the range [0.8,1.0].

	
   TODO:
	Make this self-validating.  
	Do this for all number types
		(Expr, BigRat, BigFloat, BigFloat2)
	Use the routine from "testCompatible".

   Author: Chee Yap, June 2010.

   Since Core Library 2.0
   $Id: noGarbage.cpp,v 1.2 2010/07/12 09:33:16 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
	#define CORE_LEVEL 4
#endif

#include "CORE/CORE.h"

using namespace std;

int main(int argc, char** argv){

  int m=2;	// default number to test is m/n
  int n=1;	// 
 
  switch (argc) {// argument processing
    case 3:
	n = atoi(argv[2]); 
    case 2:
  	m = atoi(argv[1]);
	break;
    case 1:
	cout << "Using default arguments: x = m/n = 2" << endl;
  }

  Expr x = Expr(BigRat(m,n));
  cout<<"This illustrates the property that Core never print garbage digits"<<endl;
  cout << "Number to be tested is x = " << m << "/" << n << " = " << x << endl;
  cout << "First we set the output precision to 60 digits" << endl;
  cout << "   NOTE: 60 digits is equal to 199.31 bits!" << endl;
  cout << setprecision(60);

  int prec[] = {2, 10, 100, 200, 400, 800};

  for (int i = 0; i<6; i++) {
    cout << "Approximate sqrt(x) to " << prec[i] << " bits." << endl;
    cout << "Output: " << sqrt(x).approx(prec[i]) << endl;
  }
  cout << "NOTE: You should see that no garbage digits is printed.\n"
       << "      No extra digits is printed even when we approximate beyond\n"
       << "      200 bits of accuracy.  If you want to see more, you need\n"
       << "      to set to higher output precision.  THUS, output depends on\n"
       << "      two parameters: known approximation and output precision." 
       << endl;
  cout << "Suppose we specify the approximation precision in terms of digits.\n"
       << "      But keeping the output precision to 60 digits, you will\n"
       << "      see no extra digits printed beyond 60." << endl;
  for (int i = 0; i<6; i++) {
    cout << "Approximate sqrt(x) to " << prec[i] << " digits."<<endl;
    cout << "Output: " << sqrt(x).approx(digits2bits(prec[i])) << endl;
  }
  cout << "Now repeat the experiment, but with output digits set to 300:"<<endl;
  cout << setprecision(300);
  for (int i = 0; i<6; i++) {
    cout << "Approximate sqrt(x) to " << prec[i] << " digits."<<endl;
    cout << "Output: " << sqrt(x).approx(digits2bits(prec[i])) << endl;
  }

  return 0;
}
//====================================== 

