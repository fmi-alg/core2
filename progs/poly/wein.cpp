/*
Date: Wed, 20 Oct 2004 09:11:17 +0200 (IST)
From: Ron Wein <wein@post.tau.ac.il>
To: Zilin Du <zilin@cs.nyu.edu>, Chee Yap <yap@cs.nyu.edu>
cc: Andreas Fabri <Andreas.Fabri@geometryfactory.com>, <sharma@cs.nyu.edu>,
        <Sylvain.Pion@sophia.inria.fr>, Efraim Fogel <efif@post.tau.ac.il>
Subject: Re: A Suspected Bug in CORE

Dear Chee, Zilin,

We installed the latest version of CORE here, but the same problem 
re-occurs. Actually, I see now that the problem is now also with the
call to numberOfRoots(), as we do not get to step (3) due to the
following assertion violation:

CORE/BigFloat.h:214: int CORE::BigFloat::sign() const: 
Assertion `!(isZeroIn())' failed.

If I set n_roots = 1 (and do not call numberOfRoots()), I still get
the same assertion violation when calling rootOf() in step (3):
*/

#include "CORE/CORE.h"

//typedef CORE::BigRat   CfNT;
typedef CORE::Expr   CfNT;
// The same happens when defining CORE::BigRat as CfNT ...

typedef CORE::Expr     CoNT;

int main()
{
  // (1) Set the polynomial:
  //     p(x) = x^3 + 1
  const int             deg = 4;
  std::vector<CfNT>     coeffs(deg + 1);

  coeffs[0] = CfNT("1/6");
  coeffs[1] = CfNT("0");
  coeffs[2] = CfNT("-5/6");
  coeffs[3] = CfNT(0);
  coeffs[4] = CfNT(1);
  // (2) Compute its number of real roots.
  CORE::Polynomial<CfNT>     p (coeffs);
  CORE::Sturm<CfNT>          sturm (p);

  CORE::Polynomial<BigInt>   pp("6x^4 -5 x^2 +1");
  CORE::Sturm<BigInt>        sturmm (pp);

  // const int                  n_roots = 1;//sturm.numberOfRoots();
  const int                  n_roots = sturm.numberOfRoots();
  const int                  nn_roots = sturmm.numberOfRoots();

  std::cout << "polynomial p is " << p << std::endl;
  std::cout << "polynomial pp is " << pp << std::endl;

  std::cout << "polynomial p should have " << n_roots
	  	<< " real root(s)." << std::endl;
  std::cout << "polynomial pp should have " << nn_roots
	  	<< " real root(s)." << std::endl;

  // (3) Compute the roots.
  std::vector<CoNT>          roots (n_roots);
  std::vector<CoNT>          rroots (nn_roots);
  int                        i;

  //std::cout << setprecision(100);

  std::cout << "The roots of p are:" << std::endl;
  for (i = 0; i < n_roots; i++)
  {
    roots[i] = rootOf(p, i + 1);
    std::cout << "    " << i + 1 << ")  " << roots[i] << std::endl;
  }

  std::cout << "The roots of pp are:" << std::endl;
  for (i = 0; i < nn_roots; i++)
  {
    rroots[i] = rootOf(pp, i + 1);
    std::cout << "    " << i + 1 << ")  " << rroots[i] << std::endl;
  }

  return (0);
}

/*
By the way, I managed to pass stage (2) successfully but fail in step (3)
when I used BigRat as CfNT and tried the polynomial (I was very glad to
find out that I can now use rootOf() on polynomials of rational numbers
as well):

  p(x) = x^4 - (5/6)x^2 + (1/6)

Best regards,
Ron

> Dear Andreas,
> Is this a new problem?  Can you please send us a file
> which reproduces the bug?
> 
> Thanks, Chee
> 
> Andreas Fabri wrote:
> 

*/

