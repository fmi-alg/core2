#define CORE_LEVEL 4
#include<stack>
#include "CORE/CORE.h"
#include "readpoly.h"
// #include "localDescartes.h"

typedef BigInt NT;
typedef Polynomial<NT> Poly;

using namespace CORE;
using namespace std;


template <typename T>
int numberOfRoots(Polynomial<T> &P){

  BFVecInterval v;
  isolateRoots(P, v);
  return v.size();
}

//Count the number of real roots of P in I
template <typename T>
int numberOfRoots(Polynomial<T> &P, BigFloat x, BigFloat y){

  BFVecInterval v;
  BFInterval I(x,y);
  isolateRoots(P, I, v);
  return v.size();
}

//Counts the number of roots strictly greater than a.
//To do this we just isolate the roots in the interval
//(a,B) where B is an upper bound on the roots.
template <typename T>
int numberOfRootsAbove(Polynomial<T> &P, BigFloat a){
  BigInt B = P.CauchyBound();

  //Check if a is a root
  if(P.eval(a) == 0)
    a += P.sepBound().div2();

  if(a >= B)
    return 0;

  BFInterval I(a, B);
  BFVecInterval v;
  isolateRoots(P, I, v);
  return v.size();
}


//Counts the number of roots strictly smaller than a.
//To do this we just isolate the roots in the interval
//(-B, a) where B is an upper bound on the roots.
template <typename T>
int numberOfRootsBelow(Polynomial<T> &P, BigFloat a){
  BigInt B = P.CauchyBound();
  //Check if a is a root
  if(P.eval(a) == 0)
    a -= P.sepBound().div2();

  if(a <= -B)
    return 0;
  
  BFVecInterval v;
  BFInterval I(-B, a);
  isolateRoots(P, I, v);
  return v.size();
}

// isolateRoot(i, x, y)
///   isolates the i-th smallest root in [x,y]
/**   If i is negative, then we want the i-th largest root in [x,y]
 *    We assume i is not zero.
 */
//To do this we isolate the roots in the interval I
//and return the i'th interval from the beginning
//or the end depending upon the sign of i; this can be
//done since the list of intervals is already sorted.
// We also guarantee that the interval returned does not
// contain a zero within it, unless the desired root is itself zero.

template <typename T>
BFInterval isolateRoot(Polynomial<T> &P, int i, BigFloat x, BigFloat y){
  BFVecInterval v;

  // We isolate the positive and negative roots separately, thus ensuring
  // that zero is not contained within an interval.
  if(sign(x) == sign(y))
    isolateRoots(P, BFInterval(x,y), v);
  else{
    isolateRoots(P, BFInterval(x, 0), v);
    if(P.coeff()[0] == T(0)) // zero is a root of P
      v.erase(v.end() -1, v.end()); // erase the entry corresponding to zero in
                                    // v since the next call we add it again
    isolateRoots(P, BFInterval(0, y), v);
  }
  
  int n= v.size(); //the precise number of real roots in I
  if (i < 0) {//then we want the n-i+1 root
    i += n+1;
    if (i <= 0)
      return BFInterval(1,0); // ERROR CONDITION
  }
  if(i > n)
    return BFInterval(1,0);  // ERROR CONDITION INDICATED

  //Now 0< i <= n
  if (n == 1) {// Thus there is only one root in (x,y). Moreover, the way
               // we isolated the root we are sure that the interval in v
               // does not contain any zero. 
    return *(v.begin());
  }

  //Otherwise traverse v and return the i'th interval
  if(i == n)//slight optimization
    return (*(v.end()-1));

  //Now 1 <= i < n
  int count =1;
  for (BFVecInterval::const_iterator it = v.begin(); ; ++it) {
    if(count == i)
      return (*it);
    else
      count ++;
  }
}


// First root above
template <typename T>
BFInterval firstRootAbove(Polynomial<T> P, const BigFloat &e){
  return isolateRoot(P, 1, e, P.CauchyBound());
}

template <typename T>
BFInterval firstRootBelow(Polynomial<T> P, const BigFloat &e){
  return isolateRoot(P, -1, -1*P.CauchyBound(), e);
}


//Given the count n of the number of roots, this function isolates
//the roots of P and checks if they are equal to n.
CORE_INLINE void testDescartes(Poly& P, int n = -1) {

  BRVecInterval v;
  Descartes::isolateRoots(P,v);
  std::cout << "   Number of roots is " << v.size() <<std::endl;
  if ((n >= 0) & (v.size() == (unsigned)n))
    std::cout << " (CORRECT!)" << std::endl;
  else
    std::cout << " (ERROR!) " << std::endl;
  int i = 0;
  for (BRVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
}// testDescartes


// Construct the Mignotte polynomial X^n - 2(aX-1)^2
void mignotte(Poly& P, int n, BigInt a)
{
  P = Poly(n); //X^n
  P.coeff[n]=1;P.coeff[0]=-2;
  P.coeff[1]=4*a;
  P.coeff[2]= -2*a*a;
}
// Construct the Mignotte polynomial X^n - (aX-1)^2
void mignotte2(Poly& P, int n, BigInt a)
{
  P = Poly(n); //X^n
  P.coeff[n]=1;P.coeff[0]=-1;
  P.coeff[1]=2*a;
  P.coeff[2]= -a*a;
}

int main( int argc, char* argv[] ) {
  /* ************************************** */
  /* Read Arguments */
  /* ************************************** */
  Poly P;
  // Read a polynomial from an MPSOLVE file defining a polynomial
  if (argc > 1)
    read_poly(argv[1], P);
  else
    read_poly("../data/polynomials/wilk20.pol", P);

  
  //read_poly("../data/polynomials/hermite20.pol", P);
  //read_poly("../data/polynomials/geom3_20.pol", P);
  //read_poly("../data/polynomials/mig1_20.pol", P);
  //read_poly("../data/polynomials/lsr_24.pol", P);
  //<Mpsolve fails to correctly isolate the real roots of this polynomial; instead
  // of four roots it shows 10, out of which 8 have very small non-zero imaginary
  //parts.

  //  P = Poly("87*x^27-27*x^28-3*x^29 + x^30+2048-3072*x-19968*x^2  + 31488*x^3 + 87552*x^4-147072*x^5-227008*x^6  + 414048*x^7 + 384072*x^8-783132*x^9-438966*x^10  + 1050015*x^11 + 334205*x^12-1026315*x^13-151695*x^14  + 740700*x^15 + 16740*x^16-395460*x^17 + 28480*x^18  + 155010*x^19-22458*x^20-43818*x^21 + 8838*x^22  + 8652*x^23-2132*x^24-1128*x^25 + 318*x^26");//1040426545468746*x^29-1875439720171204775529152399018166004054734672*x^2+3950956612561163785438507398864*x-2080853090937492");
  P.dump();
  //-2 + 95695313363608*x-1144699124969891483852347208*x^2  + x^20");//Poly("x^3 - 3*x^2 + 2*x"); Polynomial with zero as a root
  BRVecInterval v;
  BFInterval I;
  if(argc > 3 )
    I=BFInterval(argv[2], argv[3]);

  int Iter =0;
  if(argc > 4)
    Iter = atoi(argv[4]);

  if(Iter > 0)
    testDescartes(P, Iter);
  else
    {
      //P = Polynomial<NT>("x^4-15*x^3+57*x^2-73*x+30");
      //P = Polynomial<NT>("(1000000000*(x-1)^2+1)");
      //P.mulScalar(NT("100000000000"));
      //P.coeff[0]+=1;

      Timer t1;
      t1.start();
      Descartes::isolateRoots(P, v);
      t1.stop();
      std::cout << "Size of recursion tree = " << COUNTER;
      std::cout << " Time " << t1.getSeconds();
      std::cout << " Time per evaluation " << t1.getSeconds()/COUNTER << std::endl;

      int i=0;
      if(v.size() == 0)
	cout << "No Roots" << endl;
      else{
	for (BRVecInterval::const_iterator it = v.begin();
	     it != v.end(); ++it) {
	  std::cout << ++i << "th Root is in ["
		    << it->first.doubleValue() << " ; " 
		    << it->second.doubleValue() << "]" << std::endl;
	}
      }
    }

  
  
  return 0;
}
