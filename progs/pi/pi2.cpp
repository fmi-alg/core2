// from Valentina

#include "CORE.h"

// Compute Pi using Brent's algorithm:

double Pi(int  p) {
  double A=1; double Y;
  double B=1/sqrt(2.0); double T=0.25; double X=1;
  double err = BigFloat::exp2(-p);
  while ( (A - B) > err ) { std::cout << "in" << std::endl;
	Y=A; A=(A+B)/2; B=sqrt(B*Y);
	T=T-X*(A-Y)*(A-Y); X=2*X;
  }
  std::cout << "out" << std::endl;
  return A*A/T; 	
}


// Implementation of some functions in the Brent's paper 
// " Fast Multiple-Precision Evaluation of Elementary Functions" 
// Pi
int main (int argc, char *argv[]) {
  
  int eps = 54;
  if (argc > 1) eps = atoi(argv[1]);	

  //setScientificFormat();
  int outputPrec;	// desired precision in digits
  outputPrec= (int) (eps * log(2.0)/log(10.0));
  std::cout << "outputPrec = " << outputPrec << std::endl;
  double output = Pi(eps);
  output.approx(eps);
  std::cout << "Pi = " << std::setprecision(outputPrec) << output << std::endl;
  return 0;
}

