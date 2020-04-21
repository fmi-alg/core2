/*************************************************************
file: verify.cpp

This program verify the following evaluations:

	$\sin(22) = -8.85130929040388e-3$.

	$\sin(355) = -0.000030144353359488449...$

	$\sin(10^{22}) = -0.8522008497671888017727...$

	$\cos(x)$ = -4.68716592425462761112... x10^{-19}.
		where $x= (3/2)_2 x 2^{200}.

*************************************************************/

#include <fstream>

#define CORE_LEVEL 4
#include "CORE.h"
#include "MultiPrecision.h"

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  Expr val, funVal;
  if (argc < 2) {
    std::cerr << "./verify digits" << std::endl;
    exit(1);
  }
  outputPrec = atoi(argv[1]); 
  std::cerr << "output digits = " << outputPrec << std::endl;
  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  std::cerr << "eps = " << eps << std::endl;
  setDefaultAbsPrecision(eps);
  
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  val = Expr(22);
  funVal = sin(val);
  std::cout << "sin(22) = " << std::setprecision(outputPrec) << funVal << std::endl;
  
  val = Expr(355);
  funVal = sin(val);
  std::cout << "sin(355) = " << std::setprecision(outputPrec) << funVal << std::endl;
  
  val = pow(Expr(10), 22);
  funVal = sin(val);
  std::cout << "sin(10^22) = " << std::setprecision(outputPrec) << funVal << std::endl;
  
  val = Expr("3/2") * pow(Expr(2), 200);
  funVal = cos(val);
  std::cout << "cos(3/2 * 2^200) = " << std::setprecision(outputPrec) << funVal << std::endl;
  
  return 0;
}
