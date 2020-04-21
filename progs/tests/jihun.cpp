/*
 * Brendan Linn and Jihun Yu
 *
 * Experiments on the algorithm which combines compensated summation and priority queue.
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
void print(vector<double> &d);

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

  std::cout << "exact sum" << std::endl
	    << "sum = " << mpf_get_d(ans) << std::endl;
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

double comp_sum(vector<double> &d,double ans)
{
  double s = 0.0;
  double e = 0.0;
  double tmp;
  double a;
  for (vector<double>::iterator iter = d.begin();
       iter != d.end(); iter++) {
    tmp = s;
    a = *iter-e;
    s = tmp+a;
    e = (s-tmp)-a;
  }

  std::cout << "compensated_sum" << std::endl
	    << "sum = " << s << std::endl
	    << "err = " << e << std::endl
	    << "dif = " << (ans-s)-e << std::endl;
  return s;
}

double comp_sorted_sum(vector<double>& d,double ans)
{
  double cur_sum = 0.0; 	//sum
  double err = 0.0;		//error
  double old_sum;		 
  double a;

  sort(d.begin(), d.end());

  for (vector<double>::iterator iter = d.begin();
       iter != d.end(); iter++) {
    old_sum = cur_sum;
    a = *iter-err;
    cur_sum = old_sum+a;
    err = (cur_sum-old_sum)-a;
  }
  std::cout << "compensated_sorted_sum" << std::endl
	    << "sum = " << cur_sum << std::endl
	    << "err = " << err << std::endl
	    << "dif = " << (ans-cur_sum)-err << std::endl;
  return cur_sum;
}

double priority_sum(vector<double>& d,double ans)
{
  double cur_sum = 0.0; 	//sum
  double err = 0.0;		//error
  double old_sum;		 
  double a;
  double first, second;

  while(d.size() > 1) {
    sort(d.begin(), d.end());
    first = d[0];
    second = d[1]; 
 
    //assert(first<=second);
    // Orig:
    sum = first+second;
    // New:
    old_sum = second;
    a = first-err;
    cur_sum=old_sum+a;
    err=(cur_sum-old_sum)-a;
    // :End New
    d.erase(d.begin());
    d.erase(d.begin());
    d.push_back(cur_sum);
  }

  //assert(d.size()==1);
  // New:
  std::cout << "priority_sum" << std::endl
	    << "sum = " << d[0] << std::endl
	    << "err = " << err << std::endl
	    << "dif = " << (ans-d[0])-err << std::endl;
  assert(d.size()==1);
  // :End New

  return d[0];
}

double comp_priority_sum(vector<double>& d)
{
  double cur_sum = 0.0; 	//sum
  double err = 0.0;		//error
  double a;
  vector<double> e;

  while(d.size() > 1) {
    sort(d.begin(), d.end());
    double first = d[0];
    double second = d[1];
    // Old:
    //double third = d[2];
    //double fourth = d[3];
    //assert(second<=third);
    //assert(third<=fourth);
    // New:
    assert(first<=second);
    // :End New
 
    cur_sum = second+first;
    err = second-cur_sum+first;
    if(err != 0)
      e.push_back(err);
    d.push_back(cur_sum);

   // Old:
   // cur_sum = fourth+third;
   // err += fourth-cur_sum+third;
   // d.push_back(cur_sum);
   // d.push_back(err);

   // d.erase(d.begin());
   // d.erase(d.begin());
    d.erase(d.begin());
    d.erase(d.begin());
  }

  // Old:
  // //assert(d.size()==3);
  //  
  // return d[0]+d[1]+d[2];
  // New:
  assert(d.size()==1);
  return d[0];
  // :End New
}

void print(vector<double> &d) {
  for(unsigned int i=0;i<d.size();i++)
 	std::cout << "d["<<i<<"]"<<d[i]<<std::endl;
}
int main(int argc, char **argv)
{
  if (argc != 2)
    printf("usage: %s n\n", argv[0]);
  else {
    struct rusage before, after;
    int len = atoi(argv[1]);
    double ntime(0), ctime(0), atime(0), cstime(0), prtime(0), cptime(0);
    double n(0), c(0), a(0), cs(0), pr(0), cp(0);
    int num_trials = 1;
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
      c += fabs(a-comp_sum(d,a))/a;
      getrusage(RUSAGE_SELF, &after);
      ctime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
	+ float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;

      getrusage(RUSAGE_SELF, &before);
      cs += fabs(a-comp_sorted_sum(d,a))/a;
      getrusage(RUSAGE_SELF, &after);
      cstime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
	+ float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;

      getrusage(RUSAGE_SELF, &before);
      pr += fabs(a-priority_sum(d,a))/a;
      getrusage(RUSAGE_SELF, &after);
      prtime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
	+ float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;
/*
      getrusage(RUSAGE_SELF, &before);
      cp += fabs(a-comp_priority_sum(d))/a;
      getrusage(RUSAGE_SELF, &after);
      cptime += after.ru_utime.tv_sec-before.ru_utime.tv_sec\
      + float(after.ru_utime.tv_usec-before.ru_utime.tv_usec)/1000000.;
*/   }

    printf("n=%d\n", len);
    printf("naive sum:       mean rel. error %.20f, mean time %f\n", n/num_trials, ntime/num_trials);
    printf("compensated sum: mean rel. error %.20f, mean time %f\n", c/num_trials, ctime/num_trials);
    printf("comp & sort sum: mean rel. error %.20f, mean time %f\n", cs/num_trials, cstime/num_trials);
  //  printf("priorityque sum: mean rel. error %.20f, mean time %f\n", pr/num_trials, prtime/num_trials);
 //   printf("comp & prio sum: mean rel. error %.20f, mean time %f\n", cp/num_trials, cptime/num_trials);
  }
}

