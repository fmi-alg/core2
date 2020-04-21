/*************************************************************
file: pi.cpp

 This program computes pi, stores it to a file and then read 
 from file to verify.

 	Pi = 4 * ( 4 * arctan(1/5) + arctan(1/239) ) 

Usage:
	./pi precision

   
   Since Core Library 1.4
   Author: Zilin Du (zilin@cs.nyu.edu)

   $Id: pi.cpp,v 1.1 2006/03/07 04:54:38 exact Exp $
*************************************************************/
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

#define CORE_LEVEL 4
#include "CORE.h"


Expr arctan(int x, int a) {
  // compute arctan(1/x) to absolute precision 2^{-a}
  // x is a natural number and x > 1.

  int N = (int) (a / ( 2 * log(float(x))/log(float(2)))) + 1;
  //cout << " Number of terms needed in arctan, N = " << N << endl;
	
  Expr at = 1;
  Expr x2 = x * x;
  for (int i=1; i<=N; i++) {
    if (i % 2 == 1) 
      at = x2 * at - 1 / Expr(2 * i + 1);
    else
      at = x2 * at + 1 / Expr(2 * i + 1);
  }
  
  at /= pow (x2, N) * x;
  
  at.approx(CORE_posInfty, a);
  return at;
}

Expr PiOverFour(int eps) {
  return 4*arctan(5, eps+3)-arctan(239, eps+1);
}

Expr PiOverTwo(int eps) {
  return 2*PiOverFour(eps+1);
}

Expr Pi(int eps) {
  return 4*PiOverFour(eps+2);
}

Expr SqrtPi(int eps) {
  return sqrt(Pi(eps));
}

void writePi(ostream& out, int bits) {
  Expr pi = Pi(bits);
  pi.approx(CORE_posInfty, bits);
  BigFloat bf_pi = pi.BigFloatValue();
  bf_pi.writeToFile(out);
}

Expr readPi(istream& in, int bits) {
  BigFloat bf_pi;
  bf_pi.readFromFile(in, bits);
  Expr pi(bf_pi);
  pi.approx(CORE_posInfty, bits);
  return pi;
}
	
int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  if (argc < 2) {
    cout << "./pi precision" << endl;
    exit(1);
  }
  
  cerr << "Computing Pi ..." << endl;
 
  outputPrec = atoi(argv[1]);
  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
  
  // Compute Pi and store in the file	
  char pi_file[255] = "pi.big";
  ofstream ofs;
  ofs.open(pi_file);
  ofs << "# Pi written in our .big Format" << endl;
  writePi(ofs, eps);
  ofs.close();
  cout << "write pi to a file!" << endl;
  cout << "Done." << endl;
  
  ifstream ifs;
  Expr p;
  
  // Read Pi with all digits from the file 
  ifs.open(pi_file);
  p = readPi(ifs, eps);
  cout << "Read all digits in the file ... " << endl;
  cout << "Pi=" << setprecision(outputPrec) << p << endl;
  ifs.close(); 
  
  // Read Pi with half digits from the file
  ifs.open(pi_file);
  p = readPi(ifs, eps/2);
  cout << "Read half digits in the file ... " << endl;
  cout << "Pi=" << setprecision(outputPrec/2) << p << endl;
  ifs.close(); 
  
  return 0;
}
