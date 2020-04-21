/* **************************************
   File: tIntersect.cpp

   Description: 
   	basic intersection of two curves

   Usage:
        > ./tIntersect [n=1] [m=2] [eps=0.5]

	will plot the union of the n-th curve with
	the m-th curve in this program
	(currently, n and m range in 0,1,...,4).

	The output is in the file data/plot2.
	You can display this file by calling drawcurve
	    program, found in CORE_PATH/ext/graphics.

   BUGS and TODO LIST:
   	Should be able to do automatic analysis to determine
	interesting area for intersection

   Author:  Chee Yap and Vikram Sharma
   Date:    Apr 20, 2004

   Since Core Library  Version 1.7
   $Id: tt.cpp,v 1.2 2010/11/08 13:52:22 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#  define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"
#include "Curves.h"

using namespace std;

typedef BigInt				NT;
typedef Polynomial<NT>			PolyNT;
typedef std::vector<NT>			VecNT;
typedef std::vector<Polynomial<NT> >	VecPoly;

int main(int argc, char* argv[])
{
	// GET ARGUMENTS
	int curveNo1 = 2;	// First of 2 curves to intersect
	if (argc>1) curveNo1 = atoi(argv[1]);
	int curveNo2 = 3;	// Second of 2 curves to intersect
	if (argc>2) curveNo2 = atoi(argv[2]);
	BigFloat eps = 0.5;	// precision of the plot (this is delta-x)
	if (argc>3) eps = BigFloat(argv[3]);
	BigFloat x1 = -2.0;	// min x
	if (argc>4) x1 = BigFloat(argv[4]);
	BigFloat y1 = -10.0;	// min y
	if (argc>5) y1 = BigFloat(argv[5]);
	BigFloat x2 = 10.0;	// max x
	if (argc>6) x2 = BigFloat(argv[6]);
	BigFloat y2 = 10.0;	// max y
	if (argc>7) y2 = BigFloat(argv[7]);

    // SET INTERNAL PRECISION
    //setDefaultRelPrecision(CORE_posInfty); // This puts defAbsPrec in control
    int prec = 45;			// Do Sturm refinement till delta
					// is at most 2^{-prec}
    //setDefaultAbsPrecision(prec);

    // SET OUTPUT PRECISION
    cout.setf(ios::scientific, ios::floatfield);  // choose scientific notation
    cout.precision(prec/3); 		// convert prec bits into prec/3 digits

    // CONSTRUCT EQUATION OF CURVE
    // cissoid  2Y^2 - X^3
    NT cissoid_coeff0[] = {0, 0, 0, -1};
    NT cissoid_coeff2[] = {2};
    
    // ellipse: X^2 + 2Y^2 - 1
    NT ellipse_coeff0[] = {-1, 0, 1};	// x^2 - 1
    NT ellipse_coeff2[] = {2};
	
    // cubic1: Y^3 - X^3 + 2XY
    //       = Y^3 +  (2X)Y + (-X^3)
    NT cubic1_coeff0[]  = {0, 0, 0, -1};
    NT cubic1_coeff1[]  = {0, 2};
    NT cubic1_coeff3[]  = {1};
	
    // cubic2: Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2
    // 	 = Y^3 + (3X)Y^2 + (4X^2)Y + (-2X^3+2X^2).
    NT cubic2_coeff0[]  = {0, 0, 2, -2};
    NT cubic2_coeff1[]  = {0, 0, 4};
    NT cubic2_coeff2[]  = {0, 3};
    NT cubic2_coeff3[]  = {1};
	
    // cubic3: Y^2 - X(X^2-1) 
    // 	component C0 is an oval through (0,0), left of the Y-axis,
    // 	component C1 is a simple curve through (1,0) symmetric about
    // 	the X-axis
    NT cubic3_coeff0[]  = {0, 1, 0, -1};
    NT cubic3_coeff3[]  = {1};


	VecPoly vP1;

	switch (curveNo1){
	  case 0:
	    cout << " The Cissoid of Diocles [Brieskorn-Knorrer, p.9]\n"
	       << "    Equation is  - X^3 + 2Y^2 = 0." << endl;
	    vP1.push_back( Polynomial<NT>(3, cissoid_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP1.push_back( Polynomial<NT>(0, cissoid_coeff2) );	// coeff of y^2
	    //eps = 0.5; x1 = -2.0; y1 = -10.0; x2 = 10.0; y2 = 10.0;
	    break;
	  case 1:
	    cout << " Ellipse \n" 
	         << "    Equation is  X^2 + 2Y^2 = 1." << endl;
	    vP1.push_back( Polynomial<NT>(2, ellipse_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP1.push_back( Polynomial<NT>(0, ellipse_coeff2) );	// coeff of y^2
	    //eps = 0.05; x1 = -2.0; y1 = -2.0; x2 = 2.0; y2 = 2.0;
	    break;
	  case 2:
	    cout << " Cubic1: Y^3 - X^3 + 2XY = Y^3 +  (2X)Y + (-X^3)" << endl;
	    vP1.push_back( Polynomial<NT>(3, cubic1_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>(1, cubic1_coeff1) );	// coeff of y
	    vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y^2
	    vP1.push_back( Polynomial<NT>(0, cubic1_coeff3) );	// coeff of y^3
	    //eps = 0.05; x1 = -5.0; y1 = -5.0; x2 = 5.0; y2 = 5.0;
	    break;
	  case 3:
	    cout << " Cubic2:  Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2" << endl;
	    vP1.push_back( Polynomial<NT>(3, cubic2_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>(2, cubic2_coeff1) );	// coeff of y
	    vP1.push_back( Polynomial<NT>(1, cubic2_coeff2) );	// coeff of y^2
	    vP1.push_back( Polynomial<NT>(0, cubic2_coeff3) );	// coeff of y^3
	    //eps = 0.05; x1 = -2.0; y1 = -2.0; x2 = 2.0; y2 = 2.0;
	    break;
	  case 4:
	    cout << " Cubic3: Y^2 - X(X^2-1)" << endl;
	    vP1.push_back( Polynomial<NT>(3, cubic3_coeff0) );	// coeff of 1
	    vP1.push_back( Polynomial<NT>::Zero() );         // coeff of y
	    vP1.push_back( Polynomial<NT>::Zero() );         // coeff of y^2
	    vP1.push_back( Polynomial<NT>(0, cubic3_coeff3) );	// coeff of y^3
	    //eps = 0.05; x1 = -3.0; y1 = -3.0; x2 = 5.0; y2 = 5.0;
	    break;
	  default:
	    cout << "Choose a smaller number for the curve to display!" << endl;
	    exit(0);
	}

	VecPoly vP2;

	switch (curveNo2){
	    vP2.push_back( Polynomial<NT>(3, cissoid_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP2.push_back( Polynomial<NT>(0, cissoid_coeff2) );	// coeff of y^2

	    //eps = 0.5; x1 = -2.0; y1 = -10.0; x2 = 10.0; y2 = 10.0;
	    break;
	  case 1:
	    cout << " Ellipse \n" 
	         << "    Equation is  X^2 + 2Y^2 = 1." << endl;
	    vP2.push_back( Polynomial<NT>(2, ellipse_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>::Zero() );	// coeff of y
	    vP2.push_back( Polynomial<NT>(0, ellipse_coeff2) );	// coeff of y^2

	    //eps = 0.05; x1 = -2.0; y1 = -2.0; x2 = 2.0; y2 = 2.0;
	    break;
	  case 2:
	    cout << " Cubic1: Y^3 - X^3 + 2XY = Y^3 +  (2X)Y + (-X^3)" << endl;
	    vP2.push_back( Polynomial<NT>(3, cubic1_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>(1, cubic1_coeff1) );	// coeff of y
	    vP2.push_back( Polynomial<NT>::Zero() );	// coeff of y^2
	    vP2.push_back( Polynomial<NT>(0, cubic1_coeff3) );	// coeff of y^3

	    //eps = 0.05; x1 = -5.0; y1 = -5.0; x2 = 5.0; y2 = 5.0;
	    break;
	  case 3:
	    cout << " Cubic2:  Y^3 -2X^3 + 4X^2Y + 3XY^2 +2X^2" << endl;
	    vP2.push_back( Polynomial<NT>(3, cubic2_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>(2, cubic2_coeff1) );	// coeff of y
	    vP2.push_back( Polynomial<NT>(1, cubic2_coeff2) );	// coeff of y^2
	    vP2.push_back( Polynomial<NT>(0, cubic2_coeff3) );	// coeff of y^3

	    //eps = 0.05; x1 = -2.0; y1 = -2.0; x2 = 2.0; y2 = 2.0;
	    break;
	  case 4:
	    cout << " Cubic3: Y^2 - X(X^2-1)" << endl;
	    vP2.push_back( Polynomial<NT>(3, cubic3_coeff0) );	// coeff of 1
	    vP2.push_back( Polynomial<NT>::Zero() );         // coeff of y
	    vP2.push_back( Polynomial<NT>::Zero() );         // coeff of y^2
	    vP2.push_back( Polynomial<NT>(0, cubic3_coeff3) );	// coeff of y^3

	    //eps = 0.05; x1 = -3.0; y1 = -3.0; x2 = 5.0; y2 = 5.0;
	    break;
	  default:
	    cout << "Choose a smaller number for the curve to display!" << endl;
	    exit(0);
	}
	// TESTING CURVES
	Curve<NT> C1(vP1);
	Curve<NT> C2(vP2);
	C1 *= C2; //product of the two curves
	Curve<NT> CC(C1);
	CC.dump("test curve");
//Kludge
eps = 0.05; x1 = -10.0; y1 = -10.0; x2 = 10.0; y2 = 10.0;


        //C1.dump("test curve");
	//	C1.plot(eps, x1, y1, x2, y2, 2); // output in data/plot2 file
	

	//PLotting of some other curves
	//Following examples are from the paper by Sendra
	Curve<NT> C3("516 - 352 y - 35 y^2+41 y^3 + 172 x-88xy + xy^2 - 255 x^2 + 85 x^2y - 85 x^3");
	C3.dump();
	C3.plot(eps, x1, y1, x2, y2, 1); // output in data/plot2 file

	/*Curve<NT> C4("-132 + 128 y - 29 y^2 - y^3 + 28 x - 76 xy + 31 y^2 x + 75 x^2 - 41 x^2y + 17 x^3");
	C4.dump();
	//C4.plot(eps, x1, y1, x2, y2, 2); // output in data/plot2 file

	//Tacnode curve
	Curve<NT> C5(" 2 x^4 - 3 x^2 y + y^2 - 2y^3 + y^4");
	C5.dump();
	//C5.plot(eps, x1, y1, x2, y2, 2); // output in data/plot2 file
	
	//Affine Quartic case
	Curve<NT> C6("1 + x- 15 x^2 - 29 y^2 + 30 y^3 - 25 xy^2 = x^3y + 35 xy + x^4 - 6y^4 + 6xy^2");
	C6.dump();
	//C6.plot(eps, x1, y1, x2, y2, 2); // output in data/plot2 file
	

 
	Curve<NT> C7("(x^2 + y^2 - y) (y^3 + y - x^2)    (y^4 - 2y^3 + y^2 - 3 yx^2 + 2 x^4)", 1);
	C7.dump();
	*/


	cout<<"********CHECKING*********"<< endl;

	//BiPoly<NT> CC2("x + y");
	//CC2.dump();
	//cout << "CC2 y-degree = "<< CC2.getYdegree() << endl;
	//cout << "CC2 x-degree = "<< CC2.getXdegree() << endl;
	/*	cout << "3 x^2y^31 + 7 xy^2 - (x^2 + 2y^2 -1)(x+y) + 13xy+25"<<endl;
	
	cout << "(x^2 + 2y^2 -1)(x+y)+x^45"<<endl;

	BiPoly<NT> CC1("23(x^2 + 2y^2 -1)(x+y)+x^45(x^2 + 2y^2x+56 y^45))", 1);
	cout << "CC1 degree = "<< CC1.getYdegree() << endl;
	CC1.dump();
	
	BiPoly<NT> CC2("y^2 - x(x^2-1)", 1);
	CC2.dump();*/


/*
	VecPoly vP1;
	NT c3[] = {0,5};
	NT c2[] = {0, 0, 3};
	NT c0[] = {-2};
	vP1.push_back( Polynomial<NT>(0, c0) );	
	vP1.push_back( Polynomial<NT>::Zero() );	// coeff of y
	vP1.push_back( Polynomial<NT>(2,c2 ));	// coeff of y^2
	vP1.push_back( Polynomial<NT>(1, c3) );	// coeff of y^3
	
	VecPoly vP2;
	NT cc1[] = {1};
	NT cc0[] = {0,1};
	vP2.push_back( Polynomial<NT>(1, cc0) );	
	vP2.push_back( Polynomial<NT>(0,cc1) );	// coeff of y

	BiPoly<NT> C2(vP2);

	VecPoly vP3;
	NT ccc0[] = {1,0,2,-7};
	vP3.push_back( Polynomial<NT>(3, ccc0) );	
	vP3.push_back( Polynomial<NT>::Zero() );	// coeff of y

	Polynomial<NT> P(3, ccc0);
	BiPoly<NT> C3(vP3);

	BiPoly<NT> C1(vP1);
	C1.dump();
*/

	// C += C1; C.dump();
	// C -= C1; C.dump();
	
	//	cout << "Multiply " << endl;
	//C *= C1;
	//C.dump();

	//	C=C1;
	//C.dump();

	//	C.mulXpoly(P);//C*=C3;
	//C.dump();
	/*	NT c = 3;
	C.mulScalar(c);
	C.dump();
	*/
	//	C.mulYpower(2);
	//C.dump();
	//	cout << "Pseudo Remainder " << endl;
	// C1.pseudoRemainderY(C); C1.dump();
	// C.pseudoRemainderY(C1); C.dump();
	//
	// C.differentiateXY(1,0); //C.dump();
	
	// cout << "GCD " << endl;
	// C = gcd(C , C1); C.dump();
	//
	// cout << "Resultant wrt Y" << endl;
	// resY(C, C1).dump();
	
	return 0;
}

