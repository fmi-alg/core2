/* file: tElemFun.cpp
 *
 * 	We test the output from our elementary functions, i.e.,
 *
 * 		sin, cos, tan, cot,
 * 		asin, acos, atan,
 * 		exp, log, log2, log10
 *
 * 	These are all derived from MPFR,
 * 	but we provide our own I/O routines that prevent garbage digits.
 *
 * 	The tests are self-validating.
 *
 * 	Usage:
 * 		> ./tElemFun
 * 		> ./tElemFun function [value] [output digits]
 * 
 * 	Without any arguments, tElemFun will run a standard battery of self-validating
 * 	tests.
 *
 * 	DEBUGGING NOTES:
 * 	June 2010: Still unresolved:
 * 		-- for some reason, mpfr returns a value for cos(0.7) and claims
 * 		this value is exact (this is of course false, according to Lindemann's theorem).
 * 		So the test fails in this case. Is this a bug in MPFR?
 *
 * Author: Jihun/Chee (Jun 2010)
 * Since Core Library 2.0
 *  $Id: tElemFun.cpp,v 1.7 2010/06/18 15:47:42 exact Exp $
 *************************************************** */

// This program MUST be run at CORE_LEVEL 3:
#define CORE_LEVEL 3

#include "CORE.h"
#include <map>

using namespace std;

#define DEFAULT_SIN 

void function_test (string strFun, string strArg, int outDigit,  
                    bool timing = false, bool givenCompatible = false,
                    string strMaple = string(""))
{
  Expr funVal;
  prec_t relPrec = digits2bits(outDigit);

  Timer2 timer;
  timer.start();

  if (strFun == "sin")
     funVal = sin(Expr(strArg));
  if (strFun == "cos")
     funVal = cos(Expr(strArg));
  if (strFun == "tan")
     funVal = tan(Expr(strArg));
  if (strFun == "cot")
     funVal = cot(Expr(strArg));
  if (strFun == "asin")
     funVal = asin(Expr(strArg));
  if (strFun == "acos")
     funVal = acos(Expr(strArg));
  if (strFun == "atan")
     funVal = atan(Expr(strArg));
  if (strFun == "exp")
     funVal = exp(Expr(strArg));
  if (strFun == "log")
     funVal = log(Expr(strArg));
  if (strFun == "log2")
     funVal = log2(Expr(strArg));
  if (strFun == "log10")
     funVal = log10(Expr(strArg));

  funVal.approx(relPrec, CORE_INFTY); // use relative precision
  timer.stop();
  cout << "compute " << strFun << "(" << strArg << ")"
	<< " to " << relPrec << " relative precision : ";
  cout << funVal << endl;

  if (timing)
    cout << "It took " << timer.get_mseconds() << " mseconds." << endl;

  string strfunVal = funVal.toString();
  if (givenCompatible && isCompatible(strfunVal, strMaple))
    cout << "CORRECT!!! " << strFun << "(" << strArg << ")" << " correponds to Maple" << endl;
  else if (givenCompatible) {
    cout << "ERROR!!! " << strFun << "(" << strArg << ")" << " does not correpond to Maple" << endl;
    cout << "STRING: " << strfunVal << endl;
    cout << "CORE2 : " << funVal << endl;
    cout << "UPPER : " << funVal.BigFloat2Value().getRight() << endl;
    cout << "LOWER : " << funVal.BigFloat2Value().getLeft() << endl;
    cout << "Maple : " << strMaple << endl;
  }
}

int main( int argc, char *argv[] ) {

  // default function values with parameter 0.7 and 30 absolute digits.
  map<string, string> strMap;

  strMap.insert(pair<string, string>("sin",    "0.644217687237691053672614351399"));
  strMap.insert(pair<string, string>("cos",    "0.764842187284488426255859990192"));
  strMap.insert(pair<string, string>("tan",    "0.842288380463079448128135002213"));
  strMap.insert(pair<string, string>("cot",    "1.18724183212667935367236269369"));
  strMap.insert(pair<string, string>("asin",   "0.775397496610753063740353352715"));
  strMap.insert(pair<string, string>("acos",   "0.795398830184143555490968338925"));
  strMap.insert(pair<string, string>("atan",   "0.610725964389208616543758876490"));
  strMap.insert(pair<string, string>("exp",    "2.01375270747047652162454938858"));
  strMap.insert(pair<string, string>("log2",  "-0.514573172829758240428350112258"));
  strMap.insert(pair<string, string>("log",   "-0.356674943938732378912638711241"));
  strMap.insert(pair<string, string>("log10", "-0.1549019599857431692877837414074"));

  
  // default function values with argument 0.001, with 60 absolute digits.
  map<string, string> strMap2;

  strMap2.insert(pair<string, string>("sin",    "0.00099999983333334166666646825397100970015131473480865841900"));
  strMap2.insert(pair<string, string>("cos",    "0.99999950000004166666527777780257936480379188921289614586985"));
  strMap2.insert(pair<string, string>("tan",    "0.00100000033333346666672063494250441803431495977419338549240"));
  strMap2.insert(pair<string, string>("cot",  "999.999666666644444442328042116402095024315082134968695793042"));
  strMap2.insert(pair<string, string>("asin",   "0.00100000016666674166671130955419149062614469765573280956976"));
  strMap2.insert(pair<string, string>("acos",   "1.56979632662822987756461038208555995147244000203182010091770"));
  strMap2.insert(pair<string, string>("atan",   "0.00099999966666686666652380963492054401162093455426801309143"));
  strMap2.insert(pair<string, string>("exp",    "1.00100050016670834166805575399305831156307620058070146022851"));
  strMap2.insert(pair<string, string>("log2",  "-9.9657842846620870436109582884681705275944941790737418361642"));
  strMap2.insert(pair<string, string>("log",   "-6.907755278982137052053974364053092622803304465886318928099984"));
  strMap2.insert(pair<string, string>("log10", "-3.000000000000000000000000000000000000000000000000000000000000"));

  string strFun("sin");
  string strArg("0.7");
  int    outDigit = 30;;
  prec_t relPrec;

  if (argc < 2) {
    cout << "usage : ./tElemFun function [value] [output digits]" << endl;
    cout << "running default test : comparision results with those of Maple" << endl;

    setDefaultOutputDigits(outDigit);

    //int argPrec = 100;	// unused
    function_test("sin", strArg, outDigit, false, true, strMap["sin"]);  
    function_test("cos", strArg, outDigit, false, true, strMap["cos"]);  
    function_test("tan", strArg, outDigit, false, true, strMap["tan"]);  
    function_test("cot", strArg, outDigit, false, true, strMap["cot"]);  
    function_test("asin", strArg, outDigit, false, true, strMap["asin"]);  
    function_test("acos", strArg, outDigit, false, true, strMap["acos"]);  
    function_test("atan", strArg, outDigit, false, true, strMap["atan"]);  
    function_test("exp", strArg, outDigit, false, true, strMap["exp"]);  
    function_test("log2", strArg, outDigit, false, true, strMap["log2"]);  
    function_test("log", strArg, outDigit, false, true, strMap["log"]);  
    function_test("log10", strArg, outDigit, false, true, strMap["log10"]);  

    outDigit = 50;
    setDefaultOutputDigits(outDigit);
    string strArg("0.001");
    
    // argPrec = 250;	// unused
    function_test("sin", strArg, outDigit, false, true, strMap2["sin"]);  
    function_test("cos", strArg, outDigit, false, true, strMap2["cos"]);  
    function_test("tan", strArg, outDigit, false, true, strMap2["tan"]);  
    function_test("cot", strArg, outDigit, false, true, strMap2["cot"]);  
    function_test("asin", strArg, outDigit, false, true, strMap2["asin"]);  
    function_test("acos", strArg, outDigit, false, true, strMap2["acos"]);  
    function_test("atan", strArg, outDigit, false, true, strMap2["atan"]);  
    function_test("exp", strArg, outDigit, false, true, strMap2["exp"]);  
    function_test("log2", strArg, outDigit, false, true, strMap2["log2"]);  
    function_test("log", strArg, outDigit, false, true, strMap2["log"]);  
    function_test("log10", strArg, outDigit, false, true, strMap2["log10"]);  
    return 0;
  }

  if (argc > 1)
    strFun = argv[1];

  if (argc > 2)
    strArg = argv[2];

  if (argc > 3)
    outDigit = atoi(argv[3]);

  relPrec = digits2bits(outDigit); 

  setDefaultOutputDigits(outDigit);

  function_test(strFun, strArg, relPrec, true);

  return 0;
}
