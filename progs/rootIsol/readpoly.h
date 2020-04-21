/* file: readpoly.h
 *
 * THIS HAS BEEN INCORPORATED INTO CORE LIB:
 *   it reads polynomials as string as well as Frisco format
 *   Originally in ${CORE}/progs/rootIsol/
 *
 * *****************************************/

#define CORE_LEVEL 4
#include <CORE/CORE.h>
#include <string>
#include <iostream>
#include <fstream>
#include<vector>


//const double LOG2_10 = log(10.0) / log(2.0);

/***********************************************************
*       SUBROUTINE READ_POLY                               *
***********************************************************/

typedef std::vector<std::string> VecString;
char COMMENT='#';
std::string TESTDIR="../data/polynomials/";

using namespace CORE;
using namespace std;

// The file 'file' contains a sequence of strings each on its own line.
// This function creates a vector whose contents are the strings contained
// in 'file'. Basically, we have to read a line from 'file' and construct
// a corresponding string.
// Ignores all lines starting or containing a #.
// Does not handle white space in the file.
// Option specifies the type of input file: option = 1 means the file contains
// name of mpsolve files; option=2 means the file contains strings of polynomials.
void readString(char *file, VecString &v, int option){
  ifstream inFile(file);
  
  string line;

  while(getline(inFile, line, '\n')){
    if(line.find(COMMENT, 0) == string::npos)
      v.push_back(line);
  }
  if(option == 1)
    for (VecString::iterator it = v.begin();
	 it != v.end(); ++it) {
      (*it).insert(0,TESTDIR);// Insert TESTDIR at the starting of all strings
    }
}

template <typename T>
void read_poly(FILE *instr, CORE::Polynomial<T>& p)
{
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
  } else {		/* override input precision */
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

  /* validate polynomial data */
  //validate_poly(p, num_coeff);

  /* setup floating point multiprecision */
  /*
  if (p->prec_in != 0)
    mp_set_prec(p->prec_in);
  else
    mp_set_prec(2 * DBL_MANT_DIG);
  */

  /* no need to read coefficients if user polynomial */
  if (data_type[0] == 'u')
    return;

  /* read coefficients */
  T* coeffs = new T[deg+1]; // we always assume dense representation!

  for (i = 0; i < num_coeff; i++) {

    if (data_type[0] == 's') {
      fscanf(instr, "%d", &indx);
    } else
      indx = i;

    switch (data_type[1]) {	/* switch 1 */

    case 'r':			/* Real */

      switch (data_type[2]) {	/* switch 2 */

      case 'i':		/* Real - Integer Coefs */
	BigInt temp;
	if (!mpz_inp_str(temp.mp(), instr, 10))
	  std::cerr << "Error while reading coefficient" << std::endl;
	coeffs[indx]=T(temp);
	//std::cout << "coeff=" << coeffs[indx] << std::endl;
	break;

#if 0
      case 'q':		/* Real - Rational Coefs */
	if (!mpz_inp_str(mpq_numref(p->mqp_r[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	if (!mpz_inp_str(mpq_denref(p->mqp_r[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	mpq_canonicalize(p->mqp_r[indx]);
	break;

      case 'f':		/* Real - Big/Float Coefs */
	if (!mpf_inp_str(p->mfpr[indx], instr, 10))
	  error(1, "Error while reading coefficient");
	break;
#endif

      }				/* switch 2 */
      break;

#if 0
    case 'c':			/* Complex */

      switch (p->data_type[2]) {	/* switch 3 */

      case 'i':		/* Complex - Integer Coefs */
	if (!mpz_inp_str(p->mip_r[indx], instr, 10))
	  error(1, "Error while reading coefficient");
	if (!mpz_inp_str(p->mip_i[indx], instr, 10))
	  error(1, "Error while reading coefficient");
	break;

      case 'q':		/* Complex - Rational Coefs */
	if (!mpz_inp_str(mpq_numref(p->mqp_r[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	if (!mpz_inp_str(mpq_denref(p->mqp_r[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	mpq_canonicalize(p->mqp_r[indx]);
	if (!mpz_inp_str(mpq_numref(p->mqp_i[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	if (!mpz_inp_str(mpq_denref(p->mqp_i[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	mpq_canonicalize(p->mqp_i[indx]);
	break;

      case 'f':		/* Complex - Big/Float Coefs */
	if (!mpf_inp_str(mpc_Re(p->mfpc[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	if (!mpf_inp_str(mpc_Im(p->mfpc[indx]), instr, 10))
	  error(1, "Error while reading coefficient");
	break;

      }				/* switch 3 */
#endif

    }				/* switch 1 */
  }				/* for */

#if 0
  /* real begin */
  if (p->data_type[1] == 'r' && p->data_type[2] == 'f')
    for (indx = 0; indx <= p->deg; indx++) {
      if (p->data_type[0] == 's' && !p->spar[indx])
	continue;
      mpf_set(mpc_Re(p->mfpc[indx]), p->mfpr[indx]);
      mpf_set_ui(mpc_Im(p->mfpc[indx]), 0);
    }
#endif

  p = CORE::Polynomial<T>(deg, coeffs);
  delete[] coeffs;
  /* real end */
}

template <typename T>
void read_poly(const char* filename, CORE::Polynomial<T>& p)
{
  FILE* f;
  if ((f = fopen(filename, "r")) == NULL) {
    std::cerr << "error reading " << filename << std::endl;
  }
  read_poly(f, p);
  fclose(f);
}

