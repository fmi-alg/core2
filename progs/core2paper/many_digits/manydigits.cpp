/* ***************************************************
 * file: manydigits.cpp
 *    Problems from the Many Digits Competition (2007).
 *    This program must be run at CORE_LEVEL 3:
 *
 *    Usage:
 *
 *    	> ./manydigits [digits=30] [print=1] [moretest=1]
 *
 *    	where all arguments have the indicated default values.
 *
 *    	  digits : this is the relative number of digits to be computed
 *    	  print : if print>0, then show answer 
 *    	  moretest : if moretest>0, do self-validation
 *
 *    Jihun/Chee
 *    Since Core 2.0
 *    $Id: manydigits.cpp,v 1.12 2010/06/25 17:19:52 exact Exp $
 *************************************************** */

#define CORE_LEVEL 3

#include "CORE.h"
#include <iostream>

using namespace std;

// testRelativeApprox(n, expr) 
//   is a self-validating routine, returning true iff the
//   self-validation succeeds.
//
//   This test is called only if moretest>0.
//
//   It approximates expr to n, 2n and 3n relative digits
//   	of accuracy,  then verifies that these digits
//   	are compatible with each other.
//   This check is done with Core's builtin "compatible" routine.
//
bool testRelativeApprox(int digits, Expr& expr);

// Main
int main( int argc, char *argv[] ) {

  string answerMaple_C1 = "0.5645109";

  int digits = 30;	// number of relative digits to approximate
  int print = 1;	// if print>0, show answer
  int moretest = 1;	// if moretest>0, 

  if (argc > 1)
    digits = atoi(argv[1]);
  if (argc > 2)
    print = atoi(argv[2]);
  if (argc > 3)
    moretest = atoi(argv[3]);
 
  prec_t prec;		// relative bits
  prec = digits2bits(digits);
  setDefaultRelPrecision(prec); 
  setDefaultOutputDigits(digits);

  cout << endl
       << "Test of CORE library for" << endl
       << "some problems of \"many digits\" friendly competition"
       << endl << endl;

  // C01
  Timer2 timer;
  timer.start();
  Expr C1 = sin(tan(cos(Expr("1"))));
  C1.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C01 : ";
  cout << "compute sin(tan(cos(1))) to " << digits << " digits." << endl;
  if (print > 0) {
    cout << "ANSWER = " << C1 << endl;
    string answerCORE_C1 = C1.toString();
    cout << "Answer is compatible with Maple's up to "
       << compatible(answerCORE_C1,answerMaple_C1) << " digits." << endl;
  }
  if (moretest > 0 && testRelativeApprox(digits,C1) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C02 
  timer.start();
  Expr C2 = sqrt(e() / pi());
  C2.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C02 : ";
  cout << "compute sqrt(e()/pi()) to " << digits << " digits." << endl;
  if (print > 0)
    cout << "ANSWER = " << C2 << endl;
  if (moretest > 0 && testRelativeApprox(digits,C2) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C03 
  timer.start();
  Expr C3 = sin(pow((e()+1), 3));
  C3.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C03 : ";
  cout << "compute sin((e+1)^3) to " << digits << " digits." << endl;
  if (print > 0)
    cout << "ANSWER = " << C3 << endl;
  if (moretest > 0 && testRelativeApprox(digits,C3) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
 
  // C04 
  timer.start();
  Expr C4 = exp(pi() * sqrt("2011"));
  C4.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C04 : ";
  cout << "compute exp(pi*sqrt(2011)) to " << digits << " digits." << endl;
  if (print > 0)
    cout << "ANSWER = " << C4 << endl;
  if (moretest > 0 && testRelativeApprox(digits,C4) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
  
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C05 
  timer.start();
  Expr C5 = exp(exp(exp(Expr("1/2"))));
  C5.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C05 : ";
  cout << "compute exp(exp(exp(1/2))) to " << digits << " digits." << endl;
  if (print > 0) {
    cout << "ANSWER = " << C5 << endl;
  }
  if (moretest > 0 && testRelativeApprox(digits,C5) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
   cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

/*
  // C06 
  timer.start();
  //Expr ONE = CORE::Expr::OneConst;  // use static member for "1"
  Expr ONE("1");

//TEST:
  Expr test= CORE::Expr::ZeroConst;
  cout << "ZERO = " << test << endl;
return 0;

  Expr C6 = atanh(ONE - atanh(ONE - atanh(ONE - atanh(ONE/pi()))));
  C6.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C06 : ";
  cout << "compute atanh(1 - atanh(1 - atanh(1 - atanh(1/Pi)))) to "
		<< digits << "digits." << endl;
  if (print > 0) {
    cout << "ANSWER = " << C6 << endl;
  }
  if (moretest > 0 && testRelativeApprox(digits,C6) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
   cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
*/

  // C07
  timer.start();
  Expr C7 = pow(pi(), 1000);
  C7.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "============================================================" << endl;
  cout << "Problem C07 : ";
  cout << "compute pi^1000 to " << digits << " digits." << endl;
  if (print > 0)
    cout << "ANSWER = " << C7 << endl;
  if (moretest > 0 && testRelativeApprox(digits,C7) == false)
      cout << "ERROR!!! outputs are inconsistant." << endl; 
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
/* */
  // C08
  timer.start();
// overloading error with "power(6,6)" ... Chee
  Expr C8 = sin(Expr(power(BigInt(6),power(BigInt(6),BigInt(6)))));
  C8.approx(prec, CORE_INFTY);
  timer.stop();
  cout << "============================================================" << endl;
  cout << "Problem C08 : ";

  cout << "sin(6^(6^6)) to " << digits << " digits." << endl;
  if (print > 0)
    cout << "ANSWER = " << C8 << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C10
  timer.start();
  Expr C10 = root(7+root(2,5)-5*root(8,5),3)+root(4,5)-root(2,5);
  C10.approx(prec, CORE_INFTY);
  timer.stop();
  cout << "============================================================" << endl;
  cout << "Problem C10 : ";
  cout << "compute (7+2^(1/5)-5*8^(1/5))^(1/3)+4^(1/5)-2^(1/5) to " << digits << " digits." << endl;
  if (print > 0) {
    cout << "ANSWER = " << C10 << endl;
    if(C10 == 1)
      cout << "CORRECT!!! The answer is equal to 1" << endl;
    else
      cout << "ERROR!!! The answer is not equal to 1" << endl;
  }

  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
  cout << "============================================================" << endl;


  /*
  // basic test for trigonometric formulas
  // disable filter feature : trigonometric filters functions are not implemented yet.
  set_escape_bound(1000);
  
  cout << "test for sinx^2 + cosx^2 = 1" << endl;
  timer.start();
  Expr x = pi() / 2;
  Expr expr1 = sin(x);// * sin(x);// + cos(x) * cos(x);
  //expr1.approx(prec, CORE_INFTY);
  if (expr1 == Expr("1"))
    cout << "CORRECT!!! sinx^2 + cosx^2 = 1" << endl;
  else {
    cout << "ERROR!!! sinx^2 + cosx^2 != 1" << endl;
    cout << "sinx^2 + cosx^2 - 1 = " << expr1 - 1<< endl;
  }
  timer.stop();
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
  */

  return 0;
}//main

// testRelativeApprox 
//   performs a self-test of Core's approximation
//   	using Core's "compatible" routine.
//   It returns true if the self-test succeeds;
//   	returns false otherwise.
bool testRelativeApprox(int digits, Expr& expr) {
  std::ostringstream oss1, oss2, oss3;

  prec_t prec;		// relative bits of precision
  prec = digits2bits(digits);  // convert digits to bits

  expr.approx(prec, CORE_INFTY);
  setDefaultOutputDigits(digits, oss1);
  oss1 << expr;

  expr.approx(prec*2, CORE_INFTY);
  setDefaultOutputDigits(digits*2, oss2);
  oss2 << expr;

  expr.approx(prec*4, CORE_INFTY);
  setDefaultOutputDigits(digits*4, oss3);
  oss3 << expr;

  if(compatible(oss1.str(),oss2.str()) < digits-1 ||
     compatible(oss2.str(),oss3.str()) < digits*2 -1) {
    cout << compatible(oss1.str(),oss2.str()) << endl;
    cout << compatible(oss2.str(),oss3.str()) << endl;
    cout << oss1.str() << endl
         << oss2.str() << endl
         << oss3.str() << endl;
    return false;
  }
      
  return true;     
}//testRelativeApprox
