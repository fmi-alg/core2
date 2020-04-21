#include <mpfr.h>
#include <iostream>

int main(int argc, char* argv[]) {
   unsigned long prec = 100;
   int base = 10;
   mpfr_t f1;
   mpfr_t f2;
   mp_exp_t e;

   mpfr_init2(f1, prec);
   mpfr_set_ui(f1, 2, GMP_RNDN);
   mpfr_root(f1, f1, 5, GMP_RNDD);
   printf("f1 =%s\n", mpfr_get_str(NULL, &e, base, prec, f1, GMP_RNDN));

   mpfr_init2(f2, prec);
   mpfr_set_ui(f2, 2, GMP_RNDN);
   mpfr_root(f2, f2, 5, GMP_RNDU);
   printf("f2 =%s\n", mpfr_get_str(NULL, &e, base, prec, f2, GMP_RNDN));

   mpfr_clear(f1);
   mpfr_clear(f2);

  return 0;
}
