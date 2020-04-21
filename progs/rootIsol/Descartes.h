/*
This is an implementation a root isolation algorithm based
on Descartes Rule of Signs.
Author: Vikram Sharma
*/

#define CORE_LEVEL 4
#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;
typedef Polynomial<NT> Poly;

template <typename T>
void shift(T* coeff, int deg, T* shifted){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.

  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];
  
  for(int i=0; i<= deg-1;i++)
    for(int j=deg-1; j>=i; j--)
      shifted[j]+=shifted[j+1];

}

template <typename T>
void half(T* coeff, int deg, T* halved){
  for(int i=0; i<= deg; i++)
    halved[i] = coeff[i]*(BigInt(1)<<(deg-i));
}

//Given the coefficient sequence coeff of some polynomial P
//this computes the coefficient sequence contracted of the polynomial
//P(\lambda X). Since lambda can be of a different type than the
//coefficients of P we need introduce another typename to resolve this.
//We assume that T2 is more general than T1 so that the conversion from
// the latter to the former can take place without error.
template <typename T1, typename T2>
void contract(T1* coeff, int deg, T2 lambda, T2* contracted){
  T2 pow=1;
  for(int i=0; i <= deg ; i++){
    contracted[i] = coeff[i]*pow;
    pow*=lambda;
  }
}

//void shift(BigFloat* coeff, int deg, BigFloat lambda, BigFloat* shifted){}

//Computes the Taylor shift by a constant lambda. Confer the comments
//for contract above.
template <typename T1, typename T2>
void shift(T1* coeff, int deg, T2 lambda, T2* shifted){

  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];
  
  if(lambda != 0){
    for(int i=0; i<= deg-1;i++)
      for(int j=deg-1; j>=i; j--)
	shifted[j]+= lambda * shifted[j+1];
  }
}

template <typename T>
int shiftAndSigncount(T* coeff, int deg){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.
  //The advantage of this method is that it computes the coefficient of x^i
  //in n-i steps. Thus we can check for sign variation as we compute
  //the coefficients.

  //First reverse the polynomial
  T temp[deg + 1];
  for(int i=0; i<= deg ; i++)
    temp[i] = coeff[deg-i];

  
  int num=0, i;
  int lastsign =0;//The last non-zero sign
  int currsign;//The sign of the current coefficient

  //Compute the sequence of coefficients and simultaneously get
  //the number of sign variations. If the sign variations are greater
  //than one then break out of the loop, i.e. return num.
  for(i=0; i<= deg-1;i++){
    for(int j=deg-1; j>=i; j--)
      temp[j]+=temp[j+1];

    currsign = sign(temp[i]);
    if(currsign !=0){
      if (lastsign * currsign < 0) num++;
      lastsign = currsign;//lastsign is always non-zero except for the starting.
    }
     if(num > 1) return -1;
  }
  //To account the sign variation between the previous non-zero coefficient
  // and deg coeffecient.
  //This is done if the number of sign variations were less than or
  //equal to one from the above loop.
  if(lastsign*sign(temp[deg]) < 0) num++;
  //  if(sign(temp[deg-1])*sign(temp[deg]) < 0) num++;
  if(num ==2 ) return -1;
  return num;
}

/*
//Isolates real roots of P in the interval (0,1).
//The roots of P are in bijective correspondence with the roots of
//the original input polynomial P_{in}. More precisely,
//P has a root in (0,1) iff P_{in} has a root in I.
//We also ensure that the list of isolating intervals in v
//is sorted. The intervals in v are of two types:
//1) Both the end points of the interval are the same. This happens
//only if the end point is a root of P.
//2) Otherwise the interval represents an open interval which contains a root.
// Note, the end points may be roots but they will be distinct in this case.
//Advantage of this representation is that we don't have to compute the separation
//bound.
template <typename T>
void isolateRoots(Polynomial<T> P, const BFInterval I, int deg,
                    BFVecInterval &v) {
	  
	 int num = shiftAndSigncount(P.coeff(), deg);
	  //cout << "sign variations after shift " << num << endl;
	  //cout <<"Interval is ["<<I.first << ":"<< I.second << "]"<<endl;
	  //cout << "Polynomial is "; P.dump() ; cout <<endl;
	  if(num == 0) return;
	  else if(num == 1)
	    v.push_back(I);
	  else{
	    BigFloat m = I.second + I.first/2;
	    T* temp1 = new T[deg +1];
	    T* temp2 = new T[deg +1];
	
	    half(P.coeff(),deg, temp1);
	    Polynomial<T> Q(deg, temp1);
	    //    cout <<"After halving polynomial is "; Q.dump(); cout<<endl;
	    
	    shift(temp1, deg, temp2);
	    Polynomial<T> R(deg, temp2);
	    //    cout<<"Inside isolateRoots: second polynomial "<< R << endl;
	    
	    BFInterval J = std::make_pair(I.first, m);
	    BFInterval JJ = std::make_pair(m, I.second);
	    isolateRoots(Q, J, deg, v);
	    if(R.coeff()[0] == 0) 
	      v.push_back(std::make_pair(m,m));
	    isolateRoots(R, JJ, deg, v);
	  }
	}
*/

/*
//Isolates roots of P in the CLSOED interval I; assumes P is square-free.
//This is achieved by computing a polynomial Q whose roots in the
//unit interval are in bijective correspondence with the roots of
//P in I. More precisely, Q(X) = P((b-a)X + a) where I=(a,b).
//This is obtained by first computing the Taylor shift by a of the polynomial
//and then doing a contraction by b-a.
//Again v contains the sorted list of intervals.
template <typename T>
void isolateRoots(Polynomial<T>& P, BFInterval I, BFVecInterval& v){
	  int deg = P.getTrueDegree();
	  if(deg == 0)
	    cout<< "Polynomial is a constant" << endl;
	  
	  
	  BigFloat a = I.first, b=I.second;
	  
	  int n = P.getTrueDegree();
	  BigFloat temp1[n + 1], temp2[n+1];
	
	  shift(P.coeff(), n, a, temp1);
	  contract(temp1, n, b-a, temp2);
	
	
	
	  if(eval(P, a) == 0)
	    v.push_back(std::make_pair(a, a));
	
	  Polynomial<BigFloat> R(n, temp2);
	  //cout <<"Corresponding polynomial with roots in unit interval ";R.dump();
	  //cout <<endl;
	  
	  isolateRoots(R, I, n, v);
	
	 if(eval(P, b) == 0)
	    v.push_back(std::make_pair(b, b));
	
	}
*/

//Isolates all real roots of P. Assumes that the polynomial is square-free.
//v will contain the isolating intervals for P in sorted order.
//We calculate an upper bound B on the absolute value of the roots of P
// and call isolateRoots(P, (-B, B), v). This approach is slow!
/*template <typename T>
void isolateRoots(Polynomial<T>& P, BFVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;

  //Compute an upper bound on the positive roots of P
  T B = CauchyBound(P);

  BFInterval I(-B, B);
  isolateRoots(P, I, v);
}
*/

/*
//An alternative approach to isolating all roots. Here we separate the positive
//and negative roots separately. This is slightly more efficient than the method
//above.
template <typename T>
void isolateRoots(Polynomial<T>& P, BFVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;

  //Compute an upper bound on the positive roots of P
  T B = CauchyBound(P);

  bool isZeroRoot = false;
  
  if(P.coeff()[0] == 0)
    isZeroRoot = true;

  T temp1[deg+1];

  //Construct the polynomial whose roots in the unit interval correspond
  //with the roots of P in (0, B)
  contract(P.coeff(), deg, B, temp1);
  Polynomial<T> Q(deg, temp1);
  
  BFVecInterval vPos;//Stores the intervals for the positive roots of P
  BFInterval I = BFInterval(0,B);
  isolateRoots(Q, I, deg, vPos);

  //Flip the signs of the coefficients of Q to construct a polynomial whose
  //roots in the unit interval correspond with the roots of P in (-B, 0). 
  for(int i=1; i<= deg; i++){
    if(i % 2 != 0)
      Q.coeff()[i] *=-1;
  }
  
  I = BFInterval(-B, 0);
  isolateRoots(Q, I, deg, v);
  //This ensures that the interval corresponding to the roots are in sorted order 
  if(isZeroRoot)
    v.push_back(std::make_pair(0,0));
  
  for (BFVecInterval::const_iterator it = vPos.begin(); it != vPos.end(); ++it)
    v.push_back(*it);
}
*/

/*
template <typename T>
int numberOfRoots(Polynomial<T> &P){

  BFVecInterval v;
  isolateRoots(P, v);
  return v.size();
}
*/

/*
//Count the number of real roots of P in I
template <typename T>
int numberOfRoots(Polynomial<T> &P, BigFloat x, BigFloat y){

  BFVecInterval v;
  BFInterval I(x,y);
  isolateRoots(P, I, v);
  return v.size();
}
*/

//Counts the number of roots strictly greater than a.
//To do this we just isolate the roots in the interval
//(a,B) where B is an upper bound on the roots.
template <typename T>
int numberOfRootsAbove(Polynomial<T> &P, BigFloat a){
  BigInt B = CauchyBound(P);

  //Check if a is a root
  if(eval(P, a) == 0)
    a += sepBound(P)/BigFloat(2);

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
  BigInt B = CauchyBound(P);
  //Check if a is a root
  if(eval(P, a) == 0)
    a -= sepBound(P)/BigFloat(2);

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
    if(P.coeff()[0] == NT(0)) // zero is a root of P
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
  return isolateRoot(P, 1, e, CauchyBound(P));
}

template <typename T>
BFInterval firstRootBelow(Polynomial<T> P, const BigFloat &e){
  return isolateRoot(P, -1, -1*CauchyBound(P), e);
}




//Given the count n of the number of roots, this function isolates
//the roots of P and checks if they are equal to n.
//CORE_INLINE This causes compiler problems.
void testDescartes(Poly& P, int n = -1) {

  BFVecInterval v;
  isolateRoots(P,v);
  std::cout << "   Number of roots is " << v.size() <<std::endl;
  if ((n >= 0) & (v.size() == (unsigned)n))
    std::cout << " (CORRECT!)" << std::endl;
  else
    std::cout << " (ERROR!) " << std::endl;
  int i = 0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
}// testDescartes

//CORE_INLINE This causes compiler problems
void testDescartes2(Poly& P, BFInterval I, int n) {
  
  BFVecInterval v;
  
  Timer t1;
  t1.start();
  for(int i=1; i <= n ; i++)
    isolateRoots(P, I, v);
  
  t1.stop();
  //  std::cout<<"Root isolation using Descartes over " << n << " iterations takes time " 
  //	   <<t1.getSeconds()<<std::endl;
  std::cout << t1.getSeconds()/n << std::endl;
  
}
