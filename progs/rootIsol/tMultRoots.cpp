#define CORE_LEVEL 4
#include<stack>
#include "CORE/CORE.h"
#include "readpoly.h"
#include"CORE/poly/Descartes.h"

typedef BigInt NT;
typedef Polynomial<NT> Poly;

using namespace CORE;
using namespace std;


int main( int argc, char* argv[] ) {
  /* ************************************** */
  /* Read Arguments */
  /* ************************************** */
  Poly P("(x^2-x-1)^4*(x-2)^2");
  Poly Q(P);
  cout << "P = "<< P.toString() << endl;

  cout <<"P' = "<< Q.differentiate().toString() << endl;

  BigFloat x, xprime, fx, fprimex, delx;
  BigFloat y, yprime, fy, fprimey, dely;

  xprime = BigFloat(1.618);
  yprime = xprime;

  BigFloat m(1.0);


  cout.precision(40);

  do{
    x = xprime; y = yprime;
    fx = P.eval(x); fy = P.eval(y);
    fprimex = Q.eval(x); fprimey = Q.eval(y);
    delx = m*fx/fprimex; dely = (m+1)*fy/fprimey;
    xprime = x - delx; yprime = y - dely;
    
    cout << "delx = " << delx.uMSB() << endl;
    cout << "dely = " << dely.uMSB() << endl;

    if(core_abs(delx.uMSB()) < core_abs(dely.uMSB())){
      m +=BigFloat(1); // Increment the multiplicity if m+1 gives us a better convergence
      xprime=yprime;
    }
    cout <<"xprime = " << xprime << endl;
    cout <<"yprime = " << yprime << endl;

    cout << "Estimate on Multiplicity " << m << endl;


  }while(core_abs(delx.uMSB()) < 2*core_abs(dely.uMSB()));
  // We terminate because when $m$ is the multiplicity we get an order $m+1$ convergence
  // whereas for $m+1$ we will only get linear convergence. The two on the RHS verifies
  // that the right choice of $m$ gives us at least quadratic convergence.

  cout << "Multiplicity is " << m << endl;
  return 0;
}
