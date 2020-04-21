/*************************************************************
 * Purpose:  
 *          How many ways can we print the number 1?
 *          From the output below, we see basically three:
 *               (a) 1.
 *               (b) 1.000000        (6 output digits)
 *               (c) 1.000000e+00    (6 output digits)
 *           How do we explain the different formats?
 *               (a) Internally, it is a BigRat or BigInt 
 *               (b) Internally, it is a BigFloat
 *               (c) Internally, it is a BigFloat
 *           The difference between (b) and (c) is whether we 
 *           choose the Positional or Scientific formats.
 *           We can set the output digits, which is 6 here.
 *           The difference between (a) and (b) is invariably
 *           whether we use string input or use a number literal.
 *
 * Author: Chee Yap
 *
 * Since Core Library 1.7
 * Date: Aug 2, 2004
 * ************************************************************/

   #define CORE_LEVEL 3
   #include "CORE/CORE.h"
   using namespace std;
   
   int main(int argc, char* argv[])
   {
      cout << "How many ways to print the number one?" << endl;
      cout << "-- Let me count the ways ..." << endl;
      cout << ">>>>>>>>>>>>>>   cout << setprecision(6);" << endl;
      cout << setprecision(6);
      cout << ">>>>>>>>>>>>>> setPositionalFormat();" << endl;
      setPositionalFormat();
      cout << ">>>>>>>>>>>>>> setDefaultInputDigits(CORE_INFTY);" << endl;
      setDefaultInputDigits(CORE_INFTY);
      cout << "1.0 prints as                 " << 1.0 << endl;
      cout << "BigFloat(1.0) prints as       " << BigFloat(1.0) << endl;
      cout << "BigFloat(\"1.0\") prints as   " << BigFloat("1.0") << endl;
      cout << "Expr(1.0) prints as           " << Expr(1.0) <<  endl;
      cout << "Expr(\"1.0\") prints as       " << Expr("1.0") << endl;
      cout << "Expr(1/1) prints as           " << Expr(1/1) << endl;
      cout << "Expr(\"1/1\") prints as       " << Expr("1/1") << endl;
      cout << "sqrt(Expr(\"1.0\")) prints as " << sqrt(Expr("1.0")) << endl;
      cout << ">>>>>>>>>>>>>> setScientificFormat();" << endl;
      setScientificFormat();
      setDefaultInputDigits(2);
      cout << "1.0 prints as                 " << 1.0 << endl;
      cout << "BigFloat(1.0) prints as       " << BigFloat(1.0) << endl;
      cout << "BigFloat(\"1.0\") prints as   " << BigFloat("1.0") << endl;
      cout << "Expr(1.0) prints as           " << Expr(1.0) <<  endl;
      cout << "Expr(\"1.0\") prints as       " << Expr("1.0") << endl;
      cout << "Expr(1/1) prints as           " << Expr(1/1) << endl;
      cout << "Expr(\"1/1\") prints as       " << Expr("1/1") << endl;
      cout << "sqrt(Expr(\"1.0\")) prints as " << sqrt(Expr("1.0")) << endl;
      cout << ">>>>>>>>>>>>>>  MORE TESTS" << endl;

/* **************************************************
 * THIS IS no longer meaningful for Core2:
 *
      cout << "Is BigFloat(1.0) exact?       " << BigFloat(1.0).isExact()
           << endl;
      cout << "Is BigFloat(\"1.0\") exact?     " << BigFloat("1.0").isExact()
           << endl;
 * ************************************************* */
      cout << "Is Expr(1/1) equal to 1?      " << 
              (Expr(1/1) == 1 ? "true" : "false")    << endl;
      cout << "Is Expr(\"1/1\") equal to 1?    " << 
              (Expr("1/1") == 1 ? "true" : "false")    << endl;
      return 0;
   }

/* ***************************************************
 * Here is the output for Core 1.7:
 * ***************************************************
    How many ways to print the number one?
    -- Let me count the ways ...
    >>>>>>>>>>>>>>   cout << setprecision(6);
    >>>>>>>>>>>>>> setPositionalFormat();
    >>>>>>>>>>>>>> setDefaultInputDigits(CORE_INFTY);
    1.0 prints as                 1.000000
    BigFloat(1.0) prints as       1.00000
    BigFloat("1.0") prints as   1.
    Expr(1.0) prints as           1.000000
    Expr("1.0") prints as       1.
    Expr(1/1) prints as           1.000000
    Expr("1/1") prints as       1.
    sqrt(Expr("1.0")) prints as 1.
    >>>>>>>>>>>>>> setScientificFormat();
    1.0 prints as                 1.000000e+00
    BigFloat(1.0) prints as       1.00000
    BigFloat("1.0") prints as   1.
    Expr(1.0) prints as           1.000000e+00
    Expr("1.0") prints as       1.
    Expr(1/1) prints as           1.000000e+00
    Expr("1/1") prints as       1.
    sqrt(Expr("1.0")) prints as 1.
    >>>>>>>>>>>>>>  MORE TESTS
    Is BigFloat(1.0) exact?       1
    Is BigFloat("1.0") exact?     1
    Is Expr(1/1) equal to 1?      1
    Is Expr("1/1") equal to 1?    1
 * ***************************************************
 * Note: there is a minor bug in what our BigFloat understands by
 * "6 digits" of output precision (this will be fixed in future).
 * This number is the one less than for the other cases.
 * There also seems to be 2 extra white spaces in front of BigFloat
 * output!
 * ***************************************************/
