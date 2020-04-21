/*************************************************************
file: timelog.cpp

This illustrates the speed of computing log function in following
3 methods:
	a) using standard hypergeometric series w/o preprocessing
	b) using standard hypergeometric series w/ preprocessing.
	c) using transformed hypergeometric series w/ preprocessing.

The demo function computes the common elementary functions
in any sequence of desired precision of the form
	start-prec, start-prec+step, start-prec+2*step, ..., end-prec
and outputs consumed time on the screen.

Usage:
  	> ./timelog value start-prec end-prec step 

  where
        value      = desired value
  	start-prec = start precision 
  	end-prec   = end precision
  	step   = step

  Since Core Library 1.4
  Author: Zilin Du (zilin@cs.nyu.edu)
  $Id: timelog.cpp,v 1.1 2006/03/07 04:54:38 exact Exp $
*************************************************************/  

#include <iostream> 
#include <fstream>
#include <cstring>
#include <cmath>
#include <ctime>

using namespace std;

#define CORE_LEVEL 4
#include "CORE.h"
#include "MultiPrecision.h"

int main(int argc , char ** argv) {
  char str[255];
  int start, end, step;
  int outputPrec;
  long t1, t2;
  
  int eps;
  Expr funVal;
  
  if (argc < 5) {
    cout << "./timelog value start-prec end-prec step" << endl;
    exit(1);
  }
  
  strcpy(str, argv[1]);
  cerr << "Computing log(" << str << ")..." << endl;
 
  start = atoi(argv[2]);
  end = atoi(argv[3]);
  step = atoi(argv[4]);
  cerr << "start from " << start << " to " << end << " by step " << step << endl;
  
  outputPrec = start;
  while (outputPrec <= end) {
    eps = (int)(outputPrec*log(10.0)/log(2.0)); 
    cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;

    Expr val(str, CORE_posInfty);
    setDefaultAbsPrecision(eps);
   
    // using standard series w/o preprocessing 
    setHyperVersion(0);
    t1 = clock();
    funVal = slog(val);
    funVal.approx(CORE_posInfty, eps);
    t2 = clock();
      
    cerr << std::setprecision(outputPrec) << funVal << endl;
    cout << outputPrec << "\t" << (float)(t2-t1)/CLOCKS_PER_SEC;

    // using standard series w/ preprocessing 
    setHyperVersion(1);
    t1 = clock();
    funVal = slog(val);
    funVal.approx(CORE_posInfty, eps);
    t2 = clock();
      
    cerr << std::setprecision(outputPrec) << funVal << endl;
    cout << "\t" << (float)(t2-t1)/CLOCKS_PER_SEC;

    // using transformed series w/ preprocessing 
    t1 = clock();
    funVal = log(val);
    funVal.approx(CORE_posInfty, eps);
    t2 = clock();
      
    cerr << std::setprecision(outputPrec) << funVal << endl;
    cout << "\t" << (float)(t2-t1)/CLOCKS_PER_SEC << endl;


    // Makes output from Core Library consistent with Maple's output:
    //  if (fabs(funVal) < 0.1)	// use scientific notation
    //    cout.setf(ios::scientific, ios::floatfield);
    //  else			// use positional notation
    //    cout.setf(ios::fixed, ios::floatfield);
   
    outputPrec += step;
  }
  
  return 0;
}
