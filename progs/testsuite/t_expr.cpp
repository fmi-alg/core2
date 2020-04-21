#include <CORE/CORE.h>
#include <iostream>

using namespace std;

static int
cputime ()
{
  struct rusage rus;
  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

int main(int argc, char* argv[]) {
  //unsigned long prec = 30;
  size_t prec = 30; // 30 digits
  int st, st0;

  if (argc > 1)
    prec = atoi(argv[1]);

  st = cputime();

  cout << "100 digits is equal to " << digits2bits(100) << " bits!" << endl;

  Expr x = root(Expr(2), 4);
  Expr xx = root(Expr(4), 4);
  Expr xxx = root(Expr(8), 4);
  Expr xxxx = root(Expr(16), 4);
  Expr y = Expr(7) + x - 5*xxx;
  cout << "x: 4th root of 2 =" << x.r_approx(digits2bits(prec)) << endl;
  cout << "xx: 4th root of 4=" << xx.r_approx(digits2bits(prec))<< endl;
  cout << "xxx: 4th root of 8=" << xxx.r_approx(digits2bits(prec)) << endl;
  cout << "xxxx: 4th root of 16=" << xxxx.r_approx(digits2bits(prec)) << endl;
  cout << "y: 7 + x - 5 xxx = " << y.r_approx(digits2bits(prec)) << endl;

  Expr r = cbrt(xxxx) + xx - x;
  cout << "r=" << r.r_approx(digits2bits(prec)) << endl;
  cout << "r.sign()=" << r.sign() << endl; 
  cout << "r.uMSB()=" << r.uMSB() << endl; 
  cout << "r.lMSB()=" << r.lMSB() << endl; 
  cout << "digits2bits(prec)=" << digits2bits(prec) << endl; 

  r = cbrt(Expr(17));
  x = root(Expr(17), 3);
  cout << "cbrt(17) = " << r.r_approx(digits2bits(prec)) << endl;
  cout << "root(17, 3) = " << x.r_approx(digits2bits(prec)) << endl;
  if (x==r) 
      cout << "cbrt(17) = root(17,3) -- CORRECT!" << endl;
  else
      cout << "cbrt(17) != root(17,3) -- ERROR!" << endl;

  Expr::KT value = r.r_approx(digits2bits(prec));
  cout << "r.diam=" << value.abs_diam() << endl;
  
  st0 = cputime();
  cout << "r=" << value << endl;
  st0 = cputime() - st0;

  cout << "Cputime: " << cputime() - st << 
    "ms (output " << st0 << "ms)" << endl;

  Expr e = sqrt(Expr(6)) - sqrt(Expr(2))*sqrt(Expr(3));
  //Expr e = sqrt(Expr(2))*sqrt(Expr(3));
  cout << "e.sign()=" << e.sign() << endl; 
  cout << "e.r_approx()=" << e.r_approx(digits2bits(prec)) << endl; 

  Expr ee = sqrt(Expr(7));
  cout << "ee.sign()=" << ee.sign() << endl; 
  cout << "ee.r_approx()=" << ee.r_approx(digits2bits(prec)) << endl; 

  Expr eee = ee - e;
  cout << "eee.sign()=" << eee.sign() << endl; 
  cout << "eee.r_approx()=" << eee.r_approx(digits2bits(prec)) << endl; 
  cout << "eee=" << eee << endl; 

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  // Testing rootOf(poly,...) constructions:
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  BigInt coeff[3] = {-2, 0, 1};
  const Polynomial<BigInt> poly = Polynomial<BigInt>(2, coeff);
  		//poly = x^2 - 2 
  Expr aa = rootOf<BigInt>(poly); // default n=0 smallest positive root
  cout << "aa=sqrt(2) = " << aa.r_approx(digits2bits(prec)) << endl;

  if (aa*aa == Expr(2))
      cout << "aa*aa == 2:  CORRECT!" << endl;
  else
      cout << "aa*aa != 2:  ERROR!" << endl;

  //
  const BFInterval bfi(1,2);
  Expr bb = CORE::rootOf(poly, bfi);
  if (aa == bb)
      cout << "aa == bb:  CORRECT!" << endl;
  else
      cout << "aa != bb:  ERROR!" << endl;
  //
  const BigFloat left(1);
  const BigFloat right(2);
  Expr cc = rootOf(poly, left, right);
  if (aa == cc)
      cout << "aa == cc:  CORRECT!" << endl;
  else
      cout << "aa != cc:  ERROR!" << endl;

  Expr dd = CORE::rootOf(poly, BFInterval(-2,-1));
  cout << "dd= -sqrt(2) = " << dd.r_approx(digits2bits(prec)) << endl;
  if (aa + dd == 0)
      cout << "aa ++ dd == 0:  CORRECT!" << endl;
  else
      cout << "aa != dd     :  ERROR!" << endl;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  // Testing golden ratio via rootOf(poly,...) 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/
  BigInt coef[3] = {-1, -1, 1};
  const Polynomial<BigInt>pp = Polynomial<BigInt>(2, coef);
  		//pp = x^2 - x - 1 
  aa = rootOf(pp, 1, 2); // aa is equal to phi=1.618...
  cout << "Golden Ratio, phi = " << aa.r_approx(digits2bits(prec)) << endl;
  bb = rootOf(pp, -1, 0); // bb is equal to phi=-0.618...
  cout << "Golden Ratio, phi-hat = " << bb.r_approx(digits2bits(prec)) << endl;
  if (aa*aa == aa + Expr(1))
      cout << "phi * phi ==  phi + 1:  CORRECT!" << endl;
  else
      cout << "phi * phi !=  phi + 1:  ERROR!" << endl;
  if (aa * (aa- Expr(1)) == Expr(1))
      cout << "1/phi ==  phi - 1:  CORRECT!" << endl;
  else
      cout << "1/phi !=  phi - 1:  ERROR!" << endl;

  return 0;
}
