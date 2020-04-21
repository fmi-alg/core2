//
// test all functions of BigRat
//
#ifdef CORE_LEVEL
#undef CORE_LEVEL
#endif
#define CORE_LEVEL 4
#include <CORE/CORE.h>
#include <iostream>

using namespace std;

typedef BigRat NT;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

int main() {
  // testing constructors
  NT x0;
  NT xx(x0);
  NT x1(char(1));
  NT x2(uchar(1));
  NT x3(short(1));
  NT x4(ushort(1));
  NT x5(int(1));
  NT x6(uint(1));
  NT x7(long(1));
  NT x8(ulong(1));
  NT x9(1.0f);
  NT x10(1.0);
  NT x11("12345");
  NT x12(string("12345"));
  
  // test assignment
  x0.set(xx);
  x1.set(char(1));
  x2.set(uchar(1));
  x3.set(short(1));
  x4.set(ushort(1));
  x5.set(int(1));
  x6.set(uint(1));
  x7.set(long(1));
  x8.set(ulong(1));
  x9.set(1.0f);
  x10.set(1.0);
  x11.set("12345");
  x12.set(string("12345"));

  // test conversion functions

  // test arithmetic functions
  xx.add(x0, char(1));
  xx.add(x0, uchar(1));
  xx.add(x0, short(1));
  xx.add(x0, ushort(1));
  xx.add(x0, int(1));
  xx.add(x0, uint(1));
  xx.add(x0, long(1));
  xx.add(x0, ulong(1));
  xx.add(x0, 1.0f);
  xx.add(x0, 2.3);

  xx.add(char(1), x0);
  xx.add(uchar(1), x0);
  xx.add(short(1), x0);
  xx.add(ushort(1), x0);
  xx.add(int(1), x0);
  xx.add(uint(1), x0);
  xx.add(long(1), x0);
  xx.add(ulong(1), x0);
  xx.add(1.0f, x0);
  xx.add(2.3, x0);

  // test c++ operator
  NT x(210);
  NT y(10);
  NT z;

  y++;
  cout << "y=" << y << endl;
  ++y;
  cout << "y=" << y << endl;
  x += 2000;
  cout << "x=" << x << endl;
  x += y;
  cout << "x=" << x << endl;
  
  cout << "z=x" << endl;
  z = x;
  cout << "z=x" << endl;

  z = x + y;
  cout << "z=" << z << endl;
  z = x + 100;
  cout << "z=" << z << endl;
  z = 100 + x;
  cout << "z=" << z << endl;
  cout << "x=" << x << endl;
  cout << "y=" << y << endl;
  
  // test comparison operator
  bool b;
  b = x == y;
  cout << "True or False? " << x << " == " << y << " ? " << b << endl;
  b = x == 100;
  cout << "True or False? " << x << " == 100 ? " << b << endl;
  b = 100 == x;
  cout << "True or False? 100 == " << x << " ? " << b << endl;

  BigInt zz = x.get_z();
  return 0;
}

