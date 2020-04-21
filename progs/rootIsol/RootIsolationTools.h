/*
  A collection of essential tools and data structures for performing root 
  isolation using subdivision methods.

  Author: Jihun 2010
 */
#ifndef __ROOTISOLATIONTOOLS_H__
#define __ROOTISOLATIONTOOLS_H__


#define CORE_LEVEL 4
#include <CORE/CORE.h>
#include"lowerBounds.h"
#include<stack>

using namespace CORE;
using namespace std;

typedef std::pair<BigRat, BigRat>  BRInterval;
typedef std::vector<BRInterval> BRVecInterval;
/*******************************************************/
/**********DATA STRUCTURES******************************/
/*******************************************************/

// Template parameters
// 	RT= number type for endpoints of interval
// 	T = number type for coefficients of polynomial

// Stores the interval endpoints and a polynomial (can be in either basis)
template < typename RT, typename T >
struct SubDivData 
{
  RT a, b;
  Polynomial<T> P;
  
  SubDivData(const RT a_, const RT b_, const Polynomial<T>& P_){
    a = a_; b = b_; P= Polynomial<T>(P_);
  }
  //  SubDivData( SubDivData SD){
  //    a = SD.a; b = SD.b; P = SD.P;
  //  }
  
  SubDivData(int sz) {
    P= Polynomial<T>(sz);
  }
  
};

template < typename RT, typename T >
struct CFDivData
{

  RT a, b, c, d;
  int sv;
  Polynomial<T> P;

  CFDivData(const RT a_, const RT b_, const RT c_, const RT d_, int sv_, const
	    Polynomial<T>& P_){
    a = a_; b = b_; c=c_; d=d_; sv = sv_; P= Polynomial<T>(P_);// Assignment operation
  }
  //  SubDivData( SubDivData SD){
  //    a = SD.a; b = SD.b; P = SD.P;
  //  }
  
  CFDivData(int sz) {
    P= Polynomial<T>(sz); // CAUTION: constant coefficient is set to one
  }
};


template<typename RT,typename T, typename Prec>
  struct IntvData{
    RT a, b;
    T s;
    Prec p;

    IntvData(){}
    IntvData(RT a_, RT b_, T s_, Prec p_){
      a = a_; b= b_; s=s_; p=p_;
    }
  };

template < typename RT, typename T >
  struct SlvSubDivData
  {
    RT a, b;
    std::vector<T> slv;
    unsigned int depth;

    SlvSubDivData(RT a_, RT b_, std::vector<T> &slv_){
      a=a_; b=b_; slv=slv_; depth=0;
    }
    SlvSubDivData(int sz){
      slv= std::vector<T>(sz);
    }
    
  };
template < typename RT, typename T >
  struct SlvSubDivDataDouble
  {
    RT a, b;
    std::vector<T> upSlv;
    std::vector<T> lowSlv;
    unsigned int depth;

    SlvSubDivDataDouble(RT a_, RT b_, std::vector<T> &upslv_, std::vector<T> &lowslv_){
      a=a_; b=b_; upSlv=upslv_; lowSlv=lowslv_; depth=0;
    }
    SlvSubDivDataDouble(int sz){
      upSlv= std::vector<T>(sz);
      lowSlv= std::vector<T>(sz);
    }
    
  };

/*******************************************************/
/**********PROCEDURES***********************************/
/*******************************************************/


// Required for sorting vectors of BFIntervals
template <typename T>
inline bool operator <(const std::pair<T, T>& I, const std::pair<T, T>& J){
  if(I.second <= J.first)
    return true;
  if(I.first >= J.second)
    return false;
}

// Checking if the sign variations are 0, 1, or greater than one.
// Deg should be the true degree, otherwise we may return an incorrect answer.
template <typename T>
int signVar(T* coeff, int deg){
  int num=0;
  int lastsign =0;//The last non-zero sign
  int currsign;//The sign of the current coefficient

  for(int i=0; i <= deg; i++){
    currsign = sign(coeff[i]);
    if(currsign !=0){
      if (lastsign * currsign < 0) num++;
      lastsign = currsign;//lastsign is always non-zero except for the starting.
    }
     if(num > 1) return num;
  }
  return num;
}


//Counts the total number of sign variations in the coefficients of P
//while disregarding any change of sign from zero to non-zero coefficients.
template<typename NT>
int signVariation(Polynomial<NT>& P)
{
  int num=0;
  int lastsign =0;//The last non-zero sign
  int currsign;//The sign of the current coefficient

  //Get the number of sign variations. 
  for(int i=0; i<= P.getTrueDegree();i++){
    currsign = sign(P.coeff()[i]);
    if(currsign !=0){
      if (lastsign * currsign < 0) num++;
      lastsign = currsign;//lastsign is always non-zero except for the starting.
    }
  }
  return num;
}



/*
// Taylor shift by one on polynomial represented by coeff; output is in shifted
// Deg should be the true degree, otherwise we may return an incorrect answer.
template <typename T>
void shift(T* coeff, int deg, T* shifted){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.
  //  if(*shifted != *coeff)
  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];

  for(int i=0; i<= deg-1;i++)
    for(int j=deg-1; j>=i; j--)
      shifted[j]+=shifted[j+1];

}
*/
// Scale the variable by 2 of the polynomial represented by coeff; 
// output is halved.
// Deg should be the true degree, otherwise we may return an incorrect answer
template <typename T>
void half(const T* coeff, int deg, T* halved){
  for(int i=0; i<= deg; i++)
    halved[i] = coeff[i]*(BigInt(1)<<(deg-i));
}

// Scale by a positive power of two.
// Deg should be the true degree, otherwise we may return an incorrect answer
template <typename T1, typename T2>
void scale(T1* coeff, int deg, T2 k, T2* scaled){
  for(int i=0; i<= deg; i++)
    scaled[i] = coeff[i]*(BigInt(1)<<(k*i));
}


//Given the coefficient sequence coeff of some polynomial P
//this computes the coefficient sequence contracted of the polynomial
//P(\lambda X). Since lambda can be of a different type than the
//coefficients of P we need introduce another typename to resolve this.
//We assume that T2 is more general than T1 so that the conversion from
// the latter to the former can take place without error.
// Deg should be the true degree, otherwise we may return an incorrect answer
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
//for contract above..
// Deg should be the true degree, otherwise we may return an incorrect answer
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

/*
//This is the ascending coefficient method suggested by
//Krandick in Isolierung reeller Nullstellen von Polynomen
//( English version is called Isolation of Polynomial Real Roots)
//to compute the Taylor shift of a polynomial by one.
//The advantage of this method is that it computes the coefficient of x^i
//in n-i steps. Thus we can check for sign variation as we compute
//the coefficients.
// Deg should be the true degree, otherwise we may return an incorrect answer
template <typename T>
int shiftAndSigncount(T* coeff, int deg){

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
     if(num > 1) return num;
  }
  //To account the sign variation between the previous non-zero coefficient
  // and deg coeffecient.
  //This is done if the number of sign variations were less than or
  //equal to one from the above loop.
  if(lastsign*sign(temp[deg]) < 0) num++;
  //  if(sign(temp[deg-1])*sign(temp[deg]) < 0) num++;
  if(num ==2 ) return num;
  return num;
}
*/

// Decasteljau's algorithm for the special case when the 
// the upper and the lower sleeve are given explicitly as a pair of vectors
// and we always subdivide at the midpoint.
template <typename T>
void deCasteljau(const std::vector<T>& upSlv, const std::vector<T>& lowSlv, 
			std::vector<T>& upSlvL, std::vector<T>& lowSlvL, 
			std::vector<T>& upSlvR, std::vector<T>& lowSlvR, 
			int deg){
  
  upSlvL[0] = upSlv[0]; lowSlvL[0] = lowSlv[0];
  upSlvR[deg] = upSlv[deg]; lowSlvR[deg] = lowSlv[deg];


  T ud[deg], ld[deg];
  for(int i=0; i < deg; i++){
    ud[i] = upSlv[i]+ upSlv[i+1];
    ld[i] = lowSlv[i]+ lowSlv[i+1];
  }

  upSlvL[1] = ud[0]/2; lowSlvL[1] = ld[0]/2;
  upSlvR[deg-1] = ud[deg-1]/2; lowSlvR[deg-1] = ld[deg-1]/2;
  
  T pow2;
  for(int i=2; i <= deg; i++){
    for(int j=0; j <= deg-i; j++){
      ud[j] += ud[j+1]; 
      ld[j] += ld[j+1];
    }
    pow2 = exp2(i);
    upSlvL[i] = ud[0]/pow2; lowSlvL[i] = ld[0]/pow2;
    upSlvR[deg-i] = ud[deg-i]/pow2; lowSlvR[deg-i] = ld[deg-i]/pow2;
  }
}

// Given the Bernstein coefficients of P w.r.t. the interval [a, b] (by default [0,1]).
// Perform deCasteljau's algorithm on P at the point u in [a,b].
// The Bernstein coefficients w.r.t. [a,u] are in PL and w.r.t. [u,b] in PR. 
// This is the NON fraction-free variant.
// Deg should be the true degree, otherwise we may return an incorrect answer.
// isExactArith = true means increase the precision of the BigFloat2 arithmetic;
// isExactArith = false means do BigFlaot2 arithmetic with fixed precision prec.
template <typename Vec, typename T2, typename PT>
  void deCasteljau (const Vec& P, Vec& PL, Vec& PR, T2 u, T2 a, T2 b, int deg,
		    bool isExactArith, PT prec){
  //  cout <<" Inside deCasteljau "<<endl;  
  //  cout<<"den = "<< den << " denLg = "<<denLg << endl;
  PL[0] = P[0]; PR[deg] = P[deg];

  Vec coefft(deg);
  T2 x, xT, y;
  y.sub(u, a, prec);
  xT.sub(b, a, prec);
  x.div(y, xT, prec);
  /*  cout << "Precision of u " << u.get_prec() << endl;
  cout << "Precision of a " << a.get_prec() << endl;
  cout << "Precision of b " << b.get_prec() << endl;
  cout << "Precision of b-a " << xT.get_prec() << endl;
  cout << "Precision of x " << x.get_prec() << endl;
  */
  if(isExactArith){
    //cout <<" Precision before addition is "<< coefft[0].get_prec()<<endl;
    for(int i=0; i < deg; i++){
      coefft[i] = (1-x)*P[i]+ x*P[i+1];    
    }

    //    cout <<" Precision after addition is "<< coefft[0].get_prec()<<endl;
    PL[1] = coefft[0]; PR[deg-1] = coefft[deg-1];
    
    for(int i=2; i <= deg; i++){
      for(int j=0; j <= deg-i; j++){
	coefft[j] = (1-x)*coefft[j]+ x*coefft[j+1];    
      }
      PL[i] = coefft[0]; PR[deg-i] = coefft[deg-i];
    }
    //    cout <<" Precision after addition is "<< PL[1].get_prec()<<endl;
  }else{
    //    cout <<" Subdivision ratio  "<< x<<endl;
    T2 temp, temp1, OneMinusX;
    OneMinusX.sub(1, x, prec);
    //    cout <<" Precision before addition is "<< coefft[0].get_prec()<<endl;
    for(int i=0; i < deg; i++){
      // coefft[i] = (1-x)*P[i]+ x*P[i+1];    
      temp1.mul(x, P[i+1], prec);// temp1 = x*P[i+1]
      temp.mul(OneMinusX, P[i], prec);// temp = (1-x)*P[i]
      coefft[i].add(temp1, temp, prec);
    }

    //    cout <<" Precision after addition is "<< coefft[0].get_prec()<<endl;
    PL[1] = coefft[0]; PR[deg-1] = coefft[deg-1];

    for(int i=2; i <= deg; i++){
      for(int j=0; j <= deg-i; j++){
	// Precision controled version of 
	// coefft[j] = (1-x)*coefft[j]+ x*coefft[j+1];    
	temp.mul(OneMinusX, coefft[j], prec);// temp = (1-x) * coefft[j]
	temp1.mul(x, coefft[j+1], prec);// temp1 = x* coefft[j+1]
	coefft[j].add(temp, temp1, prec);
      }
      PL[i] = coefft[0]; PR[deg-i] = coefft[deg-i];
    }
    //    cout <<" Precision after addition is "<< PL[1].get_prec()<<endl;
  }
}


// Perform deCasteljau's algorithm on P with the subdivision point
// at num/den, and stores the left coefficients in PL and right 
// coefficients in PR. This is the fraction-free variant, i.e., 
// we compute the Bernstein coefficients of (den)^n P(X), n = deg,
// and hence avoid the appearance of fractions in our algorithm.
// Deg should be the true degree, otherwise we may return an incorrect answer
template <typename T1>
void deCasteljau (const T1* P, T1* PL, 
		  T1* PR, int deg,  BigInt num, BigInt den){
  
  unsigned long denLg = floorLg(den);
  if(den == 1)
    denLg =1;
  //  cout<<"den = "<< den << " denLg = "<<denLg << endl;
  PL[0] = P[0]<<(deg*denLg); PR[deg] = P[deg]<<(deg*denLg);
  BigInt den_num = den - num;

  T1 coefft[deg];
  for(int i=0; i < deg; i++)
    coefft[i] = P[i]*(den_num)+num*P[i+1];
  
  PL[1] = coefft[0]<<((deg-1)*denLg); PR[deg-1] = coefft[deg-1]<<((deg-1)*denLg);

  for(int i=2; i <= deg; i++){
    for(int j=0; j <= deg-i; j++)
      coefft[j] = coefft[j]*(den_num)+ num*coefft[j+1];    
    PL[i] = coefft[0]; PR[deg-i] = coefft[deg-i];
    PL[i] <<= ((deg-i)*denLg); PR[deg-i] <<=((deg-i)*denLg);
  }
}


// Performs subdivision at lambda on SD and stores the output in SDL and SDR.
// Deg should be the true degree of SD.P, otherwise we may return an incorrect answer
template <typename RT, typename T>
void subdivide (const SubDivData<RT, T>& SD, SubDivData<RT, T>& SDL, 
		SubDivData<RT, T>& SDR, int deg, BigInt num =1, BigInt den=2){

  deCasteljau(SD.P.coeff(), SDL.P.coeff(), SDR.P.coeff(), deg, num, den);
  SDL.a = SD.a;  SDL.b = (SD.a + SD.b).div2();
  SDR.a = SDL.b; SDR.b = SD.b;
}


// This method is used for the power basis variant of the Descartes method.
// Performs subdivision on SD and stores the output in SDL and SDR.
// The inputs represent polynomials in power basis.
// Deg should be the true degree of SD.P, otherwise we may return an incorrect answer
template <typename RT, typename T>
void subdividePow(const SubDivData<RT, T>& SD, SubDivData<RT, T>& SDL, 
		SubDivData<RT, T>& SDR, int deg){
  SDL.a = SD.a; SDR.b = SD.b;
  SDL.b = (SD.a + SD.b).div2();
  SDR.a = SDL.b;

  half(SD.P.coeff(),deg, SDL.P.coeff());
    //    cout <<"After halving polynomial is "; Q.dump(); cout<<endl;
    
  shift(SDL.P.coeff(), deg, SDR.P.coeff());

}

// This method is used for the continued fraction based approach for isolation.
// Performs subdivision on SD and stores the output in SDL and SDR.
// The inputs represent polynomials in power basis.
// We also pass the pointer to the function we want to use for computing
// lower bounds. The bool value tells us whether we constructed the left child
// or not.
// Deg should be the true degree of SD.P, otherwise we may return an incorrect answer
template <typename RT, typename T>
bool subdivide(const CFDivData<RT, T>& SD, CFDivData<RT, T>& SDL, 
	       CFDivData<RT, T>& SDR, int deg, 
	       BigInt (*Bound)(const Polynomial<T>&) = HongLowerBound){


  //  cout<<"Input polynomial"<<endl;
  //  SD.P.dump(); cout<<endl;  

  BigInt B = Bound(SD.P);
  //  cout<<"Bound = "<< B<<endl;
  Polynomial<T> temp(deg);

  RT a=SD.a, b = SD.b, c = SD.c, d = SD.d;
  for(int i=0; i <= deg; i++)
    temp.coeff()[i] = SD.P.coeff()[i];
  
  if(B >= 1){
    unsigned long Bmsb = floorLg(B);
    if(B > 1)// for B == 1 nothing to do
      for(int i=0; i <= deg; i++) 
	temp.coeff()[i] *= (T(1)<<(i*Bmsb)); // scale the variable by B
    // Shift by one
    for(int i=0; i<= deg-1;i++)
      for(int j=deg-1; j>=i; j--)
	temp.coeff()[j]+=temp.coeff()[j+1];
    a *= B; b +=a;
    c *= B; d +=c;
  }

  shift(temp.coeff(), deg, SDR.P.coeff());
  //  cout<<"Right polynomial"<<endl;
  //  SDR.P.dump(); cout<<endl;
  SDR.sv = signVariation(SDR.P);
  SDR.a = a; SDR.b = b+a;
  SDR.c = c; SDR.d = c+d;

  bool leftCreated = false;//Flag to signal whether left node was created
  int r=0;
  if(SDR.P.coeff()[0] == T(0)) // There is a root at the endpoint
    r=1;

  if(SDR.sv + r < SD.sv){
    leftCreated = true;
    temp.reverse();
    shift(temp.coeff(), deg, SDL.P.coeff());
    //    cout<<"Left polynomial"<<endl;
    //    SDL.P.dump(); cout<<endl;
    SDL.a = b; SDL.b = SDR.b;
    SDL.c = d; SDL.d = SDR.d;
    SDL.sv = signVariation(SDL.P);
  }
  return leftCreated;
}

// Construct the interval with endpoints M(0), M(infty), where M(X) is
// the Mobius transformation defined by SD. Note SD.d cannot be zero, and both
// SD.a and SD.c cannot be zero simultaneously.
template <typename RT, typename T, typename FT>
 void getInterval(const CFDivData<RT,T>& SD, std::vector<std::pair<FT, FT> >& v, 
		  BigInt CB){


  //  cout <<"a= "<< SD.a << "b= "<< SD.b<< "c= "<<SD.c <<"d= " <<SD.d << endl;
    if(SD.c != 0){
      if(SD.b*SD.c < SD.a*SD.d) // Correctly order the end-points of the interval 
	v.push_back(std::make_pair(FT(SD.b,SD.d), FT(SD.a,SD.c)));
      //cout << "Root in ["<< b/d << " : " << a/c << "]" << endl;
      else 
	v.push_back(std::make_pair(FT(SD.a, SD.c), FT(SD.b, SD.d)));
    }else{
      if(sign(SD.a) > 0)
	v.push_back(std::make_pair(FT(SD.b, SD.d), CB));
      else // This is needed when isolating negative roots
	v.push_back(std::make_pair(-1*CB, FT(SD.b, SD.d)));
      // cout << "Root in ["<< b/d << " : " << CB << "]" << endl;
    }

}

/* FUNCTIONS FOR THE INTERVAL DESCARTES ALGORITHM */

// The vector B contains the binomial coefficients for deg.
// Two ways -- first, \Theta(d) computations involving d multiplications and d exact
// divisions, but the intermediate results are sometimes larger than the output;
// and second, dynmaic programming approach based upon recursion.
template <class C>
void computeMatrix(unsigned deg, std::vector<C>& B)
{
  B.push_back(C(1));
  C temp;
  for(unsigned j=1; j<= deg/2; j++)
    {
      temp = (B[j-1]*(deg - j+1))/j;
      B.push_back(temp);
      //      cout << B[j] << std::endl;
    }
}

template<typename V1, typename V2, typename Bd>
  void getInterval(V1 &vT, V2 &v, Bd B, bool isPos, bool isUnit){

  BigRat tempa, tempb;

  if(isUnit){
    if(isPos){
      for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it)
	v.push_back(BRInterval((*it).first, (*it).second));
    }else{
      for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it){
	tempa = (*it).first; tempb = (*it).second;
	v.push_back(BRInterval(-tempb, -tempa));
      }
    }
  }else{
    if(isPos){
      for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it){
	tempa = (*it).first; tempb = (*it).second;
	if((*it).first != 0)
	  v.push_back(BRInterval(1/tempb, 1/tempa));
	else
	  v.push_back(BRInterval(1/tempb, BigRat(B)));
      }
    }else{
      for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it){
	tempa = (*it).first; tempb = (*it).second;
	if(tempa != 0)
	  v.push_back(BRInterval(-1/tempa, -1/tempa));
	else
	  v.push_back(BRInterval(-B, -1/tempb));
      }
    }
  }
}

template<typename V1, typename V2, typename Bd>
  void getInterval(V1 &vT, V2 &v, Bd B, bool isPos){
    BRInterval I;
    BigRat tempa, tempb;

    if(!isPos){
      for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it){
	tempa = (*it).first; tempb = 1- tempa;
	I.second = -1* tempa/tempb;
	if((*it).second != 1){
	  tempa = (*it).second; tempb = 1-tempa;
	  I.first = -1* tempa/tempb;
	}else{
	  I.first = -B;
	}
	v.push_back(I);
      }
    }else{
      for (BFVecInterval::const_iterator it = vT.begin(); it != vT.end(); ++it){
	//    std::cout << ++i << "th Root is in ["
	//    << it->first << " ; " << it->second << "]" << std::endl;
	tempa = (*it).first; tempb = 1-tempa;
	I.first = tempa/tempb;
	if((*it).second != 1){
	  tempa = (*it).second; tempb = 1-tempa;
	  I.second = tempa/tempb;
	}else{
	  I.second = B;
	}
	v.push_back(I);
      }
    }
}
/*
  The number of times the sleeve defined by ubp and dbp intersects
  the x-axis. Returns 0, 1, or 2. The last
  value is returned if the sleeve intersects with the x-axis more than
  once. This procedure is useful if we want to know whether the sleeve intersects
  strictly, not strictly, or not at all.
 */

template <typename VECT>
int sleeveVar(VECT& slv, int sz)
{
  int lastsign=0, currsign, num=0;
  int ui, di;
  for(int i=0; i<= sz; i++){
    ui = sign(slv[i].getRight()); di = sign(slv[i].getLeft());
    //    cout<<"sgn ubp["<<i<<"]="<< ui << " sgn dbp["<<i<<"]="<< di << std::endl;
    if(ui * di >= 0){
      currsign = sign(ui+di); // currsign is zero only if both ui and di are zero
      if(currsign != 0){
	if(lastsign * currsign < 0) num++;
	if(num > 1) return num;
	lastsign = currsign;
      }
    }
    if(ui * di < 0){
      if(i == 0 || i == sz -1) // Cannot decide if there is an indecisive crossing in starting or end.
	return 2;
      else{
	lastsign *=-1;
	num ++;
	if(num > 1) return num;
      }
    }
  }
  return num; //either zero or one
}

template <typename VECT>
int sleeveVar(VECT& upSlv, VECT& lowSlv, int sz){
 int lastsign=0, currsign, num=0;
  int ui, di;
  for(int i=0; i<= sz; i++){
    ui = sgn(upSlv[i]); di = sgn(lowSlv[i]);
    //    cout<<"sgn ubp["<<i<<"]="<< ui << " sgn dbp["<<i<<"]="<< di << std::endl;
    if(ui * di >= 0){
      currsign = sign(ui+di); // currsign is zero only if both ui and di are zero
      if(currsign != 0){
	if(lastsign * currsign < 0) num++;
	if(num > 1) return num;
	lastsign = currsign;
      }
    }
    if(ui * di < 0){
      if(i == 0 || i == sz -1) // Cannot decide if there is an indecisive crossing in starting or end.
	return 2;
      else{
	lastsign *=-1;
	num ++;
	if(num > 1) return num;
      }
    }
  }
  return num; //either zero or one

}


/*
  Computes the sleeve for the polynomial P (in Bernstein basis w.r.t. [0,1]) on the
  interval [a,b], where the scaling to be done is s and the starting precision
  is p. Sleeve is returned in slv and the precision, if increased, in p.
  B contains the binomial coefficients.
  The boolean variable isExactArith tells us whether the Bigfloat2 precision
  is increased during arithmetic or we keep it fixed to EPS.
 */
template <typename T, typename RT, typename Vec1, typename Vec2, typename FT>
void initializeSleeve(Polynomial<T> &P, int deg, RT a, RT b, unsigned int s, 
		      prec_t &p, Vec1 &slv, Vec2 &B, bool isExactArith, FT EPS){
  //  cout <<"Inside initalizeSleeve "<< EPS << endl;
  
  Polynomial<T> Q(P);
  //  Q.dump() ; cout << endl;

  if(s != 0)
    for(int i=0; i <= deg; i++)
      Q.coeff()[i] <<= s;

  //  cout <<"Scaled the polynomial by "<< s << " Interval is ["
  //       << a << " , "<< b << "]"<< endl;
  
  bool suffprec = false;
  long logWidth;

  std::vector<BigFloat2> SLV(deg+1); 
  
  do{

    suffprec = true;
    //monomialToBezier(Q, deg, a, b, SLV, B, p);
    bezierToBezier(Q, deg, a, b, SLV, isExactArith, p);
    //    cout <<"Called bezierToBezier " << endl;
    //    cout <<"Precision of high prec sleeve = "<< SLV[0].get_prec() <<endl;
    //    cout.precision(SLV[0].get_prec());
    
    for(int i=0; i<= deg; i++){
      slv[i].set(SLV[i], DOUBLE_PREC);		       
      //      cout <<"SLV["<<i<<"] = ["<< SLV[i].getLeft() << " , " << SLV[i].getRight() << "]"
      //   << " slv["<<i<<"] = ["<< slv[i].getLeft() << " , " << slv[i].getRight() << "]" << endl;
    }
    
    for ( int i=0; i <= deg; i++ ){
      logWidth = slv[i].abs_diam().uMSB();// If the interval endpoints are same them Zero is returned
      if( (logWidth != 0) && (logWidth > 2-EPS) ){
	  //	  &&	  (logWidth > floorlg(slv[i].get_max()) + 2-EPS) ){
	// EITHER ABS OR REL. APPROX.
	//	cout <<" Not sufficient precision: slv " << slv[i].abs_diam().uMSB() << " SLV  " 
	//		     << SLV[i].abs_diam().uMSB() << " Required "<< 2-EPS << endl;
	suffprec = false;
	break;
      }
    }
    
    //    std::cout<<"Suff prec =" << suffprec << std::endl;
    if(!suffprec){
      p <<= 1;     //mpfr_set_default_prec(p);
      //      for(int i=0; i <= deg; i++)
      //      	SLV[i].set_prec(p);
      //      std::cout<<"Precision increased to "<< p << std::endl;
    }

  }while(!suffprec);

  //  mpfr_set_default_prec(oldp);
  //  cout <<"Exiting initializesleeve: Width is "
  //       << slv[0].abs_diam().uMSB() << " Desired is "<< (2-EPS) << endl;
}

// All arithmetic is BigFloat2 arithmetic
template <typename T, typename RT, typename T2, typename Vec, typename PREC>
void monomialToBezier(Polynomial<T> &P, int deg, RT a, RT b, int s, 
		      T2& bz, Vec &Bin, PREC p){

  Polynomial<BigFloat2> PP(P);
  BigFloat2 A(a, p), B(b, p);

  // Jan'19, Chee: Error since != is not defined for BigFloat2...
  if(A != BigFloat2(0)) 
    shift(PP.coeff(), deg, A);

  if(B-A != BigFloat2(1))
    scale(PP.coeff(), deg, B-A, PP.coeff());

  bz[0] = PP.coeff()[0]; bz[deg] = PP.coeff()[deg];
  for(unsigned i = 1; i < deg; i++){
      if(i <= deg/2)
	bz[i] = PP.coeff()[i]/BigFloat2(Bin[i]);
      else
	bz[i] = PP.coeff()[i]/BigFloat2(Bin[deg-i]);
  }

  for(unsigned j=1; j <= deg; j++)
    for(unsigned i = deg; i >=j; i--)
      bz[i] += bz[i-1];
}


// All arithmetic is BigFloat2 arithmetic. We know the Bernstein coefficients
// of P on the unit interval, but we want its Berstein coefficients on
// [a,b]. We use the auto version of BigFloat2 arithmetic, i.e., the precision
// of the interval endpoints is increased automatically. This isn't much because
// the increase is by logarithm of the degree, as we do only O(n^2) operations.
template <typename T, typename RT, typename T2, typename PT>
  void bezierToBezier(Polynomial<T> &P,
	  int deg, RT a, RT b, T2& bz, bool isExactArith, PT prec){
  //  cout<<"Inside bezierToBezier"<<endl;

  BigFloat2 A(a, prec), B(b, prec), One(1, prec), Zero(0, prec);

  //  cout << " Initial precision is "<< A.get_prec() << endl;

  std::vector<BigFloat2> tempL(deg+1), tempR(deg+1);  
  for(int i=0; i<=deg; i++)
    bz[i] = BigFloat2(P.coeff()[i], prec);

  //  cout << " Precision before deCasteljau "<< bz[0].get_prec() << endl;  
  // First subdivide [0,1] at A if it's not zero
  if(A != Zero ){
    //    cout<<"Subdividing at "<< A << endl;
    deCasteljau(bz, tempL, tempR, A, Zero, One, deg, isExactArith, prec);
    //    cout << " Precision after deCasltejau "<< tempL[0].get_prec() << endl;  
    /*    for(int i= 0; i <= deg; i++)
      cout << "Input  "<<i<<" = " << bz[i] 
	   << " Left  "<<i<<" = " << tempL[i]
	   << " Right  "<<i<<" = " << tempR[i] << endl;
    */
    bz = tempR;
  }


  // Then subdivide [A, 1], the right half, at B if it's not one
  if(B != One){
    //    cout<<"Subdividing at "<< B << endl;
    deCasteljau(bz, tempL, tempR, B, A, One, deg, isExactArith, prec);
    /*    for(int i= 0; i <= deg; i++)
      cout << "Input  "<<i<<" = " << bz[i]
	   << " Left  "<<i<<" = " << tempL[i]
	   << " Right  "<<i<<" = " << tempR[i] << endl;
    */
    bz = tempL;
  }
}

/*
  Returns the Bernstein coefficient of P on [0,1] in Q
  if isPos=true; otherwise the Bernstein coefficients are w.r.t. [-1,0].
 */
template <typename T, typename FT, typename VEC>
  void convertToBernstein(const Polynomial<T> &P, Polynomial<FT> &Q, 
			  int deg, VEC &Binomial,
			  bool isPos, bool fractionFree){

  for(int i=0; i <= deg; i++){
    Q.coeff()[i] = P.coeff()[i];
  }
  
  if(!isPos){
    for(int i=0; i <= deg; i++)
      if(i % 2 != 0){
	Q.coeff()[i] *= -1;
      }
  }

  if(fractionFree){
    BigInt degfact=1, ifact=1;
    for(int i=1; i<= deg; i++)
      degfact *=i;
    
    Q.coeff()[0]*= degfact; Q.coeff()[deg] *= degfact; 
    degfact = div_exact(degfact, deg);
    
    for(int i=1; i< deg; i++){
      Q.coeff()[i] *= ifact * degfact;
      ifact *= (i+1); degfact = div_exact(degfact, deg -i);
    }
  }else{
    for(int i = 1; i < deg; i++){
      if(i <= deg/2){
	Q.coeff()[i] /= Binomial[i];
      }
      else{
	Q.coeff()[i] /= Binomial[deg-i];
      }
    }
  }
  for(int i =1; i<= deg; i++)
    for(int j=deg; j >= i; j--){
      Q.coeff()[j]+= Q.coeff()[j-1];
    }
}

/*
  FUNCTIONS FOR THE EVAL ALGORITHM
 */
// Function for evaluating all the Taylor coefficients of a polynomial 
// around a point x. That is, outputs P^(j)(X)/j! for any point X.
// Involves only linear number of multiplications and divisions; 
// the additions are still quadratic.
template <typename T, typename RT>
void evalAllLin(Polynomial<T>& P, int deg, RT x, MAX_TYPE(RT, T) *seq)
{
  //std::cout << "X = " << x << std::endl;
  if(x != 0){
    MAX_TYPE(RT, T) powx=1;
    for(int i=0; i <= deg; i++){
      seq[i] = P.coeff()[i]*powx;
      powx *= x;
      /*      
      std::cout << "coeff[" << i <<"] = "<< P.coeff()[i] << std::endl;
      std::cout << "powx = "<< powx << std::endl;
      std::cout << "seq[" << i <<"] = "<< seq[i] << std::endl;
      */
    }
    powx =1;
    for(int i=0; i < deg; i++){
      for(int j=deg-1; j >= i; j--){
	seq[j] += seq[j+1];
      }
      std::cout << "seq[" << i <<"] = "<< seq[i] << " powx = " << powx << std::endl;
      seq[i] = div_exact(seq[i], powx);// Divide out the suitable power of x.
      powx *=x;
    }
    seq[deg] = P.coeff()[deg];
  }else{
    for(int i=0; i <= deg; i++)
      seq[i] = P.coeff()[i];
  }
}


template <typename T, typename RT>
  std::pair<MAX_TYPE(RT, T), MAX_TYPE(RT, T)> 
  box(const std::pair<RT, RT>& seg, Polynomial<T> &P, int deg)
{
  typedef MAX_TYPE(RT, T) MT;

  RT midpoint = (seg.first + seg.second).div2();
  //Polynomial <T> temp(P);
  std::cout <<"Input interval " << seg.first <<": " << seg.second << std::endl;
  MT evals[deg+1];
  evalAllLin(P, deg, midpoint, evals);
  std::cout <<"After evaluating all derivatives " << std::endl;
  MT val=0;
  RT halfwidth = (seg.second - seg.first).div2();
  MT t;
  for(int i=deg; i > 0; i--){
    std::cout << "Val before " << val <<std::endl;
    val *=halfwidth;
    val += core_abs(evals[i]);
    std::cout << "Val after " << val <<std::endl;
  }
  val *= halfwidth;
  std::pair<MT, MT> J = std::make_pair(-val,val);
  t = evals[0];
  J.first += t; J.second +=t;
  std::cout << "Range returned " << J.first << ":" << J.second <<std::endl;
  return J;
}

// Do two closed intervals I, J overlap
template <typename RT>
bool ifIntersect(const std::pair<RT, RT>& I, const std::pair<RT, RT>& J){
  if(I.second < J.first || J.second < I.first)
    return false;
  else
    return true;
}


// Interval Newton operator. Computes the interval Newton operator of P, I.
// In our implementation zero is always contained in PDIFF(I)=PdiffI, and hence
// the interval Newton operator will return intervals that are unbounded
// on at least one side. Thus we internally check whether these 
// intervals intersect with the left half and right half of I; if left half
// intersects then we set leftInter to true, and similarly for right half.
// We assume that the midpoint of I is not a root, and by default leftInter
// and rightInter are false.
template <typename T, typename RT>
  void newton(const std::pair<RT, RT>& I, const Polynomial<T> &P, 
	      const std::pair<MAX_TYPE(RT, T), MAX_TYPE(RT, T)> &PdiffI, 
	      unsigned int deg, bool& leftInter, bool& rightInter){
  typedef MAX_TYPE(RT, T) MT; // We assume MAX_TYPE is BigFloat2

  RT m =  (I.first + I.second).div2();
  MT Pm = P.eval(m);
  BigFloat2 M(m), PM(Pm), PDIFFI(PdiffI.first, PdiffI.second), J;
  J = M - PM/PDIFFI;
  BigFloat2 L(I.first, m), R(m, I.second);

  if((J - L).isZeroIn())
    leftInter=true;
  if((J - R).isZeroIn())
    rightInter=true;
}



#endif
