// badOutputDigits.cpp

/* **************************************************
 * Apr 2010:
 * Core2 has garbage output digits.
 * Here is the output from this program:
 * (I have inserted a comma before the garbage digits)
 *
 * > ./badOutputDigits 
 * sqrt(2) to 80 digits
 * sqrt(2) =1.5			// approx(2)
 * sqrt(2) =1.414 0625		// approx(10)
 * sqrt(2) =1.414213562373095048801688724209,1762499253118869660345518308661172390827687195269  //approx(100)
 * sqrt(2) =1.414213562373095048801688724209698078569671875376948073176679,6228945468916789744  //approx(200)
 * sqrt(2) =1.414213562373095048801688724209698078569671875376948073176679737990732478462107  //approx(400)
 * sqrt(2) =1.414213562373095048801688724209698078569671875376948073176679737990732478462107  //approx(800)
 *
 * June 2010: Problem is fixed.  Here is the correct output:
 * > ./badOutputDigits 
 * sqrt(2) to 80 digits
 * sqrt(2) =1.5
 * sqrt(2) =1.41
 * sqrt(2) =1.41421356237309504880168872421
 * sqrt(2) =1.41421356237309504880168872420969807856967187537694807317668
 * sqrt(2) =1.414213562373095048801688724209698078569671875376948073176679737990732478462107
 * sqrt(2) =1.414213562373095048801688724209698078569671875376948073176679737990732478462107
 * 
 * ************************************************* */

#include "CORE.h"
using namespace std;

int main(){
  double x=2;
  cout << "sqrt(2) to 80 digits" << endl;
  cout << setprecision(80);
  cout << "sqrt(2) =" << sqrt(x).approx(2) << endl;
  cout << "sqrt(2) =" << sqrt(x).approx(10) << endl;
  cout << "sqrt(2) =" << sqrt(x).approx(100) << endl;
  cout << "sqrt(2) =" << sqrt(x).approx(200) << endl;
  cout << "sqrt(2) =" << sqrt(x).approx(400) << endl;
  cout << "sqrt(2) =" << sqrt(x).approx(800) << endl;
  return 0;
}
