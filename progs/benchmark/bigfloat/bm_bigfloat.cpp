#define CORE_LEVEL 4
#include <CORE/CORE.h>

int main(int argc, char* argv[]) {
  int n = 100;
  prec_t prec = 54;

  if (argc > 1)
    n = atoi(argv[1]);
  if (argc > 2)
    prec = atoi(argv[2]);

  Timer2 timer;
  timer.start();
  BigFloat x;
  for (int i=2; i<n; ++i) {
     x.sqrt(BigFloat(i), prec);
     //std::cout << "sqrt(" << i << ")=" << x << std::endl;
  }
  timer.stop();

  std::cout << "it took " << timer.get_mseconds() << " mseconds!" << std::endl;

  return 0;
}
