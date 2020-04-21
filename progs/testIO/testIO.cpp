/* ************************************************
   File: testIO.cpp

   Purpose:  To test the numerical output routines.
	     These are considered very important functions
	     in our library, as the integrity of the
	     internal computation cannot be verified
	     unless we can exactly control the input and
	     output of numbers. 
   Usage:
        % testIO

   To Do: Make these routines "self-validating".

   Author: Chee Yap (yap@cs.nyu.edu)

   Since Core Library 1.4
   $Id: testIO.cpp,v 1.7 2010/09/14 20:03:50 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

int main( int argc, char *argv[] ) {

//  CORE_init(8);	// This method is recommended in all CORE programs.
		// The parameter 8 sets the std::cout precision to 8.
  setDefaultOutputDigits(8);

  double one = 1.0;
  double three = 3.0;
  double one_third =  one / three;

  cout << "OUTPUT FOR THE Expr VALUE 1.0/3.0\n";
  cout << "  computed to relative precision 120: \n" ;
  setDefaultPrecision(120, CORE_posInfty);

  cout << "  (1) Output In Positional Notation\n";
  cout << "    Default cout precision (=8): " << one_third << endl;
  cout << "    Cout precision 12:      " << setprecision(12) << one_third << endl;
  cout << "    Cout precision 36:      " << setprecision(36) << one_third << endl;
  cout << "    Cout precision 37:      " << setprecision(37) << one_third << endl;
  cout << "    Cout precision 38:      " << setprecision(38) << one_third << endl;
  cout << "    Cout precision 39:      " << setprecision(39) << one_third << endl;
  cout << "    Cout precision 40:      " << setprecision(40) << one_third << endl;
  cout << "    Cout precision 41:      " << setprecision(41) << one_third << endl;
  cout << "    Cout precision 42:      " << setprecision(42) << one_third << endl;
  cout << "    Cout precision 43:      " << setprecision(43) << one_third << endl;
  cout << "    Cout precision 44:      " << setprecision(44) << one_third << endl;
  cout << "    Cout precision 46:      " << setprecision(46) << one_third << endl;
  cout << "    Cout precision 48:      " << setprecision(48) << one_third << endl;
  cout << "  N.B. 120 bits = 36.123 digits.  Above, we have asked to see \n";
  cout << "  more and more bits of the computed value.  The output will indeed \n";
  cout << "  show more and more bits, until all significant bits are used up.\n";
  cout << "  This should happen AFTER 37 digits but hopefully before 48 digits\n";

  cout << "  (2) Output In Scientific Notation\n";
  // presently scientific manipulator is not supported by g++
  	cout << setprecision(10);
  	cout.setf(ios::scientific, ios::floatfield);
  cout << "    Default Cout precision:" << one_third << endl;
  cout << "    Cout precision 12:  " << setprecision(12) << one_third << endl;
  cout << "    Cout precision 24:  " << setprecision(24) << one_third << endl;
  cout << "    Cout precision 25:  " << setprecision(25) << one_third << endl;
  cout << "    Cout precision 36:  " << setprecision(36) << one_third << endl;
  cout << "    Cout precision 48:  " << setprecision(48) << one_third << endl;

  cout.setf(ios::fixed, ios::floatfield);
  cout << setprecision(10);

  cout << "  (3) Default Print Digits (a Core library parameter)\n";
  cout.precision(6);
  cout << "Output precision: " << cout.precision() 
       << ", precision [r=67, a=INFTY]" << endl;
  cout << "    one_third = " << one_third << endl << endl; 

  cout.precision(20);
  cout << "Output precision: " << cout.precision() 
       << ", precision [r=67, a=INFTY]" << endl;
  cout << "    one_third = " << one_third << endl << endl;

  cout.precision(30);
  cout << "The precision is not sufficient for 30 decimal digits!" << endl;
  cout << "Output precision: " << cout.precision() 
       << ", precision [r=67, a=INFTY]" << endl;
  cout << "one_third = " << one_third  << endl << endl;

  cout << "Set the precision to [r=100, a=INFTY] to get it right: " << endl;
  setDefaultPrecision(100, CORE_posInfty);
  cout << "Output precision: " << cout.precision() 
       << ", precision [r=100, a=INFTY]" << endl;
  cout << "one_third = " << one_third << endl << endl; 

  // test rounding and etc...
  setDefaultPrecision(54, CORE_posInfty);
  cout.precision(16);
  cout << "Precisions: [54, INFTY] (simulating machine double precision) \n"
       << "           This print out 16 digits..." << endl;
  cout.precision(10);
  double e0 = 0.01234;
  double e1 = 0.00000000012345;
  double e2 = 0.0000000001234567890123;
  double e3 = 1.2;
  double e4 = 123.456;
  double e5 = 123456789;
  double e6 = 123456789.0123456;
// debugging:
  double e7 = BigFloat("1234567890123.456");
  double e8 = 1234567890123456.0;
  double e9 = 0.023;
  double e10 = 123456789.50001;
  double e11 = 123456789.49999;
  double e12 = 199999999.50001;
  double e13 = 999999999.50001;
    
  double ne0 = -.01234;
  double ne1 = -0.00000000012345;
  double ne2 = -0.0000000001234567890123;
  double ne3 = -1.2;
  double ne4 = -123.456;
  double ne5 = -123456789;
  double ne6 = -123456789.0123456;
  double ne7 = -1234567890123.456;
  double ne8 = -1234567890123456.0;       
  double ne9 = -0.023;

  cout << " e0 ( 0.01234                   ) : " << e0 << endl;
  cout << " e1 ( 0.00000000012345          ) : " << e1 << endl;
  cout << " e2 ( 0.0000000001234567890123  ) : " << e2 << endl;
  cout << " e3 ( 1.2                       ) : " << e3 << endl;
  cout << " e4 ( 123.456                   ) : " << e4 << endl;
  cout << " e5 ( 123456789                 ) : " << e5 << endl;
  cout << " e6 ( 123456789.0123456         ) : " << e6 << endl;
  cout << " e7 ( 1234567890123.456         ) : " << e7 << endl;
  cout << " e8 ( 1234567890123456.0        ) : " << e8 << endl;
  cout << " e9 ( 0.023                     ) : " << e9 << endl;
#if CORE_LEVEL == 1
  cout << " e9 ( 0.023                     ) : " << setprecision(30) <<e9 << endl;
#endif
  cout << " e10 ( 123456789.50001          ) : " << e10 << endl;
  cout << " e11 ( 123456789.49999          ) : " << e11 << endl;
  cout << " e12 ( 199999999.50001          ) : " << e12 << endl;
  cout << " e13 ( 999999999.50001          ) : " << e13 << endl;
  cout << " ne0 ( -.01234                  ) : " << ne0 << endl;
  cout << " ne1 ( -0.00000000012345        ) : " << ne1 << endl;
  cout << " ne2 ( -0.0000000001234567890123) : " << ne2 << endl;
  cout << " ne3 ( -1.2                     ) : " << ne3 << endl;
  cout << " ne4 ( -123.456                 ) : " << ne4 << endl;
  cout << " ne5 ( -123456789               ) : " << ne5 << endl;
  cout << " ne6 ( -123456789.0123456       ) : " << ne6 << endl;
  cout << " ne7 ( -1234567890123.456       ) : " << ne7 << endl;
  cout << " ne8 ( -1234567890123456.0      ) : " << ne8 << endl;
  cout << " ne9 ( -0.023                   ) : " << ne9 << endl;
  cout << endl;

#if CORE_LEVEL == 3
/*
  // test Reals
  cout << "test reals" << endl;
  BigRat R(1, 3);
  Real R1("1/3");
  //cout << "error before here?" << endl;//BigRat R2("1/3");
  BigFloat B(R);
  cout.precision(8);
  Real Q = R;
  Real X = B;

  cout << "BigRat Real Q(1/3): " << Q << endl;
  cout << "Real R1(\"1/3\"): " << R1 << endl;
  cout << "BigFloat Real X(1/3): " << X << endl;
*/
  // test square root
  cout.precision(10);
  setDefaultPrecision(35, CORE_posInfty); // bug?
  double two_over_three = BigRat(2, 3);
  cout << "sqrt(2/3) = " << sqrt(two_over_three) << endl << endl;;
  
  // show the differnce between the use of string and double constant.
  cout.precision(20);
  setDefaultPrecision(67, CORE_posInfty);

  defInputDigits = 20;
  double exact1 = BigFloat("0.023");
  double inexact1 = 0.023; // this can not be represented in banary exactly.

  double exact2 = BigFloat("1234.567e-5");
  double inexact2 = 1234.567e-5;

  cout << "The literal double constant is not always exact:" << endl; 
  cout << "construct from double constant - (0.023): " << inexact1 << endl;
  cout << "construct from string - (0.023): " << exact1 << endl;
  cout << "construct from double constant - (1234.567e-5): " << inexact2 << endl;
  cout << "construct from string - (1234.567e-5): " << exact2 << endl;

  // =============================
  BigRat RR(100010, 100010);
  cout << "100010/100010 = " << RR << endl;
  // =============================

  cout.precision(20);
  setDefaultPrecision(67, CORE_posInfty);
  double r1 = "0.123456789";
  cout << "r1 = " << r1 << endl;
  double r2("0.1234567890123456789", 10, CORE_posInfty);
  cout << "r2 = " << setprecision(24) << r2 << ", read in exactly" << endl;
  double r3("0.1234567890123456789", 10, 9);
  cout << "r3 = " << setprecision(24) << r3 << ", read in with precision 10^{-9}" << endl;
#ifdef INTERACTIVE
  double r4;
  setDefaultInputDigitsision(5);
  cout << "r4 = ";
  cin >> r4;
  cout << "r4 = " << r4 << ", with an error less than 10^{-5}" << endl;
  double r5;
  setDefaultInputDigits(CORE_posInfty);
  cout << "r5 = ";
  cin >> r5;
  cout << "r5 = " << r5 << ", without any error in reading-in" << endl;
#endif // INTERACTIVE

double q = BigRat(1, 3);
setDefaultAbsPrecision(67); // about 20 digits
cout << "q = " << setprecision(10) << q << ", in 10 digits" << endl;
cout.precision(30); // default to output 30 digits.
cout.setf(ios::scientific, ios::floatfield); // use scientific notation
cout << "q = " << q << ", in scientific notation." << endl;
// cout.setf(0, ios::floatfield); // reset the format to default.
setPositionalFormat();
cout << "q = " << q << ", in positional notation." << endl;

// scientific format

  double s1 = 1.234e-1;
  cout << "s1 = 1.234e-1; in machine precision, s1 =" << s1 << endl;

  defInputDigits = 20;
  double s2 = "1.234e-1";
  cout << "s2 = \"1.234e-1\"; with defInputDigits = 20, , s2 =" << s2 << endl;

// rational format

  double a1 = 123/456;
  cout << "a1 = 123/456; in machine precision, a1 =" << a1 << endl;

  defInputDigits = 20;
  double a2 = "123/456";
  cout << "a2 = \"123/456\"; with defInputDigits = 20, a2 = " << a2 << endl;

  defInputDigits = CORE_INFTY;
  double a3 = "123/456";
  cout << "a3 = \"123/456\"; with defInputDigits = INFTY, a3 = " << a3 << endl;

// Mar 9, 2009 (jihun/chee/willi mann): 
  defInputDigits = CORE_INFTY;
  double b1 = "0.009";
  cout << "b1 = \"0.009\"; with defInputDigits = INFTY, b1 = " << b1 << endl;
  cout << "   NOTE: b1 is to test that zeros after the decimal point and before the MSB is correctly treated\n";
  double b2 = "001.234";
  cout << "b2 = \"001.234\"; with defInputDigits = INFTY, b2 = " << b2 << endl;
  cout << "   NOTE: b2 is to test that zeros before the decimal point AND before the MSB is correctly treated\n";
  double b3 = "010.0234";
  cout << "b3 = \"010.0234\"; with defInputDigits = INFTY, b3 = " << b3 << endl;
  cout << "   NOTE: b3 is to test that zeros between the MSB and after the decimal point is correctly treated\n";
  double b4 = "-010.0234";
  cout << "b4 = \"-010.0234\"; with defInputDigits = INFTY, b4 = " << b4 << endl;
  cout << "   NOTE: b4 is to test negative signs is correctly treated\n";

#endif //LEVEL 3

  // Tests for output/input streams
  // Moving a bigfloat value into a output stream
  // and move a stream back to a bigfloat value
  // They should be equal
  BigFloat tmp1(13), tmp2(0);
  
  std::ostringstream os;
  os << tmp1;
  std::istringstream is(os.str());
  is >> tmp2;
  assert (tmp1 == tmp2);
  

  return 0;

}//main
