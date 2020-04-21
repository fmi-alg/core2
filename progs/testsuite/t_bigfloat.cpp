#ifdef CORE_LEVEL
#undef CORE_LEVEL
#endif
#define CORE_LEVEL 4
#include <CORE/CORE.h>
#include <iostream>

using namespace std;

typedef BigFloat2 NT;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

void show_msg(const char* msg)
{ cout << msg << endl; }

template <typename T>
void test_constructor(const T& v, const char* type) {
  NT x(v);
  cout << "constructor for " << type << ": x=" << x << endl; 
  NT xx(v, 10);
  cout << "constructor for " << type << " with prec=10: xx=" << xx << endl; 
}

template <typename T>
void test_set(const T& v, const char* type) {
  NT x, xx;
  x.set(v);
  cout << "set for " << type << ": x=" << x << endl; 
  xx.set(v, 10);
  cout << "set with prec=10 for " << type << ": xx=" << xx << endl; 
}

void test_constructors() {
  show_msg("test constructors");
  NT x;
  cout << "default constructor: x=" << x << endl; 
  test_constructor<char>(1, "char");
  test_constructor<uchar>(2, "uchar");
  test_constructor<short>(3, "short");
  test_constructor<ushort>(4, "ushort");
  test_constructor<int>(5, "int");
  test_constructor<uint>(6, "uint");
  test_constructor<long>(7, "long");
  test_constructor<ulong>(8, "ulong");
  test_constructor<float>(0.1, "float");
  test_constructor<double>(0.3, "double");
  test_constructor<BigInt>(1234567, "BigInt");
  test_constructor<BigRat>(BigRat(1, 3), "BigRat");
  test_constructor<BigFloat>(0.01, "BigFloat");

  show_msg("test assignment functions");
  test_set<char>(1, "char");
  test_set<uchar>(2, "uchar");
  test_set<short>(3, "short");
  test_set<ushort>(4, "ushort");
  test_set<int>(5, "int");
  test_set<uint>(6, "uint");
  test_set<long>(7, "long");
  test_set<ulong>(8, "ulong");
  test_set<float>(0.1, "float");
  test_set<double>(0.3, "double");
  test_set<BigInt>(1234567, "BigInt");
  test_set<BigRat>(BigRat(1, 3), "BigRat");
  test_set<BigFloat>(0.01, "BigFloat");
  x.set_prec(40);
  x.set("1234.567890");
  cout << "set for const char*: x=" << x << endl; 
  x.set("1234.567890", 10, 10);
  cout << "set with prec=10 for const char*: x=" << x << endl; 
}

int main() {
  BigInt zz(1234567);
  BigFloat f(zz, 10);
  cout << "f=" << f << endl;

  test_constructors();

  NT x(100);
  NT y(200);
  NT z(100);

  cout << "test auto version" << endl;
  z.add(x, y);
  cout << "z=" << z << endl;
  z.add(x, 100);
  cout << "z=" << z << endl;
  z.add(100, y);
  cout << "z=" << z << endl;

  cout << "test fixed version" << endl;
  z.add(x, y, 100);
  cout << "z=" << z << endl;
  z.add(x, 100, 100);
  cout << "z=" << z << endl;
  z.add(100, y, 100);
  cout << "z=" << z << endl;

  cout << "test raw version" << endl;
  z.set_prec(100);
  z.r_add(x, y);
  cout << "z=" << z << endl;
  z.r_add(x, 100);
  cout << "z=" << z << endl;
  z.r_add(100, y);
  cout << "z=" << z << endl;

/*
  Mpfr ff;
  //ff.set_prec(Mpfr::count_prec("1234.567890", 10));
  for (int p = 5; p<20; p++) {
  ff.set_prec(p);
  ff.set("1234.567890", 10, GMP_RNDD);
  cout << ff << endl;
  cout << ff.get_str(0, 2) << endl;
  ff.set("1234.567890", 10, GMP_RNDU);
  cout << ff << endl;
  cout << ff.get_str(0, 2) << endl;
  }
*/

  return 0;
}

