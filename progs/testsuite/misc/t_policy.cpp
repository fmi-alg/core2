// alternative design

/* Definition of rounding modes (DON'T USE GMP_RNDNA!)*/
typedef enum {
  GMP_RNDN=0, GMP_RNDZ, GMP_RNDU, GMP_RNDD, GMP_RND_MAX,
  GMP_RNDNA=-1
} rnd_t;

#include <iostream>
//#include <CORE/Policies.hpp>

using namespace std;


inline void debug(const char* s)
{ cout << s << endl; }

class Double {
  double _v;
public:
  Double() : _v(0) {}
  Double(double v) : _v(v) {}

  void set_prec(int) 
  { }

  // auto version
  void add(const Double& x, const Double& y, rnd_t rnd = GMP_RNDN)
  { debug("auto add"); set_prec(add_prec(x, y)); r_add(x, y, rnd); }
  template <typename T>
  void add(const Double& x, const T& y, rnd_t rnd = GMP_RNDN)
  { set_prec(add_prec(x, y)); r_add(x, y, rnd); }
  template <typename T>
  void add(const T& x, const Double& y, rnd_t rnd = GMP_RNDN)
  { set_prec(add_prec(x, y)); r_add(x, y, rnd); }

  // fix version
  void add(const Double& x, const Double& y, int prec, rnd_t rnd = GMP_RNDN)
  { debug("fix add"); set_prec(prec); r_add(x, y, rnd); }
  template <typename T>
  void add(const Double& x, const T& y, int prec, rnd_t rnd = GMP_RNDN)
  { set_prec(prec); r_add(x, y, rnd); }
  template <typename T>
  void add(const T& x, const Double& y, int prec, rnd_t rnd = GMP_RNDN)
  { set_prec(prec); r_add(x, y, rnd); }

  // raw version
  void r_add(const Double& x, const Double& y, rnd_t rnd = GMP_RNDN)
  { debug("raw add"); _v = x._v + y._v; }
  template <typename T>
  void r_add(const Double& x, const T& y, rnd_t rnd = GMP_RNDN)
  { _v = x._v + y; }
  template <typename T>
  void r_add(const T& x, const Double& y, rnd_t rnd = GMP_RNDN)
  { _v = x + y._v; }

  static int add_prec(const Double& x, const Double& y)
  { return 10; }
};

// auto version
template <typename T, typename TL = T, typename TR = T>
struct AutoArithmeticPolicy {
  static bool add(T& z, const TL& x, const TR& y, int prec, rnd_t rnd)
  { z.add(x, y, rnd); return true; }
};

// fix version
template <typename T, typename TL = T, typename TR = T>
struct FixedArithmeticPolicy {
  static bool add(T& z, const TL& x, const TR& y, int prec, rnd_t rnd)
  { z.add(x, y, prec, rnd); return true; }
};

// raw version
template <typename T, typename TL = T, typename TR = T>
struct RawArithmeticPolicy {
  static bool add(T& z, const TL& x, const TR& y, int prec, rnd_t rnd)
  { z.r_add(x, y, rnd); return true; }
};

class IA {
  Double m_l;
  Double m_r;
  bool m_exact;
  typedef Double FT;
public:
  IA() : m_l(0), m_r(0), m_exact(true)  {}
  IA(double v) : m_l(v), m_r(v), m_exact(true) {}

  bool is_exact() const
  { return m_exact; }
  bool set_exact(bool b)
  { m_exact = b; return m_exact; }

  void add(const IA& x, const IA& y)
  { debug("add(double, double)"); _add<AutoArithmeticPolicy>(x, y); }
  template <typename T>
  void add(const IA& x, const T& y)
  { debug("add(double, T)"); _add<AutoArithmeticPolicy, T>(x, y); }
  template <typename T>
  void add(const T& x, const IA& y)
  { debug("add(T, double)"); _add<AutoArithmeticPolicy, T>(x, y); }

  void add(const IA& x, const IA& y, int prec)
  { debug("add(double, double, p)"); _add<FixedArithmeticPolicy>(x, y, prec); }
  template <typename T>
  void add(const IA& x, const T& y, int prec)
  { debug("add(double, T, p)"); _add<FixedArithmeticPolicy, T>(x, y, prec); }
  template <typename T>
  void add(const T& x, const IA& y, int prec)
  { debug("add(T, double, p)"); _add<FixedArithmeticPolicy, T>(x, y, prec); }

  void r_add(const IA& x, const IA& y)
  { debug("r_add(double, double)"); _add<RawArithmeticPolicy>(x, y); }
  template <typename T>
  void r_add(const IA& x, const T& y)
  { debug("r_add(double, T)"); _add<RawArithmeticPolicy, T>(x, y); }
  template <typename T>
  void r_add(const T& x, const IA& y)
  { debug("r_add(T, double)"); _add<RawArithmeticPolicy, T>(x, y); }

private:
  template <template <typename T, typename TL=T, typename TR=T> class Policy>
  bool _add(const IA& x, const IA& y, int prec = 0) {
    debug("_add(double, doube)");
    if (x.is_exact()) 
      return _add<Policy, FT>(x.m_l, y, prec);
    else if (y.is_exact()) 
      return _add<Policy, FT>(x, y.m_l, prec);
    else {
      Policy<FT>::add(m_l, x.m_l, y.m_l, prec, GMP_RNDD);
      Policy<FT>::add(m_r, x.m_r, y.m_r, prec, GMP_RNDU);
      return set_exact(false);
    }
  }
  template <template <typename, typename, typename> class Policy, typename T>
  bool _add(const IA& x, const T& y, int prec = 0) {
    debug("_add(double, T)");
    Policy<FT, FT, T>::add(m_l, x.m_l, y, prec, GMP_RNDD);
    if (!x.is_exact()) {
      Policy<FT, FT, T>::add(m_r, x.m_r, y, prec, GMP_RNDU);
      return set_exact(false);
    } else
      return set_exact(true);
  }

  template <template <typename, typename, typename> class Policy, typename T>
  bool _add(const T& x, const IA& y, int prec = 0) {
    debug("_add(T, double)");
    Policy<FT, T, FT>::add(m_l, x, y.m_l, prec, GMP_RNDD);
    if (!y.is_exact()) {
      Policy<FT, T, FT>::add(m_r, x, y.m_r, prec, GMP_RNDU);
      return set_exact(false);
    } else
      return set_exact(true);
  }
};

int main() {

/*
  Double x(1.0);
  Double y(2.0);
  Double z;

  // auto  
  z.add(x, y);
  z.add(x, y, GMP_RNDN);

  // fix
  z.add(x, y, 100);
  z.add(x, y, int(GMP_RNDN));
*/

  IA x(1.0);
  IA y(2.0);

  IA z;

  // auto
  z.add(x, y);
  z.add(x, 100);
  z.add(100, y);
  // fix
  z.add(x, y, 100);
  z.add(x, 100, 100);
  z.add(100, y, 100);
  // raw
  z.r_add(x, y);
  z.r_add(x, 100);
  z.r_add(100, y);

  return 0;
}
