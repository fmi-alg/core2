
#include <CORE/CORE.h>
#include <CORE/poly/Poly.h>
#include <CORE/BigInt.h>
#include <CORE/BigRat.h>
#include <CORE/BigFloat.h>
#include <iostream>

using namespace CORE;
using namespace std;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

int main() {
  NT coeff1[5] = {1, 2, 3, 4, 5};
  NT coeff2[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

  PolyNT P1(4, coeff1);
  PolyNT P2(8, coeff2);

  cout << "P1=" << P1 << endl;
  cout << "P2=" << P2 << endl;

  cout << "P1+P2=" << (P1+P2) << endl;

  if (P1+P2 == P2+P1) cout << "CORRECT!!! P1+P2=P2+P1" << endl;
  else cout << "ERROR!!! P1+P2 not equal P2+P1" << endl;

  cout << "P1-P2=" << (P1-P2) << endl;
  cout << "P2-P1=" << (P2-P1) << endl;

  if (P1-P2 == (P2-P1).negate())
	cout << "CORRECT!!!  P1-P2=(P2-P1).negate()" << endl;
  else cout << "ERROR!!!  P1-P2= (P2-P1).negate()" << endl;

  cout << "P1*P2=" << (P1*P2) << endl;
  cout << "P2*P1=" << (P2*P1) << endl;

  if (P1*P2 == P2*P1) cout << "CORRECT!!! P1*P2=P2*P1" << endl;
  else cout << "ERROR!!! P1*P2 not equal P2*P1" << endl;
  
  BigInt x(100);
  BigRat y(100);
  BigFloat z(100);
  
  cout << "x.rc=" << x.get_rc() << endl;
  cout << "y.rc=" << y.get_rc() << endl;
  cout << "z.rc=" << z.get_rc() << endl;
  return 0;
}
