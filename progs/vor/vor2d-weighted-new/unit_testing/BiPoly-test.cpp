#include <assert.h>
#include <iostream>
#include "../BiPoly.h"

void test_creation() {
  BiPoly p;
  p.add_monomial(1, 0, 0);
}

void test_mul() {
  BiPoly p;
  p.add_monomial(1, 1, 0);
  p.add_monomial(1, 0, 0);

  BiPoly q;
  q.add_monomial(1, 0, 1);
  
  BiPoly r;
  r.add_monomial(1, 0, 1);
  r.add_monomial(1, 1, 1);

  BiPoly pq = p * q;
  
  assert(r == pq);
}

void test_xdir() {
  BiPoly p;
  p.add_monomial(1, 2, 0);

  BiPoly px;
  px.add_monomial(2, 1, 0);

  assert(p.partial_x() == px);
}

void test_ydir() {
  BiPoly p;
  p.add_monomial(1, 0, 2);

  BiPoly py;
  py.add_monomial(2, 0, 1);

  assert(p.partial_y() == py);
}

void test_grad() {
  BiPoly p;
  p.add_monomial(1, 1, 0);
  p.add_monomial(2, 0, 1);

  BiPoly px;
  px.add_monomial(1, 0, 0);

  BiPoly py;
  py.add_monomial(2, 0, 0);

  pair<BiPoly, BiPoly> pgrad = p.gradient();
  
  assert(pgrad.first == px);
  assert(pgrad.second == py);
}

void test_neg() {
  BiPoly p;
  p.add_monomial(1, 1, 0);
  p.add_monomial(2, 0, 1);

  BiPoly pn;
  p.add_monomial(-1, 1, 0);
  p.add_monomial(-2, 0, 1);

  assert(-1 * p == pn);
}

int main(int argc, char** argv) {
  test_neg();
  test_creation();
  test_mul();
  test_xdir();
  test_ydir();
  test_grad();
  cout << "All tests passed.\n";
  return 0;
}
