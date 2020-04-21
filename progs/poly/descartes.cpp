#define CORE_LEVEL 4
#include "CORE/CORE.h"
typedef BigInt NT;
typedef Polynomial<NT> Poly;

using namespace CORE;
using namespace std;

//Returns the number of sign variations in the sequence.
int countsign(NT* coeff, int deg){
 int num=0, i=1;

 for(i=0; i < deg; i++){
   if(sign(coeff[i])*sign(coeff[i+1]) < 0) num++;
 }
 
 /*
 // The following code checks if there are one or no sign variations
 // otherwise returns -1.
 while(num <= 1){
    if(coeff[i-1]*coeff[i] < 0){
      num++; i++;
    }
  }
  if(num > 1) return -1;
 */
  return num;
}
void shift(NT* coeff, int deg, NT* shifted){
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

void half(NT* coeff, int deg, NT* halved){
  
  for(int i=0; i<= deg; i++)
    halved[i] = coeff[i]<<(deg-i);
}


int shiftAndSigncount(NT* coeff, int deg){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.
  //The advantage of this method is that ir computes the coefficient of x^i
  //in n-i steps. Thus we can check for sign variation as we compute
  //the coefficients.

  NT temp[deg + 1];
  for(int i=0; i<= deg ; i++)
    temp[i] = coeff[deg-i];

  
  //Compute one of the coefficients.
  for(int j=deg-1; j>=0; j--)
    temp[j]+=temp[j+1];
  
  int num=0, i;
  
  for(i=1; i<= deg-1;i++){
    for(int j=deg-1; j>=i; j--)
      temp[j]+=temp[j+1];
    if(sign(temp[i-1])*sign(temp[i]) < 0) num++;
    if(num > 1) break;
  }
  //To account the sign variation between deg-1 and deg coeffecient.
  if(sign(temp[deg-1])*sign(temp[deg]) < 0) num++;

  if(num == 2) return -1;
  return num;
}


void isolateRoots(Poly& P, const BFInterval I, int deg,
                    BFVecInterval &v) {
  
  
  int num = shiftAndSigncount(P.coeff, deg);
  //  cout << "sign variations after shift " << num << endl;
  
  if(num == 0) return;
  else if(num == 1)
    v.push_back(I);
  else{
    BigFloat m = (I.second + I.first).div2();
    NT* temp1 = new NT[deg +1];
    NT* temp2 = new NT[deg +1];
    half(P.coeff,deg, temp1);
    Poly Q(deg, temp1);
    //    cout<<"Inside isolateRoots "<< Q << endl;
    
    shift(temp1, deg, temp2);
    Poly R(deg, temp2);
    //    cout<<"Inside isolateRoots: second polynomial "<< R << endl;
    
    if(R.coeff[0] == 0) 
      v.push_back(std::make_pair(m,m));
    BFInterval J = std::make_pair(I.first, m);
    BFInterval JJ = std::make_pair(m, I.second);
    isolateRoots(Q, J, deg, v);
    isolateRoots(R, JJ, deg, v);
  }
  
}

//Isolates real roots of P. Assumes that the polynomial is square-free.
//v will contain the isolating intervals for P.
void isolateRoots(Poly& P, BFVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;
  
  NT temp[deg];
  Poly Q;
  //Check whether zero is a root of P.
  if(P.coeff[0] == 0){
    v.push_back(std::make_pair(0,0));
    for(int i=1; i <= deg;i++)
      temp[i-1]=P.coeff[i];
    Q = Poly(deg-1, temp);
  }else{
    Q=P;
  }

  //  Q.dump();
  
  //Compute an upper bound on the positive roots of Q.
  BigInt B = Q.CauchyBound().BigIntValue();

  
  //We now construct a polynomial whose roots in the unit interval
  //correspond with the roots of Q in (0, B).
  //This is obtained by the transformation Q(X) to Q(B*X)
  int n = Q.getTrueDegree();
  NT temp1[n + 1];
  BigInt powB=1;

  for(int i=0; i <= n ; i++){
    temp1[i] = Q.coeff[i]*powB;
    powB*=B;
  }
    
  Poly R(n, temp1);
  //  R.dump();
  
  
  BFInterval I(0, B);
  isolateRoots(R, I, n, v);
  for(int i=1; i<= n; i++){
    if(i % 2 != 0)
      temp1[i] *=-1;
  }

  R = Poly(n, temp1);
  BFVecInterval vNeg;//Stores the isolating intervals for positive roots.
  isolateRoots(R, I, n, vNeg);

  //Flip the sign of the intervals and push them in v
  for (BFVecInterval::const_iterator it = vNeg.begin();
       it != vNeg.end(); ++it) {
    v.push_back(std::make_pair(-1*it->second,-1*it->first));
  }

}



int main( int argc, char* argv[] ) {
  
  //  for(int i=0; i <= deg; i++)
  //    cout << i <<"th coefficient is "<< temp[i] << endl;

  NT cs[] = {-113, 0, 224, 0, 200, 0, -184, 0, -98, 0, 64, 0, 8, 0, -8, 0, 1};
  Poly P(16, cs);

  BFVecInterval v;
  isolateRoots(P, v);
  int i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
  
  cout <<"=============================================================\n"
       << "TEST 3:   Legendre Polynomial 35 X^4 - 20 X^2 + 3 \n"
    "=============================================================" << endl;
  // Legendre : 4 real roots between [-1;1]
  NT cs2[] = {3, 0, -30, 0, 35};
  Poly P2(4, cs2);
  v.clear();
  isolateRoots(P2, v);
   i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
    
  cout <<"=============================================================\n"
       << "TEST 4:   Mignotte Polynomial,  P(X) = X^20 - 2(100X-1)^2 \n" <<
    "=============================================================" << endl;
    // Mignotte's family of polynomials are P(X) = X^n - 2(aX-1)^2
    // where n>2 and a>2 are integers.
    //    The polynomial is irreducible, and has two real roots which
    //    are very close to 1/a.  In fact, they
    //    they are at most 2a^{-(n+2)/2} apart.
    // See [Yap's Book, page 173, Exercise 6.7.3]
    // For our example here, n=20, a=100, we see that the two roots
    //	  are very close to 1/100 = 0.01.  In fact, the 2 real roots
    //	  are within the interval 0.01 +/- 10^{-22}.  This can be
    //	  seen in the output.
    Poly P3(20);
    P3.setCoeff(20, 1); 
    P3.setCoeff(0, -2); 
    P3.setCoeff(1, 400); 
    P3.setCoeff(2, -20000); // P3 = X^20 - 2(100 X-1)^2

  v.clear();
  isolateRoots(P3, v);
   i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }


    // REMARK: there is another variation used in the Frisco Suite.
    // They define A(X)= X^n+(aX+1)^m. They used a complex value for a,
    // e.g.,  a=100i.   If we use a real value a=100
    // then a polynomial like X^20 + (100 X+1)^2 has no real roots.


    cout <<"=============================================================\n"
      << "TEST 5:   Roots of Unity,  P(X) = X^99 - 1 \n" <<
      "=============================================================" << endl;
    Poly P4(99);
    P4.setCoeff(99, 1);
    P4.setCoeff(0, -1);
  v.clear();
  isolateRoots(P4, v);
   i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }

  //    testNewtonSturm(P4, prec, 1);

    cout <<"=============================================================\n"
      << "TEST 6:   Wilkinson Polynomial of degree 6 (roots are 1,..,6)\n" <<
      "=============================================================" << endl;
    NT cs5[] = {720, -1764, 1624, -735, 175, -21, 1};
    Poly P5(6, cs5);
  v.clear();
  isolateRoots(P5, v);
   i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
  //    testNewtonSturm(P5, prec, 6);

    cout <<"=============================================================\n"
      << "TEST 6a:   Perturbed Wilkinson6 (add 1 to constant coeff)\n" 
      << "   NOTE:   Roots are more sensitive to perturbations in higher degree coeffs\n" <<
      "=============================================================" << endl;
    NT cs6[] = {721, -1764, 1624, -735, 175, -21, 1};
    Poly P6(6, cs6);
    //    testNewtonSturm(P6, prec, 6);
  v.clear();
  isolateRoots(P6, v);
   i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }


    cout <<"=============================================================\n"
      << "TEST 7:   Wilkinson Polynomial of degree 10 (roots are 1,..,10)\n" 
      << "       	(THIS USED TO BE SLOW, BUT NO LONGER)\n" <<
      "=============================================================" << endl;
    NT cs9[] = {3628800, -10628640, 12753576, -8409500, 3416930, -902055,
	    		157773, -18150, 1320, -55, 1};
    Poly P9(10, cs9);
  v.clear();
  isolateRoots(P9, v);
 i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
  /*
  BigFloat coef[] = {1,5,-6,3,.7,8,9.5,1,0,10,1};
  Polynomial<BigFloat> P10(10, coef);
  v.clear();
  isolateRoots(P10, v);
 i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
  */

    
  return 0;
}
