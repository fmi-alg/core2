#include <CORE/Mpfr.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  prec_t prec = 3;

  Mpfr f1(1);
  Mpfr f2(3);
   
  // initial f with precision 100 bits
  Mpfr f(0, prec);
  Mpfr ff(0, prec);
  f.div(f1, f2, GMP_RNDD);
  ff.div(f1, f2, GMP_RNDU);

  BigInt z;
  exp_t e = f.get_z_exp(z);
  std::cout << "z.sizeinbase(2)=" << z.sizeinbase() << std::endl;
  std::cout << "z=" << z.get_str(2) << std::endl;
  std::cout << "e=" << e << std::endl;

  BigInt zz;
  exp_t ee = ff.get_z_exp(zz);
  std::cout << "zz.sizeinbase(2)=" << zz.sizeinbase() << std::endl;
  std::cout << "zz=" << zz.get_str(2) << std::endl;
  std::cout << "ee=" << ee << std::endl;

  Mpfr diff(0, prec+10);
  diff.sub(f, ff);
  std::cout << "diff=" << diff.get_str() << std::endl;

  return 0;
}
