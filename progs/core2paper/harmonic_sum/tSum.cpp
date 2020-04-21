#define CORE_LEVEL_4
#include <vector>
#include <CORE/CORE.h>
#include <CORE/Timer.h>

#ifdef _USE_SUM
// NOTE: this is builtin Core2 function (called summation):
template <typename Function, typename T>
Expr sum(Function fun, T start, T end) {
  std::vector<ExprRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>*> c;
  SumOpRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>* rep
    = new SumOpRepT<Expr::RootBdT, Expr::FilterT, Expr::KernelT>(c);
  for (T it = start; it <= end; ++it) {
    rep->insert(fun(it).rep());
  }

  return Expr(rep);
}
#else
template <typename Function, typename T>
Expr sum(Function fun, T start, T end) {
  Expr sum(0);
  for(T it = start; it <= end; ++it)
    sum += fun(it);
  return sum;
}

#endif

Expr term(int i) {
  return Expr(1)/Expr(i);
}

int main(int argc, char** argv) {
  int num = 100000;
  if (argc >= 2)
    num = atoi(argv[1]);

  if(num < 2)
	std::cerr << "ERROR!!! Input argument should be greater than 1." << std::endl;
 
  Timer2 timer;
  BigFloat r;

  timer.start();
  Expr e = sum(term, 1, num);
  r = e.approx(60, CORE_INFTY);
  timer.stop();

  std::cout << "r=" << r << std::endl;
  std::cout << "it took " << timer.get_mseconds()<< " microseconds." << std::endl;

  return 0;
}
