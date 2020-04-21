#define CORE_LEVEL 4
#include "CORE/CORE.h"
#include "readpoly.h"
#include "DescartesP.h"
#include "DescartesB.h"
#include "DescartesB2.h"
#include "IntervalIsol.h"
#include "ContFrac.h"
#include "IntervalNewtonIsol.h"
//#include "IntervalDescartesB.h"
#include "IntervalDescartes.h"
#include "IntervalDescartesDouble.h"

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

int main(int argc, char* argv[]) {
  VecString v;
  int option=1;
  // Select the test option
  // Option 1: Test all algorithms on a polynomial input as a string
  // Option 2: Test all algorithms on a file of mpsolve type polynomials (see ../data/polynomials)
  // Option 3: Test all algorithms on a file of polynomials as strings
  if(argc <= 1){
    cout<<"Usage: ./tRootIsol [option] [file/polynomial] [iter]"<<endl;
    cout<<"If [option] = 1 test all algorithms on [polynomial]"<<endl;
    cout<<"If [option] = 2 test all algorithms on mpsolve polynomials in [file]"<<endl;
    cout<<"If [option] = 3 test all algorithms on string polynomials in [file]"<<endl;
    cout<<"[iter] is the number of iterations for the test to be performed"<<endl;
  }
  if(argc > 1)
    option = atoi(argv[1]);

  switch(option){
  case 1:
    if(argc > 2)
      v.push_back(string(argv[2]));
    else// Push wilkinson 20 as the default polynomial
      v.push_back("-17626570956801 + 3101726329798816*x^4-248094093696052480*x^8+ 11906403888393584640*x^12-380937320940052807680*x^16 + 8531481670913844314112*x^20-136479477644328091779072*x^24  + 1559488554903253423226880*x^28-12473693208113486380400640*x^32 + 66514549210671708666593280*x^36-212808750863162101240692736*x^40  + 309485009821345068724781057*x^44");
    //"2432902008176640000-8752948036761600000*x + 13803759753640704000*x^2 -12870931245150988800*x^3 + 8037811822645051776*x^4-3599979517947607200*x^5 + 1206647803780373360*x^6 -311333643161390640*x^7 + 63030812099294896*x^8-10142299865511450*x^9 + 1307535010540395*x^10 -135585182899530*x^11 + 11310276995381*x^12-756111184500*x^13 + 40171771630*x^14 -1672280820*x^15 + 53327946*x^16-1256850*x^17 + 20615*x^18 -210*x^19 + x^20");

    break;
  case 2:
    if(argc > 2)
      readString(argv[2], v, 1); //Reads in a file of mpsolve polynomials    
    else// The default mpsolve file is "../data/polynomials/deg10"
      readString("../data/polynomials/deg10", v, 1);
    break;
  case 3:
    if(argc > 2)
      readString(argv[2], v, 2); //Reads in a file of polynomials given as strings
    else
      readString("../data/d30_n50.dat", v, 2);
    break;
  default:;
  }

  //Other options are:
  //../data/polynomials/deg10[20,40,80,100,100_slow,squarefree,nonsqrfree]
  int iter = 10;
  if(argc > 3)
    iter = atoi(argv[3]); 

  PolyNT P;

  if(option == 1)
    if(argc > 2)
      cout<<"Test Polynomial is "<< argv[2] <<endl;
    else
      cout<<"../data/polynomials/wilk20"<<endl;
  
  if(option == 2)
    if(argc > 2)
      cout<<"Test File is "<< argv[2] <<endl;
    else
      cout<<"../data/polynomials/deg10"<<endl;

  if(option == 3)
    if(argc > 2)
      cout<<"Test File is "<< argv[2] <<endl;
    else
      cout<<"../data/d30_n50.dat"<<endl;


  //This loop gives the timing for all methods applied to a certain polynomial
  for (VecString::const_iterator it = v.begin();
	 it != v.end(); ++it) {
    if(option == 2)
      read_poly((*it).c_str(), P);// If it's an mpsolve file need a special function
    else
      P = PolyNT((*it).c_str());
    //cout << *it << " " <<endl;
    P.sqFreePart();
    //    P.dump();

    //cout <<"Descartes Bernstein:    ";
    //    DescartesB::test(P, iter);
    
    cout <<"Descartes Bernstein V2: ";
    DescartesB2::test(P, iter);
    cout <<"Descartes Power:        ";
    DescartesP::test(P, iter);
    /*cout <<"Continued Fraction:     ";
    ContFrac::test(P, iter);*/
    //cout <<"EVAL Centered Form:  ";
    //IntervalIsol::test(P, iter);
    //cout <<"Interval Newton:     ";
    //IntervalNewtonIsol::test(P, iter);
    //    cout <<"Interval Descartes BF2: ";
    //    IntervalDescartes::test(P, iter);
    cout <<"Interval Descartes Dbl: ";
    IntervalDescartesDouble::test(P, iter);
    
    cout << endl;
 }  

  /*
  BigFloat2 a, b;
  a = BigFloat2(1, DOUBLE_PREC); b=BigFloat2(3, DOUBLE_PREC);
  double aa=1, bb=3;
  Timer2 t1;


  t1.start();
  for(int i=0; i < 1000000; i++)
    a += b;
  t1.stop();
  cout << " Time for BigFloat2 addition " << t1.get_seconds();

  t1.start();
  for(int i=0; i < 1000000; i++)
    aa += bb;
  t1.stop();
  cout << " Time for Double addition " << t1.get_seconds();
  */

    
  return 0;
}


