/* **************************************
   File: wbigfloat.cpp

   Purpose: This is a simple example using weak bigfloat number type
	to control output precision of four aritmetic operations
        - addition, subtraction, multiplication, division

        set_wbf_mode(bool b) turns on and off the weak bigfloat mode.
        set_wbf_prec(prec p) specifies the desired output precision.

        Example:

        set_wbf_prec(100);
        A = B*C;

        In weak bigfloat mode, the precision of A will be set to 100, such that
        the accuracy of multiplication is guaranteed to 100-bit relative precision.
	This is the standard IEEE model of accuracy.

   TODO:
   1.  There is some ambiguity about input.  Below, we have:

  		BigFloat x(1.23456789), y(2.98765432);

	The question is: what the internal value of x and y (which
	must be rounded from decimal to binary)?  This has to be
	checked...
   2.  If the user gives you the precision and the arguments to test,
	we need to use Expr to generate the correct answer to test.
   3.  We should test the full gamut of Core 2 operators, including
	transcendentals.

   Since CORE Library Version 2.0
   $Id: wbigfloat.cpp,v 1.4 2010/05/14 10:01:24 exact Exp $
************************************** */
#define CORE_LEVEL 2

#include "CORE/CORE.h"

using namespace std;

enum {
	ADD,
	SUB,
	MUL,
	DIV,
	COUNT
};

char* opStr[COUNT] = { "+",
	               "-",
		       "*",
		       "/",
};

//Jihun(Oct, 2007)
//compute x op y accurately up to relative precision prec.
//compare the value with the exact answer of string type.
bool check(BigFloat& x, BigFloat& y, int op, prec_t prec, char* ans) {
  set_wbf_mode(true);
  set_wbf_prec(prec);

  BigFloat r;
	
  switch(op) {
    case ADD:
        r = x + y;	    
      break;
    case SUB:
        r = x - y;	    
      break;
    case MUL:
        r = x * y;	    
      break;
    case DIV:
        r = x / y;	    
      break;
  }

  if (r.get_str(prec) != ans) {
    cout << "ERROR!!! x " << opStr[op] << " y is wrongly computed.\n";
    cout << "  To precision " << prec
       << " the computed value = "<< r.get_str() << "; Answer = " << ans << endl;

    return false;
  } else {
    cout << "CORRECT!!! x " << opStr[op] << " y is correctly computed.\n";
    cout << "  To precision " << prec
       << " the computed value = "<< r.get_str() << "; Answer = " << ans << endl;
  }
  return true;
}

int main(int argc, char** argv) {
  set_output_precision(digits2bits(6));

  prec_t prec=digits2bits(3), prec2=digits2bits(6);
  
  //BigFloat x(1.23456789), y(2.98765432), z;
  BigFloat x("1.23456789"), y("2.98765432"), z;

  if (argc > 1) x.set_str(argv[1]);
  if (argc > 2) y.set_str(argv[2]);
  if (argc > 3) prec = digits2bits(atoi(argv[3]));
  if (argc > 4) prec2 = digits2bits(atoi(argv[4]));

// TODO: if you set the precision from the user, then
// we need to automatically compute the answer using Expression!
//
  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  cout << "low precision = " << bits2digits(prec) << endl;
  cout << "hi precision = " << bits2digits(prec2) << endl;

  ////////////////////////////////////////////////////////////
  // addition
  ////////////////////////////////////////////////////////////
  check(x, y, ADD, prec,  "4.22");
  check(x, y, ADD, prec2, "4.22222");
  ////////////////////////////////////////////////////////////
  // subtraction
  ////////////////////////////////////////////////////////////
  check(x, y, SUB, prec,  "-1.7530"); // exact ans: −1.75308643
  check(x, y, SUB, prec2, "-1.7530864"); 
  ////////////////////////////////////////////////////////////
  // multiplication
  ////////////////////////////////////////////////////////////
  check(x, y, MUL, prec,  "3.69"); // exact ans:3.6884620898917848
  check(x, y, MUL, prec2, "3.68846");
  ////////////////////////////////////////////////////////////
  // division
  ////////////////////////////////////////////////////////////
  check(x, y, DIV, prec,  "0.413"); //exact ans:0.41322313687213987995773219...
  check(x, y, DIV, prec2, "0.413223");
}

