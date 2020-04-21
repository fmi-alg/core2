/* **************************************
   File: tBiPoly.cpp

   Description: 
	Unit testing of Poly<NT> and BiPoly<NT>.

	One main motivation is to introduce toString() methods
	for Polynomial and BiPoly -- this a natural and very easy to use. 

	All the toString methods defined here have been transferred into
	Poly.h and Curves.h and Curves.tcc.   However, the code here
	is useful for further experimentation and development.

   Usage:

        > ./tBiPoly [testMode=0]

	where
		testMode = 0 means non-interactive (suitable for "make test")
		testMode = 1 means testing Polynomials only 
		testMode = 2 means testing BiPoly only 
		testMode = 3 means testing both 

	This program is compiled with NT=BigInt as default.

	NOTES:
	(1) The case NT=BigInt has been extensively tested.  But
	other choices of NT have not been checked.

 	If you define one of the variables
		_NT_INT, _NT_RAT, _NT_EXP, _NT_BF
	then you get (respectively)
		NT=int, NT=BigRat, NT=Expr, NT=BigFloat.
	(2) It might be useful to do a "reverse" version of toString()
	in analogy to toStringRev() in the case of univariate polynomials.

   Author:  Chee Yap 
   Date:    Nov 6, 2010

   Since Core Library  Version 2.1
   $Id: tBiPoly.cpp,v 1.8 2012/04/13 15:24:00 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 4
#endif

#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"	// this is the official version of Curves.h
using namespace std;

// =========================================================
// DEFAULT number type NT
// =========================================================
		// Default number type should be either NT=BigInt or NT=IntWrapper
		
		#if defined _NT_INT
		  typedef int NT;
		#elif defined _NT_LON
		  typedef long NT;
		#elif defined _NT_EXP
		  typedef Expr NT;
		#elif defined _NT_RAT
		  typedef BigRat NT;
		#elif defined _NT_BF
		  typedef BigFloat NT;
		#elif defined _NT_BIGINT
		  typedef BigInt NT;
		#else
		  typedef IntWrapper NT;
		  //typedef BigInt NT;
		#endif
	
		typedef Polynomial<NT>			PolyNT;
		typedef BiPoly<NT>			BiPolyNT;
		typedef std::vector<NT>			VecNT;
		typedef std::vector<Polynomial<NT> >	VecPolyNT;
		typedef std::vector<BiPoly<NT> >	VecBiPolyNT;
		typedef std::vector<string >		VecString;
	
	// Predeclarations:
	string toString(PolyNT p, char var='X');
	string toStringRev(PolyNT p, char var='X');
	string toString(BiPolyNT bp, char var='X', char var='Y');
	void show(string msg);
	void errShow(string errMsg);
	
	// Global Vars
	int errorCount = 0;	
	int verbose = 0; // as silent as possible
	int testMode = 0;

	// =========================================================
	// MAIN
	// =========================================================
	int main(int argc, char* argv[])
	{
	if (argc > 1) testMode = atoi(argv[1]);
	if (argc > 2) verbose = atoi(argv[1]);
	cout << " ================ START OF TEST BIPOLY ================ " << endl;
  if (testMode==0) {
	cout << "================================================\n";
	cout << "BiPOLY CONSTRUCTORS \n";
		cout << "================================================\n";
	
	  //construct from string:
		BiPoly<NT> bp1 = BiPoly<NT>("-1Y+0X");
	  //construct from array:
		PolyNT c[] = { PolyNT("x^2+1"),  PolyNT("x+4"), PolyNT("1")};
		BiPolyNT bp3(2, c);	// construct from array...
			show(string("c[0] = " + toString(c[0])));
			show(string("c[1] = " + toString(c[1])));
			show(string("c[2] = " + toString(c[2])));
			show(string("bp3= " + toString(bp3)));
	  //construct from vector of PolyNT:
	  //construct a univariate polynomial viewed as bivariate:
	}

	// testMode=1 if only checking PolyNT:
	
  if (testMode == 0 || testMode==1 || testMode==3) {
	cout << "================================================\n";
	cout << "POLY String Output \n";
		cout << "================================================\n";
	
		Polynomial<NT> p0 = Polynomial<NT>().Zero();
			show(string("Zero: " + toString(p0)));
		p0 = Polynomial<NT>().Unity();
			show(string( "Unity: " + toString(p0)));
		p0 = Polynomial<NT>().NegUnity();
			show(string( "NegUnity: " + toString(p0)));
		p0 = Polynomial<NT>().Identity();
			show(string( "Identity: " + toString(p0)));
		p0 = Polynomial<NT>();
			show(string( "def Constructor: " + toString(p0)));
		PolyNT p2("-1x^2 -x +1");
			show(string( "string Constructed: " + toString(p2)));
		string vecs[] = {"0", "1", "x",
				 "x^3", "123x^12", "-456x^11", "x^2+0",
				 "3+4x", "-333x -444",
				 "-x^2+x-5", "x-5 = x^2",
				"321x^8+23x^4-99x-x^7", "100x^30 - 50x^10 +1",
				"(x-1)^5",
				"(7x-6x^3+2)(3x^2-2)",
				"-1x+0x", "0+0"
				};
		string names[] = {"zero poly", "one poly", "identity poly",
				  "mono1", "mono2", "mono3", "mono disguised",
				  "lin1", "lin2",
				  "quad1", "quad2",
				  "sparse1", "sparse2",
				  "pow1",
				  "prod1",
				  "odd1", "odd2"
				};
	
		int n=17;
		//int n=5; //for debugging
		
		Polynomial<NT> *vp = new Polynomial<NT>[n]  ;
		Polynomial<NT> *vp2 = new Polynomial<NT>[n]  ;
		//Polynomial<NT> *vp3 = new Polynomial<NT>[n]  ;
	
		// NOW we do a battery of tests:
		  for (int i=0; i<n ; i++){
			show("Name of polynomial: " + names[i]);
			show("   Input string: " + vecs[i]);
			vp[i] = Polynomial<NT>(vecs[i]);
			show("   Output string: " + toString(vp[i]));
			show("   Output of poly.h: " + vp[i].toString('z'));
			show("   RevOutput string: " + toStringRev(vp[i]));
			show("   RevOutput of poly.h: " + vp[i].toStringRev('z'));
			vp2[i] = Polynomial<NT>(vp[i].toString());
			show("   Regurgitated: " + vp2[i].toString());
			(vp2[i] == vp[i]) ?
				show("CORRECT! String I/O are inverses") :
				errShow("ERROR! String I/O are non-inverses");
		  }//for
		if (verbose==0){
		 if (errorCount==0) 
		   cout << "ALL CORRECT in testing PolyNT String I/O" << endl;
  		 else
		   cout << errorCount <<
			   " ERRORS found in testing PolyNT String I/O" << endl;
		}
	} // if(testMode==1 or 3)

  if (testMode == 0 || testMode==2 || testMode==3) {
	cout << "================================================\n";
	cout << "BIPOLY String Output \n";
	cout << "	NOTE: we test both Core's version of bp.toString() and\n"
		<< "	the local version, toString(bp) " << endl;
		cout << "================================================\n";
	// INITIAL TEST:
		BiPolyNT bp("y^2x -3x^3 + y^2 + x -111");
			show("Initial test polynomial (using local toString): "
				+ toString(bp));
			show("Initial test polynomial (using Core's toString): "
				+ bp.toString());
		BiPolyNT bp0(toString(bp));	// using local toString()
		BiPolyNT bp2(bp.toString());	// using Core's toString()
			(bp == bp2 && bp == bp0) ?
			  show("CORRECT: bipoly string I/O is symmetric!") :
			  errShow("ERROR: bipoly string I/O is non symmetric!");
	// SPECIAL POLYNOMIALS:
		bp0 = BiPoly<NT>();
			show("default Constructor: " + bp0.toString());
			(bp0 == BiPolyNT().Zero()) ?
				show( "CORRECT! equals Zero()") :
				errShow("ERROR! unequal Zero()");
		bp0 = BiPoly<NT>().Zero();
			show( "Zero: " + toString(bp0));
			(bp0 == BiPolyNT().Zero()) ?
				show("CORRECT! equals Zero()") :
				errShow("ERROR! unequal Zero()") ;
		bp0 = BiPoly<NT>().Unity();
			show( "Unity: " + toString(bp0));
			(bp0 == BiPolyNT().Unity()) ?
				show("CORRECT! equals Unity()") :
				errShow("ERROR! unequal Unity()") ;
		bp0 = BiPoly<NT>().NegUnity();
		if (verbose>0)
			show( "NegUnity: " + toString(bp0));
			(bp0 == BiPolyNT().NegUnity()) ?
				show("CORRECT! equals NegUnity()") :
				errShow("ERROR! unequal NegUnity()") ;
		bp0 = BiPoly<NT>().IdentityX();
			show( "IdentityX: " + toString(bp0));
			(bp0 == BiPolyNT().IdentityX()) ?
				show("CORRECT! equals IdentityX()") :
				errShow("ERROR! unequal IdentityX()") ;
		bp0 = BiPoly<NT>().IdentityY();
			show( "IdentityY: " + toString(bp0));
			(bp0 == BiPolyNT().IdentityY()) ?
				show("CORRECT! equals IdentityY()") :
				errShow("ERROR! unequal IdentityY()") ;
	 
  cout << "================================================\n";
  cout << "DONE testing SPECIAL BIPOLYs" << endl;
		cout << "================================================\n";

		string vecs[] = {"0", "1",
				 "x", "y",
				 "x^3", "123x^12", "-456x^11", "x^2+0",
				 "y^3", "123y^12", "-456y^11", "y^2+0",
				 "3+4x", "-333x -444",
				 "3+4y", "-333y -444",
				 "-x^2 + x - 5", "x - 5 = x^2",
				"321x^8 + 23x^4 - 99x - x^7", "100x^30 - 50x^10 + 1",
				"321y^8 + 23y^4 - 99y - y^7", "100y^30 - 50y^10 + 1",
				"(x - 1)^3", "(y + 2)^2", "(x - 1)^3 (y + 2)^2", 
				"(7yx - 6x^3 + 2)(3x^2y^2 - 2)",
				"0x^2 + 1y", "0+0", "(x^2)y^0 + (0)y^2"
				};
		string names[] = {"zero bipoly", "one bipoly",
				  "xIdent bipoly", "yIdent bipoly",
				  "xMono1", "xMono2", "xMono3", "xMono disguised",
				  "yMono1", "yMono2", "yMono3", "yMono disguised",
				  "xLin1", "xLin2",
				  "yLin1", "yLin2",
				  "xQuad1", "xQuad2",
				  "yQuad1", "yQuad2",
				  "sparse1", "sparse2",
				  "xPow1", "yPow1", "xyPow1",
				  "randomProd1",
				  "odd1", "odd2", "odd3"
				};
	
		int n=29;
		//int n=5; //for debugging
		
		BiPoly<NT> *vp = new BiPoly<NT>[n]  ;
		BiPoly<NT> bpLocal, bpCore  ;
		string sLocal, sCore  ;
	
		// NOW we do a battery of tests:
		for (int i=0; i<n ; i++){
			show("Name of BiPoly: " + names[i]);
			show("   Input string: " + vecs[i]);
			vp[i] = BiPoly<NT>(vecs[i]);
			sLocal = toString(vp[i],'x','y');
			sCore = vp[i].toString('u','v');
			show("   Local toString : " + sLocal);
			show("   Core's toString (in u,v): " + sCore);
			(vp[i] == BiPoly<NT>(sLocal,'x','y') &&
				vp[i] == BiPoly<NT>(sCore,'u','v')  ) ?
				show("   CORRECT! String I/O are inverses") :
				errShow("   ERROR! String I/O are non-inverses");
		}
		if (verbose==0){
		 if (errorCount==0) 
		   cout << "ALL CORRECT in testing BiPoly String I/O" << endl;
  		 else
		   cout << errorCount <<
			   " ERRORS found in testing BiPoly String I/O" << endl;
		}//else if verbose
	} // if(testMode==0 or 2 or 3)
	
	   return 0;
	}//main 
	
	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// METHODS: toString(), toStringRev()
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// show(msg)
inline void show(string msg){
	if (verbose>0) cout << msg << endl;
}
// errShow(errMsg)
inline void errShow(string errMsg){
	errorCount++;
	if (verbose>0) cout << errMsg << endl;
}
	
// toString(bipoly, xvar, yvar):
// 	output a bipoly as a string that reads naturally
// 	starting from leading coefficients.
// 	NOTE: this function has been incorporated into Curves.h
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	string toString(BiPolyNT bp, char xvar, char yvar){
	  int d= bp.getTrueYdegree();
	  if (d == -1) return std::string("0");
	  std::ostringstream oss;
	  std::string s;
	  int i = d;
	  //
	  Polynomial<NT> c;	// coefficient of y powers
	  c = bp.getCoeff(d);	// first term
	  if (d==0) {// we now know that c != 0
		oss << c.toString(xvar);
		s = oss.str();
		return s;}
	  if (d==1) {
		if (c == Polynomial<NT>().Unity())
			oss << yvar;
		else if (c == Polynomial<NT>().NegUnity())
			oss << "-" << yvar;
	  	else if (c.getTrueDegree()==0)
			oss << c.toString() << yvar;
		else
			oss << "(" << c.toString(xvar) << ")" << yvar;
	  } else {	// d>1
		if (c == Polynomial<NT>().Unity())
			oss << yvar << "^" << d;
		else if (c == Polynomial<NT>().NegUnity())
			oss << "-" << yvar << "^" << i;
	  	else if (c.getTrueDegree() == 0) // c is constant, unequal to 1 or -1
			oss << c.toString(xvar) << yvar << "^" << i;
		else 	// c has degree at least 1
			oss << yvar << "^" << i << "(" << c.toString(xvar) << ")" ;
	  }// done with first term.
	  i=d-1;
	  NT cc;
	  for (; i>= 2; i--) {	// general term (non-first, deg at least 2)
		c = bp.getCoeff(i);
		if (c != Polynomial<NT>().Zero()) {
		  if (c == Polynomial<NT>().Unity())
	  		oss << " + " << yvar << "^" << i;
		  else if (c == Polynomial<NT>().NegUnity())
	  		oss << " - " << yvar << "^" << i;
		  else if (c.getTrueDegree() == 0) {// c=const, unequal to 1 or -1
			cc = c.getCoeff(0);
			if (cc > 0) // assume cc (type NT) has operator<<()
	  			oss << " + " << cc << yvar << "^" << i;
			else {
				cc *= NT(-1);  //negate c
	  			oss << " - " << cc << yvar << "^" << i; } }
		  else  // c has degree at least 1
	  		oss << " + (" << c.toString(xvar) << ")" << yvar << "^" << i; }
		  } //for (i>=2)
	  //Finally: handle linear and constant terms which are NOT leading terms:
	  if (d > 1) { // process linear term, but only if d>1 (else duplicated)
		  c = bp.getCoeff(1); // linear term
		  if (c != Polynomial<NT>().Zero()) {
			if (c == Polynomial<NT>().Unity())
				oss << " + " << yvar;
			else if (c == Polynomial<NT>().NegUnity())
				oss << " - " << yvar;
			else if (c.getTrueDegree() == 0) { // c is constant
				cc = c.getCoeff(0);
				if (cc > 0) oss << " + " << cc << yvar;
				else {
					cc *= NT(-1);
					oss << " - " << cc << yvar; } }
			else // c has degree at least 1
				oss << " + (" << c.toString(xvar) << ")" << yvar; }
	  	}// linear term if d>1
	  if (d > 0) { // process constant term, but only if d>0
	  	  c = bp.getCoeff(0); // constant term
	   	  if (c != Polynomial<NT>().Zero()) {
			if (c.getTrueDegree() == 0) {
				cc = c.getCoeff(0);
				if (cc > 0) oss << " + " << cc;
				else { // cc < 0
					cc *= NT(-1);
					oss << " - " << cc; } }
			else  // c has degree at least 1
	  			oss << " + (" << c.toString(xvar) << ")"; }
		  }// constant term if d>0
	  s=oss.str();
	  return s;
	}//toString(BiPoly,x,y)

// toString(poly, var):
// 	output polynomial as a string that reads naturally
// 	starting from leading coefficients.
// 	NOTE: this function has been incorporated into Poly.h
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
std::string toString(PolyNT p, char var){
  int d= p.getTrueDegree();
  //
  if (d == -1) return std::string("0"); 
  // FOR EXTRA ROBUSTNESS,
  // 	we check the possibility that
  //	the zero polynomial is regarded as having degree 0:
  if (d == 0 && p.getCoeff(0) == NT(0)) return std::string("0"); 
  int i=d;
  std::ostringstream oss;
  std::string s;
  NT c;
  c = p.getCoeff(i);	// first term
  if (d==0) {// we now know that c != 0
	oss << c;
	s = oss.str();
	return s;}
  if (d==1) {
	if (c == 1) oss << var;
	else if (c== -1) oss << "-" << var;
  	else oss << c << var;
  } else 	// d>1
	if (c == 1) oss << var << "^" << d;
	else if (c == -1) oss << "-" << var << "^" << i;
  	else oss << c << var << "^" << i;
  i=d; i--;
  for (; i>= 2; i--) {	// general term
	c = p.getCoeff(i);
	if (c != 0) {
	  if (c == 1)
  		oss << " + " << var << "^" << i;
	  else if (c == -1)
  		oss << " - " << var << "^" << i;
	  else if (c > 0)
  		oss << " + " << c << var << "^" << i;
	  else {
		c *= NT(-1);
  		oss << " - " << c << var << "^" << i;
	  }}} //for i>=2
  if (d > 1) { // if d=1, we must not output anything (else duplicate)
	  c = p.getCoeff(1); // linear term
	  if (c != 0) {
		if (c == 1)
	  		oss << " + " << var;
		else if (c == -1)
	  		oss << " - " << var;
		else if (c > 0)
	  		oss << " + " << c << var;
		else {
			c *= NT(-1);
	  		oss << " - " << c << var;
		}}} // if d>=1
  if (d > 0) { // if d=0, must not output here (else duplicate)
	   c = p.getCoeff(0); // constant term
	   if (c != 0) {
		if (c>0)
	  		oss << " + " << c;
		else {
			c *= NT(-1);
	  		oss << " - " << c;
		}}} // if d>=0
  s=oss.str();
  return s;
}//toString

// toStringRev(poly, var):
// 	output in reverse order
// 	(starting from tail coefficient to leading coefficient
// 	NOTE: this function has been incorporated into Poly.h
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
string toStringRev(PolyNT p, char var){
  int d= p.getTrueDegree();
  //
  if (d == -1) return std::string("0"); 
  std::ostringstream oss;
  bool seenFirstTerm = false;
  NT c;
  if (d >= 0){ 
  	c = p.getCoeff(0);
	if (c != 0) {
	  seenFirstTerm = true;
	  oss << c;
	}}
  if (d >= 1){
	c = p.getCoeff(1);
	if (c != 0) {
	  if (seenFirstTerm) {
		  if (c>0) oss << " + ";
		  else oss << " - ";
	  } else
		  seenFirstTerm = true;
	  if (c == 1 || c == -1) oss << var;
	  else {
		  if (c<0) c *= NT(-1);
		  oss << c << var;
	  }}}
  int i=2; 
  for (; i<= d; i++) {
	c = p.getCoeff(i);
	if (c != 0) {
	  if (seenFirstTerm) {
		if (c>0) oss << " + ";
		else oss << " - ";
	  } else
		  seenFirstTerm = true;
	  if (c < 0) c *= NT(-1);
	  if (c == 1) oss << var << "^" << i;
	  else oss << c << var << "^" << i;
  	}}
  std::string s=oss.str();
  return s;
}//toStringRev

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// END of file
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
