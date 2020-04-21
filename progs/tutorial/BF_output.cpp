/* ************************************************
   File: BF_output.cpp
   NOTE: This file is no longer supported in Core2

   Purpose:  To test BigFloat output behavior

   		-- when the BigFloat is exact
		-- when the BigFloat is inexact
		-- under various cout precision settings

	    Notes:  
	    	BigFloat::exp2(n) produces an exact BigFloat with
		value exp(n).  E.g. expr2(-3) = 0.125.

		If you have an inexact BigFloat b, you can also
		make it exact by calling b.makeExact();

		If b were exact, you make it inexact by calling
		b.makeInexact(), which adds one error bit to the number
		(this bit is multiplied by B^exp where B = 2^14).

		If b is an exact BigFloat, you can call b.div2()
		to divide by 2 and get an exact result.  Note that
		b/2 may not be exact (this should probably be fixed).

   Usage:
        % BF_output 

   Author: 
   	    Chee Yap

   Date: May 2003.

   Since Core Library Version 1.6
   $Id: BF_output.cpp,v 1.2 2010/05/18 10:34:12 exact Exp $
 ************************************************ */        

#define CORE_LEVEL 4
#include "CORE.h"

using namespace std;

void message(string m) {
  cout << " =========================================================\n";
  cout << m  << endl;
  cout << " =========================================================" << endl;
}


int main(int argc, char **argv)
{


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

  return 0;
}
