
// This program MUST be run at CORE_LEVEL 3:
#define CORE_LEVEL 3

#include "CORE.h"

int main( int argc, char *argv[] ) {

  prec_t prec = 4000;
  int print = 0;

  if (argc > 1)
    prec = atoi(argv[1]);
 
  if (argc > 2)
    print = atoi(argv[2]);
    
  setDefaultOutputDigits(bits2digits(prec)); 

  Timer2 timer;
  timer.start();
  Expr PI = pi();
  PI.approx(prec, CORE_INFTY);
  timer.stop();

  if (print > 0)
    std::cout << "PI  = " << PI << std::endl;
  std::cout << "compute pi to " << prec
            << " relative precision" << std::endl;
  std::cout << "it took " << timer.get_mseconds()
            << " mseconds." << std::endl;
  
  timer.start();
  Expr E = e();
  E.approx(prec, CORE_INFTY);
  timer.stop();
 
  if (print > 0)
    std::cout << "e  = " << E << std::endl;
  std::cout << "compute e to " << prec
            << " relative precision" << std::endl;
  std::cout << "it took " << timer.get_mseconds()
            << " mseconds." << std::endl;

  timer.start();
  Expr SqrtPi = sqrt(pi());
  SqrtPi.approx(prec, CORE_INFTY);
  timer.stop();
 
  if (print > 0)
    std::cout << "square root of PI  = " << SqrtPi << std::endl;
  std::cout << "compute square root of pi to " << prec
            << " relative precision" << std::endl;
  std::cout << "it took " << timer.get_mseconds()
            << " mseconds." << std::endl;

  timer.start();
  Expr E2 = pow(e(), 2);
  E2.approx(prec, CORE_INFTY);
  timer.stop();

  if (print > 0)
    std::cout << "e^2  = " << E2 << std::endl;
  std::cout << "compute e^2 to " << prec
            << " relative precision" << std::endl;
  std::cout << "it took " << timer.get_mseconds()
            << " mseconds." << std::endl;

  return 0;
}
