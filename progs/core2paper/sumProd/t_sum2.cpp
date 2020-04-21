#define CORE_LEVEL 4
#include <vector>
#include <iomanip>
#include <CORE/CORE.h>
#include <CORE/Timer.h>

template <typename RootBd, typename Filter, typename Kernel>
class SumOpRepT : public ExprRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  std::vector<ExprRep*> children;
public:
  SumOpRepT() {
      this->m_filter.set(0.0);
  }
  virtual ~SumOpRepT() {
    for (size_t i=0; i<children.size(); i++)
      (children[i])->dec_ref();
  }
  void insert(ExprRep* child) {
    children.push_back(child);
    child->inc_ref();
    this->m_filter.addsub(this->m_filter, child->m_filter, true);
  }
  virtual void compute_sign() {
    this->sign() = 1;
  }
  virtual void compute_uMSB() {
    this->uMSB() = children[0]->get_uMSB();
    for (size_t i=1; i<children.size(); i++) { 
      this->uMSB() = std::max(this->uMSB(), children[i]->get_uMSB()) + 1;
    }
    std::cout << "uMSB() = " << this->uMSB() << std::endl;
    /*
    msb_t max_uMSB = 0;
    msb_t t;
    for (size_t i=0; i<children.size(); i++) {
       t = children[i]->get_uMSB();
       if (max_uMSB < t) max_uMSB = t;
    }
    this->uMSB() = ceillg(children.size()) + max_uMSB;
    */
  }
  virtual void compute_lMSB() {
    this->lMSB() = children[0]->get_lMSB();
    for (size_t i=1; i<children.size(); i++) { 
      this->lMSB() = std::max(this->lMSB(), children[i]->get_lMSB());
    }
    std::cout << "lMSB() = " << this->lMSB() << std::endl;
    /*
    msb_t max_lMSB = 0;
    msb_t t;
    for (size_t i=0; i<children.size(); i++) {
       t = children[i]->get_lMSB();
       if (max_lMSB < t) max_lMSB = t;
    }
    this->lMSB() = max_lMSB;
    */
  }
  virtual void compute_rootBd() {
    this->rootBd().set(0L);
    for (size_t i=0; i<children.size(); i++) {
      this->rootBd().addsub(this->rootBd(), children[i]->get_rootBd());
    }
  }
  virtual bool compute_a_approx(prec_t prec) {
    int n = ceillg(children.size()) + 1;
    this->appValue().set(0, prec+n);

    mpfr_ptr* tab = new mpfr_ptr[children.size()];
    for (size_t i=0; i<children.size(); i++) {
       tab[i] = (children[i]->a_approx(prec+n)).m_l.mp();
    }
    int r = mpfr_sum(this->appValue().m_l.mp(), tab, children.size(), GMP_RNDN);
    delete[] tab;
    return r == 0;
  }
}; 
    

template <typename Function, typename T>
Expr sum(Function fun, T start, T end) {
  SumOpRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>* rep
    = new SumOpRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>();
  for (T it = start; it <= end; ++it)
    rep->insert(fun(it).rep());
  return Expr(rep);
}

template <typename TYPE>
TYPE term(int i) {
  return TYPE(1)/TYPE(i);
}

int main(int argc, char** argv) {
  int num = 100000;
  if (argc >= 2)
    num = atoi(argv[1]);
 
  Timer2 timer;

  timer.start();
  double d = 0;
  for (int i=1; i<=num; ++i) {
    d += term<double>(i);
  }
  timer.stop();
  std::cout << "d=" << std::setprecision(17) << d << std::endl;
  std::cout << "IEEE double tooks " << timer.get_mseconds() << " mseconds." << std::endl;

  Expr e = sum(term<Expr>, 1, num);
  timer.start();
  BigFloat r = e.approx(60, CORE_INFTY);
  timer.stop();

  std::cout << "r=" << r << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << " mseconds." << std::endl;

  return 0;
}
