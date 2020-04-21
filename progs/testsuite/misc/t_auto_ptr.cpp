/* test returned value from most MPFR functions
   in MPFR's documentation, page 7:

   unless documented otherwise, functions returning an int return a ternary value.
   if the ternary value is zero, it means the value stored in the destination variable
   is the exact result of the corresponding mathematical function. If the ternary value
   is positive (resp. negative), it means the value stored in the dstination variable
   is greater (resp. lower) than the exact result.

   This example shows this feature:
	
	taking the 4th root of "81" returns "3" exactly.

*/

#include <memory>
#include <iostream>

int main(int argc, char* argv[]) {
  std::auto_ptr<int> p(new int);
  std::cout << "p=" << *p << std::endl;
  if (p.get() == NULL)
    std::cout << "null" << std::endl;
  else
    std::cout << "not null" << std::endl;

  return 0;
}
