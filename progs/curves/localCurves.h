/* **************************************
   File: localCurves.h

   Description: 
   	This is the experimental version of Curves.h
		before it was incorporated into Core Library.
   	Two templated classes are defined here:
		Curve and BiPoly
	These classes are parametrized by the number type
		(called NT) which represents the
		domain of the coefficients of the underlying
		polynomials.  Standard default is NT=BigInt, but
		we will allow NT=int, NT=BigRat, NT=BigFloat, NT=Expr.
	BiPoly represents the class of bivariate polynomials,
		i.e.,  each BiPoly object is an element of NT[X,Y].
		We store each BiPoly as a list of polynomials in X.
	Curve represents the class of plane curves whose equation
		is A(X,Y)=0, for some BiPoly A(X,Y).
	Features:
		--Constructor from strings such as
			"3 x^2 + 7 xy^2 - 4 x + 13".

	To Do:
	  --Dump should produce human readable strings like
	  	"3 x^2 + 7 xy^2 - 4 x + 13".
	  --String constructor generalizations:
	  	(1) allow one "=" sign (e.g., "3 x^2 = y^2 - xy")(DONE)
		(2) allow general parenthesis
		(3) allow X and Y (DONE)
	  --We should be able to read/write
	  	curve definitions from/to files
	  --Plot currently does not look at the y-limits.(DONE)
	  --Plot should be more efficient (use previous roots
	  	to help find the next roots, there should be
	  	a "plot structure" that is persistent)
	  --Plot should have some option to show the
	  	x- and y-axes, and to label some points.
	  --BiPoly should have a equality operator (DONE)
	  --Need the plot of a polynomial function

   Author:  Vikram Sharma and Chee Yap
   Date:    April 12, 2004

   Since Core Library  Version 1.7
   $Id: localCurves.h,v 1.3 2010/11/08 13:52:21 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 4
#endif

#include <fstream>
#include <list>
#include "CORE/CORE.h"

using namespace std;

// ==================================================
// Curve Class
// ==================================================

typedef BigInt NT;
// typedef Expr NT;
typedef Polynomial<NT> PolyNT;
typedef std::vector<Expr>	VecExpr;
typedef std::vector<BigInt>	VecBigInt;
// typedef std::vector<NT>         VecNT;
// typedef std::vector<Polynomial<NT> >	VecPoly;

template <class NT>
class Monomial{
  //Helper class to store the coefficients for given x-deg and y-deg 
  //Used by string input routines
 public:
  NT coeff;
  int xdeg;
  int ydeg;

  Monomial(){
  }

  Monomial(NT& cf, int& dx, int& dy){
    coeff = cf;
    xdeg = dx;
    ydeg = dy;
  }

  void dump(){
    cout << coeff << "X^" << xdeg << " Y^" << ydeg;
  }
};


//Class of Bivariate polynomials
//	Viewed as a polynomial in Y with
//	coefficients which are polynomials in X
template <class NT>
class BiPoly{
public:
  int ydeg; //Y-degree of the polynomial
  //  Polynomial<NT> *coeffX;
  std::vector<Polynomial<NT> > coeffX; //array of (1+ydeg) polynomials in X
	// So the equation of the polynomial is  F(X,Y)
        //  = (Y^d * coeffX[d]) + (Y^{d-1} * coeffX[d-1]) +...+ (coeffX[0]).

  ////////////////////////////////////////////////////////
  //Constructors
  ////////////////////////////////////////////////////////

  //BiPoly()
  BiPoly(){ // zero polynomial
    ydeg = -1;
    coeffX.clear();
  }
  
  //BiPoly(n)
  BiPoly(int n){  // creates a BiPoly with nominal y-degree equal to n.
     //
     // To support this constructor, you need functions
     // that are equivalent to "setCoeff(i, PolyNT q)" in the Poly Class
     // You also want "getCoeff(i)" functions.
     // In tCurve, we illustrate some of this usage
     //
     // E.g. BiPoly<NT> p(10);
     //      PolyNT q(3, {1,2,3,4});
     //      p.setCoeff(10, q);
     //      p.setCoeff(0, PolyNT());
     //
    ydeg = n;
    if (n<0) return; // coeffX not needed
    for(int i=0; i<= ydeg; i++){
      Polynomial<NT>* temp = new Polynomial<NT>();
      coeffX.push_back(temp);      
    }
  }

  //BiPoly(vp)
  BiPoly(std::vector<Polynomial<NT> > vp){ // From vector of Polynomials
    ydeg = vp.size() - 1;
    if(ydeg >=0){
      coeffX = vp;
    }
  }
  
  //BiPoly(p, flag):
  //	if true, it converts polynomial p(X) into P(Y)
  // 	if false, it creates the polynomial Y-p(X)

  BiPoly(Polynomial<NT> p, bool flag=false){
    if (flag){
      ydeg = p.getTrueDegree();
      if(ydeg >=0){
        for(int i=0; i<=ydeg; i++){
	  coeffX.push_back(Polynomial<NT>(0, p.getCoeffi(i) ));
        }//for
      }//if
    } else {
      ydeg = 1;
      coeffX.push_back(p);
      coeffX.push_back(Polynomial<NT>::Unity());
    }//else
  }//BiPoly(p)


  //BiPoly(deg, d[], C[]):
  //	Takes in a list of list of coefficients.
  //	Each cofficient list represents a polynomial in X
  //
  //  deg - ydeg of the bipoly
  //  d[] - array containing the degrees of each coefficient (i.e., X poly)
  //  C[] - list of coefficients, we use array d to select the
  //      coefficients
  BiPoly(int deg, int *d, NT *C){

    ydeg = deg;
    Polynomial<NT> temp;
    int max=0;
    for(int i=0; i <=deg; i++)
      max = core_max(d[i],max);

    NT *c = new NT[max];

    for(int i=0; i<= deg; i++){
      for(int j=0; j <=d[i]; j++)
	c[j] = C[i+j];
      temp = Polynomial<NT>(d[i],c);
      coeffX.push_back(temp);      
    }
  }//BiPoly(deg,d[],C[])


  //BiPoly(String s)
  //Takes in a string of the form "3 x^2 + 7 xy^2 - 4 x + 13"
  //and produces a BiPoly from it.
  //As for now assumes no parentheses and *
  //   ToDo:
  //        GENERALIZE to allow BiPoly(String s, char myX, char myY)
  BiPoly(string s){
	  BiPoly(s, 'x', 'y');
  }

  BiPoly(string s, char myX, char myY){

    unsigned int length = s.length();
    if(length <= 0)//Zero Polynomial
      BiPoly();
    

    const char *cstr = s.c_str();

    //First we find the ydeg
    ydeg = 0;
    unsigned int loc = s.find(myY,0);
    do{
      if(loc < length && cstr[loc+1] == '^')
	ydeg = core_max(ydeg, find_deg(cstr, loc+2, myX, length));
      else
	ydeg = core_max(ydeg,1);

      loc = s.find(myY,loc+1);
    }while(loc != string::npos && loc+1 < length);

    //Now we find the xdeg corresponding to each y^i
    //xdeg[i] represents the degree of the coeff, in R[x], of y^i
    int *xdeg = new int[ydeg+1];
    for(int i=0; i<=ydeg; i++)
      xdeg[i] = -1;


    list<Monomial<NT> > lst;//This is the list containing the corresponding
                       //entries for the monomials appearing in string s

    unsigned int start, end;
    NT cf;
    int degx, degy;
    string monomial;
    start = 0;end=0;
    while(end !=length){//This loop calculates xdeg[i]
      end = core_min(s.find('+',start+1), s.find('-',start+1));
      if(end == string::npos) end = length;
      monomial = s.substr(start, end-start);
      parse_mon(monomial, cf, degx, degy, myX, myY);//Returns the coeff & degree
                                             //of myX and myY in monomial
      
      //Enter the coeffs' x-deg, y-deg into the list of monomials
      lst.push_back(Monomial<NT>(cf, degx, degy));

      //Set the xdeg[i]
      xdeg[degy] = core_max(xdeg[degy], degx);
      //cout << "xdeg[ " << degy << "] ="<< degx << endl; 
      start = end;
    }


    NT **coeff = new NT*[ydeg+1];
    for(int i=0; i <= ydeg; i++)
      coeff[i] = new NT[xdeg[i]+1];

    //Have to initialize the matrix to zero
    for(int i=0; i <=ydeg; i++)
      for(int j=0; j<=xdeg[i]; j++)
	coeff[i][j]=0;


    //This loop initialises the coeffecients
    Monomial<NT> m;
    while(lst.size() > 0){
      m = lst.front();
      coeff[m.ydeg][m.xdeg] += m.coeff;
      lst.pop_front();
    }

    for(int i=0; i <= ydeg; i++)
      coeffX.push_back(Polynomial<NT>(xdeg[i], coeff[i]));
  }//BiPoly(string)


  //Helper functions for the above constructor

  //Let S = {x,y}
  //finds the degree of S\{c} in s starting at the location "loc" till the
  //starting of the next monomial
  int find_deg(const char* s, int loc, char c, int len){
    
    //cout << "loc " << loc << endl;
    char *temp = new char[len];
    for(int i=loc; s[i]!=c && s[i]!='+' 
	  && s[i]!='-' && i <= len; i++)
      temp[i-loc] = s[i];

    //cout << "Deg = " << atoi(temp) << endl;
    return atoi(temp);
  }

  //Given a monomial returns the coeff, degree in x and degree in y
  void parse_mon(string m, NT& coeff, int &dx, int &dy, char myX, char myY){
    const char *mstr = m.c_str();
    unsigned int len = m.length();
    char *temp = new char[len+1];//Should initialize, can cause problem

    bool mark = false;
    dx =0; dy =0;
    unsigned int i,j;
    for(i=0, j=0; i <len; i++){
      if(mstr[i]!=myX && mstr[i]!= myY && mstr[i]!=' ' && !mark){
	temp[j]=mstr[i];j++;temp[j] = '\n';
      }
      if(mstr[i] == myX){
	mark = true;
	if(mstr[i+1] == '^'){
	  dx = find_deg(mstr, i+2, myY, len);
        }else{
	  dx = 1;
	}
      }
      if(mstr[i] == myY){
	mark = true;
	if(mstr[i+1] == '^'){
	  dy = find_deg(mstr, i+2, myX, len);
        }else{
	  dy = 1;
	}
      }
    }

    if(j > 1)
      coeff = atof(temp);
    else if(j == 0 && (dx > 0 || dy > 0)) //For the case "x"
      coeff = 1;
    else{
      if(temp[0] == '-')
	coeff = -1;
      else if(temp[0] == '+')
	coeff = 1;
      else coeff = atof(temp);//For the case "3 xy"
    }
  }


  //Trial version for parentheses
  //As for now assumes no *.
  //This assumption rules out products of constants, as in 2*3 etc.
  // b=false means that s does not contain a parentheses.
  //b=true is for the case when there is either one '=' sign
  //or parentheses or both.
  BiPoly(string s, const bool b, char myX, char myY){

    unsigned int length = s.length();
    if(length <= 0)//Zero Polynomial
      BiPoly();

    if((s.find('(', 0) == string::npos) || b == false)
      BiPoly(s, myX, myY);

    //To handle the case when there is one '=' sign
    //Suppose s is of the form s1 = s2. Then we assign s to
    //s1 + (-1)(s2) and reset length
    unsigned int loc;
    if((loc=s.find('=',0)) != string::npos){
      s.replace(loc,1,1,'+');
      string s3 = "(-1)(";
      s.insert(loc+1, s3);
      length = s.length();
      s.insert(length, 1, ')');
      cout << " String after replacing '=' " << s << endl;
    }

    length = s.length();
    const char *cstr = s.c_str();

    BiPoly<NT> *temp = new BiPoly<NT>();//Accumulate the polynomial in this
    unsigned int start, end;
    NT cf;
    string monomial;
    start = 0;end=0;

//cout << "Length of the input string " << length << endl;

    while(end !=length && (start+1) != length ){//Main Loop which constructs 
                                                //the polynomial
      end = core_min(s.find('+',start+1), 
		     s.find('-',start+1), s.find('(', start+1));

      if(end == string::npos){
	monomial = s.substr(start, end-start);
      }else if(cstr[end] != '(' && cstr[start] != '(')//to avoid the case 
	monomial = s.substr(start, end-start);//when s starts with '('
      else{
	unsigned int e = get_mon(s, start);//e represents the end of a 
	monomial = s.substr(start, e - start);//parentheses monomial
	end = e;
      }

//cout << " Monomial is " << monomial << "Starts at " << start << " ends at " << end << endl;

       (*temp) += remove_paren(monomial, myX, myY);

       start = end;
    }

    (*this) = (*temp);

  }//BiPoly(string)


  //get_mon(s, start)
  //	returns a monomial in the string s beginning a position start. 
  //    Monomials can have parentheses. We asssume that all the 
  //non-parentheses terms lead the parentheses ones and do not contain
  //any +/-, i.e, of the form x^m or y^n. Also there are no '*' separating
  //such terms. As of now we only assume one level of nesting of parentheses. 
  unsigned int get_mon(string s, unsigned int start){
//    unsigned int len = s.length();
    const char* cstr = s.c_str();
    int count = 0;
    unsigned int j=s.find('(', start);//start char can be '(' or '+/-'

//    cout << "start of paren mon = " << start << " " << cstr[start] << endl;
    do{
      if(cstr[j] == '('){
	count++;
      }
      if(cstr[j] == ')'){
	count--;
      }
      j++;      
    }while(count != 0 );//j is one more than the matching ')'

//    cout << "end of paren mon = " << j << endl;
    unsigned int st = core_min(s.find('(', j),s.find('+',j), s.find('-',j));
    if(st != string::npos){
      if(cstr[st] == '(')
	j = core_max(j, get_mon(s, st)); 
    }
//    cout << " Return value of end of mon " << j << endl;
    return j;
  }


  //Takes as input a monomial which may or may not contain '(...)'.
  //There are only two possible types depending upon whether the 
  //monomial has a sign associated with it or not.
  //We assume only one level of nesting of parentheses.
  //Also, the input monomial is not just a constant.
  //Any non-parentheses term can occur in between such a monomial
  //but has the restriction mentioned in get_mon above.
  //Returns a bi-poly equivalent to the corresponding monomial.
  BiPoly<NT>& remove_paren(string mon, char myX, char myY){

    //  start and end denote the starting and end of a sub-string
    // of mon that represents a "valid" polynomial. This particular
    //sub-string may or may not contain parentheses.
    
    const char *m_str = mon.c_str();
    unsigned int loc=0;

    //Get rid of the white spaces at the beginning of the monomial
    for(loc=0; loc < mon.length() && m_str[loc] == ' '; loc++){};

    unsigned int start = mon.find('(', loc);
    unsigned int end;
    BiPoly<NT> *one = new BiPoly<NT>("1", myX, myY);

    if(start == string::npos && mon.find(myX ,0) == string::npos &&
       mon.find(myY, 0) == string ::npos)
      return (*one);//One because we are in a monomial. This is returned
                    //when we have reached the end of the monomial

    BiPoly<NT> *temp;
    if(start == string::npos){//Case of the normal monomial with no parens
      temp = new BiPoly<NT>(mon, myX, myY);
    }
    else if(start == loc){//monomial starts with a '('
      end = mon.find(')', 0);
      temp = new BiPoly<NT>(mon.substr(start+1, end-start-1), myX, myY);
      (*temp) *= remove_paren(mon.substr(end+1, mon.length()-end -1), myX, myY);
    }
    else{//monomial starts with powers of x or y, start > loc
      temp = new BiPoly<NT>(mon.substr(loc, start-loc), myX, myY);
      (*temp) *= remove_paren(mon.substr(start, mon.length()-start), myX, myY);
    }

    return (*temp);
  }


  //Destructor
  ~BiPoly(){
  if (ydeg >= 0)
    coeffX.clear();  
  }


  ////////////////////////////////////////////////////////
  // METHODS
  ////////////////////////////////////////////////////////
  
  // filedump (msg, ofs, com)
  // 	where msg and com are strings.
  // 	msg is an message and com is the character preceding each line
  // 	(e.g., msg=""  and com="# ")
  // This is called by the other dump functions
  void filedump(std::ostream & os, std::string msg = "",
		  std::string com = "# ") const {
    if (msg != "")
      os << msg << endl;
    if(ydeg == -1) {
      os << com << " Zero Polynomial" << endl;
      return;
    }
    bool first = true;
    os << com;
    for (int i=0; i <= ydeg; i++){
      if (!zeroP(coeffX[i])){
	if (i % 3 == 0) os << endl << com ;  // output 3 coefficients per line
	if (first) first = false;
	else os << " + ";
	os << "[";
	coeffX[i].filedump(os,"","");
	os << "]";
        if (i > 0) os <<" * y^"<< i ;
      }
    }
  }//dump

  // dump(ofs, msg, com) -- dump to file
  void dump(std::ofstream & ofs, std::string msg,
              std::string com="# ") const {
    std::ostream * osp = & ofs;  // convert ofstream to ostream
    filedump((*osp), msg, com);
  }

  // dump(msg, com) -- dump to std output
  void dump(std::string msg="", std::string com="") const {
    filedump(std::cout, msg, com);
  }


  /*Cannot work with these two functions right now.
    BiPoly as per now can only handle BigInt and int since
    Expr cannot be handled by Polynomial class.*/
  
  // yPolynomial(x) 
  //   returns the polynomial (in Y) when we substitute X=x
  
  /* BiPoly<NT> yPolynomial(const Expr & x) {

    VecExpr vE;

    for (int i=0; i<= ydeg; i++) {
      vE.push_back(coeffX[i].eval(x));
    }
    
    return BiPoly<NT>(vE);
  }//yPolynomial
  */

  Polynomial<NT> yPolynomial(const NT & x) {
    NT coeffVec[ydeg+1];
    int d=-1;
    for(int i=ydeg; i >= 0 ; i--){
      coeffVec[i] = coeffX[i].eval(x);
      if ((d < 0) && (coeffVec[i] != 0))
	      d = i;
    }
    return Polynomial<NT>(d, coeffVec);
  }

  // specialized version of yPoly (temporary hack)
  Polynomial<Expr> yExprPolynomial(const Expr & x) {
    Expr coeffVec[ydeg+1];
    int d=-1;
    for(int i=ydeg; i >= 0 ; i--){
      coeffVec[i] = coeffX[i].eval(x);
      if ((d < 0) && (coeffVec[i] != 0))
	      d = i;
    }
    return Polynomial<Expr>(d, coeffVec);
  }

  // xPolynomial(y) 
  //   returns the polynomial (in X) when we substitute Y=y
  //   
  //   N.B. May need the
  //   		Polynomial<Expr> xExprPolynomial(Expr y)
  //   version too...
  //
  Polynomial<NT> xPolynomial(const NT & y) {
    Polynomial<NT> res = coeffX[0];
    NT yPower(y);
    for(int i=1; i <= ydeg ; i++){
      res += coeffX[i].mulScalar(yPower);
      yPower *= y;
    }
    return res;
  }//xPolynomial
  

  // getYdegree()
  int getYdegree() const {
    return ydeg;
  }
  
  // getXdegree()
  int getXdegree(){
    int deg=-1;
    for(int i=0; i <=ydeg; i++)
      deg = max(deg, coeffX[i].getTrueDegree());
    return deg;
  }

  // getTrueYdegree
  int getTrueYdegree(){
    for (int i=ydeg; i>=0; i--){
      coeffX[i].contract();
      if (!zeroP(coeffX[i]))
	return i;
    }
    return -1;	// Zero polynomial
  }//getTrueYdegree


  //eval(x,y)
  Expr eval(Expr x, Expr y){//Evaluate the polynomial at (x,y)
    Expr e = 0;

    for(int i=0; i <=ydeg; i++){
      e += coeffX[i].eval(x)*pow(y,i);
    }
    return e;
  }//eval

  ////////////////////////////////////////////////////////
  // Polynomial arithmetic (these are all self-modifying)
  ////////////////////////////////////////////////////////
  
  // Expands the nominal y-degree to n;
  //	Returns n if nominal y-degree is changed to n
  //	Else returns -2

  int expand(int n) {
    if ((n <= ydeg)||(n < 0))
      return -2;
    
    for(int i=ydeg+1; i <=n ;i++)
      coeffX.push_back(Polynomial<NT>::Zero());
    
    ydeg = n;
    return n;
  }//expand

  // contract() gets rid of leading zero polynomials
  //	and returns the new (true) y-degree;
  //	It returns -2 if this is a no-op

  int contract() {
    int d = getTrueYdegree();
    if (d == ydeg)
      return (-2);  // nothing to do
    else{
      for (int i = ydeg; i> d; i--)
	coeffX.pop_back();
      ydeg = d;
    }
    return d;
  }//contract

  // Self-assignment
  BiPoly<NT> & operator=( const BiPoly<NT>& P) {
  if (this == &P)
    return *this;	// self-assignment
  ydeg = P.getYdegree();
  coeffX = P.coeffX;
  return *this;
  }//operator=


  // Self-addition
  BiPoly<NT> & BiPoly<NT>::operator+=( BiPoly<NT>& P) { // +=
    int d = P.getYdegree();
    if (d > ydeg)
      expand(d);
  for (int i = 0; i<=d; i++)
    coeffX[i] += P.coeffX[i];

  return *this;
  }//operator+=
   
  // Self-subtraction
  BiPoly<NT> & BiPoly<NT>::operator-=( BiPoly<NT>& P) { // -=
    int d = P.getYdegree();
    if (d > ydeg)
      expand(d);
  for (int i = 0; i<=d; i++)
    coeffX[i] -= P.coeffX[i];

  return *this;
  }//operator-=

  // Self-multiplication
  BiPoly<NT> & BiPoly<NT>::operator*=( BiPoly<NT>& P) { // *=
    int d = ydeg + P.getYdegree();

    std::vector<Polynomial<NT> > vP;

    Polynomial<NT>* c = new Polynomial<NT> [d + 1];
    for(int i=0; i <=d; i++)
      c[i] = Polynomial<NT>();
    
    for (int i = 0; i<=P.getYdegree(); i++)
      for (int j = 0; j<=ydeg; j++) {
	if(!zeroP(P.coeffX[i]) && !zeroP(coeffX[j]))
	  c[i+j] += P.coeffX[i] * coeffX[j];
      }

    for(int i=0; i <= d; i++)
      vP.push_back(c[i]);

    coeffX = vP;
    ydeg = d;
    return *this;
  }//operator*=
  
  // Multiply by a polynomial in X
  BiPoly<NT> & mulXpoly( Polynomial<NT> & p) {
    contract();
    if (ydeg == -1)
      return *this;

    for (int i = 0; i<=ydeg ; i++)
      coeffX[i] *= p;
    return *this;
  }//mulXpoly

  //Multiply by a constant
  BiPoly<NT> & mulScalar( NT & c) {
    for (int i = 0; i<=ydeg ; i++)
      coeffX[i].mulScalar(c);
    return *this;
  }//mulScalar

  // mulYpower: Multiply by Y^i (COULD be a divide if i<0)
  BiPoly<NT> & mulYpower(int s) {
  // if s >= 0, then this is equivalent to
  // multiplying by Y^s;  if s < 0, to dividing by Y^s
  if (s==0)
    return *this;
  int d = s+getTrueYdegree();
  if (d < 0) {
    ydeg = -1;
    coeffX.clear();
    return *this;
  }

  std::vector<Polynomial<NT> > vP;
  if(s > 0){
    for(int i=0; i < s; i ++)
      vP.push_back(Polynomial<NT>::Zero());
    for(int i=s; i<=d; i++)
      vP.push_back(coeffX[i-s]);
  }

  if (s < 0) {
    for (int j=-1*s; j <= d-s; j++)
      vP.push_back(coeffX[j]);
  }

  coeffX = vP;
  ydeg= d;
  return *this;
  }//mulYpower


  
  // Divide by a polynomial in X.
  // We replace the coeffX[i] by the pseudoQuotient(coeffX[i], P)
  BiPoly<NT> & divXpoly( Polynomial<NT> & p) {
    contract();
    if (ydeg == -1)
      return *this;

    for (int i = 0; i<=ydeg ; i++)
      coeffX[i] = coeffX[i].pseudoRemainder(p);

    return *this;
  }// divXpoly


  
  //Using the standard definition of pseudRemainder operation.
  //	--No optimization!
  BiPoly<NT>&  pseudoRemainderY (BiPoly<NT> & Q){
    contract();
    Q.contract();
    int qdeg = Q.getYdegree();
    Polynomial<NT> LC(Q.coeffX[qdeg]), temp1(LC), temp;
    BiPoly<NT> P(Q);

    std::vector<Polynomial<NT> > S;//The quotient in reverse order
    if(ydeg < qdeg){
      return (*this);
    }
    (*this).mulXpoly(temp1.power(ydeg - qdeg +1));

    while(ydeg >= qdeg){
      temp1 = coeffX[ydeg];
      temp = temp1.pseudoRemainder(LC);
      S.push_back(temp);
      P.mulXpoly(temp);
      P.mulYpower(ydeg - qdeg);
      *this -= P;
      contract();
      P = Q;//P is used as a temporary since mulXpower and mulYpower are
            // self modifying
    }

    //Correct the order of S
    std::vector<Polynomial<NT> > vP;

    for(int i= S.size()-1; i>=0; i--)
      vP.push_back(S[i]);


    BiPoly<NT>* T = new BiPoly<NT>(vP);//Return the pseudoQuotient
    return *T;
    
  }//pseudoRemainder

  //Partial Differentiation
  //Partial Differentiation wrt Y
  BiPoly<NT> & differentiateY() {	
  if (ydeg >= 0) {
    for (int i=1; i<=ydeg; i++)
      coeffX[i-1] = coeffX[i].mulScalar(i);
    ydeg--;
  }
  return *this;
  }// differentiation wrt Y

  BiPoly<NT> & differentiateX() {	
    if (ydeg >= 0)
      for (int i=0; i<=ydeg; i++)
	coeffX[i].differentiate();
    
    return *this;
  }// differentiation wrt X

  BiPoly<NT> & differentiateXY(int m, int n) {//m times wrt X and n times wrt Y
    assert(i >=0); assert(j >=0);
    for(int i=1; i <=m; i++)
      (*this).differentiateX();
    for(int i=1; i <=n; i++)
      (*this).differentiateY();
    
    return *this;
  }

  //Represents the bivariate polynomial in (R[X])[Y] as a member
  //of (R[Y])[X].
  //But since our polynomials in X can only have NT coeff's thus
  // to represent the above polynomial we switch X and Y once
  // the conversion has been done.
  //NOTE: This is different from replacing X by Y which was
  //      done in the case of the constructor from a polynomial in X
  //Needed to calculate resultant wrt X.
  BiPoly<NT> & convertXpoly(){
    getTrueYdegree();
    if(ydeg == -1) return (*this);
    NT *cs = new NT[ydeg +1];
    int xdeg = getXdegree();
    std::vector<Polynomial<NT> > vP;

    for(int i=0; i<=xdeg; i++){
      for(int j=0; j<=ydeg; j++){
	cs[j] = coeffX[j].getCoeffi(i);
      }
      
      vP.push_back(Polynomial<NT>(ydeg, cs));
    }
      
    ydeg = xdeg;
    coeffX = vP;
    return (*this);
  }

  //Set Coeffecient to the polynomial passed as a parameter
  bool setCoeff(int i, Polynomial<NT> p){
    if(i < 0 || i > ydeg)
      return false;
    coeffX[i] = p;
    return true;
  }
  
  void reverse() {
    Polynomial<NT> tmp;
    for (int i=0; i<= ydeg/2; i++) {
      tmp = coeffX[i];
      coeffX[i] =   coeffX[ydeg-i];
      coeffX[ydeg-i] = tmp;
    }
  }//reverse

  //replaceYwithX()
  //   used used when the coeffX in BiPoly are constants,
  //   to get the corresponding univariate poly in X
  //   E.g., Y^2+2*Y+9 will be converted to X^2+2*X+9
  Polynomial<NT>& replaceYwithX(){
    int m = getTrueYdegree();
    NT *cs = new NT[m+1];
    for(int i=0; i <= m ; i++)
      cs[i]=coeffX[i].getCoeffi(0);

    Polynomial<NT> * p = new Polynomial<NT> (m,cs);
    return *p;
  }//replaceYwithX
  

};//BiPoly Class

  ////////////////////////////////////////////////////////
  // Helper Functions
  ////////////////////////////////////////////////////////

// zeroPinY(P)
//	checks whether a Bi-polynomial is a zero Polynomial
template <class NT>
bool zeroPinY(BiPoly<NT> & P){
    if(P.getTrueYdegree() == -1)
      return true;
    return false;
}

// gcd(P,Q)
//   This gcd is based upon the subresultant PRS to avoid
//   exponential coeffecient growth and gcd computations, both of which 
//   are expensive since the coefficients are polynomials

template <class NT>
BiPoly<NT> & gcd( BiPoly<NT>& P ,BiPoly<NT>& Q){
    int m = P.getTrueYdegree();
    int n = Q.getTrueYdegree();

    //If both Bi-Polys are zero then return zero
    if( m==-1 && n==-1){
      BiPoly<NT>* zero = new BiPoly<NT>();//Had to do this for the type of
      return *zero;                       //return value o/w causes problem in
    }                                     //assignment operation

    if(m < n) {
      return gcd(Q, P);
    }

    //If one of the Bi-Polys are zero then return the other
    if(n==-1)
      return P;

    //When the two BiPolys are just univariate Polynomials in X
    if(m==0 && n==0){
      std::vector<Polynomial<NT> > vP;
      vP.push_back(gcd(P.coeffX[0], Q.coeffX[0]));
      BiPoly<NT> * temp = new BiPoly<NT>(vP);//Had to do this for the type of
      return *temp;                          //return value
      }
    
    int delta = m - n;
    Polynomial<NT> a(Q.coeffX[n]);
    std::vector<NT> vN;
    vN.push_back(pow(BigInt(-1),delta + 1));
    Polynomial<NT> beta(vN);
    Polynomial<NT> phi(power(a, delta)), t;
    m = n;
    BiPoly<NT> temp;
    P.pseudoRemainderY(Q);

    while(!zeroPinY(P)){
      P.divXpoly(beta);
      n = P.getTrueYdegree();
      delta = m - n;
      beta = power(phi, delta)*a.mulScalar(pow(BigInt(-1),delta+1));
      a = P.coeffX[n];
      t = power(phi, delta -1);
      phi = power(a,delta).pseudoRemainder(t);
      m = n;
      temp = Q;//Swapping the pseudo-remainder for the next step
      Q = P;
      P = temp;
      P.pseudoRemainderY(Q);
  }
    return Q;
}//gcd

// resY(P,Q):
//      Resultant of Bi-Polys P and Q w.r.t. Y.
//      So the resultant is a polynomial in X
//
// NOTE: probably we should return by reference for efficiency...
template <class NT>
Polynomial<NT>  resY( BiPoly<NT>& P ,BiPoly<NT>& Q){

  int m = P.getTrueYdegree();
  int n = Q.getTrueYdegree();

  //If either polynomial is zero, return zero
  if( m == -1 || n == -1) return Polynomial<NT>();//::Zero();

  if(n > m)
    return resY(Q, P);

  Polynomial<NT> b(Q.coeffX[n]);
  Polynomial<NT> lc(P.coeffX[m]), C, temp;
  BiPoly<NT> r;
  
  r = P.pseudoRemainderY(Q);
  C = b * r.coeffX[r.getTrueYdegree()];
  C = C.pseudoRemainder(lc);

  if(zeroPinY(P) && n > 0)
    return Polynomial<NT>();//::Zero();

  if(Q.getTrueYdegree() == 0 && zeroPinY(P))
    return power(Q.coeffX[0], m);

  int l = P.getTrueYdegree();

  temp = power(b, m-l).mulScalar(pow(BigInt(-1),m*n))*resY(Q,P);
  temp = temp.pseudoRemainder(power(C,n));
  return temp;

}//resY

// resX(P,Q):
//      Resultant of Bi-Polys P and Q w.r.t. X.
//      So the resultant is a polynomial in Y
//	We first convert P, Q to polynomials in X. Then 
// 	call resY and then turns it back into a polynomial in Y
//	QUESTION: is this last switch really necessary???
template <class NT>
BiPoly<NT>  resX( BiPoly<NT>& P ,BiPoly<NT>& Q){
  P.convertXpoly();
  Q.convertXpoly();

  // Polynomial<NT> p(resY(P,Q));
  // return BiPoly<NT>(p);
  return(resY(P,Q)); // avoid the switch back
}//resX


//Equality operator for BiPoly
template <class NT>
bool operator==(const BiPoly<NT>& P, const BiPoly<NT>& Q) {	// ==
  P.contract();
  Q.contract();
  if(P.getYdegree() != Q.getYdegree())
    return false;
  else{
    for(int i=0; i <= P.getYdegree() ; i++){
      if(P.coeffX[i] != Q.coeffX[i])
	return false;
    }
  }
}

  ////////////////////////////////////////////////////////
  //Curve Class
  //  	extends BiPoly Class
  ////////////////////////////////////////////////////////

template < class NT >
class Curve : public BiPoly<NT> {
public:
  // Colors for plotting curves

  const static int NumColors=7;
  static double red_comp(int i){
  	static double RED_COMP[] = {0.9, 0.8, 0.7, 0.6, 0.8, 0.8, 0.7};
	return RED_COMP[i % NumColors];
  }
  static double green_comp(int i){
  	static double GREEN_COMP[] = {0.5, 0.9, 0.3, 0.9, 0.7, 0.55, 0.95};
	return GREEN_COMP[i % NumColors];
  }
  static double blue_comp(int i){
  	static double BLUE_COMP[] = {0.8, 0.3, 0.8, 0.5, 0.4, 0.85, 0.35};
	return BLUE_COMP[i % NumColors];
  }

  Curve(){} // zero polynomial
  
  //Curve(vp):
  //    construct from a vector of polynomials
  Curve(std::vector<Polynomial<NT> > vp)
	  : BiPoly<NT>(vp){
  }
  
  //Curve(p):
  //	Converts a polynomial p(X) to a BiPoly in one of two ways:
  // 	    (1) if flag is false, the result is Y-p(X) 
  // 	    (2) if flag is true, the result is p(Y) 
  //    The default is (1) because we usually want to plot the
  //        graph of the polynomial p(X)
  Curve(Polynomial<NT> p, bool flag=false)
	  : BiPoly<NT>(p, flag){
  }

  //Curve(deg, d[], C[]):
  //	Takes in a list of list of coefficients.
  //	Each cofficient list represents a polynomial in X
  //
  //  deg - ydeg of the bipoly
  //  d[] - array containing the degrees of each coefficient (i.e., X poly)
  //  C[] - list of coefficients, we use array d to select the
  //      coefficients
  Curve(int deg, int *d, NT *C)
	  : BiPoly<NT>(deg, d, C){
  }

  Curve(BiPoly<NT> &P)
	  : BiPoly<NT>(P){
  }

  //Curve(n)
  Curve(int n)
	  : BiPoly<NT>(n){// creates a Curve with nominal y-degree equal to n
  }

  //Creates a curve from a string (no parentheses, no *)
  Curve(string s, char myX ='x', char myY ='y')
	  : BiPoly<NT>(s, myX, myY){
  }

  //Creates a curve from a string (no parentheses, no *)
  //	Note: bool ought to be last argument?
  Curve(string s, bool asd, char myX ='x', char myY ='y')
	  : BiPoly<NT>(s, asd, myX, myY){
  }

  // verticalIntersections(x, vecI, aprec=0):
  //    The list vecI is passed an isolating intervals for y's such that (x,y)
  //    lies on the curve.
  //    If aprec is non-zero (!), the intervals have with < 2^{-aprec}.
  //    Return is -2 if curve equation does not depend on Y
  //    	-1 if infinitely roots at x,
  //    	0 if no roots at x
  //    	1 otherwise

  int verticalIntersections(const BigFloat & x, BFVecInterval & vI,
	int aprec=0) {
    int d= getTrueYdegree();
    if(d <= 0) return(-2);
    	// This returns a NULL vI, which should be caught by caller
    Polynomial<Expr> PY = yExprPolynomial(x);

    d = PY.getTrueDegree();
    if(d <= 0) return(d);

    Sturm<Expr> SS(PY);
    SS.isolateRoots(vI);


    int s = vI.size();
    if ((aprec != 0) && (s>0))
	SS.newtonRefineAllRoots(vI, aprec);
    
    return s;
  }
  
  // plot(eps, x1, y1, x2, y2)
  //
  // 	All parameters have defaults
  //
  //    Gives the points on the curve at resolution "eps".  Currently,
  //    eps is viewed as delta-x step size (but it could change).
  //    The display is done in the rectangale 
  //    defined by [(x1, y1), (x2, y2)].
  //    The output is written into a file in the format specified
  //    by our drawcurve function (see COREPATH/ext/graphics).
  //
  //    Heuristic: the open polygonal lines end when number of roots
  //    changes...
  //
  int  plot( BigFloat eps=0.1, BigFloat x1=-1.0,
	BigFloat y1=-1.0, BigFloat x2=1.0, BigFloat y2=1.0, int fileNo=1){

  // Should be possible to set "filename" from a global variable!
  const char* filename[] = {"data/input", "data/plot", "data/plot2"};

  ofstream outFile;
  outFile.open(filename[fileNo]); // ought to check if open is successful!
  outFile << "########################################\n";
  outFile << "# Curve equation: \n";
  dump(outFile,"", "# ");
  outFile << endl;
  outFile << "# Plot parameters:  step size (eps) = " << eps << endl;
  outFile << "#                   x1 = " << x1 << ",\t y1 = " << y1 << endl;
  outFile << "#                   x2 = " << x2 << ",\t y2 = " << y2 << endl;
  outFile << "########################################\n";
  outFile << "# X-axis " << endl;
  outFile << "o 2" << endl;
  outFile << "0.99 \t 0.99 \t 0.99" << endl;
  outFile << x1 << "\t" << 0 << endl;
  outFile << x2 << "\t" << 0 << endl;
  outFile << "########################################\n";
  outFile << "# Y-axis " << endl;
  outFile << "o 2" << endl;
  outFile << "0.99 \t 0.99 \t 0.99" << endl;
  outFile << 0 << "\t" << y1 << endl;
  outFile << 0 << "\t" << y2 << endl;
  // assert(eps>0)
  int aprec = -(eps.lMSB()).toLong(); // By definition, eps.lMSB() <= lg(eps)

  BigFloat xCurr = x1;
  BigFloat xLast = x1;
  unsigned int numRoots=0;
  unsigned int numPoints=0;
  BFVecInterval vI;

  //===================================================================
  // FIRST locate the first x-value where there are roots for plotting
  //===================================================================
  do {
    vI.clear();
    if (verticalIntersections(xCurr, vI, aprec) > 0) {
      numRoots = vI.size();
    }
    xCurr += eps;

  } while ((numRoots == 0) && (xCurr < x2));//numRoots <= ydeg

  if (numRoots == 0 && x2 <= xCurr) {//if numRoots > 0 then there exists a
                                     //valid point for plotting
	  return -1; // nothing to plot!
  }

// cout << "First place where plot starts " << xCurr -eps
// 	<< " number of intersections " << numRoots << endl;

  int limit = ((x2 - xCurr + eps)/eps).intValue()+1;
  //cout << "Limit = " << limit << endl;
  machine_double plotCurves[getTrueYdegree()][limit];	// PLOT BUFFER 
  machine_double yval;

  for (unsigned int i=0; i< numRoots; i++) {
     yval = (vI[i].first + vI[i].second).doubleValue()/2;
     if (yval < y1) plotCurves[i][numPoints] = y1.doubleValue();
     else if (yval > y2) plotCurves[i][numPoints] = y2.doubleValue();
     else plotCurves[i][numPoints] = yval;
  }

  vI.clear();
  xLast = xCurr - eps;  // -eps to compensate for the forward value of xCurr
  numPoints = 1;

  //===================================================================
  // Get all the curves in a main loop
  // 	-- dump the curves when an x-interval is discovered
  // 	We define an "x-interval" to be a maximal interval
  // 	where the number of curves is constant (this is a heuristic!)
  // 	Note that this includes the special case where number is 0.
  //===================================================================

  BigFloat tmp; // used to step from xLast to xCurr in loops

  while (xCurr < x2) { //main loop
    //cout << "Doing verticalintersec at " << xCurr << endl;
    verticalIntersections(xCurr, vI, aprec);
    if (vI.size() != numRoots) { // an x-interval discovered!
	// write previous x-interval to output file
        outFile << "########################################\n";
        outFile << "# New x-interval with " << numRoots << " roots\n";
	for (unsigned int i=0; i< numRoots; i++) {
          outFile << "#=======================================\n";
          outFile << "# Curve No. " << i+1 << endl;
	  outFile << "o " << numPoints << endl;
	  outFile << red_comp(i) << "\t"
		  << green_comp(i)  << "\t"
		  << blue_comp(i) << endl;
	  tmp = xLast;
	  for (unsigned int j=0; j< numPoints; j++) {

		  outFile << tmp << "	\t\t"
			  << plotCurves[i][j] << "\n";
		  tmp += eps;
	  }//for j
	}//for i
	numPoints = 0;          // reset
	numRoots = vI.size();   // reset
	xLast = xCurr;		// reset
    }//if vI.size() !=
    if (numRoots>0){ // record curr. vertical intersections if numRoots>0
      for (unsigned int i=0; i< numRoots; i++) {
         yval = (vI[i].first + vI[i].second).doubleValue()/2;
         if (yval < y1) plotCurves[i][numPoints] = y1.doubleValue();
         else if (yval > y2) plotCurves[i][numPoints] = y2.doubleValue();
         else plotCurves[i][numPoints] = yval;
      }//for i
      vI.clear();  //Should clear the intersection points
      numPoints++;
    }//if
    xCurr += eps;
   }//main while loop

   // Need to flush out the final x-interval:
   if ((numRoots>0) && (numPoints >0)) { 
	// write to output file
        outFile << "########################################\n";
        outFile << "# New x-interval with " << numRoots << " roots\n";
	for (unsigned int i=0; i< numRoots; i++) {
          outFile << "#=======================================\n";
          outFile << "# Curve No. " << i+1 << endl;
	  outFile << "o " << numPoints << endl;
	  outFile << red_comp(i) << "\t"
		  << green_comp(i)  << "\t"
		  << blue_comp(i) << endl;
	  tmp = xLast;
	  for (unsigned int j=0; j< numPoints; j++) {

		  outFile << tmp << "	\t\t"
			  << plotCurves[i][j] << "\n";
		  tmp += eps;
	  }//for j
	}//for i
    }//if

    // Put out the final frame (this hides the artificial cut-off of curves
    outFile << "########################################\n";
    outFile << "# FRAME around our figure " << endl;
    outFile << "p 4" << endl;
    outFile << "0.99 \t 0.99 \t 0.99" << endl;
    outFile << x1 << "\t" << y1 << endl;
    outFile << x2 << "\t" << y1 << endl;
    outFile << x2 << "\t" << y2 << endl;
    outFile << x1 << "\t" << y2 << endl;
    outFile << "######### End of File ##################\n";
    outFile.close();
    return 0;
  }//plot

// selfIntersections():
//   this should be another member function that lists
//   all the self-intersections of a curve
//
//  template <class NT>
//  void selfIntersections(BFVecInterval &vI){
//  ...
//  }

};// Curve class

  ////////////////////////////////////////////////////////
  // Curve helper functions
  ////////////////////////////////////////////////////////


//Xintersections(C, D, vI):
//  returns the list vI of x-ccordinates of possible intersection points.
//  Assumes that C & D are quasi-monic.(or generally aligned)
template <class NT>
void  Xintersections( Curve<NT>& P ,Curve<NT>& Q, BFVecInterval &vI){
  Sturm<NT> SS(resY(P, Q));
  SS.isolateRoots(vI);
}

//Yintersections(C, D, vI):
//	similar to Xintersections
template <class NT>
void  Yintersections( Curve<NT>& P ,Curve<NT>& Q, BFVecInterval &vI){
  Sturm<NT> SS(resX(P, Q));
  SS.isolateRoots(vI);
}

// Display Intervals
// 
void showIntervals(char* s, BFVecInterval &vI) {
   cout << s;
   for (unsigned int i=0; i< vI.size(); i++) {
   	cout << "[ " << vI[i].first << ", " 
   		<< vI[i].second << " ],  " ;
   }
   cout << endl;
}


/* ************************************************************************** */
// END
/* ************************************************************************** */

