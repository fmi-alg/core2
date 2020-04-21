/* solution to problem C03 from the "many digits friendly competition":
   compute the first 10^N decimal digits after the decimal point of
   sin((e+1)^3).

   We compute with precision p and rounding to nearest:
   x = o(e)
   y = o(x+1)
   z = o(y^2)
   t = o(z*y)
   u = o(sin(t))
   
   We have with theta a generic value such that |theta| <= 2^(-p):
   x = e*(1+theta)
   y = (x+1)*(1+theta) = (e*(1+theta)+1)*(1+theta) = (e+1)*(1+theta)^2
   z = y^2*(1+theta) = (e+1)^2*(1+theta)^5
   t = z*y*(1+theta) = (e+1)^3*(1+theta)^8

   p=3;
   x = [approx(exp(1),p,3), approx(exp(1),p,2)]
   y = [approx(x[1]+1,p,3), approx(x[2]+1,p,2)]
   z = [approx(y[1]^2,p,3), approx(y[2]^2,p,2)]
   t = [approx(z[1]*y[1],p,3), approx(z[2]*y[2],p,2)]

   For p >= 3, we have 40 <= t <= 64, and (1+theta)^8 can be written
   1+13*theta when |theta| <= 2^(-p), so the absolute error on t is
   bounded by 13*2^(6-p).

   Thus t = (e+1)^3 + r with |r| <= 13*2^(6-p).
   sin(t) = sin((e+1)^3) + r * cos(a) thus the final error, taking into
   account the rounding error on u, is bounded by 1/2*ulp*(u) + 13*2^(6-p)
   <= 2^(-p-1) + 13*2^(6-p) <= 2^(10-p).

   Thus for 2^(10-p) <= 1/2*10^(-M), i.e. p >= 11 + M*log(10)/log(2),
   the error on u is bounded by 1/2ulp of the output value.
*/

#include "many.h"

int
main (int argc, char *argv[])
{
  unsigned long M, N = atoi (argv[1]);
  mp_prec_t p;
  mpfr_t i, j;
  char *lo;
  mp_exp_t exp_lo;
  int st, st0;

  fprintf (stderr, "Using GMP %s and MPFR %s\n", gmp_version, mpfr_version);
  st = cputime ();

  mpfr_init (i);
  mpfr_init (j);

  M = N;

  do
    {
      M += 10;
      mpfr_set_prec (i, 32);
      mpfr_set_d (i, LOG2_10, GMP_RNDU);
      mpfr_mul_ui (i, i, M, GMP_RNDU);
      mpfr_add_ui (i, i, 11, GMP_RNDU);
      p = mpfr_get_ui (i, GMP_RNDU);
      fprintf (stderr, "Setting precision to %lu\n", p);

      mpfr_set_prec (i, p);
      mpfr_set_prec (j, 2);

      mpfr_set_ui (j, 1, GMP_RNDN);
      mpfr_exp (i, j, GMP_RNDN); /* i = e */
      mpfr_add_ui (i, i, 1, GMP_RNDN); /* e+1 */
      mpfr_set_prec (j, p);
      mpfr_mul (j, i, i, GMP_RNDN); /* (e+1)^2 */
      mpfr_mul (j, j, i, GMP_RNDN); /* (e+1)^3 */

      st0 = cputime ();
      mpfr_sin (i, j, GMP_RNDN);
      st0 = cputime () - st0;
      fprintf (stderr, "mpfr_sin took %dms\n", st0);

      st0 = cputime ();
      lo = mpfr_get_str (NULL, &exp_lo, 10, M, i, GMP_RNDN);
      st0 = cputime () - st0;
    }
  while (can_round (lo, N, M) == 0);

  lo[N] = '\0';
  printf ("%s\n", lo);

  mpfr_clear (i);
  mpfr_clear (j);

  fprintf (stderr, "Cputime: %dms (output %dms)\n", cputime () - st, st0);
  return 0;
}
