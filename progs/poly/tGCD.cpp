/*************************************************************
 * File: tGCD.cpp
 * Purpose:
 * 	Checking the various functions related to GCD
 * 		square-free part,
 * 		principal subresultant coefficients, etc.
 *
 * 	Usage:   tGCD
 *
 *
 * Author: Zilin Du
 * Date:  May 22, 2003
 * Since Core Library version 1.6
 *************************************************************/


#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

// checkGcd(p,q,g):
//     returns true if g=GCD(p,q)
//     returns false otherwise
bool checkGcd(Polynomial<NT>& p, Polynomial<NT>& q, Polynomial<NT>& g){
  
  if(zeroP(p) && zeroP(q))//If both the polynomials are zero then GCD is zero
    return(zeroP(g));

  if(zeroP(g))//Zero polynomial cannot be a GCD except for the case above
    return false;
  
  if(zeroP(p)) //If P is zero then GCD(P, Q) should be same as Q
    return(q.primPart() == g.primPart());//, up to multiplicity

  if(zeroP(q))
    return(p.primPart() == g.primPart());  //Similar to above

  /*  Polynomial<NT> temp1(p), temp2(q), temp, quo;
  while(!zeroP(temp2)){
    quo = temp1.pseudoRemainder(temp2);

    if(!isDivisible(temp1, g))
      return false;

    temp = temp2;//Changing the divisor to the dividend and the
    temp2 = temp1;// remainder to the divisor.
    temp1 = temp;
    }*/

  NT x = 0;
  for(int i=0; i < g.getTrueDegree(); i++)
    x += core_abs(psc(i, p, q));
           // psc(i,p,q) = i-th principal subresultant coeff.

  if(isDivisible(p, g) && isDivisible(q, g) && x == 0 
     && psc(g.getTrueDegree(), p, q) != 0)
    return true;

  return false;
}

void checkBounds(Polynomial<NT>& p){
  for (int i=0; i<1000; i++){  /// WHAT IS THIS FOR?  
	CauchyUpperBound(p);
	CauchyLowerBound(p);
  }
}

//Given the original polynomial P and it's square-free part Q
bool checkSquareFree( Polynomial<NT> &p, Polynomial<NT>& q){
  if(p.getTrueDegree() <= 1)
    return true;

  Polynomial<NT> r = differentiate(q);
  Polynomial<NT> temp(p), temp1(p);
  temp = temp.pseudoRemainder(q);
  //temp.dump("temp");

  //Q is squarefree and Q divides P and P/Q is proportional to gcd(P, P')
  if(res(q,r) != 0 && isDivisible(p, q) 
     && checkGcd(p, temp1.differentiate(), temp))
    return true;

  //  cout << "Check Gcd " << checkGcd(p, temp1.differentiate(), temp) << endl;
  //cout << "Res " << res(q, r) << endl;
  //cout << "Is Div " << isDivisible(p, q) << endl;
  return false;
}


//bool checkPrimPart(){
//}

int main(int argc , char ** argv) {

  NT cs1[] = {-45, 390, -1260, 2040, -2115, 1350};
  Polynomial<NT> P1(5, cs1);
 
  NT cs2[] = {3, -2, 5};
  Polynomial<NT> P2(2, cs2);

  NT cs3[] = {9, -6, 15};
  Polynomial<NT> P3(2, cs3);

  NT cs4[] = {15, -9, 6};
  Polynomial<NT> P4(2, cs4);

  NT cs5[] = {5};
  Polynomial<NT> P5(0, cs5);
  
  Polynomial<NT> G, zero;
  
  //Two Polynomials which have a non-constant GCD
    G = gcd(P1, P2);
  if(checkGcd(P1, P2, G))
    cout << "CORRECT! GCD calculated properly" << endl;
  else
    cout << "ERROR! GCD calculated wrongly" << endl;


  //Two Polynomials which differ by a constant multiple
  G = gcd(P2, P3);
  G.dump();
  if(checkGcd(P2, P3, G))
    cout << "CORRECT! GCD calculated properly" << endl;
  else
    cout << "ERROR! GCD calculated wrongly" << endl;
  
  //Two Polynomials which have a constant GCD
  G = gcd(P3, P4);
  if(checkGcd(P3, P4, G))
    cout << "CORRECT! GCD calculated properly" << endl;
  else
    cout << "ERROR! GCD calculated wrongly" << endl;

  //Two Polynomials one of which is a zero polynomial
  G = gcd(P3, zero);
  if(checkGcd(P3, zero, G))
    cout << "CORRECT! GCD calculated properly" << endl;
  else
    cout << "ERROR! GCD calculated wrongly" << endl;

  //Two Polynomials both of which are zero polynomial
  G = gcd(zero, zero);
  if(checkGcd(zero, zero, G))
    cout << "CORRECT! GCD calculated properly" << endl;
  else
    cout << "ERROR! GCD calculated wrongly" << endl;


  //Two Polynomials which have a constant GCD
  G = gcd(P5, P4);
  if(checkGcd(P5, P4, G))
    cout << "CORRECT! GCD calculated properly" << endl;
  else
    cout << "ERROR!!! GCD calculated wrongly" << endl;
  
  // TEST OF KNOWN GCD's 
  
  Polynomial<NT> C;
  C = P2*P3;	//  So, gcd(P2,C)=P2;
  if (gcd(C,P2) != P2)
	  cout << "ERROR!!!  gcd(A*B,A) != A" << endl;
  else
	  cout << "CORRECT!  gcd(A*B,A) = A" << endl;
  if (gcd(P3,C) != P3)
	  cout << "ERROR!!!  gcd(B,A*B) != B" << endl;
  else
	  cout << "CORRECT!  gcd(B,A*B) = B" << endl;

  Polynomial<NT> D(P2);
  D.setCoeff(0,4);
  if(checkGcd(P2,C,D)) 
    cout << "ERROR!!!  D is considered gcd" << endl;
  else
    cout << "CORRECT!  D is not the gcd" << endl;
  
  checkBounds(P1);
  checkBounds(P2);
  checkBounds(P3);
  checkBounds(P4);
  checkBounds(P5);
  
  //
  //Example from Modern Computer Algebra, v.z.Gathen, Pg 172
  //==========================================
  NT cs6[] = {-764, -979, -741, -814, -65, 824};
  Polynomial<NT> P6(5, cs6);
  NT cs7[] = {617, 916, 880, 663, 216};
  Polynomial<NT> P7(4, cs7);

  //==========================================
  NT p = psc(0, P6, P7);
  if (p != BigInt("31947527181400427273207648"))
      {if (p == BigInt("-31947527181400427273207648"))
	cout << "psc(0, P6, P7) is CORRECT up to sign -- minor bug" << endl;
       else 
	cout << "ERROR!!! in psc(0)" << endl;
     }
  cout << "psc(0, P6, P7) "<< p<<endl;
  //==========================================
  p = psc(1, P6, P7);
  if (p != BigInt("27754088254928081728")) cout<<"ERROR!!! in psc(1)"<<endl;
  cout << "psc(1, P6, P7) "<< p<<endl;
  //==========================================
  p = psc(2, P6, P7);
  if (p != BigInt("-41344606374560")) cout<<"ERROR!!! in psc(2)"<<endl;
  cout << "psc(2, P6, P7) "<< p<<endl;
  //==========================================
  p = psc(3, P6, P7);
  if (p != BigInt(176909472)) cout<<"ERROR!!! in psc(3)"<<endl;
  cout << "psc(3, P6, P7) "<< p<<endl;
  //==========================================
  // The next one is an extremal test:
  // According to Yap's book [p.91], if deg(P)>deg(Q)=n, then
  //       sres_n(P,Q) = lead(Q)^{m-n-1} Q.
  // So, sres_4(P6,P7)= P7, and psc(5,P6,P7)= lead(P7)= 216.
  p = psc(4, P6, P7);
  if (p != BigInt(216)) cout<<"ERROR!!! in psc(4)"<<endl;
  cout << "psc(4, P6, P7) "<< p<<endl;
  //==========================================
  // Another extremal test:
  // According to Yap's book [p.91], if m=deg(P)>deg(Q)=n, then
  //       sres_m(P,Q) = P.
  // So, sres_5(P6,P7)= P6, and psc(5,P6,P7)= lead(P6)= 824.
  p = psc(5, P6, P7);
  if (p != BigInt(824))
	  cout<<"INCORRECT! in psc(5).  Known minor bug"<<endl;
  cout << "psc(5, P6, P7) "<< p<<endl;
  //==========================================

  //Various Tests for the squarefree part
  //When the polynomial is already squarefree
  G = P2;
  P2.dump("Polynomial P2 is ");
  G.sqFreePart();
  G.dump("\n Square Free Part is ");
  if(checkSquareFree(P2, G))
    cout <<"\n CORRECT! SquareFree part calculation" << endl;
  else
    cout <<"\n ERROR! SquareFree part calculation" << endl;
  
  //When their is a non-trivial squarefree part
  G = P1;
  P1.dump("Polynomial P1 is ");
  G.sqFreePart();
  G.dump("\n Square Free Part is");

  if(checkSquareFree(P1, G))
    cout <<"\n CORRECT! SquareFree part calculation" << endl;
  else
    cout <<"\n ERROR!!! SquareFree part calculation" << endl;

  //Constant's are always squarefree
  G = P5;
  P5.dump("Polynomial is constant, ");
  G.sqFreePart();
  G.mapleDump();
  if(checkSquareFree(P5, G))
    cout <<"\n CORRECT! SquareFree part calculation" << endl;
  else
    cout <<"\n ERROR!!! SquareFree part calculation" << endl;

}
