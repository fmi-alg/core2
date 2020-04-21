/*************************************************************
 * File: tUC.cpp
 * Purpose:
 * 	Verify some small expression from the website:
 * 		http://www.simon-langley.net/UC/
 *
 * Usage:   
 * 		> ./tUC [prec=40]
 *
 * 	where the default values are shown above.
 *
 * Author: Chee Yap, Zilin Du
 * Date:  May 22, 2003
 *
 * Since Core Library version 1.6
 *************************************************************/

#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  outputPrec =  40;	// default output precision
  if (argc >= 2) {
    outputPrec = atoi(argv[1]);
  }

  cout	<< "Testing some small expressions\n"
	<< "==============================" << endl;

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
 
  setDefaultOutputDigits(outputPrec);
  
  Expr e1 = radical(NT(2), 8) - Expr("494/453");
  cout << "2^{1/8}-494/453 \n      = " << e1 << endl;
  
  Expr e2 = radical(NT(10), 8) - Expr("4/3");
  cout << "10^{1/8}-4/3 \n      = " << e2 << endl;
  
  Expr e3 = radical(NT(176), 2) + radical(NT(195), 2) + Expr(45) \
	  - radical(NT(190), 2)  - radical(NT(398), 2) - radical(NT(1482), 2);

  cout << "sqrt(176)+sqrt(195)+45-sqrt(190)-sqrt(398)-sqrt(1482) \n      = "
	  << e3 << endl;

// The expressions  sqrt(2)+sqrt(3) and sqrt(3)+sqrt(6) are not equivalent.
}
