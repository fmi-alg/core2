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
void mul1(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;

  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(prec+1);
    r.mul(x, y);
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}
// using exact ring operation to computing z=x*y; w/ remove zeros
void mul2(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;
 
  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(x.get_prec() + y.get_prec());
    r.mul(x, y);
    r.remove_trailing_zeros();
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}

// using exact ring operation to computing z=x*y; w/o remove zeros
void mul3(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;
 
  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(x.get_prec() + y.get_prec());
    r.mul(x, y);
  }
  timer.stop();
  //std::cout << "r=" << r.get_str(0, 2) << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << std::endl;
  verify(z, r);
}

// using exact ring operation to computing z=x*y; w/o remove zeros + pre-comput
void mul4(const Mpfr& x, const Mpfr& y, const Mpfr& z, prec_t prec, int times) {
  Mpfr r;
  prec_t p = x.get_prec() + y.get_prec();
 
  Timer2 timer;
  timer.start();
  for (int i=0; i<times; ++i) {
    r.set_prec(p);
    r.mul(x, y);
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
  x.set_prec(prec+2); 
  y.set_prec(prec+2);
  z.set_prec(prec); 

  // first example
  //x.set(BigRat("1/3"));
  //y.set(BigRat("2/3"));
  //z.set(BigRat("2/9"));

  // second example
  x.sqrt(2);
  y.sqrt(3);
  z.sqrt(6);
  //std::cout << "z=" << z.get_str(0, 2) << std::endl;

  // using error analysis to computing z;
  mul1(x, y, z, prec, times);

  // using exact ring operation to computing z;
  mul2(x, y, z, prec, times);
  mul3(x, y, z, prec, times);
  mul4(x, y, z, prec, times);

  return 0;
}
