#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include <CORE.h>

using namespace std;

void add_wo_removing_zeros(BigFloat& sum, const BigFloat& y) {
  prec_t prec = sum.add_prec(sum,y);
  sum.add(sum,y,prec);  
}
void add_w_removing_zeros(BigFloat& sum, const BigFloat& y) {
  add_wo_removing_zeros(sum,y);
  sum.remove_trailing_zeros();
}

void mul_wo_removing_zeros(BigFloat& prod, const BigFloat& y) {
  prod.mul(prod,y,prod.mul_prec(prod,y));  
}
void mul_w_removing_zeros(BigFloat& prod, const BigFloat& y) {
  mul_wo_removing_zeros(prod,y);
  prod.remove_trailing_zeros();
}

int main(int argc, char* argv[]) {

  set_output_precision(10);

  int n=100;
  int removing=0;
  int add=0;
  if(argc > 1)
    n=atoi(argv[1]);

  if(argc > 2)
    removing=atoi(argv[2]);

  if(argc > 3)
    add=atoi(argv[3]);

  if(n < 2) {
    cerr << "Input parameter is wrong!!!. n must be greater than 1."
         << endl;
  }

  // initialize bigfloat numbers
  BigFloat* arrayNum = new BigFloat[n-1];
  for(int i=2;i<=n;++i) {
    arrayNum[i-2] = i;
    BigInt bi(i);
    arrayNum[i-2].prec_round(bi.ceillg());
  }

  BigFloat sum(1); 
  sum.prec_round(2);

  Timer2 timer; timer.start();
  if(removing) {
     for(int i=2;i<=n;++i) {
       if(add)  
         add_w_removing_zeros(sum,arrayNum[i-2]);
        else
         mul_w_removing_zeros(sum,arrayNum[i-2]);
    }
  } else {
    for(int i=2;i<=n;++i) {  
       if(add)  
         add_wo_removing_zeros(sum,arrayNum[i-2]);
       else
         mul_wo_removing_zeros(sum,arrayNum[i-2]);
    }
  }
  timer.stop();

  delete[] arrayNum;

  // output results
  cout << "Computing the " << (!add ? "prod" : "sum") << " from 1 to " << n
       << (removing ? " w " : " w/o ") << "removing zeros" << endl;
//  cout << "prod = " << sum << endl;
  cout << "Estimated Prec = " << sum.get_prec() << endl;
  cout << "Timing = " << timer.get_mseconds() << endl << endl;

  return 0;
}
