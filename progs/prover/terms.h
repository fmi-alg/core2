/***************************************************
  file: terms.h
   	-- include file for terms.cc

   author: Daniela Tulone (August, 1999)

   Since CORE Library version 1.2
   $Id: terms.h,v 1.1 2006/03/07 04:51:25 exact Exp $
 ***************************************************/


#ifndef TERMS_H
#define TERMS_H
 
#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include "CORE.h"

class Term{

  double coeff;
  int ind1;
  int ind2;
  int expo;
  Term* next;

public:

  Term();

  Term(double a, int b, int c);

  Term(double a, int b, int c, int d);

  Term(double a, int b, int c, Term *n);

  Term(double a, int b, int c, int d, Term *n);

  Term(double a);

  Term(double a, Term *n);

  void write(int m);

  int singleInd(){ return(ind2 == -1) ? 1 : 0;}

  Term* getNextTerm(){return next;}

  void setNextTerm(Term *n ){next = n;}

  int getExpo(){return expo;}

  /* return 1 if var i is in the term, i goes 0 ... n-1 */
  int indVar(int i, int m){return((ind1 == i+m)||(ind2 == i+m)) ? 1 : 0;}

  double getCoeff(){return coeff;}
 
  int getInd1(){return ind1;}

  int getInd2(){return ind2;}

  int getOtherInd(int i, int m);

};


#endif








