/*************************************************************
file: timehyper.cpp

This illustrates the speed of the hypergeometric package.

The demo function computes the common elementary functions
in any sequence of desired precision of the form
	start-prec, start-prec+step, start-prec+2*step, ..., end-prec
and outputs consumed time on the screen.
Elementary functions such as log, exp, sin, etc are supported.

Usage:
  	> ./timehyper function value start-prec end-prec step 

  where
  	function   = exp/erf/log/sin/cos/tan/cot/arcsin/arccos/arctan
        value      = desired value
  	start-prec = start precision 
  	end-prec   = end precision
  	step   = step

  Since Core Library v1.4
  Author: Zilin Du (zilin@cs.nyu.edu)
  $Id: timehyper.cpp,v 1.1 2006/03/07 04:54:38 exact Exp $

*************************************************************/  

#include <iostream> 
#include <fstream>
#include <cstring>
#include <cmath>
#include <ctime>

using namespace std;

using namespace std;

#define CORE_LEVEL 4
#include "CORE.h"
#include "MultiPrecision.h"

int main(int argc , char ** argv) {
  char fun_name[20];
  char str[255];
  int start, end, step;
  int outputPrec;
  long t1, t2;
  
  int eps;
  Expr funVal;
  
  if (argc < 6) {
    cout << "./timehyper function value start-prec end-prec step" << endl;
    exit(1);
  }
  
  strcpy(fun_name, argv[1]);
  strcpy(str, argv[2]);
  cerr << "Computing " << fun_name << "(" << str << ")..." << endl;
 
  start = atoi(argv[3]);
  end = atoi(argv[4]);
  step = atoi(argv[5]);
  cerr << "start from " << start << " to " << end << " by step " << step << endl;
  
  ofstream ofs;
  ofs.open("time.m");
  ofs << "plot([[0,0]" ;
  outputPrec = start;
  while (outputPrec <= end) {
    eps = (int)(outputPrec*log(10.0)/log(2.0)); 
    cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
    Expr val(str, CORE_posInfty);
    setDefaultAbsPrecision(eps);
    
    t1 = clock();
    if (strcmp(fun_name, "exp") == 0)
      funVal = exp(val);
    else if (strcmp(fun_name, "erf") == 0)
      funVal = erf(val);
    else if (strcmp(fun_name, "slog") == 0)
      funVal = slog(val);
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
    t2 = clock();
    // Makes output from Core Library consistent with Maple's output:
    //  if (fabs(funVal) < 0.1)	// use scientific notation
    //    cout.setf(ios::scientific, ios::floatfield);
    //  else			// use positional notation
    //    cout.setf(ios::fixed, ios::floatfield);
    
    cerr << std::setprecision(outputPrec) << funVal << endl;
    cout << outputPrec << "\t" << (float)(t2-t1)/CLOCKS_PER_SEC << endl;
    ofs << ",[" << outputPrec << "," << (float)(t2-t1)/CLOCKS_PER_SEC << "]";
    outputPrec += step;
  }
  ofs << " ]);" << endl;
  ofs.close();
  
  return 0;
}
