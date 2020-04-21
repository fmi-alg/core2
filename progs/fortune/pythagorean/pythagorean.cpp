/* *****************************************************
 File: pythagorean.cpp

 	This program creates N number of Pythagorean triples
	(x,y,z) where x^2 + y^2 = z^2.
	We can view the triple (x,y,z) as representing
	the point (x/z, y/z) on the unit circle.  Such a
	highly degenerate set of points can be used to
	test robustness of various Voronoi diagram programs.
	In particular, we use it as input for the Fortune algorithm.

Usage:
	> pythagorean [N=10 [X=3 Y=4 Z=5 [OPTION=0]]

	The output is either points (OPTION=0) or triples (OPTION=1)
	or debugging info (OPTION=2).  This is written to standard output.

REMARK:
	The initial triple is (3,4,5).  
	Should generalize to allow any initial triple.

Author: Joaquin Grech
Date: April, 2002
Since Core Library Version 1.5.

 ***************************************************** */
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif
#include <ctime>
#include "CORE.h"

using namespace std;

struct Ptriple {
	long n, m, p;
};

Ptriple nextTriple(Ptriple a, Ptriple b) {
	Ptriple result;
	result.n=-a.m*b.m + a.n*b.n;
	result.m=a.n*b.m + a.m*b.n;
	result.p=a.p*b.p;
	return result;
}

void generatePoints(int n, int m, int p, int howmany, int option, int prec=100) {
	Ptriple triple;
	triple.n=n;
	triple.m=m;
	triple.p=p;

	Ptriple tri;
	tri.n=n;
	tri.m=m;
	tri.p=p;

	setDefaultPrecision(prec, CORE_INFTY);
	setDefaultOutputDigits(prec/4);   // 4 bits per digit

	for (int i=0; i<howmany; i++) {
	  switch(option){
	    case 0: //output points
		  cout << (BigFloat)tri.n/tri.p << " " << (BigFloat)tri.m/tri.p << endl;
		  break;
	    case 1: //output triples
		  cout << tri.n << " " << tri.m << " " << tri.p << endl;
		  break;
	    case 2: //make positive points WHY ARE WE DOING THIS?
		  cout << (BigFloat)(tri.n/tri.p)+1.0 << " " << (BigFloat)(tri.m/tri.p)+1.0 << endl;
		  break;
	    default: //debug
		  if (tri.n*tri.n + tri.m*tri.m == tri.p*tri.p)
		  	cout << "good triple: ";
		  else
		  	cout << "bad triple: ";
		  cout << tri.n << " " << tri.m << " " << tri.p << endl;
		  break;
	  }
	  tri=nextTriple(tri,triple);
	}

} 

void generatePoints3(int u) {
//	triples=new Ptriple[u];
	for (int n=1;n<=u;n++) {
		long tmp = 8*n - 7; 		// to avoid the ambiguity of calling
						// "sqrt(8*n - 7)"
		long t=floor((3+sqrt(tmp))/2);
		long s=n-(power(t,2)-3*t+2)/2;
		long x=2*s*t;
		long y=power(t,2)-power(s,2);
		long z=power(t,2)+power(s,2);


	/*	Ptriple triple;
		triple.n=x;
		triple.m=y;
		triple.p=z;
		triples[n-1]=triple; */
	//	cout << x.toString();
		
		if (rand() % 2 == 0)
			x=x*(-1);
		if (rand() % 2 == 0)
			y=y*(-1);
		
		
		cout << x/z << " " << y/z << "\n";
	}
}

int main(int argc, char **argv) 
{	
   
   int N=10;	// default number of points
   if (argc > 1)
	   N=atoi(argv[1]);
   int X=3;
   int Y=4;
   int Z=5;
   if (argc >4){
	   X=atoi(argv[2]);
	   Y=atoi(argv[3]);
	   Z=atoi(argv[4]);
   }
   int OPTION=3;
   if (argc >5) 
	   OPTION=atoi(argv[5]);

   int prec=100; // absolute precision
   if (argc >6)
	   prec=atoi(argv[6]);


   unsigned int seed1 = (unsigned int) (time(NULL) % UINT_MAX);
   srand(seed1);
	

 //  generatePoints3(N);
   generatePoints(X,Y,Z,N,OPTION,prec);

   return 0;
}


