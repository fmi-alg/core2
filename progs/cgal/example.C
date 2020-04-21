
#include <iostream>
#include <CGAL_Expr.h> // From CORE
#include <CGAL/Cartesian.h>

typedef CORE::Expr            NT;
typedef CGAL::Cartesian<NT>   Kernel;
typedef Kernel::Point_2       Point_2;

int main()
{
  Point_2 A(NT("0.1"), NT("0.2")); // We use strings to ensure exact
  Point_2 B(NT("1.1"), NT("3.2")); // decimal representation.
  Point_2 C(NT("3.1"), NT("5.4"));

  // Compute the circumcenter of A, B, C.
  Point_2 D = CGAL::circumcenter(A, B, C);

  // Verify that D is at the same distance from A, B and C.
  NT AD = CGAL::squared_distance(A, D);
  NT BD = CGAL::squared_distance(B, D);
  NT CD = CGAL::squared_distance(C, D);

  if (AD != BD || AD != CD)
    std::cerr << "Not supposed to happen." << std::endl;
  else
    std::cout << "Exact computation." << std::endl;

  return 0;
}
