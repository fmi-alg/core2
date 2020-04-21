// use to test 
#include <CORE/Mpfr.h>
#include <CORE/Timer.h>
#include <iostream>

void verify(const Mpfr& z, const Mpfr& z_approx) {
  Mpfr diff;
  diff.set_prec(z.get_prec()+1);
  diff.sub(z_approx, z);
  std::cout << "correct bits = " << diff.uMSB() << std::endl;
}
  
// using error analysis to computing z=x*y;
void add1(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;

  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(prec+1 - 10);
    r.add(x, y);
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}
// using exact ring operation to computing z=x*y; w/ remove zeros
void add2(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;
 
  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(std::max(x.get_prec(), y.get_prec() + 13) + 1 - 10);
    r.add(x, y);
    r.remove_trailing_zeros();
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}

// using exact ring operation to computing z=x*y; w/o remove zeros
void add3(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;
 
  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(std::max(x.get_prec(), y.get_prec() + 13) + 1 - 10);
    r.add(x, y);
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}

// using exact ring operation to computing z=x*y; w/o remove zeros + pre-comput
void add4(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;
  prec_t p = std::max(x.get_prec(), y.get_prec() + 13) + 1 - 10;
 
  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(p);
    r.add(x, y);
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}

int main(int argc, char* argv[]) {
  prec_t prec = 5;
  int times = 100;
  
  if (argc > 1)
    prec = atoi(argv[1]);
  if (argc > 2)
    times = atoi(argv[2]);
  
  Mpfr x, y, z;
  x.set_prec(prec+2 + 3); 
  y.set_prec(prec+2 - 10);
  z.set_prec(prec - 10); 

  x.set(BigRat("1/7"));
  y.set(BigRat("10241/10"));
  z.set(BigRat("71697/10"));

  //std::cout << "z=" << z.get_str(0, 2) << std::endl;

  // using error analysis to computing z;
  add1(x, y, z, prec, times);

  // using exact ring operation to computing z;
  add2(x, y, z, prec, times);
  add3(x, y, z, prec, times);
  add4(x, y, z, prec, times);

  return 0;
}
