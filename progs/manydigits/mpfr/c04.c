#define USE_MPFI
#include "many.h"


int
main (int argc, char *argv[])
{
  unsigned long N = atoi (argv[1]);
  mp_prec_t p;
  mpfi_t i, j;
  char *lo, *hi;
  mp_exp_t exp_lo, exp_hi;
  int st;
  mpz_t intpart;

  fprintf (stderr, "Using GMP %s and MPFR %s\n", gmp_version, mpfr_version);
  st = cputime ();

  /* initial estimate of the working precision */
  p = (mp_prec_t) (3.321928095 * (double) N + 10.0 + 61.0);
  /* note: 60.0 is not enough for N=100000 */
  fprintf (stderr, "Setting initial precision to %lu\n", p);
  
  mpfi_init (i);
  mpfi_init (j);
  mpz_init (intpart);

  do
    {
      mpfi_set_prec (i, p);
      mpfi_set_prec (j, 12); /* enough to store 2011 exactly */

      mpfi_set_ui (j, 2011);
      mpfi_sqrt (i, j);

      mpfi_set_prec (j, p);
      mpfi_const_pi (j);
      mpfi_mul (i, i, j);
      mpfi_exp (i, i);
      mpz_set_str (intpart,
           "15287325030838530726060718956632626460288135250495712354991535",
	   10);
      mpfi_sub_z (i, i, intpart);

      lo = mpfr_get_str (NULL, &exp_lo, 10, N, LEFT(i), GMP_RNDZ);
      hi = mpfr_get_str (NULL, &exp_hi, 10, N, RIGHT(i), GMP_RNDZ);
      if (exp_lo == exp_hi && strcmp (lo, hi) == 0)
        break;
      else
        {
          p += p / 100;
          fprintf (stderr, "Increasing precision to %lu\n", p);
        }
    }
  while (1);

  printf ("0%s\n", lo);

  mpfi_clear (i);
  mpfi_clear (j);
  mpz_clear (intpart);

  fprintf (stderr, "Cputime: %dms\n", cputime () - st);
  return 0;
}
