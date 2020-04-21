/*************************************************************
This program verify the following evaluations:

*************************************************************/

#include <fstream>

#define CORE_LEVEL 4
#include "CORE.h"
#include "MultiPrecision.h"

using namespace std;

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  Expr val;
  BigFloat funVal;
  if (argc < 2) {
    cerr << "./po1 digits" << endl;
    exit(1);
  }
  outputPrec = atoi(argv[1]); 
  cerr << "output digits = " << outputPrec << endl;
  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "eps = " << eps << endl;
  setDefaultAbsPrecision(eps+2);
  
  cout.setf(ios::fixed, ios::floatfield);
/*
  val = Expr(1);
  funVal = sin(val).BigFloatValue();
  cout << "sin(1) = " << setprecision(outputPrec) << funVal << ", err=" << funVal.err() << endl;
  
  funVal.makeExact();
  val = Expr(funVal);
  funVal = sin(val).BigFloatValue();
  cout << "sin(sin(1)) = " << funVal << ", err=" << funVal.err() << endl;
  
  funVal.makeExact();
  val = Expr(funVal);
  funVal = sin(val).BigFloatValue();
*/
  funVal = sin(Expr(sin(Expr(sin(Expr(1)).BigFloatValue().makeExact())).BigFloatValue().makeExact())).BigFloatValue();
  cout << "sin(sin(sin(1))) = " << setprecision(outputPrec) << funVal << endl;
  
  return 0;
}
