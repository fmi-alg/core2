#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <math.h>
#include "gmp.h"
#include "mpfr.h"
#ifdef USE_MPFI
#include "mpfi.h"
#include "mpfi_io.h"
#endif

#define LOG2_10 3.3219280948873626 /* upper approximation of log(10)/log(2) */
#define LOG10_2 0.30102999566398120 /* upper approximation of log2/log10 */
#define LOG7_10 1.1832946624549385 /* upper approximation of log(10)/log(7) */

#define LEFT(x) (&((x)->left))
#define RIGHT(x) (&((x)->right))

static int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

/* check if first N digits after the decimal point of lo and hi agree
   Assumes strlen (lo) = strlen (hi) >= N + exp_lo.
   Return 0 if digits agree, non-zero otherwise.
 */
static int
compare (char *lo, mp_exp_t exp_lo, char *hi, mp_exp_t exp_hi, unsigned long N)
{
  if (exp_lo != exp_hi)
    return 1;

  if (exp_lo > 0) /* disregard first exp_lo digits */
    {
      if (strlen (lo) < N + exp_lo)
        abort ();
      return strncmp (lo + exp_lo, hi + exp_hi, N);
    }
  else /* exp_lo <= 0: first (-exp_lo) digits are zero */
    {
      unsigned long zeroes = -exp_lo;
      return (zeroes >= N) ? 0 : strncmp (lo, hi, N - zeroes);
    }
}

/* return the N first decimal digits of x after the decimal point */
static char*
fixed_point_output (mpfr_t x, unsigned long N)
{
  char *s, *t;
  mp_exp_t e, d;
  s = mpfr_get_str (NULL, &e, 10, 2, x, GMP_RNDZ);
  mpfr_free_str (s);
  t = malloc (N + 1);
  if (e > 0) /* need N + e digits */
    {
      s = mpfr_get_str (NULL, &d, 10, N + e, x, GMP_RNDZ);
      strcpy (t, s + e);
      mpfr_free_str (s);
    }
  else if ((unsigned) (-e) >= N)
    {
      memset (t, '0', N);
      t[N] = '\0';
    }
  else
    {
      memset (t, '0', -e);
      mpfr_get_str (t + (-e), &d, 10, N - (-e), x, GMP_RNDZ);
    }
  return t;
}

/* assuming s is a string of N1 digits, approximating a value with an error
   < 1 ulp, returns non-zero if we can round to N digits, i.e. iff the last
   N1-N digits are non all zero. */
int
can_round (char *s, unsigned long N, unsigned long N1)
{
  while (N < N1 && s[N] == '0')
    N ++;
  return N < N1;
}

#ifdef WANT_MPFR_ROOT
#if (MPFR_VERSION < MPFR_VERSION_NUM(2,2,0))
int
mpfr_root (mpfr_ptr y, mpfr_srcptr x, unsigned long k, mp_rnd_t rnd_mode)
{
  mpz_t m;
  mp_exp_t e, r, sh;
  mp_prec_t n, size_m, tmp;
  int inexact, negative;

  if (k <= 1)
    {
      if (k < 1) /* k==0 => y=x^(1/0)=x^(+Inf) */
        {
          mpfr_set_nan (y);
          return 0;
        }
      else /* y =x^(1/1)=x */
        return mpfr_set (y, x, rnd_mode);
    }

  /* Singular values */
  else if (mpfr_nan_p (x) || mpfr_inf_p (x) || mpfr_zero_p (x))
    {
      if (mpfr_nan_p (x))
        {
          mpfr_set_nan (y); /* NaN^(1/k) = NaN */
          return 0;
        }
      else if (mpfr_inf_p (x)) /* +Inf^(1/k) = +Inf
                                   -Inf^(1/k) = -Inf if k odd
                                   -Inf^(1/k) = NaN if k even */
        {
          if (mpfr_sgn (x) < 0 && (k % 2 == 0))
            {
              mpfr_set_nan (y);
              return 0;
            }
          mpfr_set_inf (y, mpfr_sgn (x));
          return 0;
        }
      else /* x is necessarily 0: (+0)^(1/k) = +0
                                  (-0)^(1/k) = -0 */
        return mpfr_set (y, x, GMP_RNDN);
    }

  /* Returns NAN for x < 0 and k even */
  else if (mpfr_sgn (x) < 0 && (k % 2 == 0))
    {
      mpfr_set_nan (y);
      return 0;
    }

  /* General case */
  mpz_init (m);

  e = mpfr_get_z_exp (m, x);                /* x = m * 2^e */
  if ((negative = mpfr_sgn (x) < 0))
    mpz_neg (m, m);
  r = e % (mp_exp_t) k;
  if (r < 0)
    r += k; /* now r = e (mod k) with 0 <= e < r */
  /* x = (m*2^r) * 2^(e-r) where e-r is a multiple of k */

  size_m = mpz_sizeinbase (m, 2);
  /* for rounding to nearest, we want the round bit to be in the root */
  n = mpfr_get_prec (y) + (rnd_mode == GMP_RNDN);

  /* we now multiply m by 2^(r+k*sh) so that root(m,k) will give
     exactly n bits: we want k*(n-1)+1 <= size_m + k*sh + r <= k*n
     i.e. sh = floor ((kn-size_m-r)/k) */
  if ((mp_exp_t) size_m + (mp_exp_t) r > (mp_exp_t) k * (mp_exp_t) n)
    sh = 0; /* we already have too many bits */
  else
    sh = (k * (mp_exp_t) n - (mp_exp_t) size_m - r) / k;
  sh = k * sh + r;
  if (sh >= 0)
    {
      mpz_mul_2exp (m, m, sh);
      e = e - sh;
    }
  else if (r > 0)
    {
      mpz_mul_2exp (m, m, r);
      e = e - r;
    }

  /* invariant: x = m*2^e, with e divisible by k */

  /* we reuse the variable m to store the cube root, since it is not needed
     any more: we just need to know if the root is exact */
  inexact = mpz_root (m, m, k) == 0;

  tmp = mpz_sizeinbase (m, 2);
  sh = tmp - n;
  if (sh > 0) /* we have to flush to 0 the last sh bits from m */
    {
      inexact = inexact || ((mp_exp_t) mpz_scan1 (m, 0) < sh);
      mpz_div_2exp (m, m, sh);
      e += k * sh;
    }

  if (inexact)
    {
      if (negative)
#define MPFR_INVERT_RND(rnd) ((rnd == GMP_RNDU) ? GMP_RNDD : \
                             ((rnd == GMP_RNDD) ? GMP_RNDU : rnd))
        rnd_mode = MPFR_INVERT_RND (rnd_mode);
      if (rnd_mode == GMP_RNDU
          || (rnd_mode == GMP_RNDN && mpz_tstbit (m, 0)))
        inexact = 1, mpz_add_ui (m, m, 1);
      else
        inexact = -1;
    }

  /* either inexact is not zero, and the conversion is exact, i.e. inexact
     is not changed; or inexact=0, and inexact is set only when
     rnd_mode=GMP_RNDN and bit (n+1) from m is 1 */
  inexact += mpfr_set_z (y, m, GMP_RNDN);
  mpfr_set_exp (y, mpfr_get_exp (y) + e / (mp_exp_t) k);

  if (negative)
    {
      mpfr_neg (y, y, GMP_RNDN);
      inexact = -inexact;
    }

  mpz_clear (m);
  return mpfr_check_range (y, inexact, rnd_mode);
}
#endif
#endif
