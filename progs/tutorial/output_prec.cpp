/* ************************************************
   File: output_prec.cpp

   Purpose:  
   
   	Suppose a user says "I want to see 100 absolute (or relative)
	digits in all my output".  What should he do?

	This is hardly obvious.  There are the 4 steps:

	(1) Convert digits to bits (i.e., how many bits is 100 digits?)
	(2) Set the default input precisions
	(3) Set the default (computing) precisions
	(4) Set the default output precisions

	This is non-trivial if you want absolute precision.
		
	(1) We can get this roughly (see "bits2digits" function below)
	(2) We probably should set the input precision to "infty"
	(3) This should be OK
	(4) Using setprecision(N) basically gives N relative digits.
		So if you want absolute precision, this is tough.

	Hence, we provide here a function "guaranteedOutput(N)"
	which ensures all the printed output is correct to N digits.

   Usage:
        % IO_prec

   Author: 
   	    Chee Yap

   Date: May 2004.

   Since Core Library Version 1.7
   $Id: output_prec.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */        

#define CORE_LEVEL 4
#include "CORE.h"

using namespace std;

void headline(string m) {
  cout << " =========================================================\n";
  cout << m  << endl;
  cout << " =========================================================" << endl;
}

// bit2digits( eps )
// 	This function is useful for converting number of bits of
// 	precision into the corresponding number of digits of precision.
// 	Caveat: This function is approximate!  Up to +/- 1 ?
int bits2digits(int eps = 54) {
  return (int) (eps * log(2.0)/log(10.0));
}

int main(int argc, char **argv)
{

  headline("How to Set 100 Digits of Output Precision");

  // Translates eps (in bits) to outputPrec (in digits)
  cout << "54 bits of precision gives " 
	  << bits2digits(54) << " digits" << endl;
  cout << "100 bits of precision gives " 
	  << bits2digits(100) << " digits" << endl;
  cout << "1000 bits of precision gives " 
	  << bits2digits(1000) << " digits" << endl;
  

/*
  message(" SETPRECISION(6)");

  for (int i = 0; i<9 ; i++) {

  	cout << "  => BigFloat exp2(-" << i << ") : " 
		<< setprecision(6)
  		<< BigFloat::exp2(-i) << endl;
  }

  message(" EXACT BigFloat exp2(-8) ");
  BigFloat bf = BigFloat::exp2(-8);

  for (int i = 0; i<13 ; i++) {

  	cout << "  => setprecision(" << i << ") : "  << setprecision(i)
  		<< bf << endl;
  }

  message(" IN-EXACT BigFloat exp2(-8) ");
  bf.makeInexact();

  for (int i = 0; i<13 ; i++) {

  	cout << "  => setprecision(" << i << ") : "  << setprecision(i)
  		<< bf << endl;
  }

  message(" More tests at SETPRECISION(15), Computed at Relation Prec(6)");
  setDefaultAbsPrecision(CORE_INFTY);
  setDefaultRelPrecision(6);
  	cout << setprecision(15);
  	Expr s = sqrt(Expr(2));
  	cout << " s = sqrt(Expr(2))    : " << s << endl;
	BigFloat sb = s.BigFloatValue();
  	cout << " sb = s.getBigFloat() : " << sb << endl;
	cout << " ==>  Next you will see some junk digits: " << endl;
	sb.makeExact();
  	cout << " eb = sb.makeExact()  : " << sb << endl;
  	cout << " eb/2                 : " << sb/2 << endl;
  	cout << " eb.div2()            : " << sb.div2() << endl;

	cout << " ==>  Next, ib/2 is not exact in some sense \n"
		<< "         but ib.div2() is exact in what sense?: " << endl;
	sb.makeInexact();
  	cout << " ib = eb.makeInexact(): " << sb << endl;
  	cout << " ib/2                 : " << sb/2 << endl;
  	cout << " ib.div2()            : " << sb.div2() << endl;
*/
  return 0;
}
