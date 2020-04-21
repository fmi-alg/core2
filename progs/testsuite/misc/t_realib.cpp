/*
 * Usage:
 * 	t_realib [method] [iterations]
 *
 */
#ifdef CORE_LEVEL
#undef CORE_LEVEL
#endif

#define CORE_LEVEL 4

#include <iostream>
#include <CORE/CORE.h>
#include <CORE/Timer.h>

using namespace std;

void bf_example1(int num) {
  Timer2 timer;

  BigFloat2 s(0.0);
  BigFloat2 one(1.0);
  timer.start();
  for (int i=1; i<=num; ++i)
    s += i;
  timer.stop();
  cout << "s=" << s << endl;
  cout << "s.get_prec()=" << s.get_prec() << endl;
  cout << "It took " << timer.get_mseconds() << " mseconds." << endl;
}

void example1(int num) {
  Timer2 timer;

  Expr s(0.0);
  Expr one(1.0);
  timer.start();  
  for (int i=1; i<=num; ++i)
    s += one / i;
  BigFloat2 r = s.r_approx(60);
  timer.stop();
  cout << "r=" << r << endl;
  cout << "It took " << timer.get_mseconds() << " mseconds." << endl;
}

void example2(int num) {
  Timer2 timer;

  Expr s(0.5);
  Expr coeff(3.75);
  Expr one(1.0);
  timer.start();  
  for (int i=1; i<=num; ++i) {
    s = coeff * (one - s) * s;
    s.sign();
  }
  BigFloat2 r = s.r_approx(53);
  timer.stop();
  cout << "r=" << r << endl;
  cout << "It took " << timer.get_mseconds() << " mseconds." << endl;
}

int main(int argc, char** argv) {
  int num = 1000;
  int method = -1;

  if (argc > 1)
    method = atoi(argv[1]);
  if (argc > 2)
    num = atoi(argv[2]);
  
  switch(method){
    case(0):
    	bf_example1(num);
	break;
    case(1):
    	example1(num);
	break;
    case(2):
    	example2(num);
	break;
    case(-1):
    	bf_example1(num);
    	example1(num);
    	example2(num);
	break;
    default:
	cout << "Usage: t_realib [method] [iterations]" << endl;
  }	
  return 0;
}//main

