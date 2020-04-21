/*
 * Brendan Linn
 * Computational Geometry homework 3, addendum:
 * Compensated Summation
 * 3/17/2009
 *
 * Comparison of naive floating-point summation with
 * 	compensated floating-point summation.
 * We use arbitrary-precision sums (from gmp) to compute
 * 	the relative errors. Sample runs:
 * 
 * n=10
 * naive sum: mean rel. error       0.00000000000000065298, mean time 0.000512
 * compensated sum: mean rel. error 0.00000000000000007870, mean time 0.000664
 * n=100
 * naive sum: mean rel. error       0.00000000000000286046, mean time 0.005828
 * compensated sum: mean rel. error 0.00000000000000008347, mean time 0.007168
 * n=1000
 * naive sum: mean rel. error       0.00000000000001534322, mean time 0.055527
 * compensated sum: mean rel. error 0.00000000000000007761, mean time 0.068784
 * n=10000
 * naive sum: mean rel. error       0.00000000000003842583, mean time 0.531717
 * compensated sum: mean rel. error 0.00000000000000008114, mean time 0.656421
 *
 * This is pretty cool: the mean relative error for the naive sum increases
 * linearly with n, but the mean relative error for the compensated sum
 * stays within the same order of magnitude.
 */

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <gmp.h>
#include <sys/resource.h>

using namespace std;

void rands(vector<double> &v, int n);

double arb_sum  (vector<double> &d);
double naive_sum(vector<double> &d);
double comp_sum (vector<double> &d);

void rands(vector<double> &v, int n)
{
  for (int i = 0; i < n; ++i)
    v.push_back(double(rand())/RAND_MAX);
}

double arb_sum(vector<double> &d)
{
  mpf_t ans, tmp; // Multiprecision floats
  mpf_init(ans);
  mpf_init(tmp);
  for (vector<double>::iterator iter = d.begin();
       iter != d.end(); iter++) {
    mpf_set_d(tmp,*iter);
    mpf_add(ans, ans, tmp);
  }
  return mpf_get_d(ans);
}

double naive_sum(vector<double> &d)
{
  double ans = 0.0;
  for (vector<double>::iterator iter = d.begin();
       iter != d.end(); iter++)
    ans += *iter;
  return ans;
}

double comp_sum(vector<double> &d)
{
  double s = 0.0;
  double e = 0.0;
  double tmp;
  double a;
  for (vector<double>::iterator iter = d.begin();
       iter != d.end(); iter++) {
    tmp = s;
    a = *iter+e;
    s = tmp+a;
    e = tmp-s+a;
  }
  return s;
}

int main(int argc, char **argv)
{
  if (argc != 2)
    printf("usage: %s n\n", argv[0]);
  else {
    struct rusage before, after;
    int len = atoi(argv[1]);
    double ntime, ctime, atime;
    double n, c, a;
    int num_trials = 1000;
    srand(time(NULL));
    vector<double> d;
    for (int i = 0; i < num_trials; ++i) {
      rands(d, len);

      getrusage(RUSAGE_SELF, &before);
      a = arb_sum(d);
      getrusage(RUSAGE_SELF, &after);
      atime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
	+ float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;

      getrusage(RUSAGE_SELF, &before);
      n += fabs(a-naive_sum(d))/a;
      getrusage(RUSAGE_SELF, &after);
      ntime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
	+ float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;


      getrusage(RUSAGE_SELF, &before);
      c += fabs(a-comp_sum(d))/a;
      getrusage(RUSAGE_SELF, &after);
      ctime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
	+ float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;
    }

    printf("n=%d\n", len);
    printf("naive sum:       mean rel. error %.20f, mean time %f\n", n/num_trials, ntime/num_trials);
    printf("compensated sum: mean rel. error %.20f, mean time %f\n", c/num_trials, ctime/num_trials);
  }
}

