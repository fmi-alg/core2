/****************************************************************/
/*    NAME: Maria Eleftheriou / Jose Moreira                    */
/*    T.J. Watson Research Center                               */
/*    LAST MODIEFIED: Tue. May. 29 , 2001                       */
/****************************************************************/ 

#ifndef __MultiPrecision_H_
#ifdef __GNUG__
#pragma interface
#endif
#include <iostream>
#include <string>
#include <vector>
#include <map>

////////////////////////////////////////////////////////////// 
// hyperVersion (int v) 
//	--chooses the method of evaluation of hypergeometric functions
//	  v = 0:  straightforward (as in the standard definition)
//	  v = 1:  optimized to pair up equivalent upper and lower parameters
//			(but only pairs that differ by 1)
//	  v = 2:  optimized to pair up equivalent upper and lower parameters
//			for all equivalence classes 
//
//	All the above uses the g(n) function for truncation error
//	NOT IMPLEMENTED YET:
//		-- alternating series
//		-- optimizations for equivalent pairs of the form (a/k, b/k).
//
////////////////////////////////////////////////////////////// 

extern int hyperVersion;
int setHyperVersion(int);

// for portablities, we use global typedef.
typedef const char* KeyType;

template <class FLT>
class MultiPrecision {
        
public:
  class HyperGeometric 
  {
  private:
  // help functions

    // return  (vec[0]+n) * (vec[1]+n) * ... 
    static FLT vecProduct(std::vector<FLT>& vec, unsigned int n) {
      FLT prod(1);
      for (unsigned int i=0; i<vec.size(); i++)
	prod *= (vec[i] + n);
      return prod;
    }
    
    // return  (vec2[0]+n) * (vec2[0]+n-1) * ... * (vec1[0]+1)
    //       * (vec2[1]+n) * (vec2[1]+n-1) * ... * (vec1[1]+1)
    //       * ... 
    static FLT vecProduct(std::vector<FLT>& vec1, std::vector<FLT>& vec2, unsigned int n) {
      FLT prod(1);
      for (unsigned int i=0; i<vec1.size(); i++)
	for (unsigned int k=0; k<vec2[i]-vec1[i]; k++)
	  prod *= (vec2[i] + n - k);
      return prod;
    }
    // compute Truncation Error
    static FLT computeTruncationError(std::vector<FLT>& E, std::vector<FLT>& F, 
      unsigned int n, FLT& x) {
      return vecProduct(E, n) / vecProduct(F, n) * fabs(x);
    }
    
  public:
    //////////////////////////////////////////////////////////////
    // F( A = upper_parameters, B = lower_parameters, C,D,E,F,
    //		x = argument, fz = normalization_constant, aPrec = absolute_prec)
    //////////////////////////////////////////////////////////////

    static FLT F(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
        switch (hyperVersion) {
          case 0: 
	    //straightforward implemetation w/ new truncation error function
            return F0(A, B, C, D, E, F, x, fz, aPrec);
          case 1:
	    //special optimized implemetation w/ new truncation error function
            return F1(A, B, C, D, E, F, x, fz, aPrec); 
          case 2:
	    //general optimized implemetation w/ new truncation error function
            return F2(A, B, C, D, E, F, x, fz, aPrec); 
          case 3: 
	    //straightforward implemetation w/ old truncation error function
            return F3(A, B, C, D, E, F, x, fz, aPrec);
          case 4:
	    //special optimized implemetation w/ old truncation error function
            return F4(A, B, C, D, E, F, x, fz, aPrec); 
          case 5:
	    //general optimized implemetation w/ old truncation error function
            return F5(A, B, C, D, E, F, x, fz, aPrec); 
          default:
            std::cerr << "Unknown version!" << std::endl;
            return FLT(0);
        }
    }
   
    // common arguments description:
    // 
    //   A -- normal upper parameters
    //   B -- normal lower parameters
    //   C -- parameter pairs with integer difference
    //   D -- differences for each pair in C (must have same size with C)
    //
    //           (A[0]+n)...(A[p]+n)     (C[0]+n)...(C[r]+n)
    //   f(n) = --------------------- * ---------------------
    //           (B[0]+n)...(B[q]+n)     (D[0]+n)...(D[r]+n)
    //           
    //           (E[0]+n)...(E[s]+n)
    //   g(n) = ---------------------
    //           (F[0]+n)...(F[t]+n)
    
    // straightforward implementation to compute hypergeometry series
    // w/ new truncation error function
    // 
    static FLT F0(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
      if ( (A.size() > B.size()) || (A.size() == B.size() && fabs(x) > 1) ) {
        std::cout << "Divergent Series!" << std::endl;
        return FLT(0);
      }
      
      FLT g; // monotone decreasing function w.r.t factor function
      FLT term = 1, sum = 1;
      FLT error, precError = FLT(1) / pow (FLT(2), aPrec);
        
      unsigned int n=0;
      do {
        term *= vecProduct(A, n) * vecProduct(C, n) 
	      / (vecProduct(B, n) * vecProduct(D, n)) * x;
	sum += term;
	g = computeTruncationError(E, F, n, x);
	n ++;
      } while (g >= FLT(1.0));

      do {
        term *= vecProduct(A, n) * vecProduct(C, n) 
	      / (vecProduct(B, n) * vecProduct(D, n)) * x;
	sum += term;
	g = computeTruncationError(E, F, n, x);
	error = fabs(term) / (1.0 - g);
	n ++;
      } while (error > precError);
      
      sum *= fz;
      return sum;
    }
    
    // special optimized implementation (for most elmentary functions) 
    // to compute hypergeometry series w/ new truncation error function
    // 
    // (only optimize for parameter pairs with lower = upper + 1)
    static FLT F1(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
      if ( (A.size() > B.size()) || (A.size() == B.size() && fabs(x) > 1) ) {
        std::cout << "Divergent Series!" << std::endl;
        return FLT(0);
      }
      
      FLT g; // monotone decreasing function w.r.t factor function
      FLT term1 = 1, term = 1, sum = 1;
      FLT error, precError = FLT(1) / pow (FLT(2), aPrec);
      
      // pre-computing
      FLT r = 1;
      for (unsigned int i=0; i<C.size(); i++)
        r *= C[i];
      
      unsigned int n=0;
      do {
	term1 *= vecProduct(A, n) / vecProduct(B, n) * x;
        term = r * term1 / vecProduct(D, n); 
	sum += term;
	g = computeTruncationError(E, F, n, x);
	n ++;
      } while (g >= FLT(1.0));

      do {
	term1 *= vecProduct(A, n) / vecProduct(B, n) * x;
        term = r * term1 / vecProduct(D, n); 
	sum += term;
	g = computeTruncationError(E, F, n, x);
	error = fabs(term) / (1.0 - g);
	n ++;
      } while (error > precError);
      
      sum *= fz;
      return sum;
    }
    
    // general optimized implementation (for most elmentary functions) 
    // to compute hypergeometry series w/ new truncation error function
    // 
    // (optimize for parameter pairs with lower = upper + integer)
    static FLT F2(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
      if ( (A.size() > B.size()) || (A.size() == B.size() && fabs(x) > 1) ) {
        std::cout << "Divergent Series!" << std::endl;
        return FLT(0);
      }
      
      FLT g; // monotone decreasing function w.r.t factor function
      FLT term1 = 1, term = 1, sum = 1;
      FLT error, precError = FLT(1) / pow (FLT(2), aPrec);
      
      // pre-computing
      FLT r = 1;
      for (unsigned int i=0; i<C.size(); i++)
	for (unsigned int k=0; k<D[i]-C[i]; k++)
          r *= (C[i] + k);
      
      unsigned int n=0;
      do {
	term1 *= vecProduct(A, n) / vecProduct(B, n) * x;
        term = r * term1 / vecProduct(C, D, n); 
	sum += term;
	g = computeTruncationError(E, F, n, x);
	n ++;
      } while (g >= FLT(1.0));

      do {
	term1 *= vecProduct(A, n) / vecProduct(B, n) * x;
        term = r * term1 / vecProduct(C, D, n); 
	sum += term;
	g = computeTruncationError(E, F, n, x);
	error = fabs(term) / (1.0 - g);
	n ++;
      } while (error > precError);
      
      sum *= fz;
      return sum;
    }
    
    // straightforward implementation to compute hypergeometry series
    // w/ old truncation error function
    // 
    static FLT F3(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
      if ( (A.size() > B.size()) || (A.size() == B.size() && fabs(x) > 1) ) {
        std::cout << "Divergent Series!" << std::endl;
        return FLT(0);
      }
      
      FLT factor, abs_factor, term = 1, sum = 1;
      FLT error, precError = FLT(1) / pow (FLT(2), aPrec);
        
      unsigned int n=0;
      do {
        factor = vecProduct(A, n) * vecProduct(C, n) 
	      / (vecProduct(B, n) * vecProduct(D, n)) * x;
	term *= factor;
	sum += term;
	abs_factor = fabs(factor);
	if (abs_factor != FLT(1.0))
	  error = fabs(term / (1.0 - abs_factor));
	else
          std::cout << "loop, n=" << n << std::endl;
	n ++;
      } while (abs_factor >= FLT(1.0) || error > precError);

      sum *= fz;
      return sum;
    }
    
    // special optimized implementation (for most elmentary functions) 
    // to compute hypergeometry series w/ old truncation error function
    // 
    // (only optimize for parameter pairs with lower = upper + 1)
    static FLT F4(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
      if ( (A.size() > B.size()) || (A.size() == B.size() && fabs(x) > 1) ) {
        std::cout << "Divergent Series!" << std::endl;
        return FLT(0);
      }
      
      FLT factor1, factor, abs_factor, term1 = 1, term = 1, sum = 1;
      FLT error, precError = FLT(1) / pow (FLT(2), aPrec);
        
      // pre-computing
      FLT r = 1;
      for (unsigned int i=0; i<C.size(); i++)
        r *= C[i];
      
      unsigned int n=0;
      do {
	factor1 = vecProduct(A, n) / vecProduct(B, n) * x;
	term *= factor1;
	factor = vecProduct(C, n) / vecProduct(D, n) * factor1;
        term = r * term1 / vecProduct(D, n); 
	abs_factor = fabs(factor);
	if (abs_factor != FLT(1.0))
	  error = fabs(term / (1.0 - abs_factor));
	else
          std::cout << "loop, n=" << n << std::endl;
	n ++;
      } while (abs_factor >= FLT(1.0) || error > precError);
      
      sum *= fz;
      return sum;
    }
    
    // general optimized implementation (for most elmentary functions) 
    // to compute hypergeometry series w/ old truncation error function
    // 
    // (optimize for parameter pairs with lower = upper + integer)
    static FLT F5(std::vector<FLT>& A, std::vector<FLT>& B, std::vector<FLT>& C, 
      std::vector<FLT>& D, std::vector<FLT>& E, std::vector<FLT>& F,
      FLT& x, FLT& fz, const long& aPrec)
    {
      if ( (A.size() > B.size()) || (A.size() == B.size() && fabs(x) > 1) ) {
        std::cout << "Divergent Series!" << std::endl;
        return FLT(0);
      }
      
      FLT factor1, factor, abs_factor, term1 = 1, term = 1, sum = 1;
      FLT error, precError = FLT(1) / pow (FLT(2), aPrec);
        
      // pre-computing
      FLT r = 1;
      for (unsigned int i=0; i<C.size(); i++)
	for (unsigned int k=0; k<D[i]-C[i]; k++)
          r *= (C[i] + k);
      
      unsigned int n=0;
      do {
	factor1 = vecProduct(A, n) / vecProduct(B, n) * x;
	term *= factor1;
	factor = vecProduct(C, n) / vecProduct(D, n) * factor1;
        term = r * term1 / vecProduct(C, D, n); 
	abs_factor = fabs(factor);
	if (abs_factor != FLT(1.0))
	  error = fabs(term / (1.0 - abs_factor));
	else
          std::cout << "loop, n=" << n << std::endl;
	n ++;
      } while (abs_factor >= FLT(1.0) || error > precError);
      
      sum *= fz;
      return sum;
    }
  };

  class ElementaryFunction
  {
  public: 
    static ElementaryFunction* getInstance(KeyType name);
    static void registration(KeyType name, ElementaryFunction* ptr);    
    
  // preprocessing parameters
    static void preprocess(KeyType name, ElementaryFunction* ptr);    

    static FLT eval(KeyType key, FLT& x, const long& aPrec);
    static void initialize();

  protected:
    ElementaryFunction(int sizeA=0, int sizeB=0): AA(sizeA), BB(sizeB) {}
    virtual ~ElementaryFunction() {}    
    virtual FLT getArgument(FLT& x) { return x; }
    virtual FLT getPrefix(FLT& x) { return FLT(1); }
    
  protected:
    std::vector<FLT> AA, BB, CC, DD, EE, FF;    
  private:
    static std::map<KeyType, ElementaryFunction *> _registry;
  };

  // exp series
  // exp(x) = 1 * F(; ; x);
  // 
  class Exp: public ElementaryFunction
  { 
  public: 
    Exp(): ElementaryFunction(0,0) {
    }
    ~Exp() {}
    static KeyType _key;
  };  

  // erf series
  // erf(x) = 2*x/sqrt(Pi) * F(1/2; 3/2; -x*x);
  // (note 2/sqrt(Pi) is missing here)
  class Erf: public ElementaryFunction
  { 
  public:
    Erf(): ElementaryFunction(1,1) {
      this->AA[0] = FLT(0.5);
      this->BB[0] = FLT(1.5);
    }
    ~Erf(){}
    virtual FLT getArgument(FLT& x) {
      return -x*x;
    }
    virtual FLT getPrefix(FLT& x) { 
      return x; 
    }
  public:
    static KeyType _key;
  }; 

  // sin series
  // sin(x) = x * F(; 3/2; -x*x/4)
  //
  class Sin: public ElementaryFunction
  {
  public:
    Sin(): ElementaryFunction(0,1) {
      this->BB[0] = FLT(1.5); 
    }
    ~Sin() {} 
    virtual FLT getArgument(FLT& x) {
      return -x*x/4;
    }
    virtual FLT getPrefix(FLT& x) { 
      return x; 
    }
    static KeyType _key;
  }; 

  // cos series
  // cos(x) = 1 * F(; 1/2; -x*x/4)
  //
  class Cos: public ElementaryFunction
  {
  public:
    Cos(): ElementaryFunction(0,1) {
      this->BB[0] = FLT(0.5);
    }
    ~Cos(){}
    virtual FLT getArgument(FLT& x) {
      return -x*x/4;
    }
  public:
    static KeyType _key;
  };
  
  // tan series
  // tan(x) = sin(x) / cos(x)
  //
  class Tan: public ElementaryFunction
  {
  public:
    Tan(): ElementaryFunction(0,0) {
    }
    ~Tan(){}
  public:
    static KeyType _key;
  };        

  // cot series
  // cot(x) = cos(x) / sin(x)
  //
  class Cot: public ElementaryFunction
  {
  public:
    Cot(): ElementaryFunction(0,0) {
    }
    ~Cot(){}
  public:
    static KeyType _key;
  };        

  // arcsin series
  // arcsin(x) = x * F(1/2,1/2; 3/2; x*x);
  //
  class Asin: public ElementaryFunction
  {
  public:
    Asin(): ElementaryFunction(2,1) {
      this->AA[0] = FLT(0.5);
      this->AA[1] = FLT(0.5);
      this->BB[0] = FLT(1.5);  
    }
    ~Asin(){}
    virtual FLT getArgument(FLT& x) {
      return x*x;
    }
    virtual FLT getPrefix(FLT& x) { 
      return x; 
    }
  public:
    static KeyType _key;
  };

  // arccos series (transform to arcsin)
  // 
  class Acos: public ElementaryFunction
  {
  public:
    Acos(): ElementaryFunction(0,0) {
    }
    ~Acos(){} 
  public:
    static KeyType _key;
  }; 

  // arctan series
  // arctan(x) = x * F(1/2,1; 3/2; -x*x);
  // 
  class Atan: public ElementaryFunction
  {
  public:
    Atan(): ElementaryFunction(2,1) {
      this->AA[0] = FLT(0.5);
      this->AA[1] = FLT(1);
      this->BB[0] = FLT(1.5);  
    }
    ~Atan(){}
    virtual FLT getArgument(FLT& x) {
      return -x*x/4;
    }
    virtual FLT getPrefix(FLT& x) { 
      return x; 
    }
  public:
    static KeyType _key;
  };

  // standard log series
  // log(x) = (x-1) * F(1,1; 2; 1-x) 
  // 
  class SLog: public ElementaryFunction
  {
  public:
    SLog(): ElementaryFunction(2,1) {
      this->AA[0] = FLT(1);
      this->AA[1] = FLT(1);
      this->BB[0] = FLT(2);
    }
    ~SLog() {}
    virtual FLT getArgument(FLT& x) {
      return 1-x;
    }
    virtual FLT getPrefix(FLT& x) { 
      return x-1; 
    }
  public:
    static KeyType _key;
  };

  // Transformed log series
  // log(x) = - 2 * (1-x)/(1+x) * F(1,1/2; 3/2; (1-x)*(1-x)/((1+x)*(1+x)))
  // 
  class Log: public ElementaryFunction
  {
  public:
    Log(): ElementaryFunction(2,1) {
      this->AA[0] = FLT(1);
      this->AA[1] = FLT(0.5);
      this->BB[0] = FLT(1.5);  
    }
    ~Log(){ }
    virtual FLT getArgument(FLT& x) {
      return (1-x)*(1-x)/((1+x)*(1+x));
    }
    virtual FLT getPrefix(FLT& x) { 
      return -2*(1-x)/(1+x); 
    }
  public:
    static KeyType _key;
  };

};

/*
template<class FLT> FLT exp(FLT x);
template<class FLT> FLT erf(FLT x);
template<class FLT> FLT sin(FLT x);
template<class FLT> FLT cos(FLT x);
template<class FLT> FLT tan(FLT x);
template<class FLT> FLT cot(FLT x);
template<class FLT> FLT asin(FLT x);
template<class FLT> FLT acos(FLT x);
template<class FLT> FLT atan(FLT x);
template<class FLT> FLT slog(FLT x);
template<class FLT> FLT log(FLT x);
*/

#include "MultiPrecision.cpp"

#endif
