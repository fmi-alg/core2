#define CORE_LEVEL 4
#include "CORE/CORE.h"
#include "readpoly.h"

typedef BigInt NT;
typedef Polynomial<NT> Poly;

using namespace CORE;
using namespace std;


int main( int argc, char* argv[] ) {
  
  //  for(int i=0; i <= deg; i++)
  //    cout << i <<"th coefficient is "<< temp[i] << endl;

  Poly P;

  /* ************************************** */
  /* Read Arguments */
  /* ************************************** */
  if (argc > 1)
	read_poly(argv[1], P);
	//P = std::string(argv[1]) ;
  else
    read_poly("../data/frisco/hermite20.pol", P);
    	//read_poly("data/wilk20.pol", p);
	//read_poly("data/geom3_20.pol", p);
	//read_poly("data/mig1_20.pol", p);

  BFVecInterval v;
  Sturm<NT> SS(P);
  SS.isolateRoots(v);
  int i=0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
    
  return 0;
}
