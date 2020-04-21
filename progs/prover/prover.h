/***************************************************
  file: prover.h
   	-- include file for prover.cc

   author: Daniela Tulone (August, 1999)

   Since CORE Library version 1.2
   $Id: prover.h,v 1.1 2006/03/07 04:51:25 exact Exp $
 ***************************************************/

#include "terms.h"

class Prover{

  int n;           /* number variables */
   
  int m;           /* number parameters */

  int temp;

  Term** f;        /* set of polynomials hypotheses */

  Term* g;         /* polynomial thesis */

  int bound;       /* Parameters are randomly choosen in [-bound, +bound] */

  double* values;  /* array of values for parameters and variables */

  int limit;      /* working variable, to indicate the first var to be computed */

  int* degrees;   /* matrix (3 x n) containing the partial degree
                     degree[i, 0] = degree of poly ith of the hypothesis
                                    system with respect to the variable i-1

                     degree[i, 1] = degree of poly ith of the hypothesis
                                    system with respect to the variable i

                     degree[i, 2] = degree of poly ith of the hypothesis
		                    system with respect to the variable i+1 */ 

  int N;            /* number of tests to do, if N examples succeed then with
                       probability 2^(-N) the conjucture is true */


public:
  Prover(char *filename, int N);

  ~Prover();

  int VerifyByExamples();

private:

  int parseParameters(char* s);

  Term* parseString(char* s, Term* last);  

  void computeDegrees();  /* compute at the beginning degrees this array will 
			     be used by computeConfiguration and computeMiniSystem 
			     to find out which method apply to solve the system */
  int random();          /* return a random integer in [-bound, bound] */
  int randSign();        /* return 1 or -1 */  
  void computeBound();
  void generateExample(); /* generate random parameters, by resetting values[0] to
			     values[m-1] */
  void resetValues();                   
  int initialization();   /* call computeDegree and handle error checking for the 
			     input */

  int computeConfiguration(); /* compute the hypothesis system by looking at degrees
				 and calling the appropriate method */

  int computeMiniSystem(int ind, int ind2); /* it compute two new variables by
					       solving the system given by
					       f[inf], f[ind2] */

  int solveKramer(Term* t1, Term* t2, int i, int j); /* compute the variables i, j
							by Kramer */

  int solveQuadr(Term* t1, Term* t2, int i, int j);  /* compute the system by substitution
							where t1 is a quadratic equation
							and t2 a linear one */

  Term* subctract(Term* t1, Term* t2, int i, int j); /* it returns the linear equation 
							obtained by substracting two 
							quadratic equations */

  int solveQuadrEq(Term* t, int i);

  int solveLinearEq(Term* t, int i);

  int vanishThesis();                     /* it checks the vanishing of the thesis by 
					     substituting parameters and variables */

  void getCoeffABC(Term *t1, double *a, double *b, double *c, int i, int j);
                                          /* in an equation in i,j a is the coeff of i,
					     b is coeff of j and c is the temine noto */


  // auxiliary functions

  int getOtherVar(int ind);              /* look in degree for an entry different from 0, 
					    if some error occurred, e.g. there is no index
					    i -2 is returned */
  
  double computeTerm(Term *t);           /* return the computed value of term t */

  void getQuadrCoeff(Term* t, double *a, double *b, double *c, int i); 
                                         /* returns the coefficients for a quadratic 
					    equation */

  int validMatrix();                     /* validates degree */

  void checkInput(double coeff, int expo, int ind1, int ind2, int index, int i);
                                         /* validates the input read from the file */

  int valLeftVar(int i);                 /* check if this variables was computed */ 



  // for debug purpose
  void writeInput();
  void writeDegrees();
  void displayVar();

};






