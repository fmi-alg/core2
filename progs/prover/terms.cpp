/***************************************************
  file: terms.cc
   	-- used by prover.cc

   author: Daniela Tulone (August, 1999)

   Since CORE Library version 1.2
   $Id: terms.cpp,v 1.1 2006/03/07 04:51:25 exact Exp $
 ***************************************************/

#include <fstream>
#include "terms.h"


Term::Term(){
}

Term::Term(double a, int b, int c){
 coeff=a; 
 expo=b; 
 ind1=c; 
 ind2=-1; 
 next=NULL;
}

Term::Term(double a, int b, int c, int d){
 coeff=a; 
 expo=b; 
 ind1=c; 
 ind2=d; 
 next=NULL;
}

Term::Term(double a, int b, int c, Term *n){
  coeff=a; 
  expo=b; 
  ind1=c; 
  ind2=-1; 
  next=n;
}

Term::Term(double a, int b, int c, int d, Term *n){
  coeff=a; 
  expo=b; 
  ind1=c; 
  ind2=d; 
  next=n;
}

Term::Term(double a){
  coeff=a; 
  expo=0; 
  ind1=-1; 
  ind2=-1; 
  next=NULL;
}

Term::Term(double a, Term *n){
  coeff=a; 
  expo=0; 
  ind1=-1; 
  ind2=-1; 
  next=n;
}


void Term::write(int m){
  int i = 0;
  Term *p = this;
  while(p){
    i++;
    if ((p->expo) == 0){
      std::cout << p->coeff << std::endl;
    }
    else{
      if (p->ind2 == -1){
	if (p->ind1 < m)
	  std::cout << p->coeff << "u[" << p->ind1 << "," << p->expo << "] ";
	else
	  std::cout << p->coeff << "x[" << p->ind1 - m<< "," << p->expo << "] ";
      }
      else{
	if (p->ind1 < m)
	  std::cout << p->coeff << "u[" << p->ind1 << "]";
	else{
	  std::cout << p->coeff << "x[" << p->ind1 - m<< "]";
	}
	if (p->ind2 < m)
	  std::cout << "u[" << p->ind2 << "] ";
	else
	  std::cout <<  "x[" << p->ind2 - m<< "] ";
      }
    }
    p = p-> next;
  }
  std::cout << "\n";
}

/* return -2 if none of the variables is i */

int Term::getOtherInd(int i, int m){
  if (ind1 == i+m)
    return ind2;
  if (ind2 == i+m)
    return ind1;
  else
    return -2;
}



