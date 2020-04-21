// Sylvain Pion (adapted by Chee)
//
//  The follow code reveals a bug in Expr's output!
//  	Let g to be the smallest double value that is > than 1. 
//  	Expr(g) will print only 1.
//
#define CORE_LEVEL 4
#include <math.h>
#include <iostream>
#include <CORE/CORE.h>
using namespace std;

int main() {
  double d = 1/3.0;
  double e = nextafter(d, 1.0);
  double f = 1.0;
  double g = nextafter(f, 2.0);
  double gg = nextafter(g, 2.0);
  // double gg = nextafter(nextafter(g, 2.0), 2.0);
  cout.precision(40);
  cout << d << endl;
  cout << e << endl;
  cout << f << endl;
  cout << g << endl;	// This ought to print "1.0000...022".
  			// But in cygwin, this prints "1"
  cout << gg << endl;
  
  printf("g  = %2.20f\n", g); // This prints properly in cygwin
  printf("g  = %2.55f\n", g); // This is to see ALL the significance of g
  printf("gg = %2.55f\n", gg); // This is to see ALL the significance of g

  if (f == g) cout << "error! f == g" << endl;
  else cout << "correct! f != g" << endl;


  // Now, to see what happens in CORE:
  Expr E = Expr(g);
  E.approx(100,100);	// Evaluate it to high precision
  cout << setprecision(55) << "Expr(g) = " << E << endl;
  			// BUG!! On cygwin, I get an output of "1"
  E = Expr(gg);
  E.approx(100,100);	// evaluate it to many bits
  setDefaultOutputDigits(100); // output to many digits
  cout << setprecision(100) << "Expr(gg) = " << E << endl;
  			// BUG!! On cygwin, I get an output of "1"

  if (E > 1) cout << "CORRECT! Expr(g) > 1" << endl;
  else if (E == 1) cout << "ERROR! Expr(g) == 1" << endl;
  else if (E == 1) cout << "ERROR! Expr(g) < 1" << endl;

  return 0;
}

