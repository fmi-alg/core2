#include <mpfr.h>
#include <iostream>

/* *******************
 * cputime() is not used.
 *
 * On cygwin, the struct "rusage"
 * is not defined.  But on MacOS, it is OK.
 * So we comment this out :
 * 
 * static int
 * cputime ()
 * {
 *   struct rusage rus;
 *   getrusage (0, &rus);
 *   return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
 * }
 * ********************/

int main(int argc, char* argv[]) {
   unsigned long prec = 100;
   mpfr_t f;
   mpfr_t f2;
   mp_exp_t e;

   int r;
   mpfr_init2(f, prec);
   r = mpfr_sqrt_ui(f, 2, GMP_RNDU);
   printf("f =%s\n", mpfr_get_str(NULL, &e, 2, prec, f, GMP_RNDU));
   printf("r=%d\n", r);
   mpfr_init2(f2, prec);
   r = mpfr_sqrt_ui(f2, 2, GMP_RNDD);
   printf("f2=%s\n", mpfr_get_str(NULL, &e, 2, prec, f2, GMP_RNDD));
   printf("r=%d\n", r);
   /* mpfr_set_prec_raw(f, prec/2); */
   mpfr_prec_round (f, prec/2, GMP_RNDN);
   printf("f =%s\n", mpfr_get_str(NULL, &e, 2, prec, f, GMP_RNDU));
   mpfr_prec_round (f2, prec/2, GMP_RNDN);
   printf("f2=%s\n", mpfr_get_str(NULL, &e, 2, prec, f2, GMP_RNDD));
   mpfr_set_prec_raw(f, prec);
   printf("f =%s\n", mpfr_get_str(NULL, &e, 2, prec, f, GMP_RNDU));
   mpfr_set_prec_raw(f2, prec);
   printf("f2=%s\n", mpfr_get_str(NULL, &e, 2, prec, f2, GMP_RNDD));
   mpfr_clear(f);
   mpfr_clear(f2);

  return 0;
}
