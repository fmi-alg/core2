#include <assert.h>
#include <iostream>
#include "../Interval.h"

const Interval p(1, 2);
const Interval n(-5, -3);
const Interval m(-7, 11);

void test_un_minus() {
  Interval i = -m;
  assert(i.a_ == -11 && i.b_ == 7);
}

void test_mult() {
  Interval i = n * m;
  assert(i.a_ == -55 && i.b_ == 35);
}

void test_pow() {
  Interval i(1.0, 2.0);

  Interval i0 = i.pow(0);  
  Interval i1 = i.pow(1);    
  Interval i2 = i.pow(2);
  
  assert(i0.a_ == 1.0 && i0.b_ == 1.0);
  assert(i1.a_ == 1.0 && i1.b_ == 2.0);
  assert(i2.a_ == 1.0 && i2.b_ == 4.0);
}

int main(int argc, char** argv) {
  test_un_minus();
  test_mult();
  test_pow();
  cout << "All tests passed.\n";
  return 0;
}
