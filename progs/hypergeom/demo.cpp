#include <fstream> 
#include "MultiPrecision.h"      
#define CORE_LEVEL 4
#include "CORE.h"

/* Usage:
  
  ./demo function start end step digits [1]
  
  function  -- exp/erf/log/sin/cos/tan/cot/arcsin/arccos/arctan
  start   -- start value
  end   -- end value
  step    -- step
  digits    -- output digits
  [1]   -- maple program only
*/  

int main(int argc , char ** argv) {
  char filename[255];
  char fun_name[20];
  char str[255];
  double start, end, step;
  int outputPrec;
  bool maple_only;
  
  int eps;
  Expr val, funVal;
  
  if (argc < 6)
    std::cout << "./demo function start end step digits" << std::endl;
  
  strcpy(fun_name, argv[1]);
  std::cerr << "Computing " << fun_name << " function... " << std::endl;
  
  start = atof(argv[2]);
  end = atof(argv[3]);
  step = atof(argv[4]);
  std::cerr << "start from " << start << " to " << end << " by step " << step << std::endl;
  
  outputPrec = atoi(argv[5]); 
  std::cerr << "output digits = " << outputPrec << std::endl;

  if (argc > 6 && atoi(argv[6]) == 1) {
    maple_only = true;
    std::cerr << "generate maple program only!" << std::endl;
  } else
    maple_only = false;
    
  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  std::cerr << "eps = " << eps << std::endl;

  setDefaultAbsPrecision(eps);
  
  strcpy(filename, "maple-");
  strcat(filename, fun_name);
  strcat(filename, ".m");
  std::ofstream ofs;
  ofs.open(filename);
  ofs << "fd := fopen(\"maple-" << fun_name << ".out\", WRITE);" << std::endl;
  double value = start;
  while (value <= end) {
    sprintf(str,"%f",value);
    ofs << "fprintf(fd, \"%a\\n\", evalf(" << fun_name << "(" << str << ")" << ", " << outputPrec << "));" << std::endl;
    if (!maple_only) {
      val = Expr(str, 10, CORE_posInfty);
    
      if (strcmp(fun_name, "exp") == 0)
        funVal = exp(val);
      else if (strcmp(fun_name, "erf") == 0)
        funVal = erf(val);
      else if (strcmp(fun_name, "log") == 0)
        funVal = log(val);
      else if (strcmp(fun_name, "sin") == 0)
        funVal = sin(val);
      else if (strcmp(fun_name, "cos") == 0)
        funVal = cos(val);
      else if (strcmp(fun_name, "tan") == 0)
        funVal = tan(val);
      else if (strcmp(fun_name, "cot") == 0)
        funVal = cot(val);
      else if (strcmp(fun_name, "arcsin") == 0)
        funVal = asin(val);
      else if (strcmp(fun_name, "arccos") == 0)
        funVal = acos(val);
      else if (strcmp(fun_name, "arctan") == 0)
        funVal = atan(val);
  
      funVal.approx(CORE_posInfty, eps);
      if (fabs(funVal) < 0.1)
        std::cout.setf(std::ios::scientific, std::ios::floatfield);
      else
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
    
      std::cerr << "computing " << fun_name << "(" << str << ")..." << std::endl;
      std::cout << std::setprecision(outputPrec) << funVal << std::endl;
    }
    value += step;
  }
  ofs << "fclose(fd);" << std::endl;
  ofs.close();
}
