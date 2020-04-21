/* test returned value from most MPFR functions
   in MPFR's documentation, page 7:

   unless documented otherwise, functions returning an int return a ternary value.
   if the ternary value is zero, it means the value stored in the destination variable
   is the exact result of the corresponding mathematical function. If the ternary value
   is positive (resp. negative), it means the value stored in the dstination variable
   is greater (resp. lower) than the exact result.

   This example shows this feature:
	
	taking the 4th root of "81" returns "3" exactly.

*/

#include <mpfr.h>
#include <iostream>

static int
cputime ()
{
  struct rusage rus;
  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

int main(int argc, char* argv[]) {
   unsigned long prec = 100;
   mpfr_t f;
   mpfr_t f2;
   mp_exp_t e;

   int r;
   mpfr_init2(f, prec);
   r = mpfr_set_ui(f, 81, GMP_RNDN);
   printf("r=%d\n", r);
   r = mpfr_root(f, f, 4, GMP_RNDU);
   printf("f =%s\n", mpfr_get_str(NULL, &e, 2, prec, f, GMP_RNDU));
   printf("r=%d\n", r);
   mpfr_init2(f2, prec);
   r = mpfr_set_ui(f2, 81, GMP_RNDN);
   printf("r=%d\n", r);
   r = mpfr_root(f2, f2, 4, GMP_RNDD);
   printf("f2=%s\n", mpfr_get_str(NULL, &e, 2, prec, f2, GMP_RNDD));
   printf("r=%d\n", r);
   mpfr_clear(f);
   mpfr_clear(f2);

  return 0;
}
