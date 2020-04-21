/****************************************************************/
/*    NAME: Maria Eleftheriou / Jose Moreira                    */
/*    T.J. Watson Research Center                               */
/*    Since Core 1.6						*/
/*    LAST MODIEFIED: Tue. May. 29 , 2001                       */
/****************************************************************/ 

#include <fstream>
#include <algorithm>
//#include "MultiPrecision.h"
#define CORE_LEVEL 4
#include "CORE.h"

//DEBUG_HYPERGEOM is for internal use only!
//#define DEBUG_HYPERGEOM

#ifdef _MSC_VER
#  define CORE_TYPENAME
#else
#  define CORE_TYPENAME typename
#endif


//////////////////////////////////////////////////////////////////////////////
// MATH CONSTANTS
//////////////////////////////////////////////////////////////////////////////
#define PI_LO_THRESHOLD 300		// This is the number of bits in
					// static constant "SmallPi"
#define PI_HI_THRESHOLD 332193		// This is the number of precomputed
					// bits of Pi in the file (=100,000 digits)
#define LOG2_LO_THRESHOLD 300		// This is the number of bits in
					// static constant "SmallLog2" (base e)
#define LOG2_HI_THRESHOLD 332193	// This is the number of precomputed bits
					// of Log2 in the file (=100,000 digits)

const Expr SmallPi  =  "3.14159265358979323846264338327950288419716939937510\
5820974944592307816406286208998628034825342117066";
const Expr SmallLog2 = "0.69314718055994530941723212145817656807550013436025\
52541206800094933936219696947156058633269964186875";

//////////////////////////////////////////////////////////////////////////////
// Global variable to choose different version
//////////////////////////////////////////////////////////////////////////////
int hyperVersion = 0;
int setHyperVersion(int newVersion) {
  int oldVersion = hyperVersion;
  hyperVersion = newVersion;
  return oldVersion;
}

//////////////////////////////////////////////////////////////////////////////
// class MultiPrecision<FLT>::ElementaryFunction

template <class FLT>
typename MultiPrecision<FLT>::ElementaryFunction* MultiPrecision<FLT>::ElementaryFunction::getInstance(KeyType name)
{
  if(_registry[name] == 0) { 
    std::cout << " ERROR:: registry" << std::endl;
    exit(1);
  }
  return _registry[name];
}
 
template <class FLT>
void MultiPrecision<FLT>::ElementaryFunction::registration(KeyType name, typename MultiPrecision<FLT>::ElementaryFunction* ptr)
{ 
  if(_registry.count(name) == 0) {
    _registry[name] = ptr;
    preprocess(name, ptr); // pre-prossing parameters
  }
}

template <class FLT>
void MultiPrecision<FLT>::ElementaryFunction::preprocess(KeyType name, typename MultiPrecision<FLT>::ElementaryFunction* ptr)
{ 
  // insert implict parameter
  (ptr->BB).push_back(FLT(1));
  // sort parameters
  std::sort((ptr->AA).begin(), (ptr->AA).end());
  std::sort((ptr->BB).begin(), (ptr->BB).end());
  // elminate common parameters
  typename std::vector<FLT>::iterator iter1, iter2;
  for (iter1 = (ptr->AA).begin(); iter1 < (ptr->AA).end(); iter1++) {
    iter2 = std::find((ptr->BB).begin(), (ptr->BB).end(), *iter1);
    if (iter2 != (ptr->BB).end()) {
      (ptr->AA).erase(iter1);
      (ptr->BB).erase(iter2);
    }
  }
  // find equivalent parameters
  for (iter1 = (ptr->AA).begin(); iter1 < (ptr->AA).end(); iter1++) {
    iter2 = std::find((ptr->BB).begin(), (ptr->BB).end(), (*iter1)+1);
    if (iter2 != (ptr->BB).end()) {
      (ptr->CC).push_back(*iter1);
      (ptr->DD).push_back(*iter2);
      (ptr->AA).erase(iter1);
      (ptr->BB).erase(iter2);
    }
  }
  // check divergence
  if ((ptr->AA).size() > (ptr->BB).size()) {
    std::cout << "Divergent Series!" << std::endl;
  }
  // compute truncation error function
  unsigned int i;
  for (i=0; i<(ptr->AA).size(); i++) {
    if ((ptr->AA)[i] > (ptr->BB)[i]) {
      (ptr->EE).push_back((ptr->AA)[i]);
      (ptr->FF).push_back((ptr->BB)[i]);
    }
  }
  for (i=(ptr->AA).size(); i<(ptr->BB).size(); i++) {
      (ptr->FF).push_back((ptr->BB)[i]);
  }
  // print useful information for registered functions
#ifdef DEBUG_HYPERGEOM
  std::cout << "function name: " << name << std::endl;
  std::cout << "Upper Parameters: ";
  for (i=0; i<(ptr->AA).size(); i++) {
    std::cout << (ptr->AA)[i] << "," ;
  }
  std::cout << std::endl;

  std::cout << "Lower Parameters: ";
  for (i=0; i<(ptr->BB).size(); i++) {
    std::cout << (ptr->BB)[i] << "," ;
  }
  std::cout << std::endl;

  std::cout << "Equivalent Parameters: ";
  for (i=0; i<(ptr->CC).size(); i++) {
    std::cout << (ptr->CC)[i] << "," ;
  }
  std::cout << std::endl;
  
  std::cout << "Truncation Error Function: ";
  if (!(ptr->EE).empty()) {
    for (i=0; i<(ptr->EE).size(); i++) {
     std::cout << "(" << (ptr->EE)[i] << "+n)";
    }
  } else {
    std::cout << "1";
  }
  if (!(ptr->FF).empty()) {
    std::cout << "/";
    for (i=0; i<(ptr->FF).size(); i++) {
      std::cout << "(" << (ptr->FF)[i] << "+n)";
    }
  }
  std::cout << std::endl;
#endif
}

//////////////////////////////////////////////////////////////
// eval( key, argument, abs_prec )
//////////////////////////////////////////////////////////////

template <class FLT>
FLT MultiPrecision<FLT>::ElementaryFunction::eval(
      KeyType key, FLT& x, const long& aPrec) 
{
  static bool first = true;

  if (first) {
    MultiPrecision<FLT>::ElementaryFunction::initialize();
    first = false;
  }

  CORE_TYPENAME MultiPrecision<FLT>::ElementaryFunction* fn = MultiPrecision<FLT>::ElementaryFunction::getInstance(key);
  FLT xx = fn->getArgument(x);
  FLT zz = fn->getPrefix(x);
  return MultiPrecision::HyperGeometric::F(fn->AA, fn->BB, fn->CC, fn->DD, fn->EE, fn->FF, xx, zz, aPrec);
}

template<class FLT> 
void MultiPrecision<FLT>::ElementaryFunction::initialize() 
{
  registration(MultiPrecision::Exp::_key, 
	new CORE_TYPENAME MultiPrecision<FLT>::Exp());
  registration(MultiPrecision::Erf::_key,
	new CORE_TYPENAME MultiPrecision::Erf());
  registration(MultiPrecision::Sin::_key,
	new CORE_TYPENAME MultiPrecision::Sin());
  registration(MultiPrecision::Cos::_key, 
	new CORE_TYPENAME MultiPrecision::Cos());
  registration(MultiPrecision::Tan::_key, 
	new CORE_TYPENAME MultiPrecision::Tan());
  registration(MultiPrecision::Cot::_key, 
	new CORE_TYPENAME MultiPrecision::Cot());
  registration(MultiPrecision::Asin::_key, 
	new CORE_TYPENAME MultiPrecision::Asin());
  registration(MultiPrecision::Atan::_key, 
	new CORE_TYPENAME MultiPrecision::Atan());
  registration(MultiPrecision::Acos::_key, 
	new CORE_TYPENAME MultiPrecision::Acos());
  registration(MultiPrecision::SLog::_key, 
	new CORE_TYPENAME MultiPrecision::SLog());
  registration(MultiPrecision::Log::_key, 
	new CORE_TYPENAME MultiPrecision::Log());
}

template<class FLT> 
typename std::map<KeyType,
                  CORE_TYPENAME MultiPrecision<FLT>::ElementaryFunction* >
MultiPrecision<FLT>::ElementaryFunction::_registry; 

//////////////////////////////////////////////////////////////////////////////
// class Exp, Erf, Sin, Cos, Tan, Cot, Atan, Asin, Acos, Log

template <class FLT> 
KeyType MultiPrecision<FLT>::Exp::_key = "Exp";  
template <class FLT>
KeyType MultiPrecision<FLT>::Erf::_key = "Erf";
template <class FLT>
KeyType MultiPrecision<FLT>::Sin::_key = "Sin";
template <class FLT>
KeyType MultiPrecision<FLT>::Cos::_key = "Cos";
template <class FLT>
KeyType MultiPrecision<FLT>::Tan::_key = "Tan";
template <class FLT>
KeyType MultiPrecision<FLT>::Cot::_key = "Cot";
template <class FLT>
KeyType MultiPrecision<FLT>::Atan::_key = "Atan";
template <class FLT>
KeyType MultiPrecision<FLT>::Asin::_key = "Asin";
template <class FLT>
KeyType MultiPrecision<FLT>::Acos::_key = "Acos";
template <class FLT>
KeyType MultiPrecision<FLT>::SLog::_key = "SLog";
template <class FLT>
KeyType MultiPrecision<FLT>::Log::_key = "Log";

//////////////////////////////////////////////////////////////////////////////
// read precomputed pi

Expr MathPi(int bits) {
  if (bits < PI_LO_THRESHOLD) return SmallPi;
  assert(bits < PI_HI_THRESHOLD);
  std::ifstream ifs;
  ifs.open("pi.big");
  BigFloat bf_pi;
  readFromFile(bf_pi, ifs, bits); // this has changed in Core2...
  Expr pi(bf_pi);
  pi.approx(CORE_posInfty, bits);
  return pi;
}

Expr MathPiOverTwo(int bits) {
  return MathPi(bits-1)/2;
}

Expr MathTwoPi(int bits) {
  return 2*MathPi(bits+1);
}

Expr MathLog2(int bits) {
  if (bits < LOG2_LO_THRESHOLD) return SmallLog2;
  assert(bits < LOG2_HI_THRESHOLD);
  std::ifstream ifs;
  ifs.open("log2.big");
  BigFloat bf_log2;
  readFromFile(bf_log2, ifs, bits);
  Expr val(bf_log2);
  val.approx(CORE_posInfty, bits);
  return val;
}

Expr MathNaN() {
  std::cerr << "Expr::NaN" << std::endl;
  return Expr(0); // need define NaN constants in Expr!!
}
//////////////////////////////////////////////////////////////////////////////
// function Exp, Erf, Sin, Cos, Tan, Cot, Atan, Asin, Acos, Log

template<class FLT> 
FLT exp(FLT x)
{
  int eps = defRelPrec.asLong();
  FLT LOG2 = MathLog2(eps);
  long k = longValue(floor(x/LOG2));

  FLT xx = x - k*FLT(LOG2); 
  xx.approx(CORE_posInfty, eps+k+4);

  return pow(FLT(2),k)*MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Exp::_key, xx, eps+k+1);
}

template<class FLT> 
FLT sin(FLT x)
{
  int eps = defAbsPrec.asLong();
  //std::cerr << "eps=" << eps << std::endl;
  if (x > 6.283185307179586476925286766557) {
    FLT TWOPI = MathTwoPi(eps);
    BigInt n = floor(x / TWOPI);
    FLT xx = x - TWOPI * n; 
    xx.approx(CORE_posInfty, eps+1);
    return MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Sin::_key, xx, eps+1);
  } else
    return MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Sin::_key, x, eps+1);
}

template<class FLT> 
FLT cos(FLT x)
{ 
  int eps = defAbsPrec.asLong();
  FLT TWOPI = MathTwoPi(eps);

  BigInt n = floor(x / TWOPI);
  FLT xx = x - TWOPI * n; 
  xx.approx(CORE_posInfty, eps+1);

  return MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Cos::_key, xx, eps+1);
}
 
template<class FLT> 
FLT tan(FLT x)
{ 
  int eps = defAbsPrec.asLong();
  FLT TWOPI = MathTwoPi(eps);

  BigInt n = floor(x / TWOPI);
  FLT xx = x - TWOPI * n; 

  if ( xx == MathPi(eps) )
    return MathNaN();
  else
    return sin(xx)/cos(xx);
}

template<class FLT> 
FLT cot(FLT x)
{ 
  int eps = defAbsPrec.asLong();
  FLT TWOPI = MathTwoPi(eps);

  BigInt n = floor(x / TWOPI);
  FLT xx = x - TWOPI * n; 

  if ( xx == 0 )
    return MathNaN();
  else
    return cos(xx)/sin(xx);
}

template<class FLT> 
FLT erf(FLT x)
{ 
  int eps = defAbsPrec.asLong();
  FLT PI = MathPi(eps);
  double d = x.doubleValue();
  if (d == 0)
    return 0;
  else if (d < -6)
    return -1;
  else if (d > 6)
    return 1;
  
  int sign = (x < 0) ? -1 : 1;  
  FLT xx = x * sign;

  return sign*2/sqrt(PI)*MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Erf::_key, xx, eps);
}

template<class FLT> 
FLT asin(FLT x)
{
  int eps = defAbsPrec.asLong();
  FLT PIOVERTWO = MathPiOverTwo(eps+1);

  int sign = (x < 0) ? -1 : 1;  
  FLT xx = x * sign;
  
  if(xx > FLT(1)) {
    return MathNaN();
  } else if (xx == FLT(1)) {
    return sign*PIOVERTWO;
  } else if(xx <= FLT(.5)) {
    return sign*MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Asin::_key, xx, eps);
  } else {
    xx  = sqrt((1 - x) / 2);
    return sign*(PIOVERTWO-2*MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Asin::_key, xx, eps+2));
  } 
}

template<class FLT> 
FLT acos(FLT x)
{
  int eps = defAbsPrec.asLong();
  FLT PI = MathPi(eps);

  int sign = (x < 0) ? -1 : 1;  
  FLT xx = x * sign;
  
  if(xx > FLT(1)) {
    return MathNaN();
  } else if (xx == FLT(1))
    return FLT(0);
  
  FLT arg = sqrt(FLT(1) - xx*xx);  
  if (sign == 1)
    return asin(arg);
  else  
    return PI - asin(arg);
}

template<class FLT> 
FLT atan(FLT x)
{
  int eps = defAbsPrec.asLong();
  FLT PIOVERTWO = MathPiOverTwo(eps+1);

  int sign = (x < 0) ? -1 : 1;  
  FLT xx = x*sign;

  if(xx <= FLT(1)) {
    return sign*(MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Atan::_key, xx, eps));
  } else {
    xx = -1/xx;
    return sign*(PIOVERTWO+MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Atan::_key, xx, eps+1));
  }  
}

template<class FLT> 
FLT slog(FLT x)
{
  if (x < 0)
    return MathNaN();
 
  int eps = defAbsPrec.asLong();
  FLT xx = x;
  FLT kk = 0;

  if (xx > FLT(2)) {
    long tmp = x.longValue();
    long k = flrLg(tmp);  
    kk = k*MathLog2(eps+clLg(k)+1);
    FLT xx = x / (1 << k); 
    xx.approx(CORE_posInfty, eps+2);
  }

  return kk+(MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::SLog::_key, xx, eps+2)); 
}

template<class FLT> 
FLT log(FLT x)
{
  if (x < 0)
    return MathNaN();

  int eps = defAbsPrec.asLong();
  FLT xx = x;
  FLT kk = 0;

  if (xx > FLT(2)) {
    long tmp = x.longValue();
    long k = flrLg(tmp);  
    kk = MathLog2(eps+clLg(k)+1);
    xx = x / (1 << k); 
    xx.approx(CORE_posInfty, eps+2);
  }

  return kk+MultiPrecision<FLT>::ElementaryFunction::eval(MultiPrecision<FLT>::Log::_key, xx, eps+2);
}

template Expr exp <Expr>(Expr);
template Expr sin <Expr>(Expr);
template Expr cos <Expr>(Expr);
template Expr erf <Expr>(Expr);
template Expr asin<Expr>(Expr);
template Expr acos<Expr>(Expr);
template Expr atan<Expr>(Expr);
template Expr slog<Expr>(Expr);
template Expr log <Expr>(Expr);
template Expr tan <Expr>(Expr);
template Expr cot <Expr>(Expr);

