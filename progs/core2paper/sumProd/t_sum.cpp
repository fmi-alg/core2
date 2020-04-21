#define CORE_LEVEL_4
#include <vector>
#include <CORE/CORE.h>
#include <CORE/Timer.h>

BEGIN_DEFINE_KNARY_NODE(Sum)
  BEGIN_DEFINE_RULE_INSERT
    children.push_back(c);
	c->inc_ref();    
    filter().addsub(filter(), c->filter(), true);
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_FILTER
    for(size_t i=0; i<children.size(); i++)
      filter().addsub(filter(), children[i]->filter(), true);
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_NUMTYPE
//    this->_numType = this->child->_numType;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_SIGN
    return false;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_UMSB
    uMSB() = children[0]->get_uMSB();
    for (size_t i=1; i<children.size(); i++) 
      uMSB() = std::max(uMSB(), children[i]->get_uMSB()) + 1;
    return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_LMSB
    return false;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_ROOTBD
    rootBd().set(0L);
    for (size_t i=0; i<children.size(); i++)
      rootBd().addsub(rootBd(), children[i]->get_rootBd());
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_A_APPROX
    int n = ceillg(children.size()) + 1;
    this->appValue().set(0, prec+n);

    bool exact = true;
    for (size_t i=0; i<children.size(); i++) {
       bool r = appValue().add(appValue(), children[i]->a_approx(prec+n), abs2rel(prec+n));
       if (!children[i]->is_exact() || !r) exact = false;
    }
    return exact;
  END_DEFINE_RULE
END_DEFINE_UNARY_NODE

template <typename Function, typename T>
Expr sum(Function fun, T start, T end) {
  std::vector<ExprRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>*> c;
  Sum<Expr::RootBdT, Expr::FilterT, Expr::KernelT>* rep
    = new Sum<Expr::RootBdT, Expr::FilterT, Expr::KernelT>(c);
  for (T it = start; it <= end; ++it) {
    rep->insert(fun(it).rep());
  }

  return Expr(rep);
}

Expr term(int i) {
  return Expr(1)/Expr(i);
}

int main(int argc, char** argv) {
  int num = 100000;
  if (argc >= 2)
    num = atoi(argv[1]);
 
  Timer2 timer;
  BigFloat r;

  timer.start();
  Expr e = sum(term, 1, num);
  r = e.approx(60, CORE_INFTY);
  timer.stop();

  std::cout << "r=" << r << std::endl;
  std::cout << "it tooks " << timer.get_mseconds()<< " mseconds." << std::endl;

  return 0;
}
