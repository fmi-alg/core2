/* ************************************************
   File: testMpfr.cpp

   Purpose:  To test the numerical output routines related to Mpfr.
	     These are considered very important functions
	     in our library, as the integrity of the
	     internal computation cannot be verified
	     unless we can exactly control the input and
	     output of numbers. 
   Usage:
        % testMpfr 			     -- do the default simple test
        % testMpfr 0			     -- do the default battery of test
        % testMpfr [prec] [expr] [digit] [string=""] -- do a single test

  // Q: WHAT DOES CORRECTNESS OF EVERY OUTPUT DIGIT MEAN?
  // A: We compute any relative approx x with some relative p bits of precision 
  //    Then printout any q digits of this approximation x;
  //    say x' is the printout.
  //
  //    PROPERTY 1: (No Misleading Digits)
  //    	First, suppose x' is in decimal notation.
  //    	Count the number of digits after the decimal point in x'.
  //    	Say number is d.  Note that d can be 0 (but we do not allow d<0).
  //    	Then we require |x - x'| <= 10^(-d)
  //
  //    	In case the output x' is in scientific notation,
  //    	with exponent of e (e.g., x'=1.234e-5, then d=3, e=-5)
  //    	Then we want 
  //    		|x - x'| <= 10^(e-d)
  //
  // 	PROPERTY 2: (Sufficient Accuracy)
  // 		Suppose x' has D digits starting from the most significant
  // 		digit (e.g., x'=123.456, then D=6, d=3).
  // 		An additional property must be fulfilled:
  //
  //    	If 10^q <= 2^p, then we want D=q
  //
  //    	(or D >= q, for a less strict interpretation of output)
  //
  //    REMARKS: 
  //    We use MPFR's bigfloat output routines which uses relative
  //    precision in its output digits.  E.g., if x=0.0012345, and
  //    you ask for 3 digits from MPFR, you get x'=0.00123, not 0.001.
  //
  //    We also use C++ streams' notion of "output precision", which
  //    also shows relative precision in digits: 
  //    E.g., 
  //	        cout << setprecision(6);
  //   		cout<<  -0.00123456;  //shows -0.00123456
  //            cout<<  -123456789.0; //shows -1.23457e+08
  //            cout<<  -12345600.0;  //shows -1.23456e+07
  //
  //    TO CHECK: shouldn't we use the isCompatible(s1,s2) routine of Core2
  //    in the following?
  //
  //    To Do (Bugs):
  //    -- SEE COMMENT ON TEST SIX below.  The correct rounding
  //    of elementary functions is not always correct.

   Author: Jihun and Chee (Oct 2006, July 2010)

   Since Core Library 2.0
   $Id: testMpfr.cpp,v 1.25 2010/07/09 15:20:20 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

// test(p, e, s="",d=0)
// 	will validate whether expression e, evaluated to p bits of
// 	relative precision, gives correct output of digits d. This correct
// 	output is s (but if s="", then we generate s ourselves. 
// 	If d=0 d is converted from p bits to digits).
//
void test(int prec, Expr exp, int digit=0, string ans=string(""));

// countDigits(s, &D, &d, &e)
// 	where s is input string (in decimal notation or scientific notation)
// and the output is
//   D, the number of digits after the most significant digit of s.
//   d, similar, but counts number of digits after the decimal point in s.
//   e, the exponent in scientific notation, but e=0 in decimal notation.
//   E.g., s="123.456",  D=6, d=3, e=0.
//         s="-1.23456e-4", D=6, d=5, e=-4.
//         s="-0.12345e04", D=5, d=5, e=4.
//
void countDigits(string s, int * D, int *d, int *e);

// main routine
//
int main( int argc, char *argv[] ) {
  
  //Default simple test:
  int prec = 200;		     // prec=200
  int digit = 60;		     // prec=200 bits becomes 60 digits
  Expr expr = sqrt(Expr(7));          // exp = sqrt(7)
  string str=			     // this is the answer expected (60 digits)
"2.64575131106459059050161575363926042571025918308245018036833";
  //Expr exp = "7/22";               // exp = rational approx to Pi

  if (argc>1) prec=atoi(argv[1]);
  if (argc>2) expr=argv[2];
  if (argc>3) digit=atoi(argv[3]);
  if (argc>4) str=argv[3];

  if (prec>0) {// do default simple test or one input expression
    switch(argc) {
      case 1:
      case 5:
        test(prec, expr, digit, str);
        break;
      case 2:
      case 3:
        test(prec, expr);
        break;
      case 4:
        test(prec, expr, digit);
        break;
    }
  } else {     // else, do a battery of standard tests

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST ONE:
      // illustrating C++ stream' output precision:
      cout << "====== TEST ONE: cout precision" << endl;
	cout << "-----1234 group" << endl;
      cout<< setprecision(6) << -0.01234 << endl;      //shows -0.01234
      cout<< setprecision(6) <<  0.001234 << endl;     //shows  0.001234
      cout<< setprecision(6) << -0.0001234 << endl;   //shows  -0.0001234
      cout<< setprecision(6) <<  0.00001234 << endl;   //shows  1.234e-05
      cout<< setprecision(6) <<  0.000001234 << endl;  //shows  1.234e-06
	//  THUS, when the value is less than 0.0001, it will switch to
	//  scientific notation of the form "x.xxx e-0nnn" where
	//  the negative exponent e-0nnn will have nnn equal to 5 or larger
	cout << "-----123456 group" << endl;
      cout<< setprecision(6) << -0.0123456 << endl;    //shows -0.0123456
      cout<< setprecision(6) <<  0.00123456 << endl;   //shows  0.00123456
      cout<< setprecision(6) << -0.000123456 << endl; //shows  -0.000123456
      cout<< setprecision(6) <<  0.0000123456 << endl; //shows  1.23457e-05
	//
	cout << "-----small 123456789 group" << endl;
      cout<< setprecision(6) << -0.123456789 << endl; //shows     -0.123457
      cout<< setprecision(6) <<  0.0123456789 << endl; //shows     0.0123457
      cout<< setprecision(6) << -0.00123456789 << endl; //shows   -0.00123457
      cout<< setprecision(6) <<  0.000123456789 << endl; //shows   0.000123457
      cout<< setprecision(6) << -0.0000123456789 << endl; //shows -1.23457e-05
	//
	cout << "-----large 123456789 group" << endl;
      cout<< setprecision(6) << 123456789   << endl; //shows   123456789
      cout<< setprecision(6) << 123456789.0 << endl; //shows   1.23457e+08
      cout<< setprecision(6) << -123456789.0 << endl; //shows -1.23457e+08
      cout<< setprecision(6) << -12345600.0 << endl; //shows  -1.23456e+07
      cout<< setprecision(6) << 12345600.0 << endl; //shows    1.23456e+07
	//
	cout << "-----large 123456789 integers" << endl;
      cout<< setprecision(6) << 1234567890   << endl; //shows   1234567890
      cout<< setprecision(6) << 2147300000   << endl; //shows   4223456789
    //cout<< setprecision(6) << 4223456789   << endl; //shows   4223456789
	//Compiler warning:
	//   2146300000: This is (empirically) near to the largest constant
	//          	 that does not get any compiler warning
	//   4223456789: get compiler warning that this decimal constant
	//   		 is only unsigned in ISO C90
	//   4323456789: would be too large as a constant.
	//
	// CONCLUSIONS:
	// --So the output for negated values only differs only in the "-" sign
	// --"Precision 6" means we get 6 digits starting from the
	//   most significant digit.
	// --If 6 significant digits is not enough, switch to
	//   scientific notation.  There are two case: if value is <1,
	//   then only switch when the exponent (which is negative) is 
	//   5 or less (i.e., instead of 0.  If value is >1, switch
	//   immediately (?)

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST TWO: rational expressions
      cout<< "====== TEST TWO: rational expressions" << endl;
      for (int i=10095; i<10100; i++)
	 test(i, Expr(i)/Expr(i+1));

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST THREE: squareroot expressions
      cout << "====== TEST THREE: sqrt expressions" << endl;
      for (int i=1095; i<1100; i++)
 	test(i, sqrt(Expr(i)+sqrt(Expr(i))));

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST FOUR: example from nested Sqrt (see "progs/nestedSqrt")
      // 	The expression is E = sqrt[k]{(exp(k,2)-1)} - 2
      // 		where sqrt[k]{.} means we take sqrt() k times
      // 		and exp(k,x) means we raise to 2^x k times
      // 	Hence, |E| is very small.  We use k=4, to compare to Core1.
      cout << "====== TEST FOUR: nested sqrt" << endl;
      Expr E=2;
      int k=4;  // can change if you want higher precision test
      for (int i=0;i<k;i++)
                E = E*E;
	//cout << "E = SQRE(2, " << k << ") = " << E << endl;
	//cout << "    where SQRE(2,k)= SQRE(2,k-1)**2 \n";
        E = E-1;
        for (int i=0;i<k;i++)
                E = sqrt(E);
	//cout << "e = SQRT(E-1, " << k << ") = " << E << endl;
	//cout << "    where SQRT(F,k)= sqrt(log(F,k-1)) \n";
        E = E-2;

	// This is testing against the output from Core1
	//    (200 bits=60 relative digits)
	test(200, E, 60, 
        "-.00000190736227536746916983256469775415813708674303147237637217066");

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST FIVE: trigonometric sine function
      //
      cout << "====== TEST FIVE: trigonometric sine" << endl;
      E = sin(Expr("1"));
      test(200, E, 60, "");
	  test(200, E, 60, 
        "0.841470984807896506652502321630298999622563060798371065672752");

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST SIX: trigonometric cosine function
      //
      // HACK: Jihun fixed a bug here -- since cosine is decreasing
      // in the testing range, we had to round down for upper bound,
      // and round up for lower bound!   But a systematic solution
      // is still not implemented... (July 2010)
      //
      cout << "====== TEST SIX: trigonometric cosine" << endl;
      E = cos(Expr("1"));
      test(200, E, 60, "");
//	  test(200, E, 60, 
 //       "0.540302305868139717400936607442976603732310420617922227670097");

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST SEVEN: trigonometric tangent function
      //
      cout << "====== TEST SEVEN: trigonometric tangent" << endl;
      E = tan(Expr("1"));
      test(200, E, 60, "");
	  test(200, E, 60, 
        "1.55740772465490223050697480745836017308725077238152003838395");
		
      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST EIGHT: trigonometric arcsine function
      //
      cout << "====== TEST EIGHT: trigonometric arcsine" << endl;
      E = asin(Expr("1"));
      test(200, E, 60, "");
	  test(200, E, 60, 
        "1.57079632679489661923132169163975144209858469968755291048747");
      E = asin(Expr("0.5"));
	  test(200, E, 60, 
		"0.523598775598298873077107230546583814032861566562517636829157");
       // "0.1523598775598298873077107230546583814032861566562517636829157");
		
      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST NINE: trigonometric arccosine function
      //
      cout << "====== TEST NINE: trigonometric arccosine" << endl;
      E = acos(Expr("0.5"));
      test(200, E, 60, "");
	  test(200, E, 60, 
        "1.04719755119659774615421446109316762806572313312503527365831");

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST TEN: trigonometric arctangent function
      //
      cout << "====== TEST TEN: trigonometric arctangent" << endl;
      E = atan(Expr("0.5"));
      test(200, E, 60, "");
	  test(200, E, 60, 
        "0.463647609000806116214256231461214402028537054286120263810933");
	  // The next argument for arctan is 2, in order to force arctan to do argument
	  // reduction (this argument reduction is done for x if |x|>1.
      E = atan(Expr("2"));
	  test(200, E, 60, "");
	  test(200, E, 60, 
        "1.10714871779409050301706546017853704007004764540143264667654");
	  //NOTE: 60 significant digits for arctan("2") really should be:
	  //"0.107148717794090503017065460178537040070047645401432646676539".
	  // BUT, in the above provided answer, we use only 59 significant digits!
	  // That is because Core Lib thinks that the "0.xxx" should be regarded
	  // as one significant digit.  It is as if 60 is the number of PRINTED digits,
	  // not the number of SIGNIFICANT digits.  Plan to fix this for future
	  // Core Library Release!!!
      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST ELEVEN: natural logarithm function
      //
      // This test seems to give error on MacOS platform but not on
      // Cygwin platform.... still debugging this.   Jun 2010 (Chee/Jihun)
      //
      cout << "====== TEST ELEVEN: natural logarithm " << endl;
      E = log(Expr("0.5"));
	  test(200, E, 60, "");
	  test(200, E, 60,
       "-0.69314718055994530941723212145817656807550013436025525412068");
      E = log(Expr("10"));
	  test(200, E, 60, "");
	  test(200, E, 60,
       "2.30258509299404568401799145468436420760110148862877297603333");

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST TWELVE: exponential function
      //
      cout << "====== TEST TWELVE: exponentiation " << endl;
      E = exp(Expr("0.5"));
	  test(200, E, 60, "");
	  test(200, E, 60,
       "1.64872127070012814684865078781416357165377610071014801157508");

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST THIRTEEN: constant Pi
      //
      cout << "====== TEST THIRTEEN: constant Pi " << endl;
      E = pi();
	  test(200, E, 60, "");
	  test(200, E, 60,
       "3.14159265358979323846264338327950288419716939937510582097494");
      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST FOURTEEN: constant E
      //
      cout << "====== TEST FOURTEEN: constant e " << endl;
      E = e();
	  test(200, E, 60, "");
	  test(200, E, 60,
      "2.71828182845904523536028747135266249775724709369995957496697");
      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST FIFTEEN: if g(x) is the inverse of f(x), we test f(g(x)) and g(f(x)).
      //
      cout << "====== TEST FIFTEEN: function and its inverse " << endl;
      cout << " NOT YET DONE " << endl;
	}//battery of tests

	return 0;

}//main

// testing routine
// 
void test(int prec, Expr exp, int digits, string ansstr){
  int errorFlag = 0;		  // 0=no error
  ostringstream oss;              // oss is the string we print into

  if (digits == 0)
    digits = bits2digits(prec);   // convert prec (in bits) into digits
  prec += 4;			  // to guarantee prec relative digits output,
                                  // 4 more bits is needed

  setDefaultOutputDigits(digits, oss);    // display precision
  setDefaultOutputDigits(digits);
  BigFloat bf = exp.approx(prec, CORE_INFTY);  // compute to p relative bits

  BigFloat2 bound = exp.rep()->appValue();

  // check if the relative precision is guaranteed
  if (abs((exp - bf) / exp) > pow(Expr(2), -prec)){
    cout << "ERROR!!! get_f is wrong" << endl;
    errorFlag++;
cout << "expr = " << exp << endl;
cout << "bf = " << bf << endl;
cout << "abs((expr -bf) / exp) = " << abs((exp-bf)/exp) << endl;
cout << "pow(2, -prec) = " << pow(Expr(2), -prec) << endl;
cout << "prec = " << prec << endl;
}

  if ((bf - bound.getLeft()) * (bf - bound.getRight()) > 0 ){
    cout << "ERROR!!! get_f is not contained in the boundary" << endl;
    errorFlag++;}

  if (abs((exp - bound.getLeft()) / exp) > pow(Expr(2), -prec)){
    cout << "ERROR!!! approx lower bound is out of range" << endl;
cout << "error because A>B where" << endl;
cout << "A: abs(exp - bound.getLeft()) / exp = " <<
 	(abs(exp - bound.getLeft()) / exp) << endl;
cout << "B: pow(Expr(2), -prec) = " << (pow(Expr(2), -prec)) << endl;
    errorFlag++;}
  if (abs((exp - bound.getRight()) / exp) > pow(Expr(2), -prec)){
    cout << "ERROR!!! approx upper bound is out of range" << endl;
    errorFlag++;}
  if (abs((bound.getLeft() - bound.getRight()) / exp)
	> pow(Expr(2), -prec)) {
    cout << "ERROR!!! approx upper bd - lower bd is out of range" << endl;
cout << "error because A>B where" << endl;
cout << "where A = abs[(bound.getLeft() - bound.getRight()) / exp] = " <<
  abs((bound.getLeft() - bound.getRight()) / exp) << endl;
cout << " and  B = pow(Expr(2), -prec) = " << (pow(Expr(2), -prec)) << endl;
    errorFlag++;}
  // check if upper and lower bounds actually contain the exact value
  if ((exp - bound.getLeft()) * (exp - bound.getRight()) > 0 ) {
    cout << "ERROR!!! bounds do not contaion an exact value" << endl;
    errorFlag++;}

  oss.str("");
  oss << exp;                           // print into oss

  string str = oss.str();
  if (ansstr != ""){ // in case the correct answer is provided:
    if (ansstr != str){
        errorFlag++;
    	cout << "ERROR!!! Core output string not equal to provided answer" << endl;
	core_error("ERROR!!! Core output string not equal to answer string",
			__FILE__, __LINE__, true);
    } else
    	cout << "CORRECT!!! Core output string equals answer string" <<endl;
  } else {
  int D=0; 			// D=number of significant digits
  int d=0; 			// d=number of digits beyond decimal point
  int e=0;			// value of exponent in scientific notation
  countDigits(str, &D, &d, &e);
  
  Expr diff = abs(exp - Expr(str, 10, CORE_INFTY));   // check the difference

  if (diff > pow(Expr(10), e-d)) { // PROPERTY 1 says relative error,
				   //     diff <= 10^{e-d}
    cout << "ERROR!!! Core output string is not correct" << endl;
    errorFlag++;
    core_error("ERROR!!! Core output string wrong", __FILE__, __LINE__, true);
  } else
    cout << "CORRECT!!! Core output string is correct" << endl;
  }
  if (errorFlag>0){
    cout << "printout string:"<< str << endl;
    cout << "original string:"<< ansstr << endl;
  }
}//test

// Analyzes number string:
void countDigits(string str, int * D, int *d, int *e) {

  *D=0; 			// D=number of significant digits
  *d=0; 			// d=number of digits beyond decimal point
  *e=0;				// value of exponent in scientific notation
  bool dot=false;		// if decimal point is found  
  int j=0;			// j= position of most significant digit

  if ((str[0] == '+')|| (str[0] == '-')) j++;  // Takes care of sign
  if (str[j] == 0) j++;         // Takes care of 0.XXX (but not 00.123)
  				// CAVEAT: assuming there is at most one 0.

  for (size_t i = j; i < str.size(); i++) {// this loop counts D=number
    if (str[i] == 'e' || str[i] == 'E') {  //   of printout digits
      *e=atoi(str.substr(i+1,str.size()-i).c_str());
      break; } 
    if (str[i] == '.') dot=true;	   // found dot
  
    if (str[i] >= '0' && str[i] <= '9') {
      D++;
      if (dot) d++;
    }// else error!
  }//for
}
