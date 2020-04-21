#include <gmp.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
   int base = 10;
   mpz_t x;

   mpz_init_set_ui(x, 100000);
   int len = mpz_sizeinbase(x, base);
   char* s = new char[len];
   mpz_get_str(s, base, x);
   std::string str(s);
   delete[] s;
  
   std::cout << "str=" << str << std::endl;

   mpz_clear(x);

  return 0;
}
