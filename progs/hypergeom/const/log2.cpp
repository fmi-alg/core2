/*************************************************************
file: log2.cpp

 This program computes log(2), stores it to a file and 
 then read from file to verify.

 (Using Hypergeomtric Package to compute log(2)

Usage:
	./log2 precision

   Since Core Library 1.4
   Author: Zilin Du (zilin@cs.nyu.edu)

   $Id: log2.cpp,v 1.1 2006/03/07 04:54:38 exact Exp $
*************************************************************/

#include <iostream> 
#include <fstream>
#include <cstring>
#include <cmath>
#include <ctime>

using namespace std;

#define CORE_LEVEL 4
#include "CORE.h"
#include "MultiPrecision.h"

Expr Log2(int eps) {
  Expr funVal;
  setDefaultAbsPrecision(eps);
  funVal = log(Expr(2));
  funVal.approx(CORE_posInfty, eps);
  return funVal;
}

void writeLog2(ostream& out, int bits) {
  Expr log2 = Log2(bits);
  log2.approx(CORE_posInfty, bits);
  BigFloat bf_log2 = log2.BigFloatValue();
  bf_log2.writeToFile(out);
}

Expr readLog2(istream& in, int bits) {
  BigFloat bf_log2;
  bf_log2.readFromFile(in, bits);
  Expr val(bf_log2);
  val.approx(CORE_posInfty, bits);
  return val;
}

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  if (argc < 2) {
    cout << "./log2 precision" << endl;
    exit(1);
  }
  
  cerr << "Computing log(2), result stored in file log2.big" << endl;
 
  outputPrec = atoi(argv[1]);
  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
 
 // Compute log2 and store in the file	
  char log2_file[255] = "log2.big";
  ofstream ofs;
  ofs.open(log2_file);
  ofs << "# Natural log of 2 using the .big Format" << endl;
  writeLog2(ofs, eps);
  ofs.close();
  cout << "write log2 to a file!" << endl;
  cout << "Done." << endl;
  
  ifstream ifs;
  Expr p;
  
  // Read Log2 with all digits from the file 
  ifs.open(log2_file);
  p = readLog2(ifs, eps);
  cout << "Read all digits in the file ... " << endl;
  cout << "Log2=" << setprecision(outputPrec) << p << endl;
  ifs.close(); 
  
  // Read Log2 with half digits from the file
  ifs.open(log2_file);
  p = readLog2(ifs, eps/2);
  cout << "Read half digits in the file ... " << endl;
  cout << "Log2=" << setprecision(outputPrec/2) << p << endl;
  ifs.close(); 
   
  return 0;
}
