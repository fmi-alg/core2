// Simple Example of Core with STL
//

#include <iostream>
#include <queue>
#include "CORE/CORE.h"

using namespace std;

// Simple example uses type double (i.e. Expr)

int main()
{
  priority_queue<double> Q;
  Q.push(sqrt(1.0));
  Q.push(sqrt(8.0));
  Q.push(sqrt(2.0));
  Q.push(sqrt(7.0));
  Q.push(sqrt(3.0));
  
  assert(Q.size() == 5);
  cout << "Size = " << Q.size() << endl;

  assert(Q.top() == sqrt(8.0));
  cout << "top = " << Q.top() << endl;
  Q.pop();

  assert(Q.top() == sqrt(7.0));
  cout << "top = " << Q.top() << endl;
  Q.pop();

  assert(Q.top() == sqrt(3.0));
  cout << "top = " << Q.top() << endl;
  Q.pop();

  assert(Q.top() == sqrt(2.0));
  cout << "top = " << Q.top() << endl;
  Q.pop();

  assert(Q.top() == 1);
  cout << "top = " << Q.top() << endl;
  Q.pop();

  assert(Q.empty());
  cout << "size = " << Q.size() << endl;

   return 0;
}

