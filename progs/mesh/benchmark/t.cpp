/* **************************************
   File: convertPoly.cpp

   Description: 
	Given an input polynomial p(z) with real coefficients,
	we construct the real and complex bivariate polynomials,
	u(x,y) and v(x,y) where p(x+iy)=u(x,y)+i v(x,y).
	We then compute the real and complex curves u=0 and v=0
	in the complex plane, using the cxy algorithm.

   Usage:
        > ./convertPoly  [curve="x^2-x-1"]   [ofile="outdir/out"] [verbose=0]

	The input is a univariate polynomial p(z).
	The output is written to the four files:

		$(ofile).poly		-- original poly
		$(ofile)_u.poly 	-- the u-poly
		$(ofile)_v.poly 	-- the v-poly
		$(ofile)_uv.poly 	-- the uv-poly

	The format is the "natural monomial format".
	E.g.,	 z^2 -z -1,    x^2y + 2y^3 + 11xy + 2.


   Author: Chee Yap
   Since Nov, 2010, Core Library  Version 2.1
   $Id: t.cpp,v 1.3 2010/11/09 04:44:05 exact Exp $
 ************************************** */

#define CORE_LEVEL 4
#include "CORE/Core.h"
#include "CORE/poly/Curves.h"
#include "CORE/ComplexT.h"
//#include "benchmark.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ##################################################
// typedefs
// ##################################################
	//typedef double NT; // does not work -- Curves.tcc:: getnumber cannot convert
	typedef BigInt NT; 
	typedef ComplexT<double> Complex;
	typedef Polynomial<NT> PolyNT;
	typedef BiPoly<NT> BiPolyNT;

// ##################################################
// Argument processing and global vars
	// 1
	PolyNT p = PolyNT("(x-1)^4");	// default input polynomial 
	// 2
	string ofile = "outdir/out"; 	// output file (to be appended with u,v,uv)
	// 3
	int verbose = 0;		// verbose mode

	// GLOBAL:
	ofstream ofs;

// ##################################################
// Routines to be defined:
//
	// convertPoly(p, *u, *v)
	template <typename T>
	void ConvertPoly(const Polynomial<T> &p, BiPoly<T> *real, BiPoly<T> *im);

	// write to file in std monomial format
	template <typename T>
	bool write_poly(Polynomial<T>& p, const char* filename = "outdir/out");
	
	// read from file in std monomial format
	template <typename T>
	bool read_poly(Polynomial<T>& p, const char* ifilename = "outdir/in");

	// read from frisco file
	template <typename T>
	void read_poly_frisco(FILE *instr, Polynomial<T>& p);

	// Generate random polynomials.
	void GenerateRandom(Polynomial<double> *d, const unsigned int degree,
                    const unsigned int max_coeff, const unsigned int seed);
	void GenerateRandom(BiPoly<double> *u, BiPoly<double> *v,
                    const unsigned int degree, const unsigned int max_coeff,
                    const unsigned int seed);

	// read files
	void GetPoly(const char *file_name,
             Polynomial<double> *d);
	void GetBiPoly(const char *filename,
               BiPoly<double> *u,
               BiPoly<double> *v);

	// get root bounds
	void GetRootBounds(const Polynomial<double> &poly,
                   double *lb,
                   double *ub);

	// show(msg)
	void show(string msg);

	// writeout(string ofile, string ext, string poly)	
	bool writeout(string ofile, string ext, string poly)
	{
          string ofilename = ofile + ".poly";
	  ofs.open(ofilename.c_str());		// open "out.poly"
	  if (!ofs.is_open()) {
		std::cerr << "error opening " << ofilename << std::endl;
		return false;
	  } else {
	    ofs << "# original polynomial from convertPoly for " << ofile << endl;
	    ofs << p.toString('z') << endl;
	    ofs.close();
	  }

	  ofilename = ofile + "_u.poly";
	  ofs.open(ofilename.c_str());		// open "out_u.poly"
	  if (!ofs.is_open()) {
	    std::cerr << "error opening " << ofilename << std::endl;
	    return false;
  	  } else {
	  ofs << "# output u-polynomial from convertPoly for " << ofile << endl;
	  ofs << poly << endl;
	  ofs.close();
	  }

	  ofilename = ofile + "_v.poly";
	  ofs.open(ofilename.c_str());		// open "out_v.poly"
	  if (!ofs.is_open()) {
	    std::cerr << "error opening " << ofilename << std::endl;
	    return false;
  	  } else {
	  ofs << "# output v-polynomial from convertPoly for " << ofile << endl;
	  ofs << poly << endl;
	  ofs.close();
	  }
	  return true;
	}

// ##################################################
// MAIN
int main(int argc, char **argv) {

  // Get the input polynomial a (from file or randomly generated)
	/*
	Polynomial<NT> polys[] ={
		PolyNT("x^2 -x -1"), PolyNT("(x-1)^5"), PolyNT("(x-1)^2 (x+1)^2") };
	string names[] = {
		"golden ratio", "5th roots of unity" "multiple roots" };
	*/
    	  if (argc == 1) {
	      cout << "Usage: ./t  [curve=\"x^2-x-1\"]   [ofile=\"outdir/out\"] [verbose=0]";
	      cout << "\n Try 'make t1' or 'make t2' " << endl;
	  }
	  if (argc > 1) p = PolyNT(argv[1]);
	  if (argc > 2) ofile= string(argv[2]);
	  if (argc > 3) verbose=atoi(argv[3]);

	  show("Input Polynomial is: " + p.toString('z'));
	  show("ofile = " + ofile);

  // Convert input polynomial to the real and complex parts, u, v (and uv):
	BiPoly<NT> u, v;
  	ConvertPoly(p,&u,&v); 
	    show("\n --- Real Part Polynomial is u(x,y): " + u.toString());
	    show("\n --- Complex Part Polynomial is v(x,y): " + v.toString());
	Curve<NT> uv = u*v;
  // Output to file

	writeout(ofile, string(".poly"), p.toString());
	writeout(ofile, string("_u.poly"), u.toString());
	writeout(ofile, string("_v.poly"), v.toString());
	writeout(ofile, string("_uv.poly"), uv.toString());

	return 0;
}//main

// ##################################################
// Routines needed

/// Converts a complex polynomial p(z) into two real
/// bivariate polynomials u(x, y) & v(x, y) such that
/// p(z) = u(x, y) + iv(x, y)

template <typename T>
void ConvertPoly(const Polynomial<T> &p,
                 BiPoly<T> *real,
                 BiPoly<T> *im) {
	  BiPoly<T> subst = "x + y";
	  // p.eval<BiPoly<T> >(subst) would also do the
	  // trick.
	  //
	  // composeHorner is defined in composeBiPoly.h. Note that
	  // it is not always required.
	  BiPoly<T> subst_result = composeHorner<T>(p, subst);
	  unsigned int degree = subst_result.getYdegree();
	
	  vector<Polynomial<T> > real_coeff;
	  vector<Polynomial<T> > im_coeff;
	
	  bool negate = true;
	  const vector<Polynomial<T> > &c = subst_result.coeffX;
	
	  for (unsigned int i = 0; i <= degree; ++i) {
	    if (i % 2 == 0) {
	      negate = negate ? false : true;	// flip sign
	      real_coeff.push_back(c[i]);
	      im_coeff.push_back(Polynomial<T>());
	      if (negate) {
	        real_coeff.back().negate();
	      }
	    } else {
	      im_coeff.push_back(c[i]);
	      real_coeff.push_back(Polynomial<T>());
	      if (negate) {
	        im_coeff.back().negate();
	      }
	    }
	  }
	  *real = BiPoly<T>(real_coeff);
	  *im = BiPoly<T>(im_coeff);
	}//

/// Reads a polynomial from file, in FRISCO format
template <typename T>
void read_poly_frisco(FILE *instr, Polynomial<T>& p) {
	  int indx, num_coeff, i;
	
	  /* skip blank or comment lines */
	  while ((i = fgetc(instr)) == '!' || isspace(i))
	    if(i == '!')
	      while (fgetc(instr) != '\n')
	        /* skip */ ;
	  ungetc(i, instr);
	
	  /* read data type */
	  char data_type[3];
	  fscanf(instr, "%3s", data_type);
	
	  /* read and convert prec_in from base 10 to base 2 */
	  long prec_in = -1;
	  if (prec_in == -1) {
	    fscanf(instr, "%ld", &(prec_in));
	    prec_in = (long) (prec_in * log(10.00)/log(2.0));// LOG2_10);
	  } else {    /* override input precision */
	    fscanf(instr, "%*d");
	    prec_in = prec_in; /* load default */
	  }
	
	  /* read degree */
	  int deg;
	  fscanf(instr, "%d", &(deg));
	
	  /* if sparse read num of coeff */
	  if (data_type[0] == 's')
	    fscanf(instr, "%d", &num_coeff);
	  else
	    num_coeff = deg + 1;
	
	  /* no need to read coefficients if user polynomial */
	  if (data_type[0] == 'u')
	    return;
	
	  /* read coefficients */
	  T* coeffs = new T[deg+1]; // we always assume dense representation!
	
	  for (int i = 0; i < deg + 1; ++i) {
	    coeffs[i] = 0;
	  }
	
	  //char new_line;
	  for (i = 0; i < num_coeff; i++) {
	
	    if (data_type[0] == 's') {
	      fscanf(instr, "%d", &indx);
	    } else {
	      indx = i;
	    }
	
	    if (data_type[1] == 'r') {
	      if (data_type[2] == 'i') {
	        BigInt temp;
	        if (!mpz_inp_str(temp.mp(), instr, 10)) {
	          // Exit if something happens.
	          assert(false);
	        }
	        coeffs[indx]=T(temp);
	      } else {
	        // We support only integer coefficients at this point.
	        assert(false);
	      }
	    } else {
	      // We support only polynomials with real coefficients
	      // at this point.
	      assert(false);
	    }
	  }
	
	  p = Polynomial<T>(deg, coeffs);
	  delete[] coeffs;
	}

/// Reads a templated polynomial from a filename, in natural string format
/// file format: a univariate polynomial in natural format
//	is stored in a single line.  Comment lines
//	preceding this line are indicated by first char of '#'
template <typename T>
bool read_poly(const char* ifilename, Polynomial<T>& p) {
	ifstream ifs(ifilename);
	if (!ifs.is_open()) {
	    std::cerr << "error reading " << ifilename << std::endl;
	    return false;
	}
	string line;
	// we read lines until the first character is not '#'
	bool found=false;
	while (!found && ifs.good()) {
	  	getline (ifs,line);
		if (line.at(0) != '#') found=true;
	}
	p = Polynomial<T>(line);
	ifs.close();
	return true;
}// read_poly

/// Reads a templated polynomial from a filename, in FRISCO format
template <typename T>
bool read_poly_frisco(const char* filename, Polynomial<T>& p) {
	  FILE* f;
	  if ((f = fopen(filename, "r")) == NULL) {
	    std::cerr << "error reading " << filename << std::endl;
	    return false;
	  }
	  read_poly_frisco(f, p);
	  fclose(f);
	  return true;
	}

/// Write a templated polynomial to a filename, in std monomial format:
template <typename T>
bool write_poly(Polynomial<T>& p, const char* filename = "outdir/out.poly") {
	  ofstream ofs;
	  ofs.open(filename);
	  if (ofs.is_open()) {
		  ofs << p.toString() << endl;
	  } else {
	    std::cerr << "error writing " << filename << std::endl;
	    return false;
	  }
	  ofs.close();
	  return true;
	}

/// Reads a BigInt polynomial from a filename
void GetPoly(const char *filename,
             Polynomial<BigInt> *poly) {
	  read_poly_frisco<BigInt>(filename, *poly);
	}

/// Reads a BigFloat polynomial from a filename
void GetPoly(const char *filename,
             Polynomial<BigFloat> *poly) {
	  read_poly_frisco<BigFloat>(filename, *poly);
	}

/// Reads a DoubleWrapper polynomial from a file name
void GetPoly(const char *filename,
	             Polynomial<DoubleWrapper> *poly) {
	 // cout << "********************************************************************" << endl;
	 // cout << "WARNING : You might lose precision in the polynomial coefficients " << endl
	 //     << "while operating at this core level. The polynomial you are operating " << endl
	 //     << "on might not be the polynomial you intended." << endl;
	 // cout << "********************************************************************" << endl;
	
	  Polynomial<BigInt> p_bi;
	  GetPoly(filename, &p_bi);
	  const int deg = p_bi.getDegree();
	  DoubleWrapper *dr = new DoubleWrapper[deg + 1];
	  for (int i = 0; i <= deg; ++i) {
	    dr[i] = p_bi.coeff()[i].doubleValue();
	  }
	  *poly = Polynomial<DoubleWrapper>(deg, dr);
	  delete[] dr;
	}

void GetPoly(const char *filename,
             Polynomial<Expr> *poly) {
  read_poly_frisco<Expr>(filename, *poly);
}

/// Reads a complex valued polynomial p(z) from file name, and returns
/// its real and complex bivariate BigInt polynomials: u(x,y), v(x,y)
void GetBiPoly(const char *filename,
               BiPoly<BigInt> *u,
               BiPoly<BigInt> *v) {
	  Polynomial<BigInt> poly;
	  GetPoly(filename, &poly);
	  ConvertPoly<BigInt>(poly, u, v);
	}

/// Reads a complex valued polynomial p(z) from file name, and returns
/// its real and complex bivariate BigFloat polynomials: u(x,y), v(x,y)
void GetBiPoly(const char *filename,
               BiPoly<BigFloat> *u,
               BiPoly<BigFloat> *v) {
	  Polynomial<BigFloat> poly;
	  GetPoly(filename, &poly);
	  ConvertPoly<BigFloat>(poly, u, v);
	}

/// Reads a complex valued polynomial p(z) from file name, and returns
/// its real and complex bivariate DoubleWrapper polynomials: u(x,y), v(x,y)
void GetBiPoly(const char *filename,
               BiPoly<DoubleWrapper> *u,
               BiPoly<DoubleWrapper> *v) {
	  Polynomial<DoubleWrapper> poly;
	  GetPoly(filename, &poly);
	  ConvertPoly<DoubleWrapper>(poly, u, v);
	}

/// Reads a complex valued polynomial p(z) from file name, and returns
/// its real and complex bivariate Expr polynomials: u(x,y), v(x,y)
void GetBiPoly(const char *filename,
               BiPoly<Expr> *u,
               BiPoly<Expr> *v) {
	  Polynomial<Expr> poly;
	  GetPoly(filename, &poly);
	  ConvertPoly<Expr>(poly, u, v);
	}

void GetRootBounds(const Polynomial<Expr> &poly,
                   Expr *lb,
                   Expr *ub) {
	  (*lb) = CORE::CauchyLowerBound<Expr>(poly);
	  (*ub) = CORE::CauchyUpperBound<Expr>(poly);
	}

void GetRootBounds(const Polynomial<BigFloat> &poly,
                   BigFloat *lb,
                   BigFloat *ub) {
	  BigInt temp = CORE::CauchyBound<BigFloat>(poly);
	  cout << "Root bound : " << temp << endl;
	  (*lb) = temp * -1;//CORE::CauchyLowerBound<BigFloat>(poly);
	  (*ub) = temp;//CORE::CauchyUpperBound<BigFloat>(poly);
	}

template <typename T>
void ConvertPoly(Polynomial<BigFloat> *output,
    const Polynomial<T> &in) {
  vector<BigFloat> coeff_array;
  for (int i = 0; i < in.getDegree() + 1; ++i) {
    coeff_array.push_back(in.coeff()[i]);
  }
  (*output) = Polynomial<BigFloat>(coeff_array);
}

void GetRootBounds(const Polynomial<DoubleWrapper> &poly,
                   DoubleWrapper *lb,
                   DoubleWrapper *ub) {
  Polynomial<BigFloat> poly_2;
  ConvertPoly(&poly_2, poly);

  BigInt temp = CORE::CauchyBound<BigFloat>(poly);

  (*lb) = DoubleWrapper((temp * -1).doubleValue());
  (*ub) = DoubleWrapper(temp.doubleValue());
}

void GenerateRandom(Polynomial<DoubleWrapper> *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
	  srand(seed);
	  const unsigned int MAX = 2*max_coeff;
	  machine_long max = max_coeff;
	  vector<DoubleWrapper> vec;
	  for (unsigned int i = 0; i <= degree; ++i) {
	    machine_long random = rand() % MAX;
	    vec.push_back(random - max);
	  }
	  (*d) = Polynomial<DoubleWrapper>(vec);
	}

void GenerateRandom(Polynomial<BigFloat> *d,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
	  srand(seed);
	  const unsigned int MAX = 2*max_coeff;
	  BigFloat max = max_coeff;
	  vector<BigFloat> vec;
	  for (unsigned int i = 0; i <= degree; ++i) {
	    machine_long random = rand() % MAX;
	    vec.push_back(BigFloat(random) - max);
	  }
	  (*d) = Polynomial<BigFloat>(vec);
	}

void GenerateRandom(BiPoly<DoubleWrapper> *u,
                    BiPoly<DoubleWrapper> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  Polynomial<DoubleWrapper> b;
  GenerateRandom(&b, degree, max_coeff, seed);
  ConvertPoly(b, u, v);
}

void GenerateRandom(BiPoly<BigFloat> *u,
                    BiPoly<BigFloat> *v,
                    const unsigned int degree,
                    const unsigned int max_coeff,
                    const unsigned int seed) {
  Polynomial<BigFloat> b;
  GenerateRandom(&b, degree, max_coeff, seed);
  ConvertPoly(b, u, v);
}


// Show
	void show(string msg){
		if (verbose>0) cout << msg << endl;
	}
// ##################################################
// END
// ##################################################
