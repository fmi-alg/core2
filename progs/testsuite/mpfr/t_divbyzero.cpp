#include <mpfr.h>
#include <iostream>

int main(int argc, char* argv[]) {
   unsigned long prec = 100;
   int base = 10;
   mpfr_t f1;
   mpfr_t f2;
   mpfr_t f;
   mp_exp_t e;

   mpfr_init2(f1, prec);
   mpfr_init2(f2, prec);
   mpfr_init2(f, prec);

   mpfr_set_si(f1, 10, GMP_RNDN);
   mpfr_set_si(f2, 0, GMP_RNDN);
   int r = mpfr_div(f, f1, f2, GMP_RNDN);
   printf("f=%s\n", mpfr_get_str(NULL, &e, base, prec, f, GMP_RNDN));
   printf("r=%d\n", r);
   printf("sgn(f)=%d, is_inf=%d\n", mpfr_sgn(f), mpfr_inf_p(f));

   mpfr_set_si(f1, -10, GMP_RNDN);
   mpfr_set_si(f2, 0, GMP_RNDN);
   mpfr_div(f, f1, f2, GMP_RNDN);
   printf("f=%s\n", mpfr_get_str(NULL, &e, base, prec, f, GMP_RNDN));
   printf("r=%d\n", r);

   mpfr_set_inf(f, 1);
   printf("f =%s\n", mpfr_get_str(NULL, &e, base, prec, f, GMP_RNDN));
   printf("sgn(f)=%d, is_inf=%d\n", mpfr_sgn(f), mpfr_inf_p(f));
   mpfr_set_inf(f, -1);
   printf("f =%s\n", mpfr_get_str(NULL, &e, base, prec, f, GMP_RNDN));
   printf("sgn(f)=%d, is_inf=%d\n", mpfr_sgn(f), mpfr_inf_p(f));

   mpfr_clear(f1);
   mpfr_clear(f2);
   mpfr_clear(f);

  double d1, d2, d;

  d1 = 10;
  d2 = 0;
  d = d1 / d2;
  std::cout << "d=" << d << std::endl;
  std::cout << "d>0 ? " << (d>0) << std::endl;

  d1 = -10;
  d2 = 0;
  d = d1 / d2;
  std::cout << "d=" << d << std::endl;
  std::cout << "d>0 ? " << (d>0) << std::endl;

  return 0;
}
