/*************************************************************
file: timepi.cpp

 This program demonstrates the time differece between computing
 Pi on the fly and read from the file.

Usage:
	./timepi precision

   
   Since Core Library 1.4
   Author: Zilin Du (zilin@cs.nyu.edu)

   $Id: timepi.cpp,v 1.1 2006/03/07 04:54:38 exact Exp $
*************************************************************/
#include <cmath>
#include <fstream>
#include <ctime>

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
  long t1, t2;
  
  if (argc < 2) {
    cout << "./timepi precision" << endl;
    exit(1);
  }
  
  cerr << "Timing Pi ..." << endl;
 
  outputPrec = atoi(argv[1]);
  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
  
  t1 = clock();
  Expr p1 = Pi(eps);
  cerr << "Pi=" << std::setprecision(outputPrec) << p1<< endl;
  t2 = clock();
  cout << outputPrec << "\t" << (float)(t2-t1)/CLOCKS_PER_SEC;
  
  // Compute Pi and store in the file	
  char pi_file[255] = "pi.big";
  ofstream ofs;
  ofs.open(pi_file);
  writePi(ofs, eps);
  ofs.close();
  
  // Read Pi with all digits from the file 
  ifstream ifs;
  ifs.open(pi_file);
  t1 = clock();
  Expr p2 = readPi(ifs, eps);
  t2 = clock();
  cerr << "Pi=" << std::setprecision(outputPrec) << p2 << endl;
  cout << "\t" << (float)(t2-t1)/CLOCKS_PER_SEC << endl;
  ifs.close(); 
  
  return 0;
}
