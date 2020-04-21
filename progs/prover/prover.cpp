/***************************************************
  file: prover.ccp
	-- from prover.cc, but without all the outputs
		to improve timing.

   	Prover uses a randomized approach to testing
	if a geometric theorem about ruler-and-compass
	construction is true.  It uses a generalization
	of Schwartz's lemma (generalized to radical
	expressions).

   author: Daniela Tulone (August, 1999)

   Since CORE Library version 1.2
   $Id: prover.cpp,v 1.3 2010/07/21 16:28:23 exact Exp $
 ***************************************************/
   
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3 
#endif

#include <fstream>
#include <ctime>

#include "CORE.h"
#include "prover.h"
#include "terms.h"

using namespace std;

/* read input from file and initialize the data structure:
   hypothesis system, polynomial thesis and the parameters m, n, N 
*/


Prover::Prover(char *filename, int numb){
  FILE *fp;
  if ((fp = fopen(filename, "r")) == NULL){
    perror("cannot open input file");
    exit(1);
  }
  
  N = numb;
  
  char* s;
  int read_parameters = 1;
  s = (char *) malloc (500);
  temp = -1;
  Term* last = NULL;
  while(!feof(fp)){
    fgets(s, 500, fp);
    if (!feof(fp)){
      if (read_parameters){
	if (parseParameters(s)){
	  read_parameters = 0;
	  if ((n > 0) || (m > 0 )){
	    limit = m;
	    //Term *last = NULL;
	    f = new Term*[n];
	  }
	  else{
	    perror("Please check the number of parameters and dependent variables.");
	    exit(1);
	  }
	}	
      }
      else
	last = parseString(s, last);
    }
  }
  
  fclose(fp);
  
/*
  cout << "\n";
  cout << "Hypotheses System: \n";
  cout << "\n";
  writeInput();
  
  cout << "\n";
  cout << "Polynomial Thesis: \n";
  cout << "\n";
  g->write(m);
  cout << "\n";
 */
  
  computeBound();
  degrees = new int[n*3];
  for (int i=0; i < n; i++){
    degrees[3*i] = 0;
    degrees[3*i+1] = 0;
    degrees[3*i+2] = 0;
  }
  
  int mn = m + n;
  values = new double[mn];
  resetValues();
}


int Prover::parseParameters(char* s){
  
  char* tmp;
  tmp = (char *) malloc (5);
  int i, last_char, par, var;
  int len = strlen(s);
  last_char = var = 0;
  par = 1;
  int j = 0;
  for (i = 0; i < len; i++){
    if (s[i] == '#'){
      break;
    }
    else if (s[i] != ' '){
      if (!last_char)
        last_char = 1;
      tmp[j++] = s[i];
    }
    
    else if (((s[i] == ' ')||(s[i] == '\0'))&&(last_char)){
      tmp[j] = '\0';
      if (par){
        m = atoi(tmp);
        par = 0;
        var = 1;
      }
      else if (var){
        n = atoi(tmp);
        var = 0;
        return 1;
      }
      tmp = NULL;
      tmp = (char *) malloc (20);
      last_char = 0;
      j = 0;
    }
  }
  return 0;
}


Term* Prover::parseString(char* s, Term* last){
  
  char* tmp;
  tmp = (char *) malloc (20);
  int i, last_char, first, second, third, fourth, fifth;
  int index=0, coeff, expo=0, ind1=0, ind2=0;
  int len = strlen(s);
  first = 1;
  second = third = fourth = fifth = coeff = 0;
  last_char = 0;
  int j = 0;
  for (i = 0; i < len; i++){
    if (s[i] == '#')
      break;
    else if (s[i] != ' '){
      if (!last_char)
        last_char = 1;
      tmp[j++] = s[i];
    }
    
    else if (((s[i] == ' ')||(s[i] == '\0'))&&(last_char)){
      tmp[j] = '\0';
      if (first){
        index = atoi(tmp);
        first = 0;
        second = 1;
      }
      else if (second){
        coeff = atoi(tmp);
        second = 0;
        third = 1;
      }
      else if (third){
        expo = atoi(tmp);
        third = 0;
        fourth = 1;
      }
      else if (fourth){
        ind1 = atoi(tmp);
        fourth = 0;
        fifth = 1;
      }
      else if (fifth){
        ind2 = atoi(tmp);
        fifth = 0;
      }
      tmp = NULL;
      tmp = (char *) malloc (20);
      last_char = 0;
      j = 0;
    }
  }
  
  if (fifth){
    ind2 = atoi(tmp);
    fifth = 0;
  }
  
  if (coeff == 0)
    return last;

  checkInput(coeff, expo, ind1, ind2, index, m+n);
  if (index != temp){
    if (index < n){
      f[index] = new Term(coeff, expo, ind1, ind2);
      last = f[index];
    }
    else{
      g = new Term(coeff, expo, ind1, ind2);
      last = g;
    }
  }
  else if (last && (index == temp)){
    last->setNextTerm(new Term(coeff, expo, ind1, ind2));
    last = last->getNextTerm();
  }
  else{
    perror("Please check the input file. DDDDDDDDDDDDDDDDDDDDD");
    exit(1);
  }
  temp = index;
  return last;
}


Prover::~Prover(){
  for (int i = 0; i < n; i++)
    delete[] f[i];
  delete[] f;
  delete[] g;
  delete[] values;
  delete[] degrees;
}


int Prover::VerifyByExamples(){
  
  if (initialization()){

    int seed  = time(0);
    srand(seed);

    int tests = 0;
    while(tests < N){
/*
      cout << "\n";
      cout << "Example n."<< tests+1 << endl;
*/
      generateExample();      
      if (computeConfiguration()){
	if (!vanishThesis()){
	  return 0;
	}
	tests ++;
      }
      else{
/*
	cout << "The following example has been discarded \n";
*/
        displayVar();
      }
      resetValues();
    }
    return 1;
  }
  else{
    cout << "The problem is not defined properly, please check the parameters" << endl;
    return 0;
  }
}  


void Prover::checkInput(double coeff, int expo, int ind1, int ind2, int index, int i){
  if ((coeff == 0) || (expo < 0) || (expo > 2) || (ind1 < -1) || (ind1 >= i) || (ind2 < -1) || (ind2 >= i)){
    cout << "Error in input file: equation n. " << index;
    perror("Please correct your input file ");
    exit(3);
  }
}


void Prover::writeInput(){
  for (int i= 0; i < n; i++){
    cout << "h[" << i << "] = ";
    f[i]->write(m);
  }
}


void Prover::writeDegrees(){
  cout << "\n";
  cout << " Matrix Degrees \n";
  for (int i = 0; i < n; i++){
    cout << "Row " << i << " : " << degrees[3*i] << "  " << degrees[3*i+1] << "  " << degrees[3*i+2] << endl;
  }
}
  

int Prover::initialization(){

  if (!f || !g)
    return 0;
  computeDegrees();
  if ((! degrees)|| (!validMatrix()))
    return 0;
  if (bound < 1)
    return 0; 
  else
    return 1;
}


void Prover::computeDegrees(){
  int i;
  for (i = 0; i < n; i++){
    Term* tmp = f[i];
    int ind = 3*i+1;

    while (tmp){
      int expo = tmp->getExpo();

      if ( expo != 0){
	if (tmp->singleInd()){
	  if ((i > 0) && tmp->indVar(i-1, m) && valLeftVar(i) && (expo > degrees[ind-1])){	   	    degrees[ind-1] = expo;
	  }
	  if (tmp->indVar(i,m) && (expo > degrees[ind])){
	    degrees[ind] = expo;
	  }
	  if (i < (n-1) && tmp->indVar(i+1,m) && (expo > degrees[ind+1])){
	    degrees[ind+1] = expo;
	  }
	}
	else{
	  if((i > 0) && tmp->indVar(i-1, m) && valLeftVar(i) && (expo > degrees[ind-1])){
	    degrees[ind-1] = expo;
	  }
	  if(tmp->indVar(i,m) && (expo > degrees[ind])){
	    degrees[ind] = expo;
	  }
	  if(i < (n-1) && tmp->indVar(i+1, m) && (expo > degrees[ind+1])){
	    degrees[ind+1] = expo;
	  }
	}
      }
      tmp = tmp->getNextTerm();
    }
  }
}


int Prover::valLeftVar(int i){
  if (i < 1){
    perror(" error in yhe algoritm ComputeMatrix");
    exit(4);
    return 0;
  }
  else if (i < 2)
    return 1;
  else{
    if ((degrees[3*(i-1)+1] != 0) && (degrees[3*(i-2)+2] != 0))
      return 0;
    else
      return 1;
  }
}

/* The following bound is related with Schwartz's extension */
void Prover::computeBound(){
// WARNING (Narayan): This #if is intended to expose where exactly
// this code is incompatible with Level 1, and will remain so until
// the LongWrappers are implemented. This is not meant to be a
// coding style or paradigm to be replicated else where, as its
// a pretty bad coding practice to use the #if to choose different
// code paths depending on the core level in such a situation.
//
//
#if CORE_LEVEL==1
  bound = power(float(8), long(n));
#else
  bound = power(float(8), long(n)).intValue();
#endif
  // cout << "The parameters are randomly chosen in [ " << -bound << ", " << bound << " ]" << endl;
}


int Prover::randSign(){
  int a = rand();
  if ((a % 2) == 0)
    return 1;
  else
    return -1;
}


int Prover::random(){
  int a = rand() * randSign();
  return (a % bound);
}


int Prover::validMatrix(){
  int i;
  // the degree can be at most 2
  for (i = 0; i < 3*n; i++)
    if ((degrees[i] != 0) && (degrees[i] != 1) && (degrees[i] != 2))
      return 0;
  // we can have at least one element non-zero 2nd column, and at most two
  for (i = 0; i < n; i++){
    if (!(degrees[3*i] == 0) && !(degrees[3*i+1] == 0) && !(degrees[3*i+2] == 0))
      return 0;
    if (degrees[3*i+1] == 0)
      return 0;
  }
  return 1;
}


void Prover::resetValues(){
  for (int i=0; i < n+m; i++)
    values[i] = 0;
}


void Prover::generateExample(){
  int i;
  for (i = 0; i < m; i++)
    values[i] = random();
}


int Prover::computeConfiguration(){
  int ind = 0;
  while (ind < n){
    if ((degrees[3*ind] == 0)&&(degrees[3*ind+2] == 0)){
      if (degrees[3*ind+1] == 1){
	if (!solveLinearEq(f[ind], ind))
	  return 0;
      }
      else if (degrees[3*ind+1] == 2){
	if (!solveQuadrEq(f[ind], ind))
	  return 0;
      }
      else{
	cout << "ComputeConfiguration Error!! row " << ind << " degree " << degrees[3*ind+1] << " \n";
	return 0;
      }
      ind++;
      limit ++;
    }
    else{
      int ind2 = getOtherVar(ind);
      if (ind2 == -2 ){
	cout << "ComputeConfig Error in degrees \n";
	return 0;
      }
      else{
        if (!computeMiniSystem(ind, ind2))
	  return 0;
      }
      ind += 2;
      limit += 2;
    }
  }
  // cout << " \n";
  // displayVar();
  return 1;
}


void Prover::displayVar(){
  int i;
  cout << "Random Parameters: ";
  for (i = 0; i < m; i++)
    cout << values[i] << " ";
  cout << "\n";
  cout << "Dependent Variables: ";
  for (i = m; i < m+n; i++)
    cout << values[i] << " ";
  cout << endl;   
}

int Prover::getOtherVar(int ind){

  if (((degrees[3*ind] != 0) && (degrees[3*ind+2] != 0)) || ((degrees[3*ind] == 0) && (degrees[3*ind+2] == 0)))
    return -2;
  else
    if (degrees[3*ind] == 0)
      return ind+1;
    else
      return ind-1;
}


int Prover::computeMiniSystem(int ind, int ind2){
 
  int pivot1 = 3*ind+1;
  int pivot2 = 3*ind2+1;
  int d = ind2 - ind;

  if ((degrees[pivot1] == 2)&&(degrees[pivot1+d] == 2)&&(degrees[pivot2] == 2)&&(degrees[pivot2-d] == 2)){
    Term* tmp;
    tmp = subctract(f[ind], f[ind2], ind, ind2);
    if (!solveQuadr(f[ind], tmp, ind, ind2))
      return 0;
  }

  else if((degrees[pivot1] == 2)&&(degrees[pivot1+d] == 2)&&(degrees[pivot2] < 2)&&(degrees[pivot2-d] < 2)){
    if (!solveQuadr(f[ind], f[ind2], ind, ind2))
      return 0;
  }

  else if ((degrees[pivot1] < 2)&&(degrees[pivot1+d] < 2)&&(degrees[pivot2] == 2)&&(degrees[pivot2-d] == 2)){
    if (!solveQuadr(f[ind2], f[ind], ind2, ind))
      return 0;
  }

  else if ((degrees[pivot1] < 2)&&(degrees[pivot1+d] < 2)&&(degrees[pivot2] < 2)&&(degrees[pivot2-d] < 2)){
    if (!solveKramer(f[ind], f[ind2], ind, ind2))
      return 0;
  }

  else{
    cout << "Error in compute MiniSystem! " << degrees[pivot1] << " " << degrees[pivot1+d] << " " << degrees[pivot2] << " " << degrees[pivot2-d] << endl;
    return 0;
  }
  
  return 1;
}



void Prover::getCoeffABC(Term *t1, double *a, double *b, double *c, int i, int j){
  while(t1){
    int ind;
    int expo = t1->getExpo();
    double coeff = t1->getCoeff();
    if (t1->indVar(i, m) && (expo == 1)){
      ind = t1->getOtherInd(i, m);
      if (ind == -1)
	*a += coeff;
      else if (ind == -2){
	perror("getCoeffABC index = -2 ");
	exit(2);
      }
      else if (ind < limit)
	*a += coeff * values[ind];
      else{
	perror(" getCoeffABC index to compute ");
	exit(2);
      }
    }
    else if (t1->indVar(j, m) && (expo == 1)){
      ind = t1->getOtherInd(j, m);
      if (ind == -1)
	*b += coeff;
      else if (ind == -2){
	perror(" getCoeffABC index = -2 ");
	exit(2);
      }
      else if (ind < limit)
	*b += coeff * values[ind];
      else{
	perror(" getCoeffABC index to compute ");
	exit(2);
      }
    }
    else if (expo == 0)
      *c += coeff;
    
    else if ((expo > 0)&&((t1->getInd1() < limit)&&(t1->getInd2() < limit)))
      *c += computeTerm(t1);

    t1 = t1->getNextTerm();
  }
}



int Prover::solveKramer(Term* t1, Term* t2, int i, int j){
  double a1, b1, c1, a2, b2, c2, delta;
  a1=b1=c1=a2=b2=c2=delta=0;
  getCoeffABC(t1, &a1, &b1, &c1, i, j);
  getCoeffABC(t2, &a2, &b2, &c2, i, j);
  if (c1 != 0)
    c1 = -c1;
  if (c2 != 0)
    c2 = -c2;
  delta = a1*b2 - a2*b1;
  if (delta !=0){
    values[i+m] = (c1*b2 - b1*c2) / delta;
    values[j+m] = (a1*c2 - a2*c1) / delta;
    return 1;
  }
  /* delta = 0 */
  else{
    cout << "SolveKramer: Error delta = 0, lost one equation \n";
    return 0;
  }
}


void Prover::getQuadrCoeff(Term* t, double *a, double *b, double *c, int i){
  while(t){
    int expo = t->getExpo();
    double coeff = t->getCoeff();
    if (t->indVar(i, m) && (expo == 2)){
	*a += coeff;
    }
    else if (t->indVar(i, m) && (expo == 1)){
      int ind = t->getOtherInd(i, m);
      if (ind == -1)
	*b += coeff;
      else if (ind == -2){
	perror(" solveQuadraticEquation index = -2 ");
	exit(2);
      }
      else if (ind < limit)
	*b += coeff * values[ind];
      else{
	perror(" solveQuadraticEquation index to be computed ");
	exit(2);
      }
    }
    else if (expo == 0){
      *c += coeff;
    }
    else if ((expo > 0)&&((t->getInd1() < limit)&&(t->getInd2() < limit))){
      *c += computeTerm(t);
    }
    t = t->getNextTerm();
  }
}


int Prover::solveQuadr(Term* t1, Term* t2, int i, int j){
  double a1, b1, c1, a2, b2, c2;
  Term *t;
  a1=b1=c1=a2=b2=c2=0;
  getCoeffABC(t1, &a1, &b1, &c1, i, j);
  getCoeffABC(t2, &a2, &b2, &c2, i, j);
  t = new Term((power(b2, 2) + power(a2, 2)), 2, j+m, -1);
  Term *last = t;
  last->setNextTerm(new Term((2*b2*c2 - a1*a2*b2 + b1*power(a2, 2)), 1, j+m, -1));
  last = last->getNextTerm();
     
  last->setNextTerm(new Term((power(c2, 2) - a1*a2*c2 + c1*power(a2, 2)), 0, -1, -1));
  if (!solveQuadrEq(t, j))
    return 0;
  if (!solveLinearEq(t2, i))
    return 0;
  else
    return 1;
}


Term* Prover::subctract(Term* t1, Term* t2, int i, int j){
  double a1, b1, c1, a2, b2, c2;
  Term* t;
  a1=b1=c1=a2=b2=c2=0;
  getCoeffABC(t1, &a1, &b1, &c1, i, j);
  getCoeffABC(t2, &a2, &b2, &c2, i, j);
  t = new Term((a1-a2), 1, i+m, -1);
  Term *last = t;
  last->setNextTerm(new Term((b1-b2), 1, j+m, -1));
  last = last->getNextTerm();
  last->setNextTerm(new Term((c1-c2), 0, -1, -1));
  return t;
}


int Prover::solveQuadrEq(Term* t, int i){
  double a, b, c;
  a= 0;
  b=0;
  c=0;
  
  getQuadrCoeff(t, &a, &b, &c, i);
  if ( a != 0 ){
    if ((b != 0) && (c != 0)){
      double delta = power(b,2) - 4*a*c;
      if (delta > 0)
	values[i+m] = (-b + randSign() * sqrt(delta))/(2*a);
      else if (delta == 0)
	values[i+m] = -b / (2*a);
      else{
	cout << "Quadratic Equation: delta < 0 \n";
	return 0;
      }
    }
    else if ((b == 0) && (c != 0)){
      double tmp = -c/a;
      if (tmp > 0)
	values[i+m] = randSign() * sqrt(tmp);
      else{
	cout << "Quadratic Equation: -c/a < 0 \n";
	return 0;
      }
    }
    else if ((b != 0)&&(c == 0)){
      if (randSign() == 1)
	values[i+m] = 0;
      else
	values[i+m] = -b/a;
    }
    /* if a != 0, b=c=0 */
    else
      values[i+m] = 0;
  }

  /* a = 0 */
  else if ((b == 0) && (c == 0)){
    cout << "Quadratic Equation: all coefficients = 0 \n";
    return 0;
  }
  else if ((b == 0) && (c != 0)){
    perror(" quadratic equation: ABSURD! c = 0 ");
    exit(3);
  }  
  else if ((b!=0) && (c == 0))
    values[i+m] =0;

  else if ((b!=0) && (c!=0))
    values[i+m] = -c/b;
  
  return 1;
  
}
      
int Prover::solveLinearEq(Term* t, int i){
  double a, b;
  int j;
  a = 0;
  b = 0;
  while (t){
    if (t->indVar(i, m)){
      if ((j= t->getOtherInd(i, m)) == -2 ){
	perror(" solveLinearEquation index = -2");
	exit(2);
      }
      else if (j != -1 ){
	a += values[j] * t->getCoeff();
      }
      else{
	a = a + (t->getCoeff());
      }
    }
    else{
      double d = computeTerm(t);
      b += d;
    }
    t = t->getNextTerm();
  }
  if (a == 0){
    cout << "Linear equation: coefficient a = 0 \n";
    return 0;
  }
  else 
    values[i+m] = - b / a;
  return 1;
}


double Prover::computeTerm(Term *t){
  int expo = t->getExpo();
  if (expo == 0){
    return t->getCoeff();
  }
  else{
    int ind1 = t->getInd1();
    int ind2 = t->getInd2();

    if (expo == 2)
      return (t->getCoeff() * power(values[ind1], 2));
    else if (ind2 != -1) 
      return (t->getCoeff() * values[ind1] * values[ind2]);
    else{
      return (t->getCoeff() * values[ind1]);
    }
  }
}


int Prover::vanishThesis(){

  double tot = 0;
  Term *th = g;
  while(th){
    int expo = th->getExpo();
    double coeff = th->getCoeff();
    int ind1 = th->getInd1();
    int ind2 = th->getInd2();
    
    if (expo == 0)
      tot = tot + coeff;
    else{
      if ((expo == 2) && (ind1 < limit))
	tot = tot + coeff * power(values[ind1], 2);
      else if ((expo == 1) && (ind1 < limit) && (ind2 != -1) && (ind2 < limit))
	tot = tot + coeff * values[ind1] * values[ind2];
      else if ((expo == 1) && (ind1 < limit) && (ind2 == -1))
	tot = tot + coeff * values[ind1];
      else 
	cout << "VanishThesis Error: not all variables are determined \n";
    }
    th = th->getNextTerm();
  }
  if (tot == 0)
    return 1;
  else{
    cout << "Value of the polynomial thesis: " << tot << endl;
    return 0;
  }
}


int main(int argc, char *argv[]){

  int numb;
  if (!argv[2])
    numb = 0;
  else
    numb = atoi(argv[2]);
  if (numb < 1){
    cout << "Number of tests must be > 0 \n";
    exit(1);
  }

  Prover *th = new Prover(argv[1], numb);
    
  if (th->VerifyByExamples()){
    double prob = 1 - power(float(2), -numb);
    cout << "\n";
    cout << " The geometric conjecture is TRUE with probability "<< prob <<" \n";  
  }
  else
    cout << " The geometric conjecture is FALSE ";  
   cout << "\n";
   
  return 0; 
}














