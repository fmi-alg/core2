/* ************************************************
   File: prog9

   Purpose:  Exact comparison among objects of Expr class.
	     
	     
	     
   Usage: 

        % prog9

   Author: Vikram Sharma

   Since Core Library Version 1.6
   $Id: prog10.cpp,v 1.1 2006/03/07 04:51:26 exact Exp $
 ************************************************ */

#include "CORE.h"

using namespace CORE;
using namespace std;


int main(int argc , char ** argv) {

 Expr e[2];
 Expr f[2];
 e[0] = 10.0; e[1] = 11.0;
 f[0] = 5.0;  f[1] = 18.0;
 Expr ee = sqrt(e[0])+sqrt(e[1]);
 Expr ff = sqrt(f[0])+sqrt(f[1]);
 if (ee>ff) cout << "CORRECT! sr(10)+sr(11) > sr(5)+sr(18)" << endl;
     else cout << "ERROR! sr(10)+sr(11) <= sr(5)+sr(18)" << endl;
 // prints: $sr(10)+sr(11) > sr(5)+sr(18)$

}


